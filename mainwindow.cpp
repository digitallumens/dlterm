#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "interface.h"
#include "solarized.h"
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QScrollBar>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDate>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cmdHelper(new cmdHelper::cmdHelper),
  m_cmdHistory(new cmdHistory::cmdHistory),
  m_interface(new interface::interface),
  m_preferencesDialog(new preferencesDialog::preferencesDialog) {
  ui->setupUi(this);
  QApplication::setWindowIcon(QIcon(QString::fromUtf8(":/DL.png")));
  // remove the ugly focus border
  ui->commandLine->setAttribute(Qt::WA_MacShowFocusRect, 0);
  // set solarized theme
  solarized::setStyleSheetQLineEdit(ui->commandLine);
  solarized::setStyleSheetQTextEdit(ui->outputFeed);
  solarized::setStyleSheetQFrame(ui->line);
  // configure GUI widgets
  ui->actionDisconnect->setVisible(false);
  // configure autocomplete
  ui->commandLine->setCompleter(m_cmdHelper->m_cmdCompleter);
  // catch command events
  ui->commandLine->installEventFilter(this);
  // default text before connection is established
  ui->commandLine->setPlaceholderText("Press ⌘K to establish a connection.");
  // disable tab focus policy
  ui->outputFeed->setFocusPolicy(Qt::NoFocus);
  // hint to OSX about the role of these menu items
  ui->actionAbout->setMenuRole(QAction::AboutRole);
  ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
  // connect signals and slots
  connect(m_interface, SIGNAL(connectionEstablished()), this, SLOT(on_connectionEstablished()));
  connect(m_interface, SIGNAL(connectionStatusChanged(QString)), this, SLOT(on_connectionStatusChanged(QString)));
  this->setWindowTitle("DLTerm");
  // install telegesis drivers if missing
  checkForInstalledKexts();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::checkForInstalledKexts(void) {
  QFileInfo siLabsNewDriverInfo("/Library/Extensions/SiLabsUSBDriver.kext");
  QFileInfo siLabsDriverInfo("/System/Library/Extensions/SiLabsUSBDriver.kext");
  QFileInfo siLabsDriver64Info("/System/Library/Extensions/SiLabsUSBDriver64.kext");
  if ( (!siLabsNewDriverInfo.exists() && !siLabsDriverInfo.exists() && !siLabsDriver64Info.exists()) ) {
    int result = QMessageBox::warning(this,
                        tr("Drivers not detected"),
                        tr("It looks like the drivers for the USB Wireless Adapter are not installed. Would you like to install them now?"),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
    if (result == QMessageBox::Yes) {
      // Launch installer from Resources folder
      //DLDebug(1, DL_FUNC_INFO) << "User chose to install drivers.";
      QString path = qApp->applicationDirPath() + QString("/../Resources/Install Drivers.pkg");
      QUrl url = QUrl::fromLocalFile(path);
      if (!QDesktopServices::openUrl(url)) {
        QMessageBox::critical(QApplication::activeWindow(), tr("Error"), tr("Failed to open file at %1").arg(path));
      }
    } else {
      //DLDebug(1, DL_FUNC_INFO) << "User chose NOT to install drivers.";
    }
  } else {
    //DLDebug(600, DL_FUNC_INFO) << "SiLabsUSBDriver is installed.";
  }
}

QString MainWindow::processUserRequest(QString *request) {
  QStringList argList;
  QStringList responseList;
  QString preamble;
  QString response;
  if (request->startsWith("help")) {
    solarized::setTextColor(request, solarized::SOLAR_YELLOW);
    return buildAppHelp();
  }
  // check for a helper handler
  cmdHandler_t handler = m_cmdHelper->getCmdHandler(*request);
  if (handler == NULL) {
    // not a helper command
    responseList = m_interface->queryPmu(QStringList() << *request);
    solarized::setTextColor(request, solarized::SOLAR_BASE_01);
  } else {
    argList = request->split(" ");
    argList.removeFirst();
    argList.removeFirst();
    // pass control to the helper
    responseList = handler(argList, m_interface);
    solarized::setTextColor(request, solarized::SOLAR_YELLOW);
  }
  // flatten
  foreach(QString r, responseList) {
    if (r.contains("ERROR")) {
      // remove plus from parsed responses with errors
      if (r.startsWith("+")) {
        r.remove("+");
      }
      solarized::setTextColor(&r, solarized::SOLAR_RED);
    } else if (r.startsWith("+")) {
      // parsed responses start with plus
      r.remove("+");
      solarized::setTextColor(&r, solarized::SOLAR_BLUE);
    } else if (r.contains("OK")) {
      solarized::setTextColor(&r, solarized::SOLAR_GREEN);
    } else {
      // unparsed responses
      solarized::setTextColor(&r, solarized::SOLAR_VIOLET);
    }
    response += (r + "<br>");
  }
  return response;
}

QString MainWindow::buildPrompt(void) {
  QString prompt;
  QString timestamp;
  if (ui->actionShow_Timestamp->isChecked()) {
    timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
    prompt = timestamp + " > ";
  } else {
    prompt = " > ";
  }
  solarized::setTextColor(&prompt, solarized::SOLAR_BASE_01);
  return prompt;
}

QString MainWindow::buildAppHelp(void) {
  QString response;
  QStringList helpList = m_cmdHelper->help();
  foreach(QString r, helpList) {
    if (r.startsWith("-")) {
      solarized::setTextColor(&r, solarized::SOLAR_BASE_01);
    } else {
      solarized::setTextColor(&r, solarized::SOLAR_CYAN);
    }
    response += (r + "<br>");
  }
  if (response.endsWith("<br>") == false) {
    response += "<br>";
  }
  return response;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
  QString userRequest;
  QString pmuResponse;
  QString prompt;
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    switch (keyEvent->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
      // scrub the input
      userRequest = ui->commandLine->text().simplified();
      // ignore requests until a connection is established
      if (userRequest.isEmpty() || (m_interface->isConnected() == false)) {
        ui->commandLine->clear();
        break;
      }
      // process the command
      m_cmdHistory->append(userRequest);
      prompt = buildPrompt();
      pmuResponse = processUserRequest(&userRequest);
      ui->commandLine->clear();
      ui->outputFeed->insertHtml(prompt + userRequest + "<br>");
      ui->outputFeed->insertHtml(pmuResponse + "<br>");
      // scroll to bottom
      QCoreApplication::processEvents();
      ui->outputFeed->verticalScrollBar()->setValue(ui->outputFeed->verticalScrollBar()->maximum());
      break;
    case Qt::Key_Tab:
      if (ui->commandLine->cursorPosition() != m_cmdHelper->getCurrentCompletionLength()) {
        // accept current completion
        ui->commandLine->end(false);
      } else {
        ui->commandLine->setText(m_cmdHelper->getNextCompletion());
      }
      break;
    case Qt::Key_Up:
      ui->commandLine->setText(m_cmdHistory->scrollBack());
      break;
    case Qt::Key_Down:
      ui->commandLine->setText(m_cmdHistory->scrollForward());
      break;
    case Qt::Key_Left:
      // SHIFT + LEFT clears the command line
      if (keyEvent->modifiers() & Qt::ShiftModifier) {
        ui->commandLine->clear();
      }
      break;
    case Qt::Key_Home:
      ui->commandLine->home(false);
      break;
    case Qt::Key_End:
      ui->commandLine->end(false);
      break;
    default:
      break;
    }
  }
  return QObject::eventFilter(target, event); 
}

void MainWindow::on_actionConnect_Using_FTDI_triggered() {
  m_interface->connectFTDI();
}

void MainWindow::on_actionConnect_Using_Telegesis_triggered() {
  // open the preferences dialog so the user can input the missing info
  if (m_preferencesDialog->m_serialNumber == 0) {
    m_preferencesDialog->exec();
  }
  // check serial number again before connecting
  if (m_preferencesDialog->m_serialNumber != 0) {
    m_interface->configure(m_preferencesDialog->m_networkStr, m_preferencesDialog->m_serialNumber);
    m_interface->connectTelegesis();
  }
}

void MainWindow::on_actionDisconnect_triggered() {
  m_interface->disconnect();
  ui->actionDisconnect->setVisible(false);
  ui->actionConnect_Using_FTDI->setVisible(true);
  ui->actionConnect_Using_Telegesis->setVisible(true);
  ui->actionPreferences->setDisabled(false);
  ui->commandLine->setPlaceholderText("Press ⌘K to establish a connection.");
}

void MainWindow::on_connectionEstablished(void) {
  ui->actionConnect_Using_FTDI->setVisible(false);
  ui->actionConnect_Using_Telegesis->setVisible(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionPreferences->setDisabled(true);
  ui->commandLine->setPlaceholderText("Type a command here. Terminate by pressing ENTER.");
}

void MainWindow::on_connectionStatusChanged(QString status) {
  QString response;
  response = QString("[%1]").arg(status);
  solarized::setTextColor(&response, solarized::SOLAR_BASE_01);
  ui->outputFeed->insertHtml(QString("%1<br>").arg(response));
}

void MainWindow::on_actionClear_Output_triggered() {
  ui->outputFeed->clear();
}

void MainWindow::on_actionSave_Output_to_File_triggered() {
  QString filename = QFileDialog::getSaveFileName(this, tr("Save output"), QDir::currentPath(), tr("Text File (*.txt)"));
  QFile file(filename);
  if (file.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&file);
    out << ui->outputFeed->toPlainText();
  }
}

void MainWindow::on_actionPreferences_triggered() {
  m_preferencesDialog->exec();
}

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::about(this, "About DLTerm",
                     tr("<p><b>Digital Lumens Terminal</b></p>"
                        "<p><small>Version 0.4.0</small></p>"
                        "<p><small>Build date 12/10/15</small></p>"
                        "<small>Copyright &copy; 2010-2015. All rights reserved.</small>"));
}
