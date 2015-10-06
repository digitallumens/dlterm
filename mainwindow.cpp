#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QScrollBar>
#include <QMessageBox>

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
  this->setWindowTitle("DLTerm (Disconnected)");
}

MainWindow::~MainWindow() {
  delete ui;
}

QString MainWindow::processUserRequest(QString *request) {
  QString cmd;
  QStringList argList;
  QStringList cmdList;
  QStringList pmuResponseList;
  QStringList parsedResponseList;
  QString response;
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
      response = cmdEntry->getHelp();
      m_solarized->setTextColor(request, SOLAR_YELLOW);
      m_solarized->setTextColor(&response, SOLAR_CYAN);
      return response;
    }
    // translate helper command
    cmdList = cmdEntry->buildCmd(argList);
    // command translator returned an error
    if (cmdList.at(0).startsWith("ERROR")) {
      response = cmdList.at(0);
      m_solarized->setTextColor(&response, SOLAR_RED);
      return response;
    }
    m_solarized->setTextColor(request, SOLAR_YELLOW);
  }
  // send commands & get responses
  m_interface->queryPmu(cmdList, &pmuResponseList);
  // parse responses
  if (cmdEntry == NULL) {
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
  } else if (cmdEntry->parseResponse == NULL) {
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
  } else {
    // use parser
    parsedResponseList = cmdEntry->parseResponse(pmuResponseList);
    // flatten
    foreach(QString r, parsedResponseList) {
      if (r.contains("ERROR")) {
        m_solarized->setTextColor(&r, SOLAR_RED);
      } else {
        m_solarized->setTextColor(&r, SOLAR_BLUE);
      }
      response += (r + "<br>");
    }
  }
  // an empty line between blocks adds clarity
  if (response.endsWith("<br>") == false) {
    response += "<br>";
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
  if (m_preferencesDialog->m_serialNumber == 0) {
    // open the preferences dialog so the user can input the missing info
    m_preferencesDialog->exec();
  } else {
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
  this->setWindowTitle("DLTerm (Disconnected)");
}

void MainWindow::on_connectionEstablished(void) {
  ui->actionConnect_Using_FTDI->setVisible(false);
  ui->actionConnect_Using_Telegesis->setVisible(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionPreferences->setDisabled(true);
  ui->commandLine->setPlaceholderText("Type a command here. Terminate by pressing ENTER.");
  this->setWindowTitle("DLTerm (Connected)");
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
                        "<p><small>Version 0.1.0</small></p>"
                        "<p><small>Build date 9/22/15</small></p>"
                        "<small>Copyright &copy; 2010-2014. All rights reserved.</small>"));
}
