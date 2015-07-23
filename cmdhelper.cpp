#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>

struct completerDict_t {
  const QString commands;
  const QString options;
};

struct completerDict_t dictReadOnlyRegs = { .commands = "get", .options = "firmwareVersion,temperature" };
struct completerDict_t dictReadWriteRegs = { .commands = "get,set", .options = "productCode,serialNumber,unixTime,lightManualLevel" };
struct completerDict_t dictReboot = { .commands = "reboot", .options = "dla,lightbar,batteryBackup,wirelessCard" };
struct completerDict_t dictReset = { .commands = "reset", .options = "usage,log,oldLog,eeprom,eepromToDefault" };

static void addToCompleterDictionary(QStringList * keywordList, struct completerDict_t * dict) {
  QStringList commandsList;
  QStringList optionsList;
  QStringList::const_iterator commandsIter;
  QStringList::const_iterator optionsIter;
  commandsList = dict->commands.split(",");
  optionsList = dict->options.split(",");
  for (commandsIter = commandsList.constBegin(); commandsIter != commandsList.constEnd(); ++commandsIter) {
    for (optionsIter = optionsList.constBegin(); optionsIter != optionsList.constEnd(); ++optionsIter) {
      *keywordList << (QString)(*commandsIter).toLocal8Bit().constData() + " " + (QString)(*optionsIter).toLocal8Bit().constData();
    }
  }
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  addToCompleterDictionary(&keywordList, &dictReadOnlyRegs);
  addToCompleterDictionary(&keywordList, &dictReadWriteRegs);
  addToCompleterDictionary(&keywordList, &dictReboot);
  addToCompleterDictionary(&keywordList, &dictReset);
  completer = new QCompleter(keywordList, this);
  completer->setCaseSensitivity(Qt::CaseInsensitive);
  completer->popup()->installEventFilter(this);
}

bool cmdHelper::eventFilter(QObject *target, QEvent *event) {
  // just close the completer lister when enter/return is pressed
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter)) {
      completer->popup()->close();
      return true;
    }
  }
  return QObject::eventFilter(target, event);
}
