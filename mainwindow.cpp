#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cmdHelper(new cmdHelper::cmdHelper),
  m_cmdHistory(new cmdHistory::cmdHistory) {
  ui->setupUi(this);
  // configure GUI widgets
  ui->actionDisconnect->setVisible(false);
  // configure autocomplete
  ui->lineEdit->setCompleter(m_cmdHelper->m_cmdCompleter);
  // catch command events
  ui->lineEdit->installEventFilter(this);
  // disable tab focus policy
  ui->plainTextEdit->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow() {
  delete ui;
}

QString sendPmuCommand(QString pmuCmd) {
  // todo
  if (pmuCmd == "G0000") {
    return "G0000: 02010A0F070B";
  } else {
    return "FFFF";
  }
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
      // enter a new command
      cmdRequest = ui->lineEdit->text().simplified();
      if (cmdRequest.isEmpty()) {
        break;
      }
      // append new command to history
      m_cmdHistory->append(cmdRequest);
      // find the associated helper entry
      pmu = m_cmdHelper->cmdMap[cmdRequest];
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
      ui->lineEdit->clear();
      if (ui->actionShow_Timestamp->isChecked()) {
        timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
        cmdRequest = timestamp + " > " + cmdRequest;
      } else {
        cmdRequest = " > " + cmdRequest;
      }
      ui->plainTextEdit->appendPlainText(cmdRequest);
      ui->plainTextEdit->appendPlainText(cmdResponse);
      break;
    case Qt::Key_Tab:
      if (ui->lineEdit->cursorPosition() != m_cmdHelper->getCurrentCompletionLength()) {
        // accept current completion
        ui->lineEdit->end(false);
      } else {
        // next completion
        ui->lineEdit->setText(m_cmdHelper->getNextCompletion());
      }
      break;
    case Qt::Key_Up:
      // scroll back through command history
      ui->lineEdit->setText(m_cmdHistory->scrollBack());
      break;
    case Qt::Key_Down:
      // scroll forward through command history
      ui->lineEdit->setText(m_cmdHistory->scrollForward());
      break;
    case Qt::Key_Left:
      // SHIFT + LEFT clears the command line
      if (keyEvent->modifiers() & Qt::ShiftModifier) {
        ui->lineEdit->clear();
      }
      break;
    case Qt::Key_Home:
      // move cursor to start of line
      ui->lineEdit->home(false);
      break;
    case Qt::Key_End:
      // move cursor to end of line
      ui->lineEdit->end(false);
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
  ui->actionConnect->setVisible(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(true);
  ui->actionUseTelegesisAdapter->setDisabled(true);
  ui->actionConfigure->setDisabled(true);
}

void MainWindow::on_actionDisconnect_triggered() {
  ui->actionDisconnect->setVisible(false);
  ui->actionConnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(false);
  ui->actionUseTelegesisAdapter->setDisabled(false);
  ui->actionConfigure->setDisabled(false);
}

void MainWindow::on_actionConfigure_triggered() {
  qDebug() << "configure all the things!";
}

void MainWindow::on_actionClear_Output_triggered() {
  ui->plainTextEdit->clear();
}
