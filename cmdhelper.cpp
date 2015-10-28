#include "cmdhelper.h"
#include "dllib.h"
#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

quint16 logIndex = 0;

QString toYDHMS(QString timeInSec) {
  bool ok;
  quint32 ulTimeInSec = timeInSec.toULong(&ok, 16);
  QString outTime;
  if (ulTimeInSec == 0) {
    return "0S";
  }
  if (ulTimeInSec / 31536000) {
    outTime += QString("%1Y:").arg(ulTimeInSec / 31536000);
    ulTimeInSec %= 31536000;
  }
  if (ulTimeInSec / 86400) {
    outTime += QString("%1D:").arg(ulTimeInSec / 86400);
    ulTimeInSec %= 86400;
  }
  if (ulTimeInSec / 3600) {
    outTime += QString("%1H:").arg(ulTimeInSec / 3600);
    ulTimeInSec %= 3600;
  }
  if (ulTimeInSec / 60) {
    outTime += QString("%1M:").arg(ulTimeInSec / 60);
    ulTimeInSec %= 60;
  }
  if (ulTimeInSec) {
    outTime += QString("%1S").arg(ulTimeInSec);
  }
  if (outTime.endsWith(":")) {
    outTime.truncate(outTime.length() - 1);
  }
  return outTime;
}

/*** PMU register commands ***/
QStringList get_firmwareVersion(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList << "G0000");
  if (responseList.at(0).startsWith("ERROR")) {
    return responseList;
  } else {
    bool ok;
    qulonglong verInt = responseList.at(0).toULongLong(&ok, 16);
    // format verMajor.verMinor.verBuild (buildMonth/buildDay/BuildYear)
    return QStringList() << (QString("+%1.%2.%3 (%5/%6/%4)").arg((verInt >> 40) & 0xFF).arg((verInt >> 32) & 0xFF).arg((verInt >> 24) & 0xFF).arg((verInt >> 16) & 0xFF).arg((verInt >> 8) & 0xFF).arg(verInt & 0xFF));
  }
}

QStringList get_productCode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0001");
}

QStringList set_productCode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0001 %1").arg(argList.at(0)));
}

QStringList get_serialNumber(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0002");
}

QStringList set_serialNumber(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0002 %1").arg(argList.at(0)));
}

QStringList get_unixTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0003");
}

QStringList set_unixTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0003 %1").arg(argList.at(0));
}

QStringList get_temperature(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList() << "G0004");
  if (responseList.at(0).startsWith("ERROR")) {
    return responseList;
  } else {
    bool ok;
    quint16 tInt = responseList.at(0).toUShort(&ok, 16);
    float tFloat = (tInt / 128);
    return QStringList() << QString("+%1 C").arg(tFloat);
  }
}

QStringList get_lightLevel(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G001C" // current level
          << "G0005" // light manual level
          << "G0006" // light active level
          << "G0007" // light inactive level
          << "G0008" // light overrirde active level
          << "G0009"; // light override inactive level
  responseList = iface->queryPmu(cmdList);
  return QStringList << QString("+Current level: %1").arg(responseList.at(0))
                     << QString("+Manual level: %1").arg(responseList.at(1))
                     << QString("+Active level: %1").arg(responseList.at(2))
                     << QString("+Inactive level: %1").arg(responseList.at(3))
                     << QString("+Override active level: %1").arg(responseList.at(4))
                     << QString("+Override inactive level: %1").arg(responseList.at(5));
}

QStringList set_lightManualLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0005 %1").arg(argList.at(0)));
}

QStringList set_lightOverrideActiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0008 %1").arg(argList.at(0)));
}

QStringList set_lightOverrideInactiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0009 %1").arg(argList.at(0)));
}

QStringList get_sensorDelayTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G000A");
}

QStringList get_sensorOverrideDelayTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G000B");
}

QStringList set_sensorOverrideDelayTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S000B %1").arg(argList.at(0)));
}

QStringList get_upTime(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList() << "G000C");
  return QStringList() << QString("+%1").arg(toYDHMS(responseList.at(0)));
}

QStringList get_usage(QStringList argList, interface *iface) {
  QStringList cmdlist;
  QStringList responseList;
  bool ok;
  (void) argList;
  cmdList << "G000C" // uptime
          << "G000D" // active seconds
          << "G000E" // inactive seconds
          << "G000F" // perm active seconds
          << "G0010" // perm inactive seconds
          << "G0011" // Wh
          << "G0012" // perm Wh
          << "G0013" // sensor events
          << "G0014"; // perm sensor events
  responseList = iface->queryPmu(cmdlist);
  return QStringList() << QString("+Up time: %1").arg(toYDHMS(responseList.at(0)))
                       << QString("+Active time: %1").arg(toYDHMS(responseList.at(1)))
                       << QString("+Inactive time: %1").arg(toYDHMS(responseList.at(2)))
                       << QString("+Perm active time: %1").arg(toYDHMS(responseList.at(3)))
                       << QString("+Perm inactive time: %1").arg(toYDHMS(responseList.at(4)))
                       << QString("+Power: %1 Wh").arg(responseList.at(5).toUShort(&ok, 16))
                       << QString("+Perm power: %1 Wh").arg(responseList.at(6).toUShort(&ok, 16))
                       << QString("+Sensor events: %1").arg(responseList.at(7).toUShort(&ok, 16))
                       << QString("+Perm sensor events: %1").arg(responseList.at(8).toUShort(&ok, 16));
}

QStringList get_numLogEntries(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0015");
}

QStringList get_configCalibration(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0016" // P0
          << "G0017" // P1
          << "G0018" // P2
          << "G0019"; // P3
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+P0: %1").arg(pmuReponseList.at(0))
                       << QString("+P1: %1").arg(pmuReponseList.at(1))
                       << QString("+P2: %1").arg(pmuReponseList.at(2))
                       << QString("+P3: %1").arg(pmuReponseList.at(3));
}

QStringList set_configCalibrationP0(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0016 %1").arg(argList.at(0)));
}

QStringList set_configCalibrationP1(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0017 %1").arg(argList.at(0)));
}

QStringList set_configCalibrationP2(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0018 %1").arg(argList.at(0)));
}

QStringList set_configCalibrationP3(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0019 %1").arg(argList.at(0)));
}

QStringList get_buildTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G001A");
}

QStringList set_buildTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S001A %1").arg(argList.at(0)));
}

QStringList get_sensorTimeoutCountdown(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G001B");
}

QStringList get_currentLightLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G001C");
}

QStringList get_safeMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G001D");
}

QStringList get_lightBarSelect(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G001E");
}

QStringList set_lightBarSelect(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S001E %1").arg(argList.at(0)));
}

QStringList get_powerConsumption(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList() << "G001F");
  if (responseList.at(0).startsWith("ERROR")) {
    return responseList;
  } else {
    bool ok;
    quint16 powerInt = responseList.at(0).toUShort(&ok, 16);
    return QStringList() << QString("+%1 mW").arg(powerInt);
  }
}

QStringList get_wirelessDataAggregator(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0020");
}

QStringList set_wirelessDataAggregator(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0020 %1").arg(argList.at(0)));
}

QStringList get_resetUsageTimestamp(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0021");
}

QStringList get_pwmPeriodRegister(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0022");
}

QStringList set_pwmPeriodRegister(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0022 %1").arg(argList.at(0)));
}

QStringList get_analogSensorValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0023");
}

QStringList get_analogReportingHysteresis(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0024");
}

QStringList get_zone(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0025");
}

QStringList set_zone(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0025 %1").arg(argList.at(0)));
}

QStringList get_lightTemporaryActiveLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0026");
}

QStringList set_lightTemporaryActiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0026 %1").arg(argList.at(0)));
}

QStringList get_lightTemporaryInactiveLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0027");
}

QStringList set_lightTemporaryInactiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0027 %1").arg(argList.at(0)));
}

QStringList get_sensorTemporaryDelayTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0028");
}

QStringList set_sensorTemporaryDealyTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0028 %1").arg(argList.at(0)));
}

QStringList get_temporaryOverrideTimeout(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0029");
}

QStringList set_temporaryOverrideTiemout(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0029 %1").arg(argList.at(0)));
}

QStringList get_setRemoteState(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G002A");
}

QStringList set_setRemoteState(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S002A %1").arg(argList.at(0)));
}

QStringList get_remoteStateDelayTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G002B");
}

QStringList set_remoteStateDelayTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S002B %1").arg(argList.at(0)));
}

QStringList get_remoteSecondsCountdown(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G002C");
}

QStringList get_minimumDimmingValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G002D");
}

QStringList get_powerCalibration(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << QString("G002E") // a0
          << QString("G002F") // b0
          << QString("G0030") // c0
          << QString("G0031") // ma
          << QString("G0032") // mb
          << QString("G0033") // mc
          << QString("G0034") // poff
          << QString("G0035") // pon
          << QString("G0036"); // t0
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+A0: %1").arg(responseList.at(0))
                       << QString("+B0: %1").arg(responseList.at(1))
                       << QString("+C0: %1").arg(responseList.at(2))
                       << QString("+MA: %1").arg(responseList.at(3))
                       << QString("+MB: %1").arg(responseList.at(4))
                       << QString("+MC: %1").arg(responseList.at(5))
                       << QString("+POff: %1").arg(responseList.at(6))
                       << QString("+POn: %1").arg(responseList.at(7))
                       << QString("+T0: %1").arg(responseList.at(8));
}

QStringList set_powerCalibrationA0(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S002E %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationB0(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S002F %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationC0(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0030 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationMA(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0031 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationMB(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0032 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationMC(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0033 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationPOff(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0034 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationPOn(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0035 %1").arg(argList.at(0)));
}

QStringList set_powerCalibrationT0(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0036 %1").arg(argList.at(0)));
}

QStringList get_powerEstimatorTemperatureOverride(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0037");
}

QStringList set_powerEstimatorTemperatureOverride(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0037 %1").arg(argList.at(0)));
}

QStringList get_cachedTemperatureValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0038");
}

QStringList get_eepromSize(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0039");
}

QStringList get_hardwareRevision(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G003A");
}

QStringList get_wirelessConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G003B" // pan id
          << "G003C" // channel mask
          << "G003D" // short address
          << "G003E" // role
          << "G003F" // watchdog hold
          << "G0040" // watchdog period
          << "G0073"; // network key
  responseList = iface->queryPmu(cmdList);
  bool ok;
  unsigned long long panid = responseList.at(0).toULongLong(&ok, 16);
  unsigned long chmask = responseList.at(1).toULong(&ok, 16);
  unsigned group;
  unsigned freq;
  bool encrypted;
  LRNetwork+groupAndFreqFromPanidAndChmask(panid, chmask, &group, &freq, &encrypted);
  QString netId = LRNetwork+nwidFromGroupAndFreq(group, freq, encrypted);
  QStringList parsedResponse;
  return QStringList() << QString("+Network ID: %1").arg(netId)
                       << QString("+Pan ID: %1").arg(responseList.at(0))
                       << QString("+Channel mask: %1").arg(responseList.at(1))
                       << QString("+Short address: %1").arg(responseList.at(2))
                       << QString("+Role: %1").arg(responseList.at(3))
                       << QString("+Watchdog hold: %1").arg(responseList.at(4))
                       << QString("+Watchdog period: %1").arg(responseList.at(5))
                       << QString("+Network key: %1").arg(responseList.at(6));
}

QStringList set_wirelessPanId(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S003B %1").arg(argList.at(0)));
}

QStringList set_wirelessChannelMask(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S003C %1").arg(argList.at(0)));
}

QStringList set_wirelessShortAddress(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S003D %1").arg(argList.at(0)));
}

QStringList set_wirelessRole(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S003E %1").arg(argList.at(0)));
}

QStringList set_wirelessWatchdogHold(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S003F %1").arg(argList.at(0)));
}

QStringList set_wirelessWatchdogPeriod(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0040 %1").arg(argList.at(0)));
}

QStringList set_wirelessNetworkKey(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0073 %1").arg(argList.at(0)));
}

QStringList get_firmwareCode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0041");
}

QStringList get_moduleFirmwareCode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0042");
}

QStringList get_maxTemperature(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0043" // observed temperature
          << "G0044"; // observed time
  responseList = iface->queryPmu(cmdList);
  QString temperature = responseList.at(0);
  QString time = responseList.at(1);
  time = (time.startsWith("ERROR")) ? time : toYDHMS(time);
  responseList->append(QString("+Temperature: %1").arg(temperature));
  responseList->append(QString("+Time: %1").arg(time));
}

QStringList get_overTemperatureConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0045" // low threshold
          << "G0046" // high threshold
          << "G0047"; // dimming limit
  responseList = iface(cmdList);
  return QStringList() << QString("+Low threshold: %1").arg(responseList.at(0))
                       << QString("+High threshold: %1").arg(responseList.at(1))
                       << QString("+Dimming limit: %1").arg(responseList.at(2));
}

QStringList set_overTemperatureThresholdLow(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0045 %1").arg(argList.at(0)));
}

QStringList get_overTemperatureThresholdHigh(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0046");
}

QStringList set_overTemperatureThresholdHigh(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0046 %1").arg(argList.at(0)));
}

QStringList set_overTemperatureDimmingLimit(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0047 %1").arg(argList.at(0)));
}

QStringList get_analogDimmingMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0048");
}

QStringList parse_get_analogDimmingMode(QStringList responseList) {
  QMap <QString, QString> analogDimmingModeDict;
  analogDimmingModeDict.insert("00", "Analog dimming off");
  analogDimmingModeDict.insert("01", "Analog dimming on");
  analogDimmingModeDict.insert("02", "Analog dimming on with ability to go full off");
  analogDimmingModeDict.insert("03", "Analog dimming using registers 54-56");
  analogDimmingModeDict.insert("04", "Analog dimming using registers 54-56 with full off support");
  analogDimmingModeDict.insert("05", "Ambient sensor dimming");
  return QStringList() << QString("+%1").arg(analogDimmingModeDict[responseList.at(0)]);
}

QStringList set_analogDimmingMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0048 %1").arg(argList.at(0)));
}

QStringList get_fixtureIdMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0049");
}

QStringList set_fixtureIdMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0049 %1").arg(argList.at(0)));
}

QStringList get_acFrequency(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004A");
}

QStringList get_sensorBits(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004B");
}

QStringList get_powerMeterCommand(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004C");
}

QStringList set_powerMeterCommand(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S004C %1").arg(argList.at(0)));
}

QStringList get_powerMeterRegister(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004D");
}

QStringList set_powerMeterRegister(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S004D %1").arg(argList.at(0)));
}

QStringList get_ambientTemperature(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004E");
}

QStringList get_lightSensorLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G004F");
}

QStringList get_sensorConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G004F" // sensor level
          << "G0050" // sensor 0 timeout
          << "G0051" // sensor 0 offset
          << "G0052" // sensor 1 timeout
          << "G0053"; // sensor 1 offset
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+Sensor level: %1").arg(responseList.at(0))
                       << QString("+Sensor 0 timeout: %1").arg(responseList.at(1))
                       << QString("+Sensor 0 offset: %1").arg(responseList.at(2))
                       << QString("+Sensor 1 timeout: %1").arg(responseList.at(3))
                       << QString("+Sensor 1 offset: %1").arg(responseList.at(4));
}

QStringList set_sensor0Timeout(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0050 %1").arg(argList.at(0)));
}

QStringList get_sensor0Offset(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0051");
}

QStringList set_sensor0Offset(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0051 %1").arg(argList.at(0)));
}

QStringList get_sensor1Timeout(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0052");
}

QStringList set_sensor1Timeout(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0052 %1").arg(argList.at(0)));
}

QStringList get_sensor1Offset(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0053");
}

QStringList set_sensor1Offset(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0053 %1").arg(argList.at(0)));
}

QStringList get_analogDimmingConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0054" // low value
          << "G0055" // high value
          << "G0056"; // off value
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+Low value: %1").arg(responseList.at(0))
                       << QString("+High value: %1").arg(responseList.at(1))
                       << QString("+Off value: %1").arg(responseList.at(2));
}

QStringList set_analogDimmingLowValue(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0054 %1").arg(argList.at(0)));
}

QStringList get_analogDimmingHighValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0055");
}

QStringList set_analogDimmingHighValue(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0055 %1").arg(argList.at(0)));
}

QStringList get_analogDimmingOffValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0056");
}

QStringList set_analogDimmingOffValue(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0056 %1").arg(argList.at(0)));
}

QStringList get_powerMeasurementMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0057");
}

QStringList set_powerMeasurementMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0057 %1").arg(argList.at(0)));
}

QStringList get_externalPowerMeter(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0058");
}

QStringList set_externalPowerMeter(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0058 %1").arg(argList.at(0)));
}

QStringList get_ambientSensorValue(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0059");
}

QStringList get_ambientConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0059" // sensor value
          << "G005A" // active level
          << "G005B" // inactive level
          << "G005C" // environmental gain
          << "G005D" // off hysteresis
          << "G005E" // on hysteresis
          << "G0069"; // divisor
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+Sensor value: %1").arg(responseList.at(0))
                       << QString("+Active level: %1").arg(responseList.at(1))
                       << QString("+Inactive level: %1").arg(responseList.at(2))
                       << QString("+Environmental gain: %1").arg(responseList.at(3))
                       << QString("+Off hysteresis: %1").arg(responseList.at(4))
                       << QString("+On hysteresis: %1").arg(responseList.at(5))
                       << QString("+Divisor: %1").arg(responseList.at(6));
}

QStringList set_ambientActiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S005A %1").arg(argList.at(0)));
}

QStringList get_ambientInactiveLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G005B");
}

QStringList set_ambientInactiveLevel(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S005B %1").arg(argList.at(0)));
}

QStringList get_ambientEnvironmentalGain(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G005C");
}

QStringList set_ambientEnvironmentalGain(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S005C %1").arg(argList.at(0)));
}

QStringList get_ambientOffHysteresis(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G005D");
}

QStringList set_ambientOffHysteresis(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S005D %1").arg(argList.at(0)));
}

QStringList get_ambientOnHysteresis(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G005E");
}

QStringList set_ambientOnHysteresis(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S005E %1").arg(argList.at(0)));
}

QStringList get_powerboardProtocol(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G005F");
}

QStringList get_ledOverride(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0060");
}

QStringList set_ledOverride(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0060 %1").arg(argList.at(0)));
}

QStringList get_fadeUpStep(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0061");
}

QStringList set_fadeUpStep(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0061 %1").arg(argList.at(0)));
}

QStringList get_fadeDownStep(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0062");
}

QStringList set_fadeDownStep(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0062 %1").arg(argList.at(0)));
}

QStringList get_maxBrightness(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0063");
}

QStringList set_maxBrightness(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0063 %1").arg(argList.at(0)));
}

QStringList get_i2cResets(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0064");
}

QStringList get_sensorGuardTime(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0065");
}

QStringList set_sensorGuardTime(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0065 %1").arg(argList.at(0)));
}

QStringList get_inputVoltage(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0066");
}

QStringList get_inputVoltageCalibration(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0067");
}

QStringList set_inputVoltageCalibration(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0067 %1").arg(argList.at(0)));
}

QStringList get_numLightbars(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0068");
}

QStringList set_numLightbars(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0068 %1").arg(argList.at(0)));
}

QStringList get_currentLimit(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G006A");
}

QStringList set_currentLimit(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S006A %1").arg(argList.at(0)));
}

QStringList get_bootloaderCode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G006B");
}

QStringList get_xpressMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G006C");
}

QStringList set_xpressMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S006C %1").arg(argList.at(0)));
}

QStringList get_batteryBackupStatus(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList() << "G006D");
  if (responseList.at(0).startsWith("ERROR")) {
    return responseList;
  } else {
    bool ok;
    QString response;
    QMap <int, QString> battDetectedDict;
    QMap <int, QString> testReportDict;
    QMap <int, QString> testRunningDict;
    quint32 status = responseList.at(0).toLong(&ok, 16);
    // parse batteries detected bits
    battDetectedDict.insert(0, "No batteries detected");
    battDetectedDict.insert(1, "Battery 1 detected");
    battDetectedDict.insert(2, "Battery 2 detected");
    battDetectedDict.insert(3, "Batteries 1 & 2 detected");
    response += (battDetectedDict[status & 0x3] + "<br>");
    // parse test running bits
    testRunningDict.insert(0, "No tests running");
    testRunningDict.insert(1, "Short test running");
    testRunningDict.insert(2, "Long test running");
    testRunningDict.insert(4, "Push button test running");
    response += (testRunningDict[(status >> 10) & 0x3] + "<br>");
    // parse test reports
    testReportDict.insert(0, "Passed");
    testReportDict.insert(1, "Battery disconnected");
    testReportDict.insert(2, "Battery over temperature");
    testReportDict.insert(3, "Lightbar powered from PSU");
    testReportDict.insert(4, "Lightbar voltage out of range");
    testReportDict.insert(5, "Emergency activated");
    testReportDict.insert(6, "Battery drained");
    testReportDict.insert(7, "Unexpected lightbar pattern");
    testReportDict.insert(8, "Certification mismatch");
    response += "Battery 1 test report: ";
    response += (testReportDict[(status >> 2) & 0xF] + "<br>");
    response += "Battery 2 test report: ";
    response += (testReportDict[(status >> 6) & 0xF] + "<br>");
    // parse test time
    response += "Test time: ";
    response += QString("%1 seconds").arg(status >> 16);
    return QStringList() << QString("+%1").arg(response);
  }
}

QStringList set_batteryBackupStatus(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S006D %1").arg(argList.at(0)));
}

QStringList get_sensorSeconds(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G006E");
}

QStringList get_inputVoltageTwo(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G006F");
}

QStringList get_inputVoltageTwoCalibration(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0070");
}

QStringList set_inputVoltageTwoCalibration(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0070 %1").arg(argList.at(0)));
}

QStringList get_maxRampUpSpeed(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0071");
}

QStringList set_maxRampUpSpeed(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0071 %1").arg(argList.at(0)));
}

QStringList get_maxRampDownSpeed(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0072");
}

QStringList set_maxRampDownSpeed(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0072 %1").arg(argList.at(0)));
}

QStringList get_emergencyLightLevel(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0074");
}

QStringList get_batteryBackupPowerCalibration(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0075");
}

QStringList set_batteryBackupPowerCalibration(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0075 %1").arg(argList.at(0)));
}

QStringList get_motionSensorProfile(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G0076");
}

QStringList set_motionSensorProfile(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0076 %1").arg(argList.at(0)));
}

QStringList get_powerMeterConfig(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  (void) argList;
  cmdList << "G0077" // level at off
          << "G0078" // level at min
          << "G0079" // level at max
          << "G007A"; // type
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("+Level at off: %1").arg(responseList.at(0))
                       << QString("+Level at min: %1").arg(responseList.at(1))
                       << QString("+Level at max: %1").arg(responseList.at(2))
                       << QString("+Type: %1").arg(responseList.at(3));
}

QStringList set_powerMeterLevelAtOff(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0077 %1").arg(argList.at(0)));
}

QStringList set_powerMeterLevelAtMin(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0078 %1").arg(argList.at(0)));
}

QStringList set_powerMeterLevelAtMax(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S0079 %1").arg(argList.at(0)));
}

QStringList set_powerMeterType(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S007A %1").arg(argList.at(0)));
}

QStringList get_DLAiSlaveMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G007B");
}

QStringList set_DLAiSlaveMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S007B %1").arg(argList.at(0)));
}

QStringList get_DALIBootlodingActive(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G007C");
}

QStringList get_testingMode(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G007D");
}

QStringList set_testingMode(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S007D %1").arg(argList.at(0)));
}

QStringList get_numBatteriesSupported(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "G007E");
}

QStringList set_numBatteriesSupported(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("S007E %1").arg(argList.at(0)));
}

/*** lightbar register commands ***/
QStringList get_lbVersion(QStringList argList, interface *iface) {
  QString barNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    barNum = "00";
  } else if (argList.length() == 1) {
   barNum = argList.at(0);
  } else {
    cmdList << "ERROR: expected bar number<br>";
    cmdList << "Example: get lbVersion 00";
    return cmdList;
  }
  cmdList << QString("R%1%2").arg(barNum).arg("00"); // protocol version
  cmdList << QString("R%1%2").arg(barNum).arg("01"); // firmware code high
  cmdList << QString("R%1%2").arg(barNum).arg("02"); // firmware code low
  cmdList << QString("R%1%2").arg(barNum).arg("03"); // firmware version high
  cmdList << QString("R%1%2").arg(barNum).arg("04"); // firmware version low
  responseList = iface->queryPmu(cmdList);
  QString version;
  QString code;
  QString protocol;
  if (responseList.at(3).startsWith("ERROR") || responseList.at(4).startsWith("ERROR")) {
    version = responseList.at(3);
  } else {
    bool ok;
    quint16 verHiInt = responseList.at(3).toUShort(&ok, 16);
    quint16 verLoInt = responseList.at(4).toUShort(&ok, 16);
    version = QString("%1.%2.%3").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  }
  if (responseList.at(1).startsWith("ERROR") || responseList.at(2).startsWith("ERROR")) {
    code = responseList.at(1);
  } else {
    code = QString("%1%2").arg(responseList.at(1)).arg(responseList.at(2));
  }
  protocol = responseList.at(0);
  return QStringList() << QString("+Firmware version: %1").arg(version)
                       << QString("+Firmware code: %1%2").arg(code)
                       << QString("+Protocol version: %1").arg(protocol);
}

QStringList get_lbStatus(QStringList argList, interface *iface) {
  QString barNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    barNum = "00";
  } else {
   barNum = argList.at(0);
  }
  cmdList << QString("R%1%2").arg(barNum).arg("40"); // status
  cmdList << QString("R%1%2").arg(barNum).arg("41"); // string 1 current
  cmdList << QString("R%1%2").arg(barNum).arg("42"); // string 2 current
  cmdList << QString("R%1%2").arg(barNum).arg("43"); // string 3 current
  cmdList << QString("R%1%2").arg(barNum).arg("44"); // string 4 current
  cmdList << QString("R%1%2").arg(barNum).arg("45"); // string current minimum
  cmdList << QString("R%1%2").arg(barNum).arg("46"); // temperature
  cmdList << QString("R%1%2").arg(barNum).arg("47"); // string current sum
  cmdList << QString("R%1%2").arg(barNum).arg("48"); // voltage reference
  cmdList << QString("R%1%2").arg(barNum).arg("80"); // light level
  cmdList << QString("R%1%2").arg(barNum).arg("81"); // light active slew rate
  cmdList << QString("R%1%2").arg(barNum).arg("82"); // light inactive slew rate
  QString bypass = responseList.at(0);
  QString stringCurrent1 = responseList.at(1);
  QString stringCurrent2 = responseList.at(2);
  QString stringCurrent3 = responseList.at(3);
  QString stringCurrent4 = responseList.at(4);
  QString stringCurrentMin = responseList.at(5);
  QString temperature = responseList.at(6);
  QString stringCurrentSum = responseList.at(7);
  QString voltageRef = responseList.at(8);
  QString lightLevel = responseList.at(9);
  QString lightActiveSlew = responseList.at(10);
  QString lightInactiveSlew = responseList.at(11);
  bool ok;
  quint16 val;
  if (bypass.startsWith("ERROR") == false) {
    val = responseList.at(0).toUShort(&ok, 16);
    if (statusInt & 4) {
      bypass << "active";
    } else {
      bypass << "inactive";
    }
  }
  if (stringCurrent1.startsWith("ERROR") == false) {
    val = stringCurrent1.toUShort(&ok, 16);
    stringCurrent1 = QString("%1 mA").arg(1.4 * val);
  }
  if (stringCurrent2.startsWith("ERROR") == false) {
    val = stringCurrent2.toUShort(&ok, 16);
    stringCurrent2 = QString("%1 mA").arg(1.4 * val);
  }
  if (stringCurrent3.startsWith("ERROR") == false) {
    val = stringCurrent3.toUShort(&ok, 16);
    stringCurrent3 = QString("%1 mA").arg(1.4 * val);
  }
  if (stringCurrent4.startsWith("ERROR") == false) {
    val = stringCurrent4.toUShort(&ok, 16);
    stringCurrent4 = QString("%1 mA").arg(1.4 * val);
  }
  if (stringCurrentSum.startsWith("ERROR") == false) {
    val = stringCurrentSum.toUShort(&ok, 16);
    stringCurrentSum = QString("%1 mA").arg(1.4 * val);
  }
  if (stringCurrentMin.startsWith("ERROR") == false) {
    val = stringCurrentMin.toUShort(&ok, 16);
    stringCurrentMin = QString("%1 mA").arg(1.4 * val);
  }
  if (temperature.startsWith("ERROR") == false) {
    val = temperature.toUShort(&ok, 16);
    temperature = QString("%1 C").arg((125 * val / 1024) - 40);
  }
  if (voltageRef.startsWith("ERROR") == false) {
    val = voltageRef.toUShort(&ok, 16);
    voltageRef = QString("%1 volts").arg(2.5 * val / 1024);
  }
  return QStringList() << QString("+Bypass: %1").arg(bypass)
                       << QString("+String 1 current: %1").arg(stringCurrent1)
                       << QString("+String 2 current: %1").arg(stringCurrent2)
                       << QString("+String 3 current: %1").arg(stringCurrent3)
                       << QString("+String 4 current: %1").arg(stringCurrent4)
                       << QString("+String current sum: %1").arg(stringCurrentSum)
                       << QString("+String current min: %1").arg(stringCurrentMin)
                       << QString("+Temperature: %1").arg(temperature)
                       << QString("+Voltage reference: %1").arg(voltageRef)
                       << QString("+Light level (0x029C = OFF): %1").arg(lightLevel)
                       << QString("+Light active slew rate: %1").arg(lightActiveSlew)
                       << QString("+Light inactive slew rate: %1").arg(lightInactiveSlew);
}

QStringList get_lbConfig(QStringList argList, interface *iface) {
  QString barNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    barNum = "00";
  } else {
    barNum = argList.at(0);
  }
  cmdList << QString("R%1%2").arg(barNum).arg("85"); // hardware rev
  cmdList << QString("R%1%2").arg(barNum).arg("86"); // temperature cal
  cmdList << QString("R%1%2").arg(barNum).arg("87"); // led device type
  cmdList << QString("R%1%2").arg(barNum).arg("88"); // serial number high
  cmdList << QString("R%1%2").arg(barNum).arg("89"); // serial number low
  cmdList << QString("R%1%2").arg(barNum).arg("8A"); // current sense bypass threshold
  cmdList << QString("R%1%2").arg(barNum).arg("8B"); // current sense bypass hystesis
  cmdList << QString("R%1%2").arg(barNum).arg("8C"); // estimator current sense coefficient
  cmdList << QString("R%1%2").arg(barNum).arg("8D"); // estimator current sense exponent
  cmdList << QString("R%1%2").arg(barNum).arg("8E"); // bypass override temperature
  cmdList << QString("R%1%2").arg(barNum).arg("8F"); // temperature throttle limit
  responseList = iface->queryPmu(cmdList);
  return QStringList() << QString("Hardware revision: %1").arg(responseList.at(0))
                       << QString("Temperature calibration: %1").arg(responseList.at(1))
                       << QString("LED device type: %1").arg(responseList.at(2))
                       << QString("Serial number: %1%2").arg(responseList.at(3)).arg(responseList.at(4))
                       << QString("Current sense bypass threshold: %1").arg(responseList.at(5))
                       << QString("Current sense bypass hysteresis: %1").arg(responseList.at(6))
                       << QString("Estimator current sense coefficient: %1").arg(responseList.at(7))
                       << QString("Estimator current sense exponent: %1").arg(responseList.at(8))
                       << QString("Bypass override temperature: %1").arg(responseList.at(9))
                       << QString("Temperature throttle limit: %1").arg(responseList.at(10));
}

/*** battery backup register commands ***/
QStringList get_bbVersion(QStringList argList, interface *iface) {
  QString battNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  }
  cmdList << QString("R%1%2").arg(battNum).arg("00"); // protocol version
  cmdList << QString("R%1%2").arg(battNum).arg("01"); // firmware code high
  cmdList << QString("R%1%2").arg(battNum).arg("02"); // firmware code low
  cmdList << QString("R%1%2").arg(battNum).arg("03"); // firmware version high
  cmdList << QString("R%1%2").arg(battNum).arg("04"); // firmware version low
  responseList = iface->queryPmu(cmdList);
  QString version;
  QString code;
  QString protocol;
  if (responseList.at(3).startsWith("ERROR") || responseList.at(4).startsWith("ERROR")) {
    version = responseList.at(3);
  } else {
    bool ok;
    quint16 verHiInt = responseList.at(3).toUShort(&ok, 16);
    quint16 verLoInt = responseList.at(4).toUShort(&ok, 16);
    version = QString("%1.%2.%3").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  }
  if (responseList.at(1).startsWith("ERROR") || responseList.at(2).startsWith("ERROR")) {
    code = responseList.at(1);
  } else {
    code = QString("%1%2").arg(responseList.at(1)).arg(responseList.at(2));
  }
  protocol = responseList.at(0);
  return QStringList() << QString("+Firmware version: %1").arg(version)
                       << QString("+Firmware code: %1%2").arg(code)
                       << QString("+Protocol version: %1").arg(protocol);
}

QStringList get_bbStatus(QStringList argList, interface *iface) {
  QString battNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  }
  cmdList << QString("R%1%2").arg(battNum).arg("40"); // status
  cmdList << QString("R%1%2").arg(battNum).arg("41"); // battery voltage
  cmdList << QString("R%1%2").arg(battNum).arg("42"); // battery temperature
  cmdList << QString("R%1%2").arg(battNum).arg("43"); // lightbar supply voltage
  cmdList << QString("R%1%2").arg(battNum).arg("44"); // lightbar psu current
  cmdList << QString("R%1%2").arg(battNum).arg("45"); // alarms
  cmdList << QString("R%1%2").arg(battNum).arg("46"); // time to mode change
  cmdList << QString("R%1%2").arg(battNum).arg("47"); // uptime minutes
  cmdList << QString("R%1%2").arg(battNum).arg("48"); // uptime hours
  cmdList << QString("R%1%2").arg(battNum).arg("49"); // error count
  responseList = iface->queryPmu(cmdList);
  bool ok;
  QString status = responseList.at(0);
  QString batteryVoltage = responseList.at(1);
  QString batteryTemperature = responseList.at(2);
  QString lbSupplyVoltage = responseList.at(3);
  QString lbPsuCurrent = responseList.at(4);
  QString alarms = responseList.at(5);
  QString timeToModeChange = responseList.at(6);
  QString uptimeMinutes = responseList.at(7);
  QString uptimeHours = responseList.at(8);
  QString errorCount = responseList.at(9);
  if (status.startsWith("ERROR")) {
    status = QString("+Status: %1").arg(status);
  } else {
    QString parsedResponse;
    QMap <int, QString> statusDict;
    quint32 statusInt = responseList.at(0).toLong(&ok, 16);
    // parse mode bits
    statusDict.insert(0, "Invalid");
    statusDict.insert(1, "Charging");
    statusDict.insert(2, "Standby");
    statusDict.insert(3, "Shutdown");
    statusDict.insert(4, "Error");
    statusDict.insert(5, "Emergency");
    statusDict.insert(6, "Test");
    statusDict.insert(7, "Powerdown");
    status = QString("+Mode: %1<br>").arg(statusDict[statusInt & 0xF]);
    // parse battery status bits
    statusDict.clear();
    statusDict.insert(0, "Good");
    statusDict.insert(1, "Disconnected");
    statusDict.insert(2, "Fully charged");
    statusDict.insert(3, "Fully discharged");
    statusDict.insert(4, "Needs charge");
    statusDict.insert(5, "Needs powerdown");
    status += QString("Battery status: %1<br>").arg(statusDict[(statusInt >> 4) & 0xF]);
    // parse temperature status bits
    statusDict.clear();
    statusDict.insert(0, "Good");
    statusDict.insert(1, "Charge limit exceeded");
    statusDict.insert(2, "Emergency limit exceeded");
    status += QString("Temperature status: %1<br>").arg(statusDict[(statusInt >> 8) & 0x3]);
    // parse lightbar voltage status bits
    statusDict.clear();
    statusDict.insert(0, "Good");
    statusDict.insert(1, "Exceeded limits");
    status += QString("Lightbar voltage status: %1<br>").arg(statusDict[(statusInt >> 10) & 0x1]);
    // parse lightbar current status bits
    statusDict.clear();
    statusDict.insert(0, "Good");
    statusDict.insert(1, "Exceeded limits");
    status += QString("Lightbar current status: %1<br>").arg(statusDict[(statusInt >> 11) & 0x1]);
    // parse test button status bits
    statusDict.clear();
    statusDict.insert(0, "Released");
    statusDict.insert(1, "Pressed");
    status += QString("Test button status: %1<br>").arg(statusDict[(statusInt >> 12) & 0x1]);
    // parse PSU status bits
    statusDict.clear();
    statusDict.insert(0, "42v on");
    statusDict.insert(1, "42v off");
    status += QString("PSU status: %1<br>").arg(statusDict[(statusInt >> 13) & 0x1]);
    // parse certification mark status
    statusDict.clear();
    statusDict.insert(0, "UL");
    statusDict.insert(1, "CE");
    status += QString("Certification mark: %1<br>").arg(statusDict[(statusInt >> 15) & 0x1]);
  }
  if (batteryVoltage.startsWith("ERROR") == false) {
    batteryVoltage = QString("%1 volts").arg(0.04 * batteryVoltage.toUShort(&ok, 16));
  }
  if (batteryTemperature.startsWith("ERROR") == false) {
    batteryTemperature = QString("%1 C").arg(0.125 * batteryTemperature.toUShort(&ok, 16) - 164);
  }
  if (lbSupplyVoltage.startsWith("ERROR") == false) {
    lbSupplyVoltage = QString("%1 volts").arg(0.05 * lbSupplyVoltage.toUShort(&ok, 16));
  }
  if (lbPsuCurrent.startsWith("ERROR") == false) {
    lbPsuCurrent = QString("%1 mA").arg(1.44 * lbPsuCurrent.toUShort(&ok, 16));
  }
  if (alarms.startsWith("ERROR") == false) {
    QMap <int, QString> alarmsDict;
    alarmsDict.insert(0, "None");
    alarmsDict.insert(1, "Battery voltage crossed max limit");
    alarmsDict.insert(2, "Battery voltage crossed recharge limit");
    alarms = alarmsDict[alarms.toUShort(&ok, 16)];
  }
  if (timeToModeChange.startsWith("ERROR") == false) {
    timeToModeChange = QString("%1 mintues").arg(timeToModeChange.toUShort(&ok, 16));
  }
  QString uptime;
  if (uptimeHours.startsWith("ERROR") || uptimeMinutes.startsWith("ERROR")) {
    uptime = uptimeHours;
  } else {
    uptime = QString("%1 hours, %2 minutes").arg(uptimeHours.toUShort(&ok, 16)).arg(uptimeMinutes.toUShort(&ok, 16));
  }
  if (errorCount.startsWith("ERROR") == false) {
    errorCount = QString("%1").arg(errorCount.toUShort(&ok, 16));
  }
  return QStringList() << status
                       << QString("+Battery voltage: %1").arg(batteryVoltage)
                       << QString("+Battery temperature: %1").arg(batteryTemperature)
                       << QString("+Lightbar supply voltage: %1").arg(lbSupplyVoltage)
                       << QString("+Lightbar PSU current: %1").arg(lbPsuCurrent)
                       << QString("+Alarms: %1").arg(alarms)
                       << QString("+Time to mode change: %1").arg(timeToModeChange)
                       << QString("+Uptime: %1").arg(uptime)
                       << QString("+Error count: %1").arg(errorCount);

}

QStringList get_bbConfig(QStringList argList, interface *iface) {
  QString battNum;
  QStringList cmdList;
  QStringList responseList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  }
  cmdList << QString("R%1%2").arg(battNum).arg("82"); // hardware rev
  cmdList << QString("R%1%2").arg(battNum).arg("83"); // temperature cal
  cmdList << QString("R%1%2").arg(battNum).arg("84"); // serial number high word
  cmdList << QString("R%1%2").arg(battNum).arg("85"); // serial number low word
  cmdList << QString("R%1%2").arg(battNum).arg("86"); // charge time
  cmdList << QString("R%1%2").arg(battNum).arg("87"); // trickle time
  cmdList << QString("R%1%2").arg(battNum).arg("88"); // standby time
  cmdList << QString("R%1%2").arg(battNum).arg("89"); // max battery voltage
  cmdList << QString("R%1%2").arg(battNum).arg("8A"); // min battery voltage
  cmdList << QString("R%1%2").arg(battNum).arg("8B"); // recharge battery voltage
  cmdList << QString("R%1%2").arg(battNum).arg("8C"); // max charge temperature
  cmdList << QString("R%1%2").arg(battNum).arg("8D"); // max emergency temperature
  cmdList << QString("R%1%2").arg(battNum).arg("8E"); // min emergency verify voltage
  cmdList << QString("R%1%2").arg(battNum).arg("8F"); // max emergency verify voltage
  cmdList << QString("R%1%2").arg(battNum).arg("90"); // max lightbar PSU current
  cmdList << QString("R%1%2").arg(battNum).arg("91"); // certification mark
  cmdList << QString("R%1%2").arg(battNum).arg("92"); // shutdown time
  cmdList << QString("R%1%2").arg(battNum).arg("93"); // product code low word
  cmdList << QString("R%1%2").arg(battNum).arg("94"); // product code high word
  responseList = iface->queryPmu(cmdList);
  QString hardwareRev = responseList.at(0);
  QString tempCal = responseList.at(1);
  QString snHigh = responseList.at(2);
  QString snLow = responseList.at(3);
  QString chargeTime = responseList.at(4);
  QString trickleTime = responseList.at(5);
  QString standbyTime = responseList.at(6);
  QString maxBatteryVoltage = responseList.at(7);
  QString minBatteryVoltage = responseList.at(8);
  QString rechargeBatteryVoltage = responseList.at(9);
  QString maxChargeTemp = responseList.at(10);
  QString maxEmergencyTemp = responseList.at(11);
  QString minEmergencyVerifyVoltage = responseList.at(12);
  QString maxEmergencyVerifyVoltage = responseList.at(13);
  QString maxLbPsuCurrent = responseList.at(14);
  QString certificationMark = responseList.at(15);
  QString shutdownTime = responseList.at(16);
  QString prodCodeLow = responseList.at(17);
  QString prodCodeHigh = responseList.at(18);
  bool ok;
  QString serialNum;
  if (snLow.startsWith("ERROR") || snHigh.startsWith("ERROR")) {
    serialNum = snLow;
  } else {
    serialNum = QString("%1%2").arg(snHigh).arg(snLow);
  }
  if (chargeTime.startsWith("ERROR") == false) {
    chargeTime = QString("%1 minutes").arg(chargeTime.toUShort((&ok, 16)));
  }
  if (trickleTime.startsWith("ERROR") == false) {
    trickleTime = QString("%1 minutes").arg(trickleTime.toUShort((&ok, 16)));
  }
  if (standbyTime.startsWith("ERROR") == false) {
    standbyTime = QString("%1 minutes").arg(standbyTime.toUShort((&ok, 16)));
  }
  if (shutdownTime.startsWith("ERROR") == false) {
    shutdownTime = QString("%1 minutes").arg(shutdownTime.toUShort((&ok, 16)));
  }
  if (maxBatteryVoltage.startsWith("ERROR") == false) {
    maxBatteryVoltage = QString("%1 volts").arg(maxBatteryVoltage.toUShort(&ok, 16) * 0.04)
  }
  if (minBatteryVoltage.startsWith("ERROR") == false) {
    minBatteryVoltage = QString("%1 volts").arg(minBatteryVoltage.toUShort(&ok, 16) * 0.04)
  }
  if (rechargeBatteryVoltage.startsWith("ERROR") == false) {
    rechargeBatteryVoltage = QString("%1 volts").arg(rechargeBatteryVoltage.toUShort(&ok, 16) * 0.04)
  }
  if (maxChargeTemp.startsWith("ERROR") == false) {
    maxChargeTemp = QString("%1 C").arg((maxChargeTemp.toUShort(&ok, 16) - 164) * 0.125);
  }
  if (maxEmergencyTemp.startsWith("ERROR") == false) {
    maxEmergencyTemp = QString("%1 C").arg((maxEmergencyTemp.toUShort(&ok, 16) - 164) * 0.125);
  }
  if (minEmergencyVerifyVoltage.startsWith("ERROR") == false) {
    minEmergencyVerifyVoltage = QString("%1 volts").arg(minEmergencyVerifyVoltage.toUShort(&ok, 16) * 0.05);
  }
  if (maxEmergencyVerifyVoltage.startsWith("ERROR") == false) {
    maxEmergencyVerifyVoltage = QString("%1 volts").arg(maxEmergencyVerifyVoltage.toUShort(&ok, 16) * 0.05);
  }
  if (maxLbPsuCurrent.startsWith("ERROR") == false) {
    maxLbPsuCurrent = QString("%1 mA").arg(maxLbPsuCurrent.toUShort(&ok, 16) * 2.44);
  }
  if (certificationMark.startsWith("ERROR") == false) {
    certificationMark = (certificationMark == "0000") ? "UL" : "CE";
  }
  QString productCode;
  if (prodCodeHigh.startsWith("ERROR") || prodCodeLow.startsWith("ERROR")) {
    productCode = prodCodeHigh;
  } else {
    productCode = QString("%1%2").arg(prodCodeLow).arg(prodCodeHigh);
  }
  return QStringList() << QString("+Hardware revision: %1").arg(hardwareRev)
                       << QString("+Temperature calibration: %1").arg(tempCal)
                       << QString("+Serial number: %1").arg(serialNum)
                       << QString("+Charge time: %1").arg(chargeTime)
                       << QString("+Trickle time: %1").arg(trickleTime)
                       << QString("+Standby time: %1").arg(standbyTime)
                       << QString("+Shutdown time: %1").arg(shutdownTime)
                       << QString("+Max battery voltage: %1").arg(maxBatteryVoltage)
                       << QString("+Min battery voltage: %1").arg(minBatteryVoltage)
                       << QString("+Recharge battery voltage: %1").arg(rechargeBatteryVoltage)
                       << QString("+Max charge temperature: %1").arg(maxChargeTemp)
                       << QString("+Max emergency temperature: %1").arg(maxEmergencyTemp)
                       << QString("+Min emergency verify voltage: %1").arg(minEmergencyVerifyVoltage)
                       << QString("+Max emergency verify voltage: %1").arg(maxEmergencyVerifyVoltage)
                       << QString("Max lightbar PSU current: %1 mA").arg(maxLbPsuCurrent)
                       << QString("Certification mark: %1").arg(certificationMark)
                       << QString("Product code: %1%2").arg(productCode);
}

/*** reset commands ***/
QStringList reset_usage(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!U");
}

QStringList reset_oldLog(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!L");
}

QStringList reset_log(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!K");
}

QStringList reset_logIndex(QStringList argList, interface *iface) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return iface->queryPmu(QStringList() << QString("J%1").arg(argList.at(0)));
}

QStringList reset_eeprom(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!Z");
}

QStringList reset_eepromToDefault(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!C");
}

QStringList reset_eepromToLatestMapVersion(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!E");
}

QStringList reset_network(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!N");
}

QStringList reset_networkWithoutChecking(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!N1");
}

QStringList reset_daliCommissioning(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList()<< "!Y");
}

QStringList reset_daliPowerMetering(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList()<< "!A");
}

/*** reboot commands ***/
QStringList reboot_pmu(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!R");
}

QStringList reboot_wirelessCard(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!W");
}

QStringList reboot_i2cDevices(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!X");
}

/*** reload commands ***/
QStringList reload_dlaFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!B");
}

QStringList reload_wirelessModuleFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!M");
}

QStringList reload_powerboardFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!P");
}

QStringList reload_lightbarFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!P");
}

QStringList reload_batteryBackupFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!P");
}

QStringList reload_motionSensorFirmware(QStringList argList, interface *iface) {
  (void) argList;
  return iface->queryPmu(QStringList() << "!V");
}

/*** log commands ***/
QStringList get_logIndex(QStringList argList, interface *iface) {
  QStringList responseList;
  (void) argList;
  responseList = iface->queryPmu(QStringList() << "K");
  if (responseList.at(0).startsWith("ERROR")) {
    return responseList;
  } else {
    QString arg1 = responseList.at(0);
    QString head = arg1.left(4);
    arg1.remove(0, 4);
    QString tail = arg1.left(4);
    arg1.remove(0, 4);
    QString first = arg1.left(4);
    if (first == "FFFF") {
      first = "none";
    }
    return QStringList() << QString("head: %1<br>tail: %2<br>first recent: %3").arg(head).arg(tail).arg(first);
  }
}

QStringList get_log(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  bool ok;
  QString index;
  if (argList.length() == 0) {
    index.setNum(logIndex, 16);
    // pad with leading zeros
    while (index.length() < 4) {
      index.prepend("0");
    }
  } else {
    index = argList.at(0);
    logIndex = index.toUShort(&ok, 16);
  }
  return QStringList() << QString("K%1").arg(index);
}

QStringList parse_get_log(QStringList responseList) {
  QString arg1;
  int uptimeSize, valueSize, baseTime, uptime;
  QString eventType, eventValue;
  QString log;
  bool ok;
  bool isLastEntry;
  QString timestamp;
  int numEvents = 0;
  arg1 = responseList.at(0);
  baseTime = 0;
  do {
    // assemble log entry
    uptimeSize = arg1.left(1).toInt(&ok, 16);
    // most significant bit of uptime size is last entry indicator
    if (uptimeSize > 7) {
      isLastEntry = true;
      uptimeSize -= 8;
    } else {
      isLastEntry = false;
    }
    // strip elements from the log entry
    arg1.remove(0, 1);
    valueSize = arg1.left(1).toInt(&ok, 16);
    arg1.remove(0, 1);
    eventType = arg1.left(2);
    arg1.remove(0, 2);
    uptime = arg1.left(uptimeSize * 2).toInt(&ok, 16);
    arg1.remove(0, uptimeSize * 2);
    eventValue = arg1.left(valueSize * 2);
    arg1.remove(0, valueSize * 2);
    // compute uptime
    if (uptimeSize == 4) {
      baseTime = uptime;
    } else {
      uptime += baseTime;
      baseTime = uptime;
    }
    timestamp.setNum(uptime, 16);
    timestamp = toYDHMS(timestamp);
    // parse log entry
    if (eventType == "00") {
      // build a dictionary of power events
      QMap <QString, QString> powerEventsMap;
      powerEventsMap.insert("00", "Power down");
      powerEventsMap.insert("01", "Power up");
      powerEventsMap.insert("02", "Power restored");
      powerEventsMap.insert("03", "Power soft reset");
      log += QString("%1 > %2").arg(timestamp).arg(powerEventsMap[eventValue]);
    } else if (eventType == "01") {
      // build a dictionary of activity state transition events
      QMap <QString, QString> activityStateTransitionEventsMap;
      activityStateTransitionEventsMap.insert("00", "Fixture inactive");
      activityStateTransitionEventsMap.insert("01", "Sensor 0 active");
      activityStateTransitionEventsMap.insert("02", "Sensor 1 active");
      activityStateTransitionEventsMap.insert("03", "Sensor 0 & Sensor 1 active");
      activityStateTransitionEventsMap.insert("04", "Remote sensor active");
      activityStateTransitionEventsMap.insert("05", "Remote sensor & sensor 0 active");
      activityStateTransitionEventsMap.insert("06", "Remote sensor & sensor 1 active");
      activityStateTransitionEventsMap.insert("07", "Remote sensor, sensor 0, and sensor 1 active");
      log += QString("%1 > %2").arg(timestamp).arg(activityStateTransitionEventsMap[eventValue]);
    } else if (eventType == "02") {
      // type 2 events are not implemented
      log += QString("%1 > Type 2 event: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "03") {
      log += QString("%1 > Sensor off: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "04") {
      // unspecified value
      log += QString("%1 > SerialNet watchdog tripped").arg(timestamp);
    } else if (eventType == "05") {
      log += QString("%1 > Temperature state change: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "06") {
      log += QString("%1 > Lightbar error: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "07") {
      log += QString("%1 > RTC set event: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "08") {
      int batteryNumber;
      // build a dictionary of battery backup events
      QMap <QString, QString> batteryBackupEventsMap;
      batteryBackupEventsMap.insert("00", "Power activated");
      batteryBackupEventsMap.insert("01", "Power deactivated");
      batteryBackupEventsMap.insert("02", "Power failure [battery disconnected]");
      batteryBackupEventsMap.insert("03", "Power failure [battery over temperature]");
      batteryBackupEventsMap.insert("04", "Power failure [lightbar current sourced from PSU, not battery]");
      batteryBackupEventsMap.insert("05", "Power failure [backup power voltage out of range]");
      batteryBackupEventsMap.insert("06", "Power failure [battery drained]");
      batteryBackupEventsMap.insert("07", "Power failure [unexpected lightbar pattern or pattern could not be verified]");
      batteryBackupEventsMap.insert("08", "Battery test started");
      batteryBackupEventsMap.insert("09", "Battery test stopped");
      batteryBackupEventsMap.insert("0A", "Error [battery disconnected]");
      batteryBackupEventsMap.insert("0B", "Error [charge temperature exceeded]");
      batteryBackupEventsMap.insert("0C", "Last error cleared");
      batteryBackupEventsMap.insert("0D", "Power failure [UL/CE mismatch]");
      // parse battery number from event value
      if (eventValue.at(0) != '0') {
        eventValue.replace(0, 1, "0");
        batteryNumber = 1;
      } else {
        batteryNumber = 0;
      }
      log += QString("%1 > Battery backup %2 event: %3").arg(timestamp).arg(batteryNumber).arg(batteryBackupEventsMap[eventValue]);
    } else if (eventType == "09") {
      log += QString("%1 > I2C watchdog reset event: %2").arg(timestamp).arg(eventValue);
    } else if (eventType == "0A") {
      // unspecified value
      log += QString("%1 > Registers restored from backup").arg(timestamp);
    } else {
      // unknown event
      log += QString("%1 > Type %2 event: %3").arg(timestamp).arg(eventType).arg(eventValue);
    }
    // insert a break
    log += "<br>";
    // count events
    numEvents++;
  } while (arg1.isEmpty() != true);
  // notify the user that more logs are available
  if (isLastEntry == true) {
    log += "[End of log]";
  } else {
    log += "[More events available...]";
    logIndex += numEvents;
  }
  return QStringList() << log;
}

QStringList insert_logEntry(QStringList argList, interface *iface) {
  QStringList cmdList;
  QStringList responseList;
  cmdList << QString("E%1").arg(argList.at(0));
  return iface->queryPmu(cmdList);
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_exeTable.insert("get firmwareVersion", &get_firmwareVersion);
  m_exeTable.insert("get productCode", get_productCode);
  m_exeTable.insert("set productCode", set_productCode);
  m_exeTable.insert("get serialNumber", get_serialNumber);
  m_exeTable.insert("set serialNumber", set_serialNumber);
  m_exeTable.insert("get unixTime", get_unixTime);
  m_exeTable.insert("set unixTime", set_unixTime);
  m_exeTable.insert("get temperature", get_temperature, parse_get_temperature);
  m_exeTable.insert("get lightLevel", get_lightLevel, parse_get_lightLevel);
  m_exeTable.insert("set lightManualLevel", set_lightManualLevel);
  m_exeTable.insert("set lightOverrideActiveLevel", set_lightOverrideActiveLevel);
  m_exeTable.insert("set lightOverrideInactiveLevel", set_lightOverrideInactiveLevel);
  m_exeTable.insert("get sensorDelayTime", get_sensorDelayTime);
  m_exeTable.insert("get sensorOverrideDelayTime", get_sensorOverrideDelayTime);
  m_exeTable.insert("set sensorOverrideDelayTime", set_sensorOverrideDelayTime);
  m_exeTable.insert("get upTime", get_upTime, parse_get_upTime);
  m_exeTable.insert("get usage", get_usage, parse_get_usage);
  m_exeTable.insert("get numLogEntries", get_numLogEntries);
  m_exeTable.insert("get configCalibration", get_configCalibration, parse_get_configCalibration);
  m_exeTable.insert("set configCalibrationP0", set_configCalibrationP0);
  m_exeTable.insert("set configCalibrationP1", set_configCalibrationP1);
  m_exeTable.insert("set configCalibrationP2", set_configCalibrationP2);
  m_exeTable.insert("set configCalibrationP3", set_configCalibrationP3);
  m_exeTable.insert("get buildTime", get_buildTime);
  m_exeTable.insert("set buildTime", set_buildTime);
  m_exeTable.insert("get sensorTimeoutCountdown", get_sensorTimeoutCountdown);
  m_exeTable.insert("get currentLightLevel", get_currentLightLevel);
  m_exeTable.insert("get safeMode", get_safeMode);
  m_exeTable.insert("get lightBarSelect", get_lightBarSelect);
  m_exeTable.insert("set lightBarSelect", set_lightBarSelect);
  m_exeTable.insert("get powerConsumption", get_powerConsumption, parse_get_powerConsumption);
  m_exeTable.insert("get wirelessDataAggregator", get_wirelessDataAggregator);
  m_exeTable.insert("set wirelessDataAggregator", set_wirelessDataAggregator);
  m_exeTable.insert("get resetUsageTimestamp", get_resetUsageTimestamp);
  m_exeTable.insert("get pwmPeriodRegister", get_pwmPeriodRegister);
  m_exeTable.insert("set pwmPeriodRegister", set_pwmPeriodRegister);
  m_exeTable.insert("get analogSensorValue", get_analogSensorValue);
  m_exeTable.insert("get analogReportingHysteresis", get_analogReportingHysteresis);
  m_exeTable.insert("get zone", get_zone);
  m_exeTable.insert("set zone", set_zone);
  m_exeTable.insert("get lightTemporaryActiveLevel", get_lightTemporaryActiveLevel);
  m_exeTable.insert("set lightTemporaryActiveLevel", set_lightTemporaryActiveLevel);
  m_exeTable.insert("get lightTemporaryInactiveLevel", get_lightTemporaryInactiveLevel);
  m_exeTable.insert("set lightTemporaryInactiveLevel", set_lightTemporaryInactiveLevel);
  m_exeTable.insert("get sensorTemporaryDelayTime", get_sensorTemporaryDelayTime);
  m_exeTable.insert("set sensorTemporaryDelayTime", set_sensorTemporaryDealyTime);
  m_exeTable.insert("get temporaryOverrideTimeout", get_temporaryOverrideTimeout);
  m_exeTable.insert("set temporaryOverrideTimeout", set_temporaryOverrideTiemout);
  m_exeTable.insert("get setRemoteState", get_setRemoteState);
  m_exeTable.insert("set setRemoteState", set_setRemoteState);
  m_exeTable.insert("get remoteSetDelayTime", get_remoteStateDelayTime);
  m_exeTable.insert("set remoteSetDelayTime", set_remoteStateDelayTime);
  m_exeTable.insert("get remoteSecondsCountdown", get_remoteSecondsCountdown);
  m_exeTable.insert("get minimumDimmingValue", get_minimumDimmingValue);
  m_exeTable.insert("get powerCalibration", get_powerCalibration, parse_get_powerCalibration);
  m_exeTable.insert("set powerCalibrationA0", set_powerCalibrationA0);
  m_exeTable.insert("set powerCalibrationB0", set_powerCalibrationB0);
  m_exeTable.insert("set powerCalibrationC0", set_powerCalibrationC0);
  m_exeTable.insert("set powerCalibrationMA", set_powerCalibrationMA);
  m_exeTable.insert("set powerCalibrationMB", set_powerCalibrationMB);
  m_exeTable.insert("set powerCalibrationMC", set_powerCalibrationMC);
  m_exeTable.insert("set powerCalibrationPOff", set_powerCalibrationPOff);
  m_exeTable.insert("set powerCalibrationPOn", set_powerCalibrationPOn);
  m_exeTable.insert("set powerCalibrationT0", set_powerCalibrationT0);
  m_exeTable.insert("get powerEstimatorTemperatureOverride", get_powerEstimatorTemperatureOverride);
  m_exeTable.insert("set powerEstimatorTemperatureOverride", set_powerEstimatorTemperatureOverride);
  m_exeTable.insert("get cachedTemperatureValue", get_cachedTemperatureValue);
  m_exeTable.insert("get eepromSize", get_eepromSize);
  m_exeTable.insert("get hardwareRevision", get_hardwareRevision);
  m_exeTable.insert("get wirelessConfig", get_wirelessConfig, parse_get_wirelessConfig);
  m_exeTable.insert("set wirelessPanId", set_wirelessPanId);
  m_exeTable.insert("set wirelessChannelMask", set_wirelessChannelMask);
  m_exeTable.insert("set wirelessShortAddress", set_wirelessShortAddress);
  m_exeTable.insert("set wirelessRole", set_wirelessRole);
  m_exeTable.insert("set wirelessWatchdogHold", set_wirelessWatchdogHold);
  m_exeTable.insert("set wirelessWatchdogPeriod", set_wirelessWatchdogPeriod);
  m_exeTable.insert("set wirelessNetworkKey", set_wirelessNetworkKey);
  m_exeTable.insert("get firmwareCode", get_firmwareCode);
  m_exeTable.insert("get moduleFirmwareCode", get_moduleFirmwareCode);
  m_exeTable.insert("get maxTemperature", get_maxTemperature, parse_get_maxTemperature);
  m_exeTable.insert("get overTemperatureConfig", get_overTemperatureConfig, parse_get_overTemperatureConfig);
  m_exeTable.insert("set overTemperatureThresholdLow", set_overTemperatureThresholdLow);
  m_exeTable.insert("set overTemperatureThresholdHigh", get_overTemperatureThresholdHigh);
  m_exeTable.insert("set overTemperatureDimmingLimit", set_overTemperatureDimmingLimit);
  m_exeTable.insert("get analogDimmingMode", get_analogDimmingMode, parse_get_analogDimmingMode);
  m_exeTable.insert("set analogDimmingMode", set_analogDimmingMode);
  m_exeTable.insert("get fixtureIdMode", get_fixtureIdMode);
  m_exeTable.insert("set fixtureIdMode", set_fixtureIdMode);
  m_exeTable.insert("get acFrequency", get_acFrequency);
  m_exeTable.insert("get sensorBits", get_sensorBits);
  m_exeTable.insert("get powerMeterCommand", get_powerMeterCommand);
  m_exeTable.insert("set powerMeterCommand", set_powerMeterCommand);
  m_exeTable.insert("get powerMeterRegister", get_powerMeterRegister);
  m_exeTable.insert("set powerMeterRegister", set_powerMeterRegister);
  m_exeTable.insert("get ambientTemperature", get_ambientTemperature);
  m_exeTable.insert("get lightSensorLevel", get_lightSensorLevel);
  m_exeTable.insert("get sensorConfig", get_sensorConfig, parse_get_sensorConfig);
  m_exeTable.insert("set sensor0Timeout", set_sensor0Timeout);
  m_exeTable.insert("set sensor0Offset", set_sensor0Offset);
  m_exeTable.insert("set sensor1Timeout", set_sensor1Timeout);
  m_exeTable.insert("set sensor1Offset", set_sensor1Offset);
  m_exeTable.insert("get analogDimmingConfig", get_analogDimmingConfig, parse_get_analogDimmingConfig);
  m_exeTable.insert("set analogDimmingLowValue", set_analogDimmingLowValue);
  m_exeTable.insert("set analogDimmingHighValue", set_analogDimmingHighValue);
  m_exeTable.insert("set analogDimmingOffValue", set_analogDimmingOffValue);
  m_exeTable.insert("get powerMeasurementMode", get_powerMeasurementMode);
  m_exeTable.insert("set powerMeasurementMode", set_powerMeasurementMode);
  m_exeTable.insert("get externalPowerMeter", get_externalPowerMeter);
  m_exeTable.insert("set externalPowerMeter", set_externalPowerMeter);
  m_exeTable.insert("get ambientSensorValue", get_ambientSensorValue);
  m_exeTable.insert("get ambientConfig", get_ambientConfig, parse_get_ambientConfig);
  m_exeTable.insert("set ambientActiveLevel", set_ambientActiveLevel);
  m_exeTable.insert("set ambientInactiveLevel", set_ambientInactiveLevel);
  m_exeTable.insert("set ambientEnvironmentalGain", set_ambientEnvironmentalGain);
  m_exeTable.insert("set ambientOffHysteresis", set_ambientOffHysteresis);
  m_exeTable.insert("set ambientOnHysteresis", set_ambientOnHysteresis);
  m_exeTable.insert("get powerboardProtocol", get_powerboardProtocol);
  m_exeTable.insert("get ledOverride", get_ledOverride);
  m_exeTable.insert("set ledOverride", set_ledOverride);
  m_exeTable.insert("get fadeUpStep", get_fadeUpStep);
  m_exeTable.insert("set fadeUpStep", set_fadeUpStep);
  m_exeTable.insert("get fadeDownStep", get_fadeDownStep);
  m_exeTable.insert("set fadeDownStep", set_fadeDownStep);
  m_exeTable.insert("get maxBrightness", get_maxBrightness);
  m_exeTable.insert("set maxBrightness", set_maxBrightness);
  m_exeTable.insert("get i2cResets", get_i2cResets);
  m_exeTable.insert("get sensorGuardTime", get_sensorGuardTime);
  m_exeTable.insert("set sensorGuardTime", set_sensorGuardTime);
  m_exeTable.insert("get inputVoltage", get_inputVoltage);
  m_exeTable.insert("get inputVoltageCalibration", get_inputVoltageCalibration);
  m_exeTable.insert("set inputVoltageCalibration", set_inputVoltageCalibration);
  m_exeTable.insert("get numLightbars", get_numLightbars);
  m_exeTable.insert("set numLightbars", set_numLightbars);
  m_exeTable.insert("get currentLimit", get_currentLimit);
  m_exeTable.insert("set currentLimit", set_currentLimit);
  m_exeTable.insert("get bootloaderCode", get_bootloaderCode);
  m_exeTable.insert("get xpressMode", get_xpressMode);
  m_exeTable.insert("set xpressMode", set_xpressMode);
  m_exeTable.insert("get batteryBackupStatus", get_batteryBackupStatus, parse_get_batteryBackupStatus);
  m_exeTable.insert("set batteryBackupStatus", set_batteryBackupStatus);
  m_exeTable.insert("get sensorSeconds", get_sensorSeconds);
  m_exeTable.insert("get inputVoltageTwo", get_inputVoltageTwo);
  m_exeTable.insert("get inputVoltageTwoCalibration", get_inputVoltageTwoCalibration);
  m_exeTable.insert("set inputVoltageTwoCalibration", set_inputVoltageTwoCalibration);
  m_exeTable.insert("get maxRampUpSpeed", get_maxRampUpSpeed);
  m_exeTable.insert("set maxRampUpSpeed", set_maxRampUpSpeed);
  m_exeTable.insert("get maxRampDownSpeed", get_maxRampDownSpeed);
  m_exeTable.insert("set maxRampDownSpeed", set_maxRampDownSpeed);
  m_exeTable.insert("get emergencyLightLevel", get_emergencyLightLevel);
  m_exeTable.insert("get batteryBackupPowerCalibration", get_batteryBackupPowerCalibration);
  m_exeTable.insert("set batteryBackupPowerCalibration", set_batteryBackupPowerCalibration);
  m_exeTable.insert("get motionSensorProfile", get_motionSensorProfile);
  m_exeTable.insert("set motionSensorProfile", set_motionSensorProfile);
  m_exeTable.insert("get powerMeterConfig", get_powerMeterConfig, parse_get_powerMeterConfig);
  m_exeTable.insert("set powerMeterLevelAtOff", set_powerMeterLevelAtOff);
  m_exeTable.insert("set powerMeterLevelAtMin", set_powerMeterLevelAtMin);
  m_exeTable.insert("set powerMeterLevelAtMax", set_powerMeterLevelAtMax);
  m_exeTable.insert("set powerMeterType", set_powerMeterType);
  m_exeTable.insert("get DLAiSlaveMode", get_DLAiSlaveMode);
  m_exeTable.insert("set DLAiSlaveMode", set_DLAiSlaveMode);
  m_exeTable.insert("get DALIBootloadingActive", get_DALIBootlodingActive);
  m_exeTable.insert("get testingMode", get_testingMode);
  m_exeTable.insert("set testingMode", set_testingMode);
  m_exeTable.insert("get numBatteriesSupported", get_numBatteriesSupported);
  // get & set lightbar commands
  m_exeTable.insert("get lbVersion", get_lbVersion, parse_get_lbVersion);
  m_exeTable.insert("get lbStatus", get_lbStatus, parse_get_lbStatus);
  m_exeTable.insert("get lbConfig", get_lbConfig, parse_get_lbConfig);
  // get & set battery backup commands
  m_exeTable.insert("get bbVersion", get_bbVersion, parse_get_bbVersion);
  m_exeTable.insert("get bbStatus", get_bbStatus, parse_get_bbStatus);
  m_exeTable.insert("get bbConfig", get_bbConfig, parse_get_bbConfig);
  // reset commands
  m_exeTable.insert("reset usage", reset_usage);
  m_exeTable.insert("reset oldLog", reset_oldLog);
  m_exeTable.insert("reset log", reset_log);
  m_exeTable.insert("reset logIndex", reset_logIndex);
  m_exeTable.insert("reset eeprom", reset_eeprom);
  m_exeTable.insert("reset eepromToDefault", reset_eepromToDefault);
  m_exeTable.insert("reset eepromToLatestMapVersion", reset_eepromToLatestMapVersion);
  m_exeTable.insert("reset network", reset_network);
  m_exeTable.insert("reset networkWithoutChecking", reset_networkWithoutChecking);
  m_exeTable.insert("reset daliCommissioning", reset_daliCommissioning);
  m_exeTable.insert("reset daliPowerMetering", reset_daliPowerMetering);
  // reboot commands
  m_exeTable.insert("reboot pmu", reboot_pmu);
  m_exeTable.insert("reboot wirelessCard", reboot_wirelessCard);
  m_exeTable.insert("reboot i2cDevices", reboot_i2cDevices);
  // reload commands
  m_exeTable.insert("reload dlaFirmware", reload_dlaFirmware);
  m_exeTable.insert("reload wirelessModuleFirmware", reload_wirelessModuleFirmware);
  m_exeTable.insert("reload powerboardFirmware", reload_powerboardFirmware);
  m_exeTable.insert("reload lightbarFirmware", reload_lightbarFirmware);
  m_exeTable.insert("reload batteryBackupFirmware", reload_batteryBackupFirmware);
  m_exeTable.insert("reload motionSensorFirmware", reload_motionSensorFirmware);
  // log commands
  m_exeTable.insert("get logIndex", get_logIndex, parse_get_logIndex);
  //m_cmdTable.insert("get log", get_log, parse_get_log);
  m_exeTable.insert("get log", macro_get_log);
  m_exeTable.insert("insert logEntry", insert_logEntry);
  // build the dictionary of helper commands
  m_cmdCompleter = new QCompleter(m_exeTable.keys(), this);
  m_cmdCompleter->setCaseSensitivity(Qt+CaseInsensitive);
  m_cmdCompleter->setCompletionMode(QCompleter+InlineCompletion);
  // build a dictionary of error responses
  m_errorResponses.insert("ERROR: FFFF", "ERROR: Invalid opcode");
  m_errorResponses.insert("ERROR: FFFE", "ERROR: Syntax error");
  m_errorResponses.insert("ERROR: FFFD", "ERROR: Invalid register");
  m_errorResponses.insert("ERROR: FFFC", "ERROR: Register is read only");
  m_errorResponses.insert("ERROR: FFFB", "ERROR: Invalid register length");
  m_errorResponses.insert("ERROR: FFFA", "ERROR: ARP not addressed");
  m_errorResponses.insert("ERROR: FFF9", "ERROR: Flash error");
  m_errorResponses.insert("ERROR: FFF8", "ERROR: Storage out of bounds");
  m_errorResponses.insert("ERROR: FFF7", "ERROR: Storage unaligned");
  m_errorResponses.insert("ERROR: FFF6", "ERROR: Message queue full");
  m_errorResponses.insert("ERROR: FFF5", "ERROR: I2C error");
  m_errorResponses.insert("ERROR: FFF4", "ERROR: Internal error");
  m_errorResponses.insert("ERROR: FFF3", "ERROR: Insufficient free buffers");
  m_errorResponses.insert("ERROR: FFF2", "ERROR: Bad image");
  m_errorResponses.insert("ERROR: FFF1", "ERROR: Remote install fail");
  m_errorResponses.insert("ERROR: FFF0", "ERROR: Bus error");
  m_errorResponses.insert("ERROR: FFEF", "ERROR: Bus busy");
  m_errorResponses.insert("ERROR: FFEE", "ERROR: Resource busy");
}

struct cmdEntry * cmdHelper::getCmdEntry(QString cmd) {
  return m_exeTable[cmd];
}

QString cmdHelper::getNextCompletion(void) {
  m_cmdCompleter->setCurrentRow(m_cmdCompleter->currentRow() + 1);
  return m_cmdCompleter->currentCompletion();
}

int cmdHelper::getCurrentCompletionLength(void) {
  return m_cmdCompleter->currentCompletion().length();
}

QStringList cmdHelper::help(void) {
  return QStringList() << "COMMAND VERBS:"
                       << "- get, set, reset, reboot, reload"
                       << "REGISTER MODIFIERS:"
                       << "- lb (lightBar), bb (batteryBackup)"
                       << "EXAMPLES:"
                       << "- get firmwareVersion"
                       << "- set serialNumber 04FACE15"
                       << "- reset network"
                       << "- reboot i2cDevices"
                       << "- reload lightbarFirmware"
                       << "- get bbVersion"
                       << "- get lbConfig";
}
