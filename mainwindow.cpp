#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dllib.h"
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cmdHelper(new cmdHelper::cmdHelper),
  m_cmdHistory(new cmdHistory::cmdHistory) {
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

QString MainWindow::sendPmuCommand(QString cmd) {
  QString response;
  // figure out the length NOT including the space
  int len = cmd.length();
  int i = cmd.indexOf(' ');
  if (i != -1) {
    len = cmd.left(i).length();
  }
  DLResult ret = m_pmuUSB->issueCommand(cmd, response, len);
  if (ret != DLLIB_SUCCESS) {
    // parse error response
    response = m_cmdHelper->m_errorResponses.value(response);
  }
  return response;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
  QString timestamp;
  QString cmdRequest;
  QString cmdResponse;
  struct pmu * pmu;
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    switch (keyEvent->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
      // ignore requests until a connection is established
      if (ui->actionUseFTDICable && (m_pmuUSB == NULL)) {
        ui->commandLine->clear();
        break;
      }
      // enter a new command
      cmdRequest = ui->commandLine->text().simplified();
      if (cmdRequest.isEmpty()) {
        break;
      }
      // append new command to history
      m_cmdHistory->append(cmdRequest);
      // find the associated helper entry
      pmu = m_cmdHelper->m_cmdTable[cmdRequest];
      // send the command
      if (pmu != NULL) {
        cmdResponse = sendPmuCommand(pmu->cmd);
        if (pmu->parser != NULL) {
          cmdResponse = pmu->parser(cmdResponse);
        }
      } else {
        cmdResponse = sendPmuCommand(cmdRequest);
      }
      // print results
      ui->commandLine->clear();
      if (ui->actionShow_Timestamp->isChecked()) {
        timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
        cmdRequest = timestamp + " > " + cmdRequest;
      } else {
        cmdRequest = " > " + cmdRequest;
      }
      ui->outputFeed->appendPlainText(cmdRequest);
      ui->outputFeed->appendPlainText(cmdResponse);
      break;
    case Qt::Key_Tab:
      if (ui->commandLine->cursorPosition() != m_cmdHelper->getCurrentCompletionLength()) {
        // accept current completion
        ui->commandLine->end(false);
      } else {
        // next completion
        ui->commandLine->setText(m_cmdHelper->getNextCompletion());
      }
      break;
    case Qt::Key_Up:
      // scroll back through command history
      ui->commandLine->setText(m_cmdHistory->scrollBack());
      break;
    case Qt::Key_Down:
      // scroll forward through command history
      ui->commandLine->setText(m_cmdHistory->scrollForward());
      break;
    case Qt::Key_Left:
      // SHIFT + LEFT clears the command line
      if (keyEvent->modifiers() & Qt::ShiftModifier) {
        ui->commandLine->clear();
      }
      break;
    case Qt::Key_Home:
      // move cursor to start of line
      ui->commandLine->home(false);
      break;
    case Qt::Key_End:
      // move cursor to end of line
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
