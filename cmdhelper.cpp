#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>

struct dlCmd_t {
  const QString inputCmd;
  const QString outputCmd;
};

const struct dlCmd_t dlCmdList[] = {
  { .inputCmd = "get firmwareVersion", .outputCmd = "G0000" },
  { .inputCmd = "get productCode", .outputCmd = "G0001" },
  { .inputCmd = "set productCode", .outputCmd = "S0001" },
  { .inputCmd = "get serialNumber", .outputCmd = "G0002" },
  { .inputCmd = "set serialNumber", .outputCmd = "S0002" },
  { .inputCmd = "get unixTime", .outputCmd = "G0003" },
  { .inputCmd = "set unixTime", .outputCmd = "S0003" },
  { .inputCmd = "get temperature", .outputCmd = "G0004" },
  { .inputCmd = "get lightManualLevel", .outputCmd = "G0005" },
  { .inputCmd = "set lightManualLevel", .outputCmd = "S0005" },
  { .inputCmd = "get lightActiveLevel", .outputCmd = "G0006" },
  { .inputCmd = "get lightInactiveLevel", .outputCmd = "G0007" },
  { .inputCmd = "get lightOverrideActiveLevel", .outputCmd = "G0008" },
  { .inputCmd = "set lightOverrideActiveLevel", .outputCmd = "S0008" },
  { .inputCmd = "get lightOverrideInactiveLevel", .outputCmd = "G0009" },
  { .inputCmd = "set lightOverrideInactiveLevel", .outputCmd = "S0009" },
  { .inputCmd = "get sensorDelayTime", .outputCmd = "G000A" },
  { .inputCmd = "get sensorOverrideDelayTime", .outputCmd = "G000B" },
  { .inputCmd = "set sensorOverrideDelayTime", .outputCmd = "S000B" },
  { .inputCmd = "get powerupTime", .outputCmd = "G000C" },
  { .inputCmd = "get usageActiveTime", .outputCmd = "G000D" },
  { .inputCmd = "get usageInactiveTime", .outputCmd = "G000E" },
  { .inputCmd = "get usagePermActiveTime", .outputCmd = "G000F" },
  { .inputCmd = "get usagePermInactiveTime", .outputCmd = "G0010" },
  { .inputCmd = "get usageWh", .outputCmd = "G0011" },
  { .inputCmd = "get usagePerm", .outputCmd = "G0012" },
  { .inputCmd = "get usageSenorEvents", .outputCmd = "G0013" },
  { .inputCmd = "get usagePermSensorEvents", .outputCmd = "G0014" },
  { .inputCmd = "get numberOfLogEntries", .outputCmd = "G0015" },
  { .inputCmd = "get configCalibrationP0", .outputCmd = "G0016" },
  { .inputCmd = "set configCalibrationP0", .outputCmd = "S0016" },
  { .inputCmd = "get configCalibrationP1", .outputCmd = "G0017" },
  { .inputCmd = "set configCalibrationP1", .outputCmd = "S0017" },
  { .inputCmd = "get configCalibrationP2", .outputCmd = "G0018" },
  { .inputCmd = "set configCalibrationP2", .outputCmd = "S0018" },
  { .inputCmd = "get configCalibrationP3", .outputCmd = "G0019" },
  { .inputCmd = "set configCalibrationP3", .outputCmd = "S0019" },
  { .inputCmd = "get buildTime", .outputCmd = "G001A" },
  { .inputCmd = "set buildTime", .outputCmd = "S001A" },
#if 0
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
  { .inputCmd = "", .outputCmd = "" },
#endif
};

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // build the command dictionary
  for (quint32 i = 0; i < sizeof(dlCmdList) / sizeof(dlCmdList[0]); i++) {
    keywordList << dlCmdList[i].inputCmd;
  }
  cmdCompleter = new QCompleter(keywordList, this);
  cmdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  cmdCompleter->popup()->installEventFilter(this);
}

bool cmdHelper::eventFilter(QObject *target, QEvent *event) {
  // just close the completer lister when enter/return is pressed
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter)) {
      cmdCompleter->popup()->close();
      return true;
    }
  }
  return QObject::eventFilter(target, event);
}
