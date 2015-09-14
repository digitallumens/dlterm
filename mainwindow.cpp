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

void MainWindow::processUserRequest(QString *request, QString *response) {
  QString pmuCmd;
  QString pmuResponse;
  QStringList pmuResponseList;
  // find the associated helper entry
  struct pmu *pmu = m_cmdHelper->m_cmdTable[*request];
  if (pmu != NULL) {
    // translate helper command
    pmuCmd = pmu->cmd.at(0);
    m_solarized->setColor(request, SOLAR_YELLOW);
  } else {
    // not a helper command
    pmuCmd = *request;
    m_solarized->setColor(request, SOLAR_BASE_01);
  }
  // figure out the length NOT including the space
  int len = pmuCmd.length();
  int i = pmuCmd.indexOf(' ');
  if (i != -1) {
    len = pmuCmd.left(i).length();
  }
  // send command
  if (m_pmuUSB->issueCommand(pmuCmd, pmuResponse, len) != DLLIB_SUCCESS) {
    // error response
    *response = m_cmdHelper->m_errorResponses[pmuResponse];
    m_solarized->setColor(response, SOLAR_RED);
  } else if (pmu != NULL) {
    if (pmu->parser != NULL) {
      // parse response
      pmuResponseList << pmuResponse;
      *response = pmu->parser(pmuResponseList);
      m_solarized->setColor(response, SOLAR_BLUE);
    } else {
      // no parser available
      *response = pmuResponse;
      m_solarized->setColor(response, SOLAR_VIOLET);
    }
  } else {
    // not a helper command
    *response = pmuResponse;
    m_solarized->setColor(response, SOLAR_VIOLET);
  }
}

QString MainWindow::formatRequest(QString request) {
  QString prompt;
  QString timestamp;
  if (ui->actionShow_Timestamp->isChecked()) {
    timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
    prompt = timestamp + " > ";
  } else {
    prompt = " > ";
  }
  m_solarized->setColor(&prompt, SOLAR_BASE_01);
}

QString MainWindow::formatResponse(QString response) {
  QString responseHTML;
  responseHTML =
  return responseHTML;
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
      processUserRequest(&userRequest, &pmuResponse);
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
