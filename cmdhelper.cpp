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
QStringList build_get_firmwareVersion(QStringList argList) {
  (void) argList;
  return QStringList() << "G0000";
}

QStringList parse_get_firmwareVersion(QStringList pmuResponse) {
  bool ok;
  qulonglong verInt = pmuResponse.at(0).toULongLong(&ok, 16);
  // format verMajor.verMinor.verBuild (buildMonth/buildDay/BuildYear)
  return QStringList() << QString("%1.%2.%3 (%5/%6/%4)").arg((verInt >> 40) & 0xFF).arg((verInt >> 32) & 0xFF).arg((verInt >> 24) & 0xFF).arg((verInt >> 16) & 0xFF).arg((verInt >> 8) & 0xFF).arg(verInt & 0xFF);
}

QStringList build_get_productCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0001";
}

QStringList build_set_productCode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0001 %1").arg(argList.at(0));
}

QStringList build_get_serialNumber(QStringList argList) {
  (void) argList;
  return QStringList() << "G0002";
}

QStringList build_set_serialNumber(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0002 %1").arg(argList.at(0));
}

QStringList build_get_unixTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0003";
}

QStringList build_set_unixTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0003 %1").arg(argList.at(0));
}

QStringList build_get_temperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G0004";
}

QStringList parse_get_temperature(QStringList pmuResponse) {
  bool ok;
  quint16 tInt = pmuResponse.at(0).toUShort(&ok, 16);
  float tFloat = tInt / 128;
  return QStringList() << QString("%1 C").arg(tFloat);
}

QStringList build_get_lightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G001C" // current level
                       << "G0005" // light manual level
                       << "G0006" // light active level
                       << "G0007" // light inactive level
                       << "G0008" // light overrirde active level
                       << "G0009"; // light override inactive level
}

QStringList parse_get_lightLevel(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Current level: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Manual level: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Active level: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Inactive level: %1").arg(pmuResponse.at(3));
  parsedResponse << QString("Override active level: %1").arg(pmuResponse.at(4));
  parsedResponse << QString("Override inactive level: %1").arg(pmuResponse.at(5));
  return parsedResponse;
}

QStringList build_set_lightManualLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0005 %1").arg(argList.at(0));
}

QStringList build_set_lightOverrideActiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0008 %1").arg(argList.at(0));
}

QStringList build_set_lightOverrideInactiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0009 %1").arg(argList.at(0));
}

QStringList help_lightOverrideInactiveLevel(void) {
  return QStringList() << "0009 Light Override Inactive Level (FFFF is no override) -- 2B -- RW";
}

QStringList build_get_sensorDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000A";
}

QStringList build_get_sensorOverrideDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000B";
}

QStringList build_set_sensorOverrideDelayTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S000B %1").arg(argList.at(0));
}

QStringList build_get_upTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000C";
}

QStringList parse_get_upTime(QStringList pmuResponse) {
  return QStringList() << toYDHMS(pmuResponse.at(0));
}

QStringList build_get_usage(QStringList argList) {
  (void) argList;
  return QStringList() << "G000C" // uptime
                       << "G000D" // active seconds
                       << "G000E" // inactive seconds
                       << "G000F" // perm active seconds
                       << "G0010" // perm inactive seconds
                       << "G0011" // Wh
                       << "G0012" // perm Wh
                       << "G0013" // sensor events
                       << "G0014"; // perm sensor events
}

QStringList parse_get_usage(QStringList pmuResponse) {
  bool ok;
  QStringList parsedResponse;
  parsedResponse << QString("Up time: %1").arg(toYDHMS(pmuResponse.at(0)));
  parsedResponse << QString("Active time: %1").arg(toYDHMS(pmuResponse.at(1)));
  parsedResponse << QString("Inactive time: %1").arg(toYDHMS(pmuResponse.at(2)));
  parsedResponse << QString("Perm active time: %1").arg(toYDHMS(pmuResponse.at(3)));
  parsedResponse << QString("Perm inactive time: %1").arg(toYDHMS(pmuResponse.at(4)));
  parsedResponse << QString("Power: %1 Wh").arg(pmuResponse.at(5).toUShort(&ok, 16));
  parsedResponse << QString("Perm power: %1 Wh").arg(pmuResponse.at(6).toUShort(&ok, 16));
  parsedResponse << QString("Sensor events: %1").arg(pmuResponse.at(7).toUShort(&ok, 16));
  parsedResponse << QString("Perm sensor events: %1").arg(pmuResponse.at(8).toUShort(&ok, 16));
  return parsedResponse;
}

QStringList build_get_numLogEntries(QStringList argList) {
  (void) argList;
  return QStringList() << "G0015";
}

QStringList build_get_configCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0016" // P0
                       << "G0017" // P1
                       << "G0018" // P2
                       << "G0019"; // P3
}

QStringList parse_get_configCalibration(QStringList pmuReponse) {
  QStringList parsedResponse;
  parsedResponse << QString("P0: %1").arg(pmuReponse.at(0));
  parsedResponse << QString("P1: %1").arg(pmuReponse.at(1));
  parsedResponse << QString("P2: %1").arg(pmuReponse.at(2));
  parsedResponse << QString("P3: %1").arg(pmuReponse.at(3));
  return parsedResponse;
}

QStringList build_set_configCalibrationP0(QStringList argList) {
  return QStringList() << QString("S0016 %1").arg(argList.at(0));
}

QStringList build_set_configCalibrationP1(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0017 %1").arg(argList.at(0));
}

QStringList build_set_configCalibrationP2(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0018 %1").arg(argList.at(0));
}

QStringList build_set_configCalibrationP3(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0019 %1").arg(argList.at(0));
}

QStringList build_get_buildTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G001A";
}

QStringList build_set_buildTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S001A %1").arg(argList.at(0));
}

QStringList build_get_sensorTimeoutCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G001B";
}

QStringList build_get_currentLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G001C";
}

QStringList build_get_safeMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G001D";
}

QStringList build_get_lightBarSelect(QStringList argList) {
  (void) argList;
  return QStringList() << "G001E";
}

QStringList build_set_lightBarSelect(QStringList argList) {
  return QStringList() << QString("S001E %1").arg(argList.at(0));
}

QStringList build_get_powerConsumption(QStringList argList) {
  (void) argList;
  return QStringList() << "G001F";
}

QStringList parse_get_powerConsumption(QStringList pmuResponse) {
  bool ok;
  quint16 powerInt = pmuResponse.at(0).toUShort(&ok, 16);
  return QStringList() << QString("%1 mW").arg(powerInt);
}

QStringList build_get_wirelessDataAggregator(QStringList argList) {
  (void) argList;
  return QStringList() << "G0020";
}

QStringList build_set_wirelessDataAggregator(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0020 %1").arg(argList.at(0));
}

QStringList build_get_resetUsageTimestamp(QStringList argList) {
  (void) argList;
  return QStringList() << "G0021";
}

QStringList build_get_pwmPeriodRegister(QStringList argList) {
  (void) argList;
  return QStringList() << "G0022";
}

QStringList build_set_pwmPeriodRegister(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0022 %1").arg(argList.at(0));
}

QStringList build_get_analogSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0023";
}

QStringList build_get_analogReportingHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G0024";
}

QStringList build_get_zone(QStringList argList) {
  (void) argList;
  return QStringList() << "G0025";
}

QStringList build_set_zone(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0025 %1").arg(argList.at(0));
}

QStringList build_get_lightTemporaryActiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0026";
}

QStringList build_set_lightTemporaryActiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0026 %1").arg(argList.at(0));
}

QStringList build_get_lightTemporaryInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0027";
}

QStringList build_set_lightTemporaryInactiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0027 %1").arg(argList.at(0));
}

QStringList build_get_sensorTemporaryDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0028";
}

QStringList build_set_sensorTemporaryDealyTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0028 %1").arg(argList.at(0));
}

QStringList build_get_temporaryOverrideTimeout(QStringList argList) {
  (void) argList;
  return QStringList() << "G0029";
}

QStringList build_set_temporaryOverrideTiemout(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0029 %1").arg(argList.at(0));
}

QStringList build_get_setRemoteState(QStringList argList) {
  (void) argList;
  return QStringList() << "G002A";
}

QStringList build_set_setRemoteState(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S002A %1").arg(argList.at(0));
}

QStringList build_get_remoteStateDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G002B";
}

QStringList build_set_remoteStateDelayTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S002B %1").arg(argList.at(0));
}

QStringList build_get_remoteSecondsCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G002C";
}

QStringList build_get_minimumDimmingValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G002D";
}

QStringList build_get_powerCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << QString("G002E")
                       << QString("G002F")
                       << QString("G0030")
                       << QString("G0031")
                       << QString("G0032")
                       << QString("G0033")
                       << QString("G0034")
                       << QString("G0035")
                       << QString("G0036");
}

QStringList parse_get_powerCalibration(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("A0: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("B0: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("C0: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("MA: %1").arg(pmuResponse.at(3));
  parsedResponse << QString("MB: %1").arg(pmuResponse.at(4));
  parsedResponse << QString("MC: %1").arg(pmuResponse.at(5));
  parsedResponse << QString("POff: %1").arg(pmuResponse.at(6));
  parsedResponse << QString("POn: %1").arg(pmuResponse.at(7));
  parsedResponse << QString("T0: %1").arg(pmuResponse.at(8));
  return parsedResponse;
}

QStringList build_set_powerCalibrationA0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S002E %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationB0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S002F %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationC0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0030 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationMA(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0031 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationMB(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0032 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationMC(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0033 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationPOff(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0034 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationPOn(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0035 %1").arg(argList.at(0));
}

QStringList build_set_powerCalibrationT0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0036 %1").arg(argList.at(0));
}

QStringList build_get_powerEstimatorTemperatureOverride(QStringList argList) {
  (void) argList;
  return QStringList() << "G0037";
}

QStringList build_set_powerEstimatorTemperatureOverride(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0037 %1").arg(argList.at(0));
}

QStringList build_get_cachedTemperatureValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0038";
}

QStringList build_get_eepromSize(QStringList argList) {
  (void) argList;
  return QStringList() << "G0039";
}

QStringList build_get_hardwareRevision(QStringList argList) {
  (void) argList;
  return QStringList() << "G003A";
}

QStringList build_get_wirelessConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G003B" // pan id
                       << "G003C" // channel mask
                       << "G003D" // short address
                       << "G003E" // role
                       << "G003F" // watchdog hold
                       << "G0040" // watchdog period
                       << "G0073"; // network key
}

QStringList parse_get_wirelessConfig(QStringList pmuResponse) {
  bool ok;
  unsigned long long panid = pmuResponse.at(0).toULongLong(&ok, 16);
  unsigned long chmask = pmuResponse.at(1).toULong(&ok, 16);
  unsigned group;
  unsigned freq;
  bool encrypted;
  LRNetwork::groupAndFreqFromPanidAndChmask(panid, chmask, &group, &freq, &encrypted);
  QString netId = LRNetwork::nwidFromGroupAndFreq(group, freq, encrypted);
  QStringList parsedResponse;
  parsedResponse << QString("Network ID: %1").arg(netId);
  parsedResponse << QString("Pan ID: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Channel mask: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Short address: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Role: %1").arg(pmuResponse.at(3));
  parsedResponse << QString("Watchdog hold: %1").arg(pmuResponse.at(4));
  parsedResponse << QString("Watchdog period: %1").arg(pmuResponse.at(5));
  return parsedResponse << QString("Network key: %1").arg(pmuResponse.at(6));
}

QStringList build_set_wirelessPanId(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003B %1").arg(argList.at(0));
}

QStringList build_set_wirelessChannelMask(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003C %1").arg(argList.at(0));
}

QStringList build_set_wirelessShortAddress(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003D %1").arg(argList.at(0));
}

QStringList build_set_wirelessRole(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003E %1").arg(argList.at(0));
}

QStringList build_set_wirelessWatchdogHold(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003F %1").arg(argList.at(0));
}

QStringList build_set_wirelessWatchdogPeriod(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0040 %1").arg(argList.at(0));
}

QStringList build_set_wirelessNetworkKey(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0073 %1").arg(argList.at(0));
}

QStringList build_get_firmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0041";
}

QStringList build_get_moduleFirmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0042";
}

QStringList build_get_maxTemperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G0043" // observed temperature
                       << "G0044"; // observed time
}

QStringList parse_get_maxTemperature(QStringList pmuResponse) {
  return QStringList() << QString("%1 at time %2").arg(pmuResponse.at(0)).arg(toYDHMS(pmuResponse.at(1)));
}

QStringList build_get_overTemperatureConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G0045" // low threshold
                       << "G0046" // high threshold
                       << "G0047"; // dimming limit
}

QStringList parse_get_overTemperatureConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Low threshold: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("High threshold: %1").arg(pmuResponse.at(1));
  return parsedResponse << QString("Dimming limit: %1").arg(pmuResponse.at(2));
}

QStringList build_set_overTemperatureThresholdLow(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0045 %1").arg(argList.at(0));
}

QStringList build_get_overTemperatureThresholdHigh(QStringList argList) {
  (void) argList;
  return QStringList() << "G0046";
}

QStringList build_set_overTemperatureThresholdHigh(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0046 %1").arg(argList.at(0));
}

QStringList build_set_overTemperatureDimmingLimit(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0047 %1").arg(argList.at(0));
}

QStringList build_get_analogDimmingMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0048";
}

QStringList parse_get_analogDimmingMode(QStringList pmuResponse) {
  QMap <QString, QString> analogDimmingModeDict;
  analogDimmingModeDict.insert("00", "Analog dimming off");
  analogDimmingModeDict.insert("01", "Analog dimming on");
  analogDimmingModeDict.insert("02", "Analog dimming on with ability to go full off");
  analogDimmingModeDict.insert("03", "Analog dimming using registers 54-56");
  analogDimmingModeDict.insert("04", "Analog dimming using registers 54-56 with full off support");
  analogDimmingModeDict.insert("05", "Ambient sensor dimming");
  return QStringList() << QString("%1").arg(analogDimmingModeDict[pmuResponse.at(0)]);
}

QStringList build_set_analogDimmingMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0048 %1").arg(argList.at(0));
}

QStringList build_get_fixtureIdMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0049";
}

QStringList build_set_fixtureIdMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0049 %1").arg(argList.at(0));
}

QStringList build_get_acFrequency(QStringList argList) {
  (void) argList;
  return QStringList() << "G004A";
}

QStringList build_get_sensorBits(QStringList argList) {
  (void) argList;
  return QStringList() << "G004B";
}

QStringList build_get_powerMeterCommand(QStringList argList) {
  (void) argList;
  return QStringList() << "G004C";
}

QStringList build_set_powerMeterCommand(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S004C %1").arg(argList.at(0));
}

QStringList build_get_powerMeterRegister(QStringList argList) {
  (void) argList;
  return QStringList() << "G004D";
}

QStringList build_set_powerMeterRegister(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S004D %1").arg(argList.at(0));
}

QStringList build_get_ambientTemperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G004E";
}

QStringList build_get_lightSensorLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G004F";
}

QStringList build_get_sensorConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G004F" // sensor level
                       << "G0050" // sensor 0 timeout
                       << "G0051" // sensor 0 offset
                       << "G0052" // sensor 1 timeout
                       << "G0053"; // sensor 1 offset
}

QStringList parse_get_sensorConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Sensor level: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Sensor 0 timeout: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Sensor 0 offset: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Sensor 1 timeout: %1").arg(pmuResponse.at(3));
  return parsedResponse << QString("Sensor 1 offset: %1").arg(pmuResponse.at(4));
}

QStringList build_set_sensor0Timeout(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0050 %1").arg(argList.at(0));
}

QStringList build_get_sensor0Offset(QStringList argList) {
  (void) argList;
  return QStringList() << "G0051";
}

QStringList build_set_sensor0Offset(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0051 %1").arg(argList.at(0));
}

QStringList build_get_sensor1Timeout(QStringList argList) {
  (void) argList;
  return QStringList() << "G0052";
}

QStringList build_set_sensor1Timeout(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0052 %1").arg(argList.at(0));
}

QStringList build_get_sensor1Offset(QStringList argList) {
  (void) argList;
  return QStringList() << "G0053";
}

QStringList build_set_sensor1Offset(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0053 %1").arg(argList.at(0));
}

QStringList build_get_analogDimmingConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G0054" // low value
                       << "G0055" // high value
                       << "G0056"; // off value
}

QStringList parse_get_analogDimmingConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Low value: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("High value: %1").arg(pmuResponse.at(1));
  return parsedResponse << QString("Off value: %1").arg(pmuResponse.at(2));
}

QStringList build_set_analogDimmingLowValue(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0054 %1").arg(argList.at(0));
}

QStringList build_get_analogDimmingHighValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0055";
}

QStringList build_set_analogDimmingHighValue(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0055 %1").arg(argList.at(0));
}

QStringList build_get_analogDimmingOffValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0056";
}

QStringList build_set_analogDimmingOffValue(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0056 %1").arg(argList.at(0));
}

QStringList build_get_powerMeasurementMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0057";
}

QStringList build_set_powerMeasurementMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0057 %1").arg(argList.at(0));
}

QStringList build_get_externalPowerMeter(QStringList argList) {
  (void) argList;
  return QStringList() << "G0058";
}

QStringList build_set_externalPowerMeter(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0058 %1").arg(argList.at(0));
}

QStringList build_get_ambientSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0059";
}

QStringList build_get_ambientConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G0059" // sensor value
                       << "G005A" // active level
                       << "G005B" // inactive level
                       << "G005C" // environmental gain
                       << "G005D" // off hysteresis
                       << "G005E" // on hysteresis
                       << "G0069"; // divisor
}

QStringList parse_get_ambientConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Sensor value: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Active level: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Inactive level: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Environmental gain: %1").arg(pmuResponse.at(3));
  parsedResponse << QString("Off hysteresis: %1").arg(pmuResponse.at(4));
  parsedResponse << QString("On hysteresis: %1").arg(pmuResponse.at(5));
  return parsedResponse << QString("Divisor: %1").arg(pmuResponse.at(6));
}

QStringList build_set_ambientActiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S005A %1").arg(argList.at(0));
}

QStringList build_get_ambientInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G005B";
}

QStringList build_set_ambientInactiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S005B %1").arg(argList.at(0));
}

QStringList build_get_ambientEnvironmentalGain(QStringList argList) {
  (void) argList;
  return QStringList() << "G005C";
}

QStringList build_set_ambientEnvironmentalGain(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S005C %1").arg(argList.at(0));
}

QStringList build_get_ambientOffHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G005D";
}

QStringList build_set_ambientOffHysteresis(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S005D %1").arg(argList.at(0));
}

QStringList build_get_ambientOnHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G005E";
}

QStringList build_set_ambientOnHysteresis(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S005E %1").arg(argList.at(0));
}

QStringList build_get_powerboardProtocol(QStringList argList) {
  (void) argList;
  return QStringList() << "G005F";
}

QStringList build_get_ledOverride(QStringList argList) {
  (void) argList;
  return QStringList() << "G0060";
}

QStringList build_set_ledOverride(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0060 %1").arg(argList.at(0));
}

QStringList build_get_fadeUpStep(QStringList argList) {
  (void) argList;
  return QStringList() << "G0061";
}

QStringList build_set_fadeUpStep(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0061 %1").arg(argList.at(0));
}

QStringList build_get_fadeDownStep(QStringList argList) {
  (void) argList;
  return QStringList() << "G0062";
}

QStringList build_set_fadeDownStep(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0062 %1").arg(argList.at(0));
}

QStringList build_get_maxBrightness(QStringList argList) {
  (void) argList;
  return QStringList() << "G0063";
}

QStringList build_set_maxBrightness(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0063 %1").arg(argList.at(0));
}

QStringList build_get_i2cResets(QStringList argList) {
  (void) argList;
  return QStringList() << "G0064";
}

QStringList build_get_sensorGuardTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0065";
}

QStringList build_set_sensorGuardTime(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0065 %1").arg(argList.at(0));
}

QStringList build_get_inputVoltage(QStringList argList) {
  (void) argList;
  return QStringList() << "G0066";
}

QStringList build_get_inputVoltageCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0067";
}

QStringList build_set_inputVoltageCalibration(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0067 %1").arg(argList.at(0));
}

QStringList build_get_numLightbars(QStringList argList) {
  (void) argList;
  return QStringList() << "G0068";
}

QStringList build_set_numLightbars(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0068 %1").arg(argList.at(0));
}

QStringList build_get_currentLimit(QStringList argList) {
  (void) argList;
  return QStringList() << "G006A";
}

QStringList build_set_currentLimit(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S006A %1").arg(argList.at(0));
}

QStringList build_get_bootloaderCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G006B";
}

QStringList build_get_xpressMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G006C";
}

QStringList build_set_xpressMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S006C %1").arg(argList.at(0));
}

QStringList build_get_batteryBackupStatus(QStringList argList) {
  (void) argList;
  return QStringList() << "G006D";
}

QStringList parse_get_batteryBackupStatus(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  QMap <int, QString> battDetectedDict;
  QMap <int, QString> testReportDict;
  QMap <int, QString> testRunningDict;
  quint32 status = pmuResponse.at(0).toLong(&ok, 16);
  // parse batteries detected bits
  battDetectedDict.insert(0, "No batteries detected");
  battDetectedDict.insert(1, "Battery 1 detected");
  battDetectedDict.insert(2, "Battery 2 detected");
  battDetectedDict.insert(3, "Batteries 1 & 2 detected");
  parsedResponse += (battDetectedDict[status & 0x3] + "<br>");
  // parse test running bits
  testRunningDict.insert(0, "No tests running");
  testRunningDict.insert(1, "Short test running");
  testRunningDict.insert(2, "Long test running");
  testRunningDict.insert(4, "Push button test running");
  parsedResponse += (testRunningDict[(status >> 10) & 0x3] + "<br>");
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
  parsedResponse += "Battery 1 test report: ";
  parsedResponse += (testReportDict[(status >> 2) & 0xF] + "<br>");
  parsedResponse += "Battery 2 test report: ";
  parsedResponse += (testReportDict[(status >> 6) & 0xF] + "<br>");
  // parse test time
  parsedResponse += "Test time: ";
  parsedResponse += QString("%1 seconds").arg(status >> 16);
  return QStringList() << parsedResponse;
}

QStringList build_set_batteryBackupStatus(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S006D %1").arg(argList.at(0));
}

QStringList build_get_sensorSeconds(QStringList argList) {
  (void) argList;
  return QStringList() << "G006E";
}

QStringList build_get_inputVoltageTwo(QStringList argList) {
  (void) argList;
  return QStringList() << "G006F";
}

QStringList build_get_inputVoltageTwoCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0070";
}

QStringList build_set_inputVoltageTwoCalibration(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0070 %1").arg(argList.at(0));
}

QStringList build_get_maxRampUpSpeed(QStringList argList) {
  (void) argList;
  return QStringList() << "G0071";
}

QStringList build_set_maxRampUpSpeed(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0071 %1").arg(argList.at(0));
}

QStringList build_get_maxRampDownSpeed(QStringList argList) {
  (void) argList;
  return QStringList() << "G0072";
}

QStringList build_set_maxRampDownSpeed(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0072 %1").arg(argList.at(0));
}

QStringList build_get_emergencyLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0074";
}

QStringList build_get_batteryBackupPowerCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0075";
}

QStringList build_set_batteryBackupPowerCalibration(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0075 %1").arg(argList.at(0));
}

QStringList build_get_motionSensorProfile(QStringList argList) {
  (void) argList;
  return QStringList() << "G0076";
}

QStringList build_set_motionSensorProfile(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0076 %1").arg(argList.at(0));
}

QStringList build_get_powerMeterConfig(QStringList argList) {
  (void) argList;
  return QStringList() << "G0077" // level at off
                       << "G0078" // level at min
                       << "G0079" // level at max
                       << "G007A"; // type
}

QStringList parse_get_powerMeterConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Level at off: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Level at min: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Level at max: %1").arg(pmuResponse.at(2));
  return parsedResponse << QString("Type: %1").arg(pmuResponse.at(3));
}

QStringList build_set_powerMeterLevelAtOff(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0077 %1").arg(argList.at(0));
}

QStringList build_set_powerMeterLevelAtMin(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0078 %1").arg(argList.at(0));
}

QStringList build_set_powerMeterLevelAtMax(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0079 %1").arg(argList.at(0));
}

QStringList build_set_powerMeterType(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007A %1").arg(argList.at(0));
}

QStringList build_get_DLAiSlaveMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G007B";
}

QStringList build_set_DLAiSlaveMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007B %1").arg(argList.at(0));
}

QStringList build_get_DALIBootlodingActive(QStringList argList) {
  (void) argList;
  return QStringList() << "G007C";
}

QStringList build_get_testingMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G007D";
}

QStringList build_set_testingMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007D %1").arg(argList.at(0));
}

QStringList build_get_numBatteriesSupported(QStringList argList) {
  (void) argList;
  return QStringList() << "G007E";
}

QStringList build_set_numBatteriesSupported(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007E %1").arg(argList.at(0));
}

/*** lightbar register commands ***/
QStringList build_get_lbVersion(QStringList argList) {
  QString barNum;
  QStringList cmdList;
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
  return cmdList;
}

QStringList parse_get_lbVersion(QStringList pmuResponse) {
  bool ok;
  QStringList parsedResponse;
  quint16 verHiInt = pmuResponse.at(3).toUShort(&ok, 16);
  quint16 verLoInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse << QString("Firmware version: %1.%2.%3").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  parsedResponse << QString("Firmware code: %1%2").arg(pmuResponse.at(1)).arg(pmuResponse.at(2));
  parsedResponse << QString("Protocol version: %1").arg(pmuResponse.at(0));
  return parsedResponse;
}

QStringList build_get_lbStatus(QStringList argList) {
  QString barNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    barNum = "00";
  } else if (argList.length() == 1) {
   barNum = argList.at(0);
  } else {
    cmdList << "ERROR: expected bar number<br>";
    cmdList << "Example: get lbStatus 00";
    return cmdList;
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
  return cmdList;
}

QStringList parse_get_lbStatus(QStringList pmuResponse) {
  bool ok;
  quint16 statusInt = pmuResponse.at(0).toUShort(&ok, 16);
  QStringList parsedResponse;
  if (statusInt & 4) {
    parsedResponse << "Bypass: activate";
  } else {
    parsedResponse << "Bypass: inactive";
  }
  statusInt = pmuResponse.at(1).toUShort(&ok, 16);
  parsedResponse << QString("String 1 current: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(2).toUShort(&ok, 16);
  parsedResponse << QString("String 2 current: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(3).toUShort(&ok, 16);
  parsedResponse << QString("String 3 current: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse << QString("String 4 current: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(5).toUShort(&ok, 16);
  parsedResponse << QString("String current minimum: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(6).toUShort(&ok, 16);
  parsedResponse << QString("Temperature: %1 C").arg((125 * statusInt / 1024) - 40);
  statusInt = pmuResponse.at(7).toUShort(&ok, 16);
  parsedResponse << QString("String current sum: %1 mA").arg(1.4 * statusInt);
  statusInt = pmuResponse.at(8).toUShort(&ok, 16);
  parsedResponse << QString("Voltage reference: %1 volts").arg(2.5 * statusInt / 1024);
  parsedResponse << QString("Light level (0x029C = OFF): %1").arg(pmuResponse.at(9));
  parsedResponse << QString("Light active slew rate: %1").arg(pmuResponse.at(10));
  parsedResponse << QString("Light inactive slew rate: %1").arg(pmuResponse.at(11));
  return parsedResponse;
}

QStringList build_get_lbConfig(QStringList argList) {
  QString barNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    barNum = "00";
  } else if (argList.length() == 1) {
   barNum = argList.at(0);
  } else {
    cmdList << "ERROR: expected bar number<br>";
    cmdList << "Example: get lbConfig 00";
    return cmdList;
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
  return cmdList;
}

QStringList parse_get_lbConfig(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Hardware revision: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Temperature calibration: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("LED device type: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Serial number: %1%2").arg(pmuResponse.at(3)).arg(pmuResponse.at(4));
  parsedResponse << QString("Current sense bypass threshold: %1").arg(pmuResponse.at(5));
  parsedResponse << QString("Current sense bypass hysteresis: %1").arg(pmuResponse.at(6));
  parsedResponse << QString("Estimator current sense coefficient: %1").arg(pmuResponse.at(7));
  parsedResponse << QString("Estimator current sense exponent: %1").arg(pmuResponse.at(8));
  parsedResponse << QString("Bypass override temperature: %1").arg(pmuResponse.at(9));
  parsedResponse << QString("Temperature throttle limit: %1").arg(pmuResponse.at(10));
  return parsedResponse;
}

/*** battery backup register commands ***/
QStringList build_get_bbVersion(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  } else {
    cmdList << "ERROR: expected battery number<br>";
    cmdList << "Example: get bbVersion 00";
    return cmdList;
  }
  cmdList << QString("R%1%2").arg(battNum).arg("00"); // protocol version
  cmdList << QString("R%1%2").arg(battNum).arg("01"); // firmware code high
  cmdList << QString("R%1%2").arg(battNum).arg("02"); // firmware code low
  cmdList << QString("R%1%2").arg(battNum).arg("03"); // firmware version high
  cmdList << QString("R%1%2").arg(battNum).arg("04"); // firmware version low
  return cmdList;
}

QStringList parse_get_bbVersion(QStringList pmuResponse) {
  bool ok;
  QStringList parsedResponse;
  quint16 verHiInt = pmuResponse.at(3).toUShort(&ok, 16);
  quint16 verLoInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse << QString("Firmware version: %1.%2.%3").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  parsedResponse << QString("Firmware code: %1%2").arg(pmuResponse.at(1)).arg(pmuResponse.at(2));
  parsedResponse << QString("Protocol version: %1").arg(pmuResponse.at(0));
  return parsedResponse;
}

QStringList build_get_bbStatus(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  } else {
    cmdList << "ERROR: expected battery number<br>";
    cmdList << "Example: get bbVersion 00";
    return cmdList;
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
  return cmdList;
}

QStringList parse_get_bbStatus(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  QMap <int, QString> statusDict;
  quint32 statusInt = pmuResponse.at(0).toLong(&ok, 16);
  // parse mode bits
  statusDict.insert(0, "Invalid");
  statusDict.insert(1, "Charging");
  statusDict.insert(2, "Standby");
  statusDict.insert(3, "Shutdown");
  statusDict.insert(4, "Error");
  statusDict.insert(5, "Emergency");
  statusDict.insert(6, "Test");
  statusDict.insert(7, "Powerdown");
  parsedResponse += QString("Mode: %1<br>").arg(statusDict[statusInt & 0xF]);
  // parse battery status bits
  statusDict.clear();
  statusDict.insert(0, "Good");
  statusDict.insert(1, "Disconnected");
  statusDict.insert(2, "Fully charged");
  statusDict.insert(3, "Fully discharged");
  statusDict.insert(4, "Needs charge");
  statusDict.insert(5, "Needs powerdown");
  parsedResponse += QString("Battery status: %1<br>").arg(statusDict[(statusInt >> 4) & 0xF]);
  // parse temperature status bits
  statusDict.clear();
  statusDict.insert(0, "Good");
  statusDict.insert(1, "Charge limit exceeded");
  statusDict.insert(2, "Emergency limit exceeded");
  parsedResponse += QString("Temperature status: %1<br>").arg(statusDict[(statusInt >> 8) & 0x3]);
  // parse lightbar voltage status bits
  statusDict.clear();
  statusDict.insert(0, "Good");
  statusDict.insert(1, "Exceeded limits");
  parsedResponse += QString("Lightbar voltage status: %1<br>").arg(statusDict[(statusInt >> 10) & 0x1]);
  // parse lightbar current status bits
  statusDict.clear();
  statusDict.insert(0, "Good");
  statusDict.insert(1, "Exceeded limits");
  parsedResponse += QString("Lightbar current status: %1<br>").arg(statusDict[(statusInt >> 11) & 0x1]);
  // parse test button status bits
  statusDict.clear();
  statusDict.insert(0, "Released");
  statusDict.insert(1, "Pressed");
  parsedResponse += QString("Test button status: %1<br>").arg(statusDict[(statusInt >> 12) & 0x1]);
  // parse PSU status bits
  statusDict.clear();
  statusDict.insert(0, "42v on");
  statusDict.insert(1, "42v off");
  parsedResponse += QString("PSU status: %1<br>").arg(statusDict[(statusInt >> 13) & 0x1]);
  // parse certification mark status
  statusDict.clear();
  statusDict.insert(0, "UL");
  statusDict.insert(1, "CE");
  parsedResponse += QString("Certification mark: %1<br>").arg(statusDict[(statusInt >> 15) & 0x1]);
  // other regs
  parsedResponse += QString("Battery voltage: %1 volts<br>").arg(0.04 * pmuResponse.at(1).toUShort(&ok, 16));
  parsedResponse += QString("Battery temperature: %1 C<br>").arg(0.125 * (pmuResponse.at(2).toUShort(&ok, 16) - 164));
  parsedResponse += QString("Lightbar supply voltage: %1 volts<br>").arg(0.05 * pmuResponse.at(3).toUShort(&ok, 16));
  parsedResponse += QString("Lightbar PSU current: %1 mA<br>").arg(2.44 * pmuResponse.at(4).toUShort(&ok, 16));
  statusDict.clear();
  statusDict.insert(0, "None");
  statusDict.insert(1, "Battery voltage crossed max limit");
  statusDict.insert(2, "Battery voltage crossed recharge limit");
  parsedResponse += QString("Alarms: %1<br>").arg(statusDict[pmuResponse.at(5).toUShort(&ok, 16)]);
  parsedResponse += QString("Time to mode change: %1 minutes<br>").arg(pmuResponse.at(6).toUShort(&ok, 16));
  parsedResponse += QString("Uptime: %1 hours, %2 minutes<br>").arg(pmuResponse.at(8).toUShort(&ok, 16)).arg(pmuResponse.at(7).toUShort(&ok, 16));
  parsedResponse += QString("Error count: %1").arg(pmuResponse.at(9).toUShort(&ok, 16));
  return QStringList() << parsedResponse;
}

QStringList build_get_bbConfig(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum = "C2";
    }
  } else {
    cmdList << "ERROR: expected battery number<br>";
    cmdList << "Example: get bbConfig 00";
    return cmdList;
  }
  cmdList << QString("R%1%2").arg(battNum).arg("82"); // hardware rev
  cmdList << QString("R%1%2").arg(battNum).arg("83"); // temperature cal
  cmdList << QString("R%1%2").arg(battNum).arg("84"); // serial number low word
  cmdList << QString("R%1%2").arg(battNum).arg("85"); // serial number high word
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
  return cmdList;
}

QStringList parse_get_bbConfig(QStringList pmuResponse) {
  bool ok;
  QStringList parsedResponse;
  parsedResponse << QString("Hardware revision: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Temperature calibration: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Serial number: %1%2").arg(pmuResponse.at(2)).arg(pmuResponse.at(3));
  parsedResponse << QString("Charge time: %1 minutes").arg(pmuResponse.at(4).toUShort(&ok, 16));
  parsedResponse << QString("Trickle time: %1 minutes").arg(pmuResponse.at(5).toUShort(&ok, 16));
  parsedResponse << QString("Standby time: %1 minutes").arg(pmuResponse.at(6).toUShort(&ok, 16));
  parsedResponse << QString("Shutdown time: %1 minutes").arg(pmuResponse.at(16).toUShort(&ok, 16));
  parsedResponse << QString("Max battery voltage: %1 volts").arg(pmuResponse.at(7).toUShort(&ok, 16) * 0.04);
  parsedResponse << QString("Min battery voltage: %1 volts").arg(pmuResponse.at(8).toUShort(&ok, 16) * 0.04);
  parsedResponse << QString("Recharge battery voltage: %1 volts").arg(pmuResponse.at(9).toUShort(&ok, 16) * 0.04);
  parsedResponse << QString("Max charge temperature: %1 C").arg((pmuResponse.at(10).toUShort(&ok, 16) - 164) * 0.125);
  parsedResponse << QString("Max emergency temperature: %1 C").arg((pmuResponse.at(11).toUShort(&ok, 16) - 164) * 0.125);
  parsedResponse << QString("Min emergency verify voltage: %1 volts").arg(pmuResponse.at(12).toUShort(&ok, 16) * 0.05);
  parsedResponse << QString("Max emergency verify voltage: %1 volts").arg(pmuResponse.at(13).toUShort(&ok, 16) * 0.05);
  parsedResponse << QString("Max lightbar PSU current: %1 mA").arg(pmuResponse.at(14).toUShort(&ok, 16) * 2.44);
  parsedResponse << QString("Certification mark: %1").arg((pmuResponse.at(15) == "0000") ? "UL" : "CE");
  parsedResponse << QString("Product code: %1%2").arg(pmuResponse.at(17)).arg(pmuResponse.at(18));
  return parsedResponse;
}

/*** reset commands ***/
QStringList build_reset_usage(QStringList argList) {
  (void) argList;
  return QStringList() << "!U";
}

QStringList build_reset_oldLog(QStringList argList) {
  (void) argList;
  return QStringList() << "!L";
}

QStringList build_reset_log(QStringList argList) {
  (void) argList;
  return QStringList() << "!K";
}

QStringList build_reset_logIndex(QStringList argList) {
  return QStringList() << QString("J%1").arg(argList.at(0));
}

QStringList build_reset_eeprom(QStringList argList) {
  (void) argList;
  return QStringList() << "!Z";
}

QStringList build_reset_eepromToDefault(QStringList argList) {
  (void) argList;
  return QStringList() << "!C";
}

QStringList build_reset_eepromToLatestMapVersion(QStringList argList) {
  (void) argList;
  return QStringList() << "!E";
}

QStringList build_reset_network(QStringList argList) {
  (void) argList;
  return QStringList() << "!N";
}

QStringList build_reset_networkWithoutChecking(QStringList argList) {
  (void) argList;
  return QStringList() << "!N1";
}

QStringList build_reset_daliCommissioning(QStringList argList) {
  (void) argList;
  return QStringList() << "!Y";
}

QStringList build_reset_daliPowerMetering(QStringList argList) {
  (void) argList;
  return QStringList() << "!A";
}

/*** reboot commands ***/
QStringList build_reboot_pmu(QStringList argList) {
  (void) argList;
  return QStringList() << "!R";
}

QStringList build_reboot_wirelessCard(QStringList argList) {
  (void) argList;
  return QStringList() << "!W";
}

QStringList build_reboot_i2cDevices(QStringList argList) {
  (void) argList;
  return QStringList() << "!X";
}

/*** reload commands ***/
QStringList build_reload_dlaFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!B";
}

QStringList build_reload_wirelessModuleFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!M";
}

QStringList build_reload_powerboardFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList build_reload_lightbarFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList build_reload_batteryBackupFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList build_reload_motionSensorFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!V";
}

/*** log commands ***/
QStringList build_get_logIndex(QStringList argList) {
  (void) argList;
  return QStringList() << "K";
}

QStringList parse_get_logIndex(QStringList pmuResponse) {
  QString arg1 = pmuResponse.at(0);
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

QStringList build_get_log(QStringList argList) {
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

QStringList parse_get_log(QStringList pmuResponse) {
  QString arg1;
  int uptimeSize, valueSize, baseTime, uptime;
  QString eventType, eventValue;
  QString log;
  bool ok;
  bool isLastEntry;
  QString timestamp;
  int numEvents = 0;
  arg1 = pmuResponse.at(0);
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

QStringList build_insert_logEntry(QStringList argList) {
  return QStringList() << QString("E%1").arg(argList.at(0));
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_cmdTable.insert("get firmwareVersion", new cmdEntry(build_get_firmwareVersion, parse_get_firmwareVersion));
  m_cmdTable.insert("get productCode", new cmdEntry(build_get_productCode));
  m_cmdTable.insert("set productCode", new cmdEntry(build_set_productCode));
  m_cmdTable.insert("get serialNumber", new cmdEntry(build_get_serialNumber));
  m_cmdTable.insert("set serialNumber", new cmdEntry(build_set_serialNumber));
  m_cmdTable.insert("get unixTime", new cmdEntry(build_get_unixTime));
  m_cmdTable.insert("set unixTime", new cmdEntry(build_set_unixTime));
  m_cmdTable.insert("get temperature", new cmdEntry(build_get_temperature, parse_get_temperature));
  m_cmdTable.insert("get lightLevel", new cmdEntry(build_get_lightLevel, parse_get_lightLevel));
  m_cmdTable.insert("set lightManualLevel", new cmdEntry(build_set_lightManualLevel));
  m_cmdTable.insert("set lightOverrideActiveLevel", new cmdEntry(build_set_lightOverrideActiveLevel));
  m_cmdTable.insert("set lightOverrideInactiveLevel", new cmdEntry(build_set_lightOverrideInactiveLevel));
  m_cmdTable.insert("get sensorDelayTime", new cmdEntry(build_get_sensorDelayTime));
  m_cmdTable.insert("get sensorOverrideDelayTime", new cmdEntry(build_get_sensorOverrideDelayTime));
  m_cmdTable.insert("set sensorOverrideDelayTime", new cmdEntry(build_set_sensorOverrideDelayTime));
  m_cmdTable.insert("get upTime", new cmdEntry(build_get_upTime, parse_get_upTime));
  m_cmdTable.insert("get usage", new cmdEntry(build_get_usage, parse_get_usage));
  m_cmdTable.insert("get numLogEntries", new cmdEntry(build_get_numLogEntries));
  m_cmdTable.insert("get configCalibration", new cmdEntry(build_get_configCalibration, parse_get_configCalibration));
  m_cmdTable.insert("set configCalibrationP0", new cmdEntry(build_set_configCalibrationP0));
  m_cmdTable.insert("set configCalibrationP1", new cmdEntry(build_set_configCalibrationP1));
  m_cmdTable.insert("set configCalibrationP2", new cmdEntry(build_set_configCalibrationP2));
  m_cmdTable.insert("set configCalibrationP3", new cmdEntry(build_set_configCalibrationP3));
  m_cmdTable.insert("get buildTime", new cmdEntry(build_get_buildTime));
  m_cmdTable.insert("set buildTime", new cmdEntry(build_set_buildTime));
  m_cmdTable.insert("get sensorTimeoutCountdown", new cmdEntry(build_get_sensorTimeoutCountdown));
  m_cmdTable.insert("get currentLightLevel", new cmdEntry(build_get_currentLightLevel));
  m_cmdTable.insert("get safeMode", new cmdEntry(build_get_safeMode));
  m_cmdTable.insert("get lightBarSelect", new cmdEntry(build_get_lightBarSelect));
  m_cmdTable.insert("set lightBarSelect", new cmdEntry(build_set_lightBarSelect));
  m_cmdTable.insert("get powerConsumption", new cmdEntry(build_get_powerConsumption, parse_get_powerConsumption));
  m_cmdTable.insert("get wirelessDataAggregator", new cmdEntry(build_get_wirelessDataAggregator));
  m_cmdTable.insert("set wirelessDataAggregator", new cmdEntry(build_set_wirelessDataAggregator));
  m_cmdTable.insert("get resetUsageTimestamp", new cmdEntry(build_get_resetUsageTimestamp));
  m_cmdTable.insert("get pwmPeriodRegister", new cmdEntry(build_get_pwmPeriodRegister));
  m_cmdTable.insert("set pwmPeriodRegister", new cmdEntry(build_set_pwmPeriodRegister));
  m_cmdTable.insert("get analogSensorValue", new cmdEntry(build_get_analogSensorValue));
  m_cmdTable.insert("get analogReportingHysteresis", new cmdEntry(build_get_analogReportingHysteresis));
  m_cmdTable.insert("get zone", new cmdEntry(build_get_zone));
  m_cmdTable.insert("set zone", new cmdEntry(build_set_zone));
  m_cmdTable.insert("get lightTemporaryActiveLevel", new cmdEntry(build_get_lightTemporaryActiveLevel));
  m_cmdTable.insert("set lightTemporaryActiveLevel", new cmdEntry(build_set_lightTemporaryActiveLevel));
  m_cmdTable.insert("get lightTemporaryInactiveLevel", new cmdEntry(build_get_lightTemporaryInactiveLevel));
  m_cmdTable.insert("set lightTemporaryInactiveLevel", new cmdEntry(build_set_lightTemporaryInactiveLevel));
  m_cmdTable.insert("get sensorTemporaryDelayTime", new cmdEntry(build_get_sensorTemporaryDelayTime));
  m_cmdTable.insert("set sensorTemporaryDelayTime", new cmdEntry(build_set_sensorTemporaryDealyTime));
  m_cmdTable.insert("get temporaryOverrideTimeout", new cmdEntry(build_get_temporaryOverrideTimeout));
  m_cmdTable.insert("set temporaryOverrideTimeout", new cmdEntry(build_set_temporaryOverrideTiemout));
  m_cmdTable.insert("get setRemoteState", new cmdEntry(build_get_setRemoteState));
  m_cmdTable.insert("set setRemoteState", new cmdEntry(build_set_setRemoteState));
  m_cmdTable.insert("get remoteSetDelayTime", new cmdEntry(build_get_remoteStateDelayTime));
  m_cmdTable.insert("set remoteSetDelayTime", new cmdEntry(build_set_remoteStateDelayTime));
  m_cmdTable.insert("get remoteSecondsCountdown", new cmdEntry(build_get_remoteSecondsCountdown));
  m_cmdTable.insert("get minimumDimmingValue", new cmdEntry(build_get_minimumDimmingValue));
  m_cmdTable.insert("get powerCalibration", new cmdEntry(build_get_powerCalibration, parse_get_powerCalibration));
  m_cmdTable.insert("set powerCalibrationA0", new cmdEntry(build_set_powerCalibrationA0));
  m_cmdTable.insert("set powerCalibrationB0", new cmdEntry(build_set_powerCalibrationB0));
  m_cmdTable.insert("set powerCalibrationC0", new cmdEntry(build_set_powerCalibrationC0));
  m_cmdTable.insert("set powerCalibrationMA", new cmdEntry(build_set_powerCalibrationMA));
  m_cmdTable.insert("set powerCalibrationMB", new cmdEntry(build_set_powerCalibrationMB));
  m_cmdTable.insert("set powerCalibrationMC", new cmdEntry(build_set_powerCalibrationMC));
  m_cmdTable.insert("set powerCalibrationPOff", new cmdEntry(build_set_powerCalibrationPOff));
  m_cmdTable.insert("set powerCalibrationPOn", new cmdEntry(build_set_powerCalibrationPOn));
  m_cmdTable.insert("set powerCalibrationT0", new cmdEntry(build_set_powerCalibrationT0));
  m_cmdTable.insert("get powerEstimatorTemperatureOverride", new cmdEntry(build_get_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("set powerEstimatorTemperatureOverride", new cmdEntry(build_set_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("get cachedTemperatureValue", new cmdEntry(build_get_cachedTemperatureValue));
  m_cmdTable.insert("get eepromSize", new cmdEntry(build_get_eepromSize));
  m_cmdTable.insert("get hardwareRevision", new cmdEntry(build_get_hardwareRevision));
  m_cmdTable.insert("get wirelessConfig", new cmdEntry(build_get_wirelessConfig, parse_get_wirelessConfig));
  m_cmdTable.insert("set wirelessPanId", new cmdEntry(build_set_wirelessPanId));
  m_cmdTable.insert("set wirelessChannelMask", new cmdEntry(build_set_wirelessChannelMask));
  m_cmdTable.insert("set wirelessShortAddress", new cmdEntry(build_set_wirelessShortAddress));
  m_cmdTable.insert("set wirelessRole", new cmdEntry(build_set_wirelessRole));
  m_cmdTable.insert("set wirelessWatchdogHold", new cmdEntry(build_set_wirelessWatchdogHold));
  m_cmdTable.insert("set wirelessWatchdogPeriod", new cmdEntry(build_set_wirelessWatchdogPeriod));
  m_cmdTable.insert("set wirelessNetworkKey", new cmdEntry(build_set_wirelessNetworkKey));
  m_cmdTable.insert("get firmwareCode", new cmdEntry(build_get_firmwareCode));
  m_cmdTable.insert("get moduleFirmwareCode", new cmdEntry(build_get_moduleFirmwareCode));
  m_cmdTable.insert("get maxTemperature", new cmdEntry(build_get_maxTemperature, parse_get_maxTemperature));
  m_cmdTable.insert("get overTemperatureConfig", new cmdEntry(build_get_overTemperatureConfig, parse_get_overTemperatureConfig));
  m_cmdTable.insert("set overTemperatureThresholdLow", new cmdEntry(build_set_overTemperatureThresholdLow));
  m_cmdTable.insert("set overTemperatureThresholdHigh", new cmdEntry(build_get_overTemperatureThresholdHigh));
  m_cmdTable.insert("set overTemperatureDimmingLimit", new cmdEntry(build_set_overTemperatureDimmingLimit));
  m_cmdTable.insert("get analogDimmingMode", new cmdEntry(build_get_analogDimmingMode, parse_get_analogDimmingMode));
  m_cmdTable.insert("set analogDimmingMode", new cmdEntry(build_set_analogDimmingMode));
  m_cmdTable.insert("get fixtureIdMode", new cmdEntry(build_get_fixtureIdMode));
  m_cmdTable.insert("set fixtureIdMode", new cmdEntry(build_set_fixtureIdMode));
  m_cmdTable.insert("get acFrequency", new cmdEntry(build_get_acFrequency));
  m_cmdTable.insert("get sensorBits", new cmdEntry(build_get_sensorBits));
  m_cmdTable.insert("get powerMeterCommand", new cmdEntry(build_get_powerMeterCommand));
  m_cmdTable.insert("set powerMeterCommand", new cmdEntry(build_set_powerMeterCommand));
  m_cmdTable.insert("get powerMeterRegister", new cmdEntry(build_get_powerMeterRegister));
  m_cmdTable.insert("set powerMeterRegister", new cmdEntry(build_set_powerMeterRegister));
  m_cmdTable.insert("get ambientTemperature", new cmdEntry(build_get_ambientTemperature));
  m_cmdTable.insert("get lightSensorLevel", new cmdEntry(build_get_lightSensorLevel));
  m_cmdTable.insert("get sensorConfig", new cmdEntry(build_get_sensorConfig, parse_get_sensorConfig));
  m_cmdTable.insert("set sensor0Timeout", new cmdEntry(build_set_sensor0Timeout));
  m_cmdTable.insert("set sensor0Offset", new cmdEntry(build_set_sensor0Offset));
  m_cmdTable.insert("set sensor1Timeout", new cmdEntry(build_set_sensor1Timeout));
  m_cmdTable.insert("set sensor1Offset", new cmdEntry(build_set_sensor1Offset));
  m_cmdTable.insert("get analogDimmingConfig", new cmdEntry(build_get_analogDimmingConfig, parse_get_analogDimmingConfig));
  m_cmdTable.insert("set analogDimmingLowValue", new cmdEntry(build_set_analogDimmingLowValue));
  m_cmdTable.insert("set analogDimmingHighValue", new cmdEntry(build_set_analogDimmingHighValue));
  m_cmdTable.insert("set analogDimmingOffValue", new cmdEntry(build_set_analogDimmingOffValue));
  m_cmdTable.insert("get powerMeasurementMode", new cmdEntry(build_get_powerMeasurementMode));
  m_cmdTable.insert("set powerMeasurementMode", new cmdEntry(build_set_powerMeasurementMode));
  m_cmdTable.insert("get externalPowerMeter", new cmdEntry(build_get_externalPowerMeter));
  m_cmdTable.insert("set externalPowerMeter", new cmdEntry(build_set_externalPowerMeter));
  m_cmdTable.insert("get ambientSensorValue", new cmdEntry(build_get_ambientSensorValue));
  m_cmdTable.insert("get ambientConfig", new cmdEntry(build_get_ambientConfig, parse_get_ambientConfig));
  m_cmdTable.insert("set ambientActiveLevel", new cmdEntry(build_set_ambientActiveLevel));
  m_cmdTable.insert("set ambientInactiveLevel", new cmdEntry(build_set_ambientInactiveLevel));
  m_cmdTable.insert("set ambientEnvironmentalGain", new cmdEntry(build_set_ambientEnvironmentalGain));
  m_cmdTable.insert("set ambientOffHysteresis", new cmdEntry(build_set_ambientOffHysteresis));
  m_cmdTable.insert("set ambientOnHysteresis", new cmdEntry(build_set_ambientOnHysteresis));
  m_cmdTable.insert("get powerboardProtocol", new cmdEntry(build_get_powerboardProtocol));
  m_cmdTable.insert("get ledOverride", new cmdEntry(build_get_ledOverride));
  m_cmdTable.insert("set ledOverride", new cmdEntry(build_set_ledOverride));
  m_cmdTable.insert("get fadeUpStep", new cmdEntry(build_get_fadeUpStep));
  m_cmdTable.insert("set fadeUpStep", new cmdEntry(build_set_fadeUpStep));
  m_cmdTable.insert("get fadeDownStep", new cmdEntry(build_get_fadeDownStep));
  m_cmdTable.insert("set fadeDownStep", new cmdEntry(build_set_fadeDownStep));
  m_cmdTable.insert("get maxBrightness", new cmdEntry(build_get_maxBrightness));
  m_cmdTable.insert("set maxBrightness", new cmdEntry(build_set_maxBrightness));
  m_cmdTable.insert("get i2cResets", new cmdEntry(build_get_i2cResets));
  m_cmdTable.insert("get sensorGuardTime", new cmdEntry(build_get_sensorGuardTime));
  m_cmdTable.insert("set sensorGuardTime", new cmdEntry(build_set_sensorGuardTime));
  m_cmdTable.insert("get inputVoltage", new cmdEntry(build_get_inputVoltage));
  m_cmdTable.insert("get inputVoltageCalibration", new cmdEntry(build_get_inputVoltageCalibration));
  m_cmdTable.insert("set inputVoltageCalibration", new cmdEntry(build_set_inputVoltageCalibration));
  m_cmdTable.insert("get numLightbars", new cmdEntry(build_get_numLightbars));
  m_cmdTable.insert("set numLightbars", new cmdEntry(build_set_numLightbars));
  m_cmdTable.insert("get currentLimit", new cmdEntry(build_get_currentLimit));
  m_cmdTable.insert("set currentLimit", new cmdEntry(build_set_currentLimit));
  m_cmdTable.insert("get bootloaderCode", new cmdEntry(build_get_bootloaderCode));
  m_cmdTable.insert("get xpressMode", new cmdEntry(build_get_xpressMode));
  m_cmdTable.insert("set xpressMode", new cmdEntry(build_set_xpressMode));
  m_cmdTable.insert("get batteryBackupStatus", new cmdEntry(build_get_batteryBackupStatus, parse_get_batteryBackupStatus));
  m_cmdTable.insert("set batteryBackupStatus", new cmdEntry(build_set_batteryBackupStatus));
  m_cmdTable.insert("get sensorSeconds", new cmdEntry(build_get_sensorSeconds));
  m_cmdTable.insert("get inputVoltageTwo", new cmdEntry(build_get_inputVoltageTwo));
  m_cmdTable.insert("get inputVoltageTwoCalibration", new cmdEntry(build_get_inputVoltageTwoCalibration));
  m_cmdTable.insert("set inputVoltageTwoCalibration", new cmdEntry(build_set_inputVoltageTwoCalibration));
  m_cmdTable.insert("get maxRampUpSpeed", new cmdEntry(build_get_maxRampUpSpeed));
  m_cmdTable.insert("set maxRampUpSpeed", new cmdEntry(build_set_maxRampUpSpeed));
  m_cmdTable.insert("get maxRampDownSpeed", new cmdEntry(build_get_maxRampDownSpeed));
  m_cmdTable.insert("set maxRampDownSpeed", new cmdEntry(build_set_maxRampDownSpeed));
  m_cmdTable.insert("get emergencyLightLevel", new cmdEntry(build_get_emergencyLightLevel));
  m_cmdTable.insert("get batteryBackupPowerCalibration", new cmdEntry(build_get_batteryBackupPowerCalibration));
  m_cmdTable.insert("set batteryBackupPowerCalibration", new cmdEntry(build_set_batteryBackupPowerCalibration));
  m_cmdTable.insert("get motionSensorProfile", new cmdEntry(build_get_motionSensorProfile));
  m_cmdTable.insert("set motionSensorProfile", new cmdEntry(build_set_motionSensorProfile));
  m_cmdTable.insert("get powerMeterConfig", new cmdEntry(build_get_powerMeterConfig, parse_get_powerMeterConfig));
  m_cmdTable.insert("set powerMeterLevelAtOff", new cmdEntry(build_set_powerMeterLevelAtOff));
  m_cmdTable.insert("set powerMeterLevelAtMin", new cmdEntry(build_set_powerMeterLevelAtMin));
  m_cmdTable.insert("set powerMeterLevelAtMax", new cmdEntry(build_set_powerMeterLevelAtMax));
  m_cmdTable.insert("set powerMeterType", new cmdEntry(build_set_powerMeterType));
  m_cmdTable.insert("get DLAiSlaveMode", new cmdEntry(build_get_DLAiSlaveMode));
  m_cmdTable.insert("set DLAiSlaveMode", new cmdEntry(build_set_DLAiSlaveMode));
  m_cmdTable.insert("get DALIBootloadingActive", new cmdEntry(build_get_DALIBootlodingActive));
  m_cmdTable.insert("get testingMode", new cmdEntry(build_get_testingMode));
  m_cmdTable.insert("set testingMode", new cmdEntry(build_set_testingMode));
  m_cmdTable.insert("get numBatteriesSupported", new cmdEntry(build_get_numBatteriesSupported));
  // get & set lightbar commands
  m_cmdTable.insert("get lbVersion", new cmdEntry(build_get_lbVersion, parse_get_lbVersion));
  m_cmdTable.insert("get lbStatus", new cmdEntry(build_get_lbStatus, parse_get_lbStatus));
  m_cmdTable.insert("get lbConfig", new cmdEntry(build_get_lbConfig, parse_get_lbConfig));
  // get & set battery backup commands
  m_cmdTable.insert("get bbVersion", new cmdEntry(build_get_bbVersion, parse_get_bbVersion));
  m_cmdTable.insert("get bbStatus", new cmdEntry(build_get_bbStatus, parse_get_bbStatus));
  m_cmdTable.insert("get bbConfig", new cmdEntry(build_get_bbConfig, parse_get_bbConfig));
  // reset commands
  m_cmdTable.insert("reset usage", new cmdEntry(build_reset_usage));
  m_cmdTable.insert("reset oldLog", new cmdEntry(build_reset_oldLog));
  m_cmdTable.insert("reset log", new cmdEntry(build_reset_log));
  m_cmdTable.insert("reset logIndex", new cmdEntry(build_reset_logIndex));
  m_cmdTable.insert("reset eeprom", new cmdEntry(build_reset_eeprom));
  m_cmdTable.insert("reset eepromToDefault", new cmdEntry(build_reset_eepromToDefault));
  m_cmdTable.insert("reset eepromToLatestMapVersion", new cmdEntry(build_reset_eepromToLatestMapVersion));
  m_cmdTable.insert("reset network", new cmdEntry(build_reset_network));
  m_cmdTable.insert("reset networkWithoutChecking", new cmdEntry(build_reset_networkWithoutChecking));
  m_cmdTable.insert("reset daliCommissioning", new cmdEntry(build_reset_daliCommissioning));
  m_cmdTable.insert("reset daliPowerMetering", new cmdEntry(build_reset_daliPowerMetering));
  // reboot commands
  m_cmdTable.insert("reboot pmu", new cmdEntry(build_reboot_pmu));
  m_cmdTable.insert("reboot wirelessCard", new cmdEntry(build_reboot_wirelessCard));
  m_cmdTable.insert("reboot i2cDevices", new cmdEntry(build_reboot_i2cDevices));
  // reload commands
  m_cmdTable.insert("reload dlaFirmware", new cmdEntry(build_reload_dlaFirmware));
  m_cmdTable.insert("reload wirelessModuleFirmware", new cmdEntry(build_reload_wirelessModuleFirmware));
  m_cmdTable.insert("reload powerboardFirmware", new cmdEntry(build_reload_powerboardFirmware));
  m_cmdTable.insert("reload lightbarFirmware", new cmdEntry(build_reload_lightbarFirmware));
  m_cmdTable.insert("reload batteryBackupFirmware", new cmdEntry(build_reload_batteryBackupFirmware));
  m_cmdTable.insert("reload motionSensorFirmware", new cmdEntry(build_reload_motionSensorFirmware));
  // log commands
  m_cmdTable.insert("get logIndex", new cmdEntry(build_get_logIndex, parse_get_logIndex));
  m_cmdTable.insert("get log", new cmdEntry(build_get_log, parse_get_log));
  m_cmdTable.insert("insert logEntry", new cmdEntry(build_insert_logEntry));
  // build the dictionary of helper commands
  m_cmdCompleter = new QCompleter(m_cmdTable.keys(), this);
  m_cmdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  m_cmdCompleter->setCompletionMode(QCompleter::InlineCompletion);
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
  return m_cmdTable[cmd];
}

QString cmdHelper::parseError(QString pmuResponse) {
  return m_errorResponses[pmuResponse];
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
