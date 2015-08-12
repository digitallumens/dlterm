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
  ui->lineEdit->setCompleter(m_cmdHelper->cmdCompleter);
  // catch command events
  ui->lineEdit->installEventFilter(this);
}

MainWindow::~MainWindow() {
  delete ui;
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
  QString timestamp;
  QString cmdRequest;
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
      // process the command
      if (ui->actionShow_Timestamp->isChecked()) {
        timestamp = QDate::currentDate().toString(Qt::ISODate) + " " + QTime::currentTime().toString(Qt::ISODate);
        cmdRequest = timestamp + " > " + cmdRequest;
      } else {
        cmdRequest = " > " + cmdRequest;
      }
      ui->plainTextEdit->appendPlainText(cmdRequest);      
      ui->plainTextEdit->appendPlainText(m_cmdHelper->m_cmds.at(0)->m_parsePmuResponse("02010A0F070B"));
      ui->lineEdit->clear();
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
