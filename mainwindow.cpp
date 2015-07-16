#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

QString keywordsReadOnly = "firmwareVersion,temperature";
QString keywordsReadWrite = "productCode,serialNumber,unixTime,lightManualLevel";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  uiInit();
  completerInit();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::uiInit()
{
  ui->actionDisconnect->setVisible(false);
}

void MainWindow::completerInit()
{
  QStringList list;
  QStringList::const_iterator constIterator;
  // build a dictionary of keywords
  list = keywordsReadOnly.split(",");
  for (constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) {
    keywordList << (QString)"get " + (QString)(*constIterator).toLocal8Bit().constData();
  }
  list = keywordsReadWrite.split(",");
  for (constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) {
    keywordList << (QString)"get " + (QString)(*constIterator).toLocal8Bit().constData();
    keywordList << (QString)"set " + (QString)(*constIterator).toLocal8Bit().constData();
  }
  completer = new QCompleter(keywordList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  ui->lineEdit->setCompleter(completer);
}

void MainWindow::on_lineEdit_returnPressed()
{
  ui->plainTextEdit->appendPlainText(ui->lineEdit->text());
  ui->lineEdit->clear();
}

void MainWindow::on_actionUseFTDICable_triggered()
{
  ui->actionUseTelegesisAdapter->setChecked(false);
}

void MainWindow::on_actionUseTelegesisAdapter_triggered()
{
  ui->actionUseFTDICable->setChecked(false);
}

void MainWindow::on_actionConnect_triggered()
{
  ui->actionConnect->setVisible(false);
  ui->actionDisconnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(true);
  ui->actionUseTelegesisAdapter->setDisabled(true);
  ui->actionConfigure->setDisabled(true);
}

void MainWindow::on_actionDisconnect_triggered()
{
  ui->actionDisconnect->setVisible(false);
  ui->actionConnect->setVisible(true);
  ui->actionUseFTDICable->setDisabled(false);
  ui->actionUseTelegesisAdapter->setDisabled(false);
  ui->actionConfigure->setDisabled(false);
}

void MainWindow::on_actionConfigure_triggered()
{
  qDebug() << "configure all the things!";
}
