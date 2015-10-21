#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QScrollBar>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cmdHelper(new cmdHelper::cmdHelper),
  m_cmdHistory(new cmdHistory::cmdHistory),
  m_solarized(new solarized::solarized),
  m_interface(new interface::interface),
  m_preferencesDialog(new preferencesDialog::preferencesDialog) {
  ui->setupUi(this);
  QApplication::setWindowIcon(QIcon(QString::fromUtf8(":/DL.png")));
  // remove the ugly focus border
  ui->commandLine->setAttribute(Qt::WA_MacShowFocusRect, 0);
  // set solarized theme
  m_solarized->setStyleSheetQLineEdit(ui->commandLine);
  m_solarized->setStyleSheetQTextEdit(ui->outputFeed);
  m_solarized->setStyleSheetQFrame(ui->line);
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
  this->setWindowTitle("DLTerm: Disconnected");
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
  QString cmd;
  QStringList argList;
  QStringList cmdList;
  QStringList pmuResponseList;
  QStringList parsedResponseList;
  QString response;
  if (request->startsWith("help")) {
    m_solarized->setTextColor(request, SOLAR_MAGENTA);
    return buildAppHelp();
  }
  if (request->contains(" ")) {
    // helper command format "verb object argList"
    // ex: set serialNumber 0400DEAD
    argList = request->split(" ");
    cmd = argList.at(0) + " " + argList.at(1);
    argList.removeFirst();
    argList.removeFirst();
  } else {
    // doesn't look like a helper command...
    cmd = *request;
  }
  // find the associated helper entry
  struct cmdEntry *cmdEntry = m_cmdHelper->getCmdEntry(cmd);
  if (cmdEntry == NULL) {
    // not a helper command
    cmdList << *request;
    m_solarized->setTextColor(request, SOLAR_BASE_01);
  } else {
    // show help
    if (request->contains("help")) {
      m_solarized->setTextColor(request, SOLAR_YELLOW);
      return buildHelp(cmdEntry);
    }
    // translate helper command
    cmdList = cmdEntry->buildCmd(argList);
    // command translator returned an error
    if (cmdList.at(0).startsWith("ERROR")) {
      return buildUserInputError(cmdList);
    }
    m_solarized->setTextColor(request, SOLAR_YELLOW);
  }
  // send commands & get responses
  m_interface->queryPmu(cmdList, &pmuResponseList);
  // parse responses
  if (cmdEntry == NULL) {
    response = buildBasicCommandResponse(pmuResponseList);
  } else if (cmdEntry->parseResponse == NULL) {
    response = buildUnparsedResponse(cmdList, pmuResponseList);
  } else {
    response = buildParsedResponse(cmdEntry, cmdList, pmuResponseList);
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
  m_solarized->setTextColor(&prompt, SOLAR_BASE_01);
  return prompt;
}

QString MainWindow::buildParsedResponse(struct cmdEntry * cmdEntry, QStringList cmdList, QStringList pmuResponseList) {
  QString preamble;
  QString response;
  QStringList parsedResponseList;
  if (ui->actionShow_PMU_Command->isChecked()) {
    preamble = QString("PMU command: %1").arg(cmdList.at(0));
    if (cmdList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(cmdList.length() - 1);
    }
    preamble += "<br>";
    preamble += QString("PMU response: %1").arg(pmuResponseList.at(0));
    if (pmuResponseList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(pmuResponseList.length() - 1);
    }
    preamble += "<br>";
    m_solarized->setTextColor(&preamble, SOLAR_BASE_01);
  }
  // use parser
  parsedResponseList = cmdEntry->parseResponse(pmuResponseList);
  // flatten
  foreach(QString r, parsedResponseList) {
    if (r.contains("ERROR")) {
      m_solarized->setTextColor(&r, SOLAR_RED);
    } else {
      m_solarized->setTextColor(&r, SOLAR_BLUE);
    }
    response.append(r + "<br>");
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return preamble + response;
}

QString MainWindow::buildUnparsedResponse(QStringList cmdList, QStringList pmuResponseList) {
  QString preamble;
  QString response;
  if (ui->actionShow_PMU_Command->isChecked()) {
    preamble = QString("PMU command: %1").arg(cmdList.at(0));
    if (cmdList.length() > 1) {
      preamble += QString(" [plus %1 others...]").arg(cmdList.length() - 1);
    }
    preamble += "<br>";
    m_solarized->setTextColor(&preamble, SOLAR_BASE_01);
  }
  // no parser available, flatten responses
  foreach (QString s, pmuResponseList) {
    response.append(s);
  }
  if (response.startsWith("OK")) {
    m_solarized->setTextColor(&response, SOLAR_GREEN);
  } else if (response.startsWith("ERROR")) {
    response = m_cmdHelper->parseError(response);
    m_solarized->setTextColor(&response, SOLAR_RED);
  } else {
    m_solarized->setTextColor(&response, SOLAR_VIOLET);
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return preamble + response;
}

QString MainWindow::buildBasicCommandResponse(QStringList pmuResponseList) {
  QString response;
  // not a helper command, flatten responses
  foreach (QString s, pmuResponseList) {
    response.append(s);
  }
  if (response.startsWith("OK")) {
    m_solarized->setTextColor(&response, SOLAR_GREEN);
  } else if (response.startsWith("ERROR")) {
    response = m_cmdHelper->parseError(response);
    m_solarized->setTextColor(&response, SOLAR_RED);
  } else {
    m_solarized->setTextColor(&response, SOLAR_VIOLET);
  }
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

QString MainWindow::buildHelp(struct cmdEntry * cmdEntry) {
  QString response;
  QStringList helpList = cmdEntry->getHelp();
  foreach(QString h, helpList) {
    response.append(h + "<br>");
  }
  m_solarized->setTextColor(&response, SOLAR_CYAN);
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

QString MainWindow::buildAppHelp(void) {
  QString response;
  QStringList helpList = m_cmdHelper->help();
  foreach(QString h, helpList) {
    response.append(h + "<br>");
  }
  m_solarized->setTextColor(&response, SOLAR_BASE_01);
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

QString MainWindow::buildUserInputError(QStringList cmdList) {
  QString response = cmdList.at(0);
  m_solarized->setTextColor(&response, SOLAR_RED);
  if (response.endsWith("<br>") == false) {
    response.append("<br>");
  }
  return response;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
  QString userRequest;
  QString pmuResponse;
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
      pmuResponse = processUserRequest(&userRequest);
      ui->commandLine->clear();
      ui->outputFeed->insertHtml(buildPrompt() + userRequest + "<br>");
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
  this->setWindowTitle(QString("DLTerm: %1").arg(status));
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
                        "<p><small>Version 0.3.0</small></p>"
                        "<p><small>Build date 10/21/15</small></p>"
                        "<small>Copyright &copy; 2010-2015. All rights reserved.</small>"));
}
