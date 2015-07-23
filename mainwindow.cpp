#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

struct completerDict_t {
  const QString commands;
  const QString options;
};

struct completerDict_t dictReadOnlyRegs = { .commands = "get", .options = "firmwareVersion,temperature" };
struct completerDict_t dictReadWriteRegs = { .commands = "get,set", .options = "productCode,serialNumber,unixTime,lightManualLevel" };
struct completerDict_t dictReboot = { .commands = "reboot", .options = "dla,lightbar,batteryBackup,wirelessCard" };
struct completerDict_t dictReset = { .commands = "reset", .options = "usage,log,oldLog,eeprom,eepromToDefault" };
QStringList keywordList;
QCompleter *completer;

void addToCompleterDictionary(struct completerDict_t * dict) {
  QStringList commandsList;
  QStringList optionsList;
  QStringList::const_iterator commandsIter;
  QStringList::const_iterator optionsIter;
  commandsList = dict->commands.split(",");
  optionsList = dict->options.split(",");
  for (commandsIter = commandsList.constBegin(); commandsIter != commandsList.constEnd(); ++commandsIter) {
    for (optionsIter = optionsList.constBegin(); optionsIter != optionsList.constEnd(); ++optionsIter) {
      keywordList << (QString)(*commandsIter).toLocal8Bit().constData() + " " + (QString)(*optionsIter).toLocal8Bit().constData();
    }
  }
}

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
  addToCompleterDictionary(&dictReadOnlyRegs);
  addToCompleterDictionary(&dictReadWriteRegs);
  addToCompleterDictionary(&dictReboot);
  addToCompleterDictionary(&dictReset);
  completer = new QCompleter(keywordList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->popup()->installEventFilter(this);
  ui->lineEdit->setCompleter(completer);
  connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(custom_on_lineEdit_returnPressed()));
}

void MainWindow::custom_on_lineEdit_returnPressed() {
  // eat leading whitespace
  QString cmdRequest = ui->lineEdit->text().simplified();
  // discard empty commands
  if (cmdRequest == "") {
    return;
  }
  if (completer->currentCompletion() != "") {
    //return;
  }
  qDebug() << "got a command";
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

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_Return) {
      qDebug() << "intercepted return";
      completer->popup()->close();
      return true;
    }
  }
  return QObject::eventFilter(target, event);
}
