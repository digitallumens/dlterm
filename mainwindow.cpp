#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dllib.h"
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cmdHelper(new cmdHelper::cmdHelper),
  m_cmdHistory(new cmdHistory::cmdHistory),
  m_solarized(new solarized::solarized) {
  ui->setupUi(this);
  // remove the ugly focus border
  ui->commandLine->setAttribute(Qt::WA_MacShowFocusRect, 0);
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
  // create the discovery agent
  m_discoveryAgent = new DiscoveryAgent();
  Q_CHECK_PTR(m_discoveryAgent);
  connect(m_discoveryAgent, SIGNAL(signalPMUDiscovered(PMU*)), this, SLOT(slotPMUDiscovered(PMU*)));
  this->setWindowTitle("DLTerm (Disconnected)");
}

MainWindow::~MainWindow() {
  delete ui;
}

QString MainWindow::queryPmu(QStringList cmdList, QStringList *responseList) {
  QString response;
  foreach (const QString &cmd, cmdList) {
    // figure out the length NOT including the space
    int len = cmd.length();
    int i = cmd.indexOf(' ');
    if (i != -1) {
      len = cmd.left(i).length();
    }
    if (m_pmuUSB->issueCommand(cmd, response, len) != DLLIB_SUCCESS) {
      // return error
      return response;
    } else {
      responseList->append(response);
    }
  }
  return NULL;
}

QString MainWindow::processUserRequest(QString *request) {
  QString cmd;
  QStringList argList;
  QStringList cmdList;
  QStringList responseList;
  QString errorResponse;
  QString response;
  // request format "verb object argList"
  // ex: set serialNumber 0400DEAD
  argList = request->split(" ");
  cmd = argList.at(0) + " " + argList.at(1);
  argList.removeFirst();
  argList.removeFirst();
  // find the associated helper entry
  struct pmu *pmu = m_cmdHelper->m_cmdTable[cmd];
  if (pmu == NULL) {
    // not a helper command
    cmdList << *request;
    m_solarized->setColor(request, SOLAR_BASE_01);
  } else {
    // translate helper command
    cmdList = pmu->cmd(argList);
    m_solarized->setColor(request, SOLAR_YELLOW);
  }
  // send commands & get responses
  errorResponse = queryPmu(cmdList, &responseList);
  // parse responses
  if (errorResponse != NULL) {
    // translate error response
    response = m_cmdHelper->m_errorResponses[errorResponse];
    m_solarized->setColor(&response, SOLAR_RED);
  } else if (pmu == NULL) {
    // not a helper command, flatten responses
    foreach (QString s, responseList) {
      response.append(s);
    }
    if (response.contains("OK")) {
      m_solarized->setColor(&response, SOLAR_GREEN);
    } else {
      m_solarized->setColor(&response, SOLAR_VIOLET);
    }
  } else if (pmu->parser == NULL) {
    // no helper parser available, flatten responses
    foreach (QString s, responseList) {
      response.append(s);
    }
    if (response.contains("OK")) {
      m_solarized->setColor(&response, SOLAR_GREEN);
    } else {
      m_solarized->setColor(&response, SOLAR_VIOLET);
    }
  } else {
    // use helper parser
    response = pmu->parser(responseList);
    m_solarized->setColor(&response, SOLAR_BLUE);
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
  m_solarized->setColor(&prompt, SOLAR_BASE_01);
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
      if (userRequest.isEmpty() || m_discoveryAgent->m_pmuList.isEmpty()) {
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
      ui->commandLine->end(false);
      break;
    case Qt::Key_Down:
      ui->commandLine->setText(m_cmdHistory->scrollForward());
      ui->commandLine->end(false);
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

void MainWindow::on_actionUseFTDICable_triggered() {
  ui->actionUseTelegesisAdapter->setChecked(false);
}

void MainWindow::on_actionUseTelegesisAdapter_triggered() {
  ui->actionUseFTDICable->setChecked(false);
}

void MainWindow::on_actionConnect_triggered() {
  // establish a connection
  if (ui->actionUseFTDICable) {
    m_discoveryAgent->clearLists();
    do {
      m_discoveryAgent->discoverPMU_usbs();
      MSLEEP(500);
      QApplication::processEvents();
    }
    while (m_discoveryAgent->m_pmuList.isEmpty());
  } else if (ui->actionUseTelegesisAdapter) {
    // todo
  }
}

void MainWindow::on_actionDisconnect_triggered() {
  ui->actionDisconnect->setVisible(false);
  ui->actionConnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(false);
  ui->actionUseTelegesisAdapter->setDisabled(false);
  ui->actionConfigure->setDisabled(false);
  ui->commandLine->setPlaceholderText("Press ⌘K to establish a connection.");
  this->setWindowTitle("DLTerm (Disconnected)");
}

void MainWindow::on_actionConfigure_triggered() {
  qDebug() << "configure all the things!";
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

void MainWindow::slotPMUDiscovered(PMU* pmu) {
  m_pmuUSB = qobject_cast<PMU_USB*>(pmu);
  if (!m_pmuUSB) {
    return;
  }
  // update controls
  ui->actionConnect->setVisible(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(true);
  ui->actionUseTelegesisAdapter->setDisabled(true);
  ui->actionConfigure->setDisabled(true);
  ui->commandLine->setPlaceholderText("Type a command here. Terminate by pressing ENTER.");
  this->setWindowTitle("DLTerm (Connected)");
}
