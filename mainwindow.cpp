#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setupApp();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::setupApp() {
  // configure GUI widgets
  ui->actionDisconnect->setVisible(false);
  // configure autocomplete
  m_cmdHelper = new cmdHelper(this);
  ui->lineEdit->setCompleter(m_cmdHelper->completer);
  // connect signals to callbacks
  connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(custom_on_lineEdit_returnPressed()));
}

void MainWindow::custom_on_lineEdit_returnPressed() {
  // eat leading whitespace
  QString cmdRequest = ui->lineEdit->text().simplified();
  // discard empty commands
  if (cmdRequest == "") {
    return;
  }
  ui->plainTextEdit->appendPlainText(cmdRequest);
  ui->lineEdit->clear();
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
