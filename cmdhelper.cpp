#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

QString toYDHMS(QString timeInSec) {
  bool ok;
  quint32 ulTimeInSec = timeInSec.toULong(&ok, 16);
  QString outTime;
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

QString help_firmwareVersion(void) {
  return "0000 Firmware Version -- 6B -- R";
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

QString help_productCode(void) {
  return "0001 Product Code  -- 4B -- RW -- BAK";
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

QString help_serialNumber(void) {
  return "0002 SerialNumber -- 4B -- RW -- BAK";
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

QString help_unixTime(void) {
  return "0003 UnixTime (RTC) -- 4B -- RW";
}

QStringList build_get_temperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G0004";
}

QStringList parse_get_temperature(QStringList pmuResponse) {
  (void) pmuResponse;
  return QStringList() << "todo";
}

QString help_temperature(void) {
  return "0004 Temperature  -- 2B -- R";
}

QStringList build_get_lightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0005" // light manual level
                       << "G0006" // light active level
                       << "G0007" // light inactive level
                       << "G0008" // light overrirde active level
                       << "G0009"; // light override inactive level
}

QStringList parse_get_lightLevel(QStringList pmuResponse) {
  QStringList parsedResponse;
  parsedResponse << QString("Manual level: %1").arg(pmuResponse.at(0));
  parsedResponse << QString("Active level: %1").arg(pmuResponse.at(1));
  parsedResponse << QString("Inactive level: %1").arg(pmuResponse.at(2));
  parsedResponse << QString("Override active level: %1").arg(pmuResponse.at(3));
  parsedResponse << QString("Override inactive level: %1").arg(pmuResponse.at(4));
  return parsedResponse;
}

QStringList build_set_lightManualLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0005 %1").arg(argList.at(0));
}

QString help_lightManualLevel(void) {
  return "0005 Light Manual Level (FFFF is no override) -- 2B -- RW";
}

QString help_lightActiveLevel(void) {
  return "0006 Light Active Level (DIPS) -- 2B -- R";
}

QString help_lightInactiveLevel(void) {
  return "0007 Light Inactive Level (DIPS) -- 2B -- R";
}

QStringList build_set_lightOverrideActiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0008 %1").arg(argList.at(0));
}

QString help_lightOverrideActiveLevel(void) {
  return "0008 Light Override Active Level (FFFF is no override) -- 2B -- RW";
}

QStringList build_set_lightOverrideInactiveLevel(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0009 %1").arg(argList.at(0));
}

QString help_lightOverrideInactiveLevel(void) {
  return "0009 Light Override Inactive Level (FFFF is no override) -- 2B -- RW";
}

QString help_get_lightLevel(void) {
  QString help = help_lightManualLevel() + "<br>";
  help += help_lightActiveLevel() + "<br>";
  help += help_lightInactiveLevel() + "<br>";
  help += help_lightOverrideActiveLevel() + "<br>";
  help += help_lightOverrideInactiveLevel();
  return help;
}

QStringList build_get_sensorDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000A";
}

QString help_sensorDelayTime(void) {
  return "000A Sensor Delay Time (DIPS, FFFF is \"always active\") -- 2B -- R";
}

QStringList build_get_sensorOverrideDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000B";
}

QString help_sensorOverrideDelayTime(void) {
  return "000B Sensor Override Delay Time<br>"
         "(FFFF is no override, FFF1 is \"always on\", FFF0 does not count down) -- 2B -- RW";
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

QString help_upTime(void) {
  return "000C Power up time in seconds -- 4B -- R";
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
  parsedResponse << QString("Power: %1 watt hours").arg(pmuResponse.at(5).toUShort(&ok, 16));
  parsedResponse << QString("Perm power: %1 watt hours").arg(pmuResponse.at(6).toUShort(&ok, 16));
  parsedResponse << QString("Sensor events: %1").arg(pmuResponse.at(7).toUShort(&ok, 16));
  parsedResponse << QString("Perm sensor events: %1").arg(pmuResponse.at(8).toUShort(&ok, 16));
  return parsedResponse;
}

QString help_usageActiveTime(void) {
  return "000D Usage: Active Seconds -- 4B -- R";
}

QString help_usageInactiveTime(void) {
  return "000E Usage: Inactive Seconds -- 4B -- R";
}

QString help_usagePermActiveTime(void) {
  return "000F Usage: Perm Active Seconds -- 4B -- R";
}

QString help_usagePermInactiveTime(void) {
  return "0010 Usage: Perm Inactive Seconds -- 4B -- R";
}

QString help_usageWh(void) {
  return "0011 Usage: Wh -- 4B -- R";
}

QString help_usagePermWh(void) {
  return "0012 Usage: Perm Wh -- 4B -- R";
}

QString help_usageSensorEvents(void) {
  return "0013 Usage: Sensor Events -- 4B -- R";
}

QString help_usagePermSensorEvents(void) {
  return "0014 Usage: Perm Sensor Events -- 4B -- R";
}

QString help_get_usage(void) {
  QString help = help_usageActiveTime() + "<br>";
  help += help_usageInactiveTime() + "<br>";
  help += help_usagePermActiveTime() + "<br>";
  help += help_usagePermInactiveTime() + "<br>";
  help += help_usageWh() + "<br>";
  help += help_usagePermWh() + "<br>";
  help += help_usageSensorEvents() + "<br>";
  help += help_usagePermSensorEvents();
  return help;
}

QStringList build_get_numberOfLogEntries(QStringList argList) {
  (void) argList;
  return QStringList() << "G0015";
}

QString help_numberOfLogEntries(void) {
  return "0015 Number of Log entries -- 2B -- R<br>"
         "(Does not increment in Gen 3)";
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

QString help_configCalibrationP0(void) {
  return "0016 Config Calibration P0 -- 4B -- RW<br>"
         "(ES/EP 0x00000384) Idle Power (mW) **Dropped in 1.0.7**";
}

QStringList build_set_configCalibrationP1(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0017 %1").arg(argList.at(0));
}

QString help_configCalibrationP1(void) {
  return "0017 Config Calibration P1 -- 4B -- RW<br>"
         "(ES/EP 0x00000C80) Power (mW) at Min Dimming 0x7A00 **Dropped in 1.0.7**";
}

QStringList build_set_configCalibrationP2(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0018 %1").arg(argList.at(0));
}

QString help_configCalibrationP2(void) {
  return "0018 Config Calibration P2 -- 4B -- RW<br>"
         "(ES/EP 0x0004D45D) Power slope (mW * 65536 per bit) **Dropped in 1.0.7**";
}

QStringList build_set_configCalibrationP3(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0019 %1").arg(argList.at(0));
}

QString help_configCalibrationP3(void) {
  return "0019 Config Calibration P3 -- 4B -- RW<br>"
         "(ES/EP 0x00028D84) Power (mw) at Max **Dropped in 1.0.7**";
}

QString help_configCalibration(void) {
  QString help = help_configCalibrationP0() + "<br>";
  help += help_configCalibrationP1() + "<br>";
  help += help_configCalibrationP2() + "<br>";
  return help + help_configCalibrationP3();
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

QString help_buildTime(void) {
  return "001A Build Time -- 4B -- RW -- BAK";
}

QStringList build_get_sensorTimeoutCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G001B";
}

QString help_sensorTimeoutCountdown(void) {
  return "001B Sensor Timeout Countdown -- 2B -- R<br>"
         "Version 1.0+<br>"
         "(Seconds left until turnoff. 0 is off, 0xffff is on)";
}

QStringList build_get_currentLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G001C";
}

QString help_currentLightLevel(void) {
  return "001C Current Light Level -- 4B -- R<br>"
         "Version 1.0+";
}

QStringList build_get_safeMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G001D";
}

QString help_safeMode(void) {
  return "001D Safe Mode -- 1B -- R<br>"
         "Version 1.0+<br>"
         "(0 normal, 1 safe due to EEPROM failure, 2 safe due to DIPS)";
}

QStringList build_get_lightBarSelect(QStringList argList) {
  (void) argList;
  return QStringList() << "G001E";
}

QStringList build_set_lightBarSelect(QStringList argList) {
  return QStringList() << QString("S001E %1").arg(argList.at(0));
}

QString help_lightBarSelect(void) {
  return "001E Light Bar Select -- 1B -- RW<br>"
         "Version 1.0+<br>"
         "(bit mask for what light bars should be used. Development use only, not EEPROM backed)";
}

QStringList build_get_currentPowerConsumption(QStringList argList) {
  (void) argList;
  return QStringList() << "G001F";
}

QString help_currentPowerConsumption(void) {
  return "001F Current Power Consumption -- 4B -- R (Power in mW)<br>"
         "Version 1.0+<br>";
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

QString help_wirelessDataAggregator(void) {
  return "0020 Wireless Data Aggregator -- 2B -- RW<br>"
         "Version 1.0+<br>"
         "(Wireless 16 bit address of data aggregator to send events to)";
}

QStringList build_get_resetUsageTimestamp(QStringList argList) {
  (void) argList;
  return QStringList() << "G0021";
}

QString help_resetUsageTimestamp(void) {
  return "0021 Reset Usage Timestamp -- 4B -- R<br>"
         "Version 1.0+";
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

QString help_pwmPeriodRegister(void) {
  return "0022 PWM period register -- 2B -- RW<br>"
         "Version 1.0+<br>"
         "(default of 0x8000 gives ~500Hz period, not EEPROM backed)";
}

QStringList build_get_analogSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0023";
}

QString help_analogSensorValue(void) {
  return "0023 Analog Sensor Value -- 2B -- R<br>"
         "Version 1.0.3+<br>"
         "(0 - 1024)";
}

QStringList build_get_analogReportingHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G0024";
}

QString help_analogReportingHysteresis(void) {
  return "0024 Analog Reporting Hysteresis -- 2B -- RW<br>"
         "Version 1.0.3+<br>"
         "(Required change in value before an analog event is fired with the new value)";
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

QString help_zone(void) {
  return "0025 Zone -- 2B -- RW (EEPROM backed Zone)<br>"
         "Version 1.0.3+";
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

QString help_lightTemporaryActiveLevel(void) {
  return "0026 Light Temporary Active Level (FFFF is no override) -- 2B -- RW<br>"
         "Version 1.0.3+";
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

QString help_lightTemporaryInactiveLevel(void) {
  return "0027 Light Temporary Inactive Level (FFFF is no override) -- 2B -- RW>br>"
         "Version 1.0.3+";
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

QString help_sensorTemporaryDelayTime(void) {
  return "0028 Sensor Temporary Delay Time (FFFF is no override) -- 2B -- RW<br>"
         "Version 1.0.3+";
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

QString help_temporaryOverrideTimeout(void) {
  return "0029 Temporary Override Timeout (FFFFFFFF is off) -- 4B -- RW<br>"
         "Version 1.0.3+<br>"
         "Number of seconds until Temporary Overrides are wiped";
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

QString help_setRemoteState(void) {
  return "002A Set Remote State (00 inactive, 01 active) -- 1B -- RW<br>"
         "Version 1.0.3+";
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

QString help_remoteStateDelayTime(void) {
  return "002B Remote Set Delay Time -- 2B -- RW<br>"
         "Version 1.0.3+<br>"
         "Number of seconds that Remote Set state sticks";
}

QStringList build_get_remoteSecondsCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G002C";
}

QString help_remoteSecondsCountdown(void) {
  return "002C Remote Seconds Countdown -- 2B -- R<br>"
         "Version 1.0.3+<br>"
         "(number of seconds left that Remote Set state has left)";
}

QStringList build_get_minimumDimmingValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G002D";
}

QString help_minimumDimmingValue(void) {
  return "002D Minimum Dimming Value -- 2B -- R<br>"
         "Version 1.0.3+<br>"
         "(Direct impact on register 0017 as this is used as the start of the linear power regime)";
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

QString help_powerCalibrationA0(void) {
  return "002E Power Calibration A0 -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(a_0 * 1000 * 1073741824).round (equivalent to shift left 30)";
}

QStringList build_set_powerCalibrationB0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S002F %1").arg(argList.at(0));
}

QString help_powerCalibrationB0(void) {
  return "002F Power Calibration B0 -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "-(b_0 * 1000 * 4096).round (equivalent to shift left 12 and negate)";
}

QStringList build_set_powerCalibrationC0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0030 %1").arg(argList.at(0));
}

QString help_powerCalibrationC0(void) {
  return "0030 Power Calibration C0 -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(c_0 * 1000 / 4).round (equivalent to shift right 2)";
}

QStringList build_set_powerCalibrationMA(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0031 %1").arg(argList.at(0));
}

QString help_powerCalibrationMA(void) {
  return "0031 Power Calibration MA -- 2B -- RW -- BAK<br>"""
         "Version 1.0.7+<br>"
         "(m_a * 1000 * 549755813888).round (equivalent to shift left 39) -- XPG: 0x4061";
}

QStringList build_set_powerCalibrationMB(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0032 %1").arg(argList.at(0));
}

QString help_powerCalibrationMB(void) {
  return "0032 Power Calibration MB -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(m_b * 1000 * 2097152).round (equivalent to shift left 21) -- XPG: 0x4FAF";
}

QStringList build_set_powerCalibrationMC(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0033 %1").arg(argList.at(0));
}

QString help_powerCalibrationMC(void) {
  return "0033 Power Calibration MC -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "-(m_c * 1000 * 32).round (equivalent to shift left 5 and negate) -- XPG: 0x2D23";
}

QStringList build_set_powerCalibrationPOff(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0034 %1").arg(argList.at(0));
}

QString help_powerCalibrationPOff(void) {
  return "0034 Power Calibration POff -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(P_Off * 1000).round";
}

QStringList build_set_powerCalibrationPOn(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0035 %1").arg(argList.at(0));
}

QString help_powerCalibrationPOn(void) {
  return "0035 Power Calibration POn -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(P_On * 1000 / 4).round (equivalent to shift right 2)";
}

QStringList build_set_powerCalibrationT0(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0036 %1").arg(argList.at(0));
}

QString help_powerCalibrationT0(void) {
  return "0036 Power Calibration T0 -- 2B -- RW -- BAK<br>"
         "Version 1.0.7+<br>"
         "(t_0 * 128).round (equivalent to shift left 7) or just put register 0004 straight into this.";
}

QString help_get_powerCalibration(void) {
  QString help = help_powerCalibrationA0() + "<br>";
  help += help_powerCalibrationB0() + "<br>";
  help += help_powerCalibrationC0() + "<br>";
  help += help_powerCalibrationMA() + "<br>";
  help += help_powerCalibrationMB() + "<br>";
  help += help_powerCalibrationMC() + "<br>";
  help += help_powerCalibrationPOn() + "<br>";
  help += help_powerCalibrationPOff() + "<br>";
  help += help_powerCalibrationT0();
  return help;
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

QString help_powerEstimatorTemperatureOverride(void) {
  return "0037 Power Estimator Temperature Override -- 2B -- RW<br>"
         "Version 1.0.7+<br>"
         "Set to 0x7FFF to disable override. Otherwise estimator will use this temperature.";
}

QStringList build_get_cachedTemperatureValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0038";
}

QString help_cachedTemperatureValue(void) {
  return "0038 Cached Temperature Value -- 2B -- R<br>"
         "Version 1.0.7+<br>"
         "The cached temperature used for power calculations. Updated once per minute.";
}

QStringList build_get_eepromSize(QStringList argList) {
  (void) argList;
  return QStringList() << "G0039";
}

QString help_eepromSize(void) {
  return "0039 EEPROM Size -- 1B -- R<br>"
         "Version 1.0.8+<br>"
         "The EEPROM size in log(2) (in bytes not bits)";
}

QStringList build_get_hardwareRevision(QStringList argList) {
  (void) argList;
  return QStringList() << "G003A";
}

QString help_hardwareRevision(void) {
  return "003A Hardware Revision -- 4B -- RW -- BAK<br>"
         "Version 1.0.8+<br>"
         "Revision of the hardware for the unit (EEPROM)";
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
  QStringList parsedResponse;
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

QString help_wirelessPanId(void) {
  return "003B Wireless Pan ID -- 8B -- RW -- -- BAK<br>"
         "Version 1.0.10+<br>"
         "The Wireless PAN ID to set the wireless to with !N";
}

QStringList build_set_wirelessChannelMask(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003C %1").arg(argList.at(0));
}

QString help_wirelessChannelMask(void) {
  return "003C Wireless Channel Mask -- 4B -- RW -- BAK<br>"
         "Version 1.0.10+<br>"
         "The Wireless Channel Mask to set the wireless to with !N";
}

QStringList build_set_wirelessShortAddress(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003D %1").arg(argList.at(0));
}

QString help_wirelessShortAddress(void) {
  return "003D Wireless Short Address -- 2B -- RW<br>"
         "Version 1.0.10+<br>"
         "The Wireless Short Source Address to set the wireless to with !N";
}

QStringList build_set_wirelessRole(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003E %1").arg(argList.at(0));
}

QString help_wirelessRole(void) {
  return "003E Wireless Role -- 1B -- RW<br>"
         "Version 1.0.10+<br>"
         "The Wireless Role to set the wireless to with !N";
}

QStringList build_set_wirelessWatchdogHold(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S003F %1").arg(argList.at(0));
}

QString help_wirelessWatchdogHold(void) {
  return "003F Wireless Watchdog Hold -- 1B -- RW<br>"
         "Version 1.0.11+<br>"
         "The hold time in minutes for the SerialNet watchdog. (0xFF becomes 0 minutes) Setting this or 0040 to 0 will disable watchdog. (EEPROM)";
}

QStringList build_set_wirelessWatchdogPeriod(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0040 %1").arg(argList.at(0));
}

QString help_wirelessWatchdogPeriod(void) {
  return "0040 Wireless Watchdog Period -- 2B -- RW<br>"
         "Version 1.0.11+<br>"
         "The period in minutes to watchdog SerialNet at. Resets will occur at times divisible by this value. (0xFFFF becomes 0 hours) (EEPROM)";
}

QStringList build_set_wirelessNetworkKey(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0073 %1").arg(argList.at(0));
}

QString help_wirelessNetworkKey(void) {
  return "0073 Wireless Network Key -- 16B -- RW -- BAK<br>"
         "Version 2.1.X+<br>"
         "The key to be used for network layer encryption. All 0s will disable encryption (default).<br>"
         "Please note that the byte order for this register is reversed by the Ember Stack, so it needs to be entered in the opposite byte order as the EZSP security key.";
}

QString help_get_wirelessConfig(void) {
  QString help = help_wirelessPanId() + "<br>";
  help += help_wirelessChannelMask() + "<br>";
  help += help_wirelessShortAddress() + "<br>";
  help += help_wirelessRole() + "<br>";
  help += help_wirelessWatchdogHold() + "<br>";
  help += help_wirelessWatchdogPeriod() + "<br>";
  return help + help_wirelessNetworkKey();
}

QStringList build_get_firmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0041";
}

QString help_firmwareCode(void) {
  return "0041 Firmware Code -- 4B -- R<br>"
         "Version 1.1.0+<br>"
         "A unique identifier for compatible firmware. Only firmware with this Firmware Code can be bootloaded.";
}

QStringList build_get_moduleFirmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0042";
}

QString help_moduleFirmwareCode(void) {
  return "0042 Module Firmware Code -- 4B -- R<br>"
         "Version 1.1.0+<br>"
         "A unique identifier for compatible firmware. Only firmware with this Firmware Code can be bootloaded.";
}

QStringList build_get_maxTemperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G0043" // observed temperature
                       << "G0044"; // observed time
}

QStringList parse_get_maxTemperature(QStringList pmuResponse) {
  return QStringList() << QString("%1 at time %2").arg(pmuResponse.at(0)).arg(toYDHMS(pmuResponse.at(1)));
}

QString help_maxTemperatureObserved(void) {
  return "0043 Max Temperature Observed -- 2B -- R<br>"
         "Version 1.1.4+<br>"
         "Highest temperature observed. (EEPROM)";
}

QString help_maxTemperatureObservedTime(void) {
  return "0044 Max Temperature Observed Time -- 4B -- R<br>"
         "Version 1.1.4+<br>"
         "Time that highest temperature was observed. (EEPROM)";
}

QString help_get_maxTemperature(void) {
  QString help = help_maxTemperatureObserved() + "<br>";
  return help + help_maxTemperatureObservedTime();
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

QString help_overTemperatureThresholdLow(void) {
  return "0045 Over Temp Threshold Low -- 2B -- RW -- BAK<br>"
         "Version 1.1.4+<br>"
         "Maximum Temperature allowed for full brightness. (EEPROM)";
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

QString help_overTemperatureThresholdHigh(void) {
  return "0046 Over Temp Threshold High -- 2B -- RW -- BAK<br>"
         "Version 1.1.4+<br>"
         "Temperature at which the maximum allowed brightness reaches Over Temp Dim Limit. (EEPROM)";
}

QStringList build_set_overTemperatureDimmingLimit(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0047 %1").arg(argList.at(0));
}

QString help_overTemperatureDimmingLimit(void) {
  return "0047 Over Temp Dimming Limit -- 2B -- RW -- BAK<br>"
         "Version 1.1.4+<br>"
         "Minimum dimming level that the protection is allowed to reduce the maximum brightness to. (EEPROM)";
}

QString help_get_overTemperatureConfig(void) {
  QString help = help_overTemperatureThresholdLow() + "<br>";
  help += help_overTemperatureThresholdHigh() + "<br>";
  return help + help_overTemperatureDimmingLimit();
}

QStringList build_get_analogDimmingMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0048";
}

QStringList build_set_analogDimmingMode(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0048 %1").arg(argList.at(0));
}

QString help_analogDimmingMode(void) {
  return "0048 Analog Dimming Mode -- 1B -- RW<br>"
         "Version 1.1.4+<br>"
         "00 = Analog Dimming OFF<br>"
         "01 = Analog DImming On<br>"
         "02 = Analog Dimming On w/ ability to go full off<br>"
         "03 = Analog Dimming using registers 54-56<br>"
         "04 = Analog Dimming using registers 54-56 w/ full off suport<br>"
         "05 = Ambient Sensor Dimming. (EEPROM)";
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

QString help_fixtureIdMode(void) {
  return "0049 Fixture ID Mode -- 3B -- RW<br>"
         "Version 1.1.4+<br>"
         "(1B mode, 2B time in seconds)<br>"
         "00 = Off<br>"
         "01 = Status LEDs<br>"
         "02 = Full Flash<br>"
         "03 = Safe Flash<br>"
         "04 = Middle Flash";
}

QStringList build_get_acFrequency(QStringList argList) {
  (void) argList;
  return QStringList() << "G004A";
}

QString help_acFrequency(void) {
  return "004A AC Frequency -- 2B -- R<br>"
         "Version 1.1.4+<br>"
         "frequency in 250kHz increments. f = 250kHz/value";
}

QStringList build_get_sensorBits(QStringList argList) {
  (void) argList;
  return QStringList() << "G004B";
}

QString help_sensorBits(void) {
  return "004B Sensor Bits -- 1B -- R<br>"
         "Version 1.2.0+<br>"
         "The current digital sensor bits";
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

QString help_powerMeterCommand(void) {
  return "004C Power Meter Command -- 1B -- RW<br>"
         "Version 1.2.1+<br>"
         "Command to execute on the power meter chip. AJAX only";
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

QString help_powerMeterRegister(void) {
  return "004D Power Meter Register -- 4B -- RW<br>"
         "Version 1.2.1+<br>"
         "Read or Write register on power meter chip. AJAX only";
}

QStringList build_get_ambientTemperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G004E";
}

QString help_ambientTemperature(void) {
  return "004E Ambient Temperature -- 2B -- R<br>"
         "Version 1.2.1+<br>"
         "Temperature from Ambient junction. 128ths of a deg C. SWAN only";
}

QStringList build_get_lightSensorLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G004F";
}

QString help_lightSensorLevel(void) {
  return "004F Light Sensor Level -- 2B -- R<br>"
         "Version 1.2.1+<br>"
         "SWAN Light Sensor (Analog, OBSOLETE!)";
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

QString help_sensor0Timeout(void) {
  return "0050 Sensor 0 Timeout -- 1B -- RW<br>"
         "Version 1.2.1+<br>"
         "The timeout of the hardware installed at Sensor 0. (Also 1.1.6)";
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

QString help_sensor0Offset(void) {
  return "0051 Sensor 0 Offset -- 1B -- RW<br>"
         "Version 1.2.1+<br>"
         "Additional delay added to hardware Sensor 0 before starting normal countdown. (Allow new sensor to act like old sensor) (Also 1.1.6)";
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

QString help_sensor1Timeout(void) {
  return "0052 Sensor 1 Timeout -- 1B -- RW<br>"
         "Version 1.2.1+<br>"
         "The timeout of the hardware installed at Sensor 1 (not AJAX) (Also 1.1.6)";
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

QString help_sensor1Offset(void) {
  return "0053 Sensor 1 Offset -- 1B -- RW<br>"
         "Version 1.2.1+<br>"
         "Same as 0051 but for Sensor 1 (not AJAX) (Also 1.1.6)";
}

QString help_get_sensorConfig(void) {
  QString help = help_lightSensorLevel() + "<br>";
  help += help_sensor0Timeout() + "<br>";
  help += help_sensor0Offset() + "<br>";
  help += help_sensor1Timeout() + "<br>";
  return help + help_sensor1Offset();
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

QString help_analogDimmingLowValue(void) {
  return "0054 Analog Dimming Low Value -- 2B -- RW<br>"
         "Version 1.2.4+<br>"
         "Value at which minimum dimming is reached. (Also 1.1.6)";
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

QString help_analogDimmingHighValue(void) {
  return "0055 Analog Dimming High Value -- 2B -- RW<br>"
         "Version 1.2.4+<br>"
         "Value at which specified brightness is reached. (Also 1.1.6)";
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

QString help_analogDimmingOffValue(void) {
  return "0056 Analog Dimming Off Value -- 2B -- RW<br>"
         "Version 1.2.4+<br>"
         "Value at which lights are powered off (must exceed low value to turn back on) (Also 1.1.6)";
}

QString help_get_analogDimmingConfig(void) {
  QString help = help_analogDimmingLowValue() + "<br>";
  help += help_analogDimmingHighValue() + "<br>";
  return help + help_analogDimmingOffValue();
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

QString help_powerMeasurementMode(void) {
  return "0057 Power Measurement Mode -- 1B -- RW<br>"
         "Version 1.2.4+<br>"
         "00 = default/native mode<br>"
         "01 = external pulse meter on Digital (Sensor) 0 input. AJAX only";
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

QString help_externalPowerMeter(void) {
  return "0058 External Power Meter mWsec per edge -- 4B -- RW<br>"
         "Version 1.2.4+<br>"
         "milliWatt-seconds per pulse edge for external pulse meter (Measurement Mode 01). AJAX only"
         "for example: 5 Amp CT has WHpP of 0.125, so value is 0.125 * 1000 * 3600 / 2 = 225000 (divide by two because we count edges, not pulses)";
}

QStringList build_get_ambientSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0059";
}

QString help_ambientSensorValue(void) {
  return "0059 Ambient Sensor Value -- 3B -- R<br>"
         "Version 1.2.6+<br>"
         "Auto Gained result of the ambient light sensor";
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

QString help_ambientActiveLevel(void) {
  return "005A Ambient Active Level -- 3B -- RW<br>"
         "Version 1.2.6+<br>"
         "Target Sensor Value when in the Active State";
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

QString help_ambientInactiveLevel(void) {
  return "005B Ambient Inactive Level -- 3B -- RW<br>"
         "Version 1.2.6+<br>"
         "Target Sensor Value when in the Inactive State";
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

QString help_ambientEnvironmentalGain(void) {
  return "005C Ambient Environment Gain -- 2B -- RW<br>"
         "Version 1.2.6+<br>"
         "Amount of Sensor change per Power Board output bit. ((Max Out - Min Out) / 255)";
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

QString help_ambientOffHysteresis(void) {
  return "005D Ambient Off Hysteresis -- 3B -- RW<br>"
         "Version 1.2.6+<br>"
         "Amount of Sensor hysteresis to apply at min power before letting fixture turn off. (>next register)";
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

QString help_ambientOnHysteresis(void) {
  return "005E Ambient On Hysteresis -- 3B -- RW<br>"
         "Version 1.2.6+<br>"
         "Amount of Sensor hysteresis to apply when off before letting fixture turn on. (Min Out - Off)";
}

QString help_ambientDivisor(void) {
  return "0069 Ambient Divisor -- 2B -- R<br>"
         "Version 2.0.8+<br>"
         "The divisor to use when calibrating Ambient Environmental Gain. (255 or 663 currently)";
}

QString help_get_ambientConfig(void) {
  QString help = help_ambientSensorValue() + "<br>";
  help += help_ambientActiveLevel() + "<br>";
  help += help_ambientInactiveLevel() + "<br>";
  help += help_ambientEnvironmentalGain() + "<br>";
  help += help_ambientOffHysteresis() + "<br>";
  help += help_ambientOnHysteresis() + "<br>";
  return help + help_ambientDivisor();
}

QStringList build_get_powerboardProtocol(QStringList argList) {
  (void) argList;
  return QStringList() << "G005F";
}

QString help_powerboardProtocol(void) {
  return "005F Power Board Protocol -- 4B -- R<br>"
         "Version 1.2.12+<br>"
         "Only on Swan";
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

QString help_ledOverride(void) {
  return "0060 LED Override -- 1B -- RW<br>"
         "Version 1.2.12+<br>"
         "Override the Status LED Outputs (0xFF is no override)";
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

QString help_fadeUpStep(void) {
  return "0061 Fade Up Step -- 2B -- RW<br>"
         "Ajax 1.2.6+<br>"
         "PWM steps per 0.01 sec when lights fading up (default 500)";
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

QString help_fadeDownStep(void) {
  return "0062 Fade Down Step -- 2B -- RW<br>"
         "Ajax 1.2.6+<br>"
         "PWM steps per 0.01 sec when lights fading down (default 500)";
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

QString help_maxBrightness(void) {
  return "0063 Max Brightness -- 2B -- RW<br>"
         "XLE 1.2.15+<br>"
         "XLE only. The maximum PWM brightness supported by fixture.";
}

QStringList build_get_i2cResets(QStringList argList) {
  (void) argList;
  return QStringList() << "G0064";
}

QString help_i2cResets(void) {
  return "0064 I2C resets -- 2B -- R<br>"
         "Version 1.2.17+<br>"
         "Number of I2C resets seen since power up.";
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

QString help_sensorGuardTime(void) {
  return "0065 Sensor Guard Time -- 2B -- RW<br>"
         "Version 1.2.18+<br>"
         "Time in milliseconds to wait after switching to inactive before allowing the sensor to re-trigger active state. (EEPROM)";
}

QStringList build_get_inputVoltage(QStringList argList) {
  (void) argList;
  return QStringList() << "G0066";
}

QString help_inputVoltage(void) {
  return "0066 Input Voltage -- 2B -- R<br>"
         "Version 2.0.1+<br>"
         "Input voltage in mV if calibrated. If 0067 is 0, then this is the ADC value instead to allow for calibration. Cowboy only.";
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

QString help_inputVoltageCalibration(void) {
  return "0067 Input Voltage Calibration -- 2B -- RW -- BAK<br>"
         "Version 2.0.1+<br>"
         "Input voltage Calibration in 3.13 format. round((42000/ADC) * (2^13)). Cowboy only";
}

QStringList build_get_numberOfLightbars(QStringList argList) {
  (void) argList;
  return QStringList() << "G0068";
}

QStringList build_set_numberOfLightbars(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0068 %1").arg(argList.at(0));
}

QString help_numberOfLightbars(void) {
  return "0068 Number of LightBars -- 1B -- R(W) -- BAK<br>"
         "Version 2.0.1+<br>"
         "Swan = 3, Cowboy = Settable between 1 and 4";
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

QString help_currentLimit(void) {
  return "006A Current Limit -- 2B -- RW<br>"
         "Version 2.0.10+<br>"
         "The maximum number of mA that the Cowboy Light Bars are allowed to pull from the 42V line combined. Cowboy only.";
}

QStringList build_get_bootloaderCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G006B";
}

QString help_bootloaderCode(void) {
  return "006B Bootloader Code -- 4B -- R<br>"
         "Version 2.0.14+<br>"
         "The unique code used to describe the bootloader installed on the Ember Chip.";
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

QString help_xpressMode(void) {
  return "006C Xpress Mode -- 1B -- RW<br>"
         "Version 2.0.X+<br>"
         "0 for normal operation. 1 for Xpress operation. (Kills radio, change DIP meanings)";
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

QString help_batteryBackupStatus(void) {
  return "006D Battery Backup -- 4B -- RW<br>"
         "Version 2.0.X+<br>"
         "Battery Backup status (only the Short and Long test start bits (10 & 11) are writable)";
}

QStringList build_get_sensorSeconds(QStringList argList) {
  (void) argList;
  return QStringList() << "G006E";
}

QString help_sensorSeconds(void) {
  return "006E Sensor Seconds -- 4B -- R<br>"
         "Version 2.0.X+<br>"
         "Number of Seconds that the sensor has been active. (EEPROM) Reset by !U";
}

QStringList build_get_inputVoltageTwo(QStringList argList) {
  (void) argList;
  return QStringList() << "G006F";
}

QString help_inputVoltageTwo(void) {
  return "006F Input Voltage Two -- 2B -- R<br>"
         "Version 2.0.X+<br>"
         "Input voltage (2nd supply) in mV if calibrated. If 0070 is 0, then this is the ADC value instead to allow for calibration. Cowboy only 36k/48k";
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

QString help_inputVoltageTwoCalibration(void) {
  return "0070 Input Voltage Two Calibration -- 2B -- RW<br>"
         "Version 2.0.X+<br>"
         "Input voltage (2nd supply) Calibration in 3.13 format. round((42000/ADC) * (2^13)). Cowboy only 36k/48k";
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

QString help_maxRampUpSpeed(void) {
  return "0071 Max Ramp Up Speed -- 2B -- RW<br>"
         "Version 2.1.X+<br>"
         "Max speed fixture will increase light level. (80us/%) (On Gen2 this rounds down multiples of 128)";
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

QString help_maxRampDownSpeed(void) {
  return "0072 Max Ramp Down Speed -- 2B -- RW<br>"
         "Version 2.1.X+<br>"
         "Max speed fixture will decrease light level. (80us/%) (On Gen2 this rounds down to multiples of 128)";
}

QStringList build_get_emergencyLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0074";
}

QString help_emergencyLightLevel(void) {
  return "0074 Emergency Light Level -- 2B -- R<br>"
         "Version 2.1.X+<br>"
         "Light level battery backup will use when in emergency mode";
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

QString help_batteryBackupPowerCalibration(void) {
  return "0075 Battery Backup Power Calibration -- 2B -- RW<br>"
         "Version 2.1.X+<br>"
         "Amount of Power (mW) used by the battery backup while charging";
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

QString help_motionSensorProfile(void) {
  return "0076 Motion Sensor Profile -- 1B -- RW -- BAK<br>"
         "Version 2.1.X+<br>"
         "Sensor Profile Setting (FF to use Custom Profile)";
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

QString help_powerMeterLevelAtOff(void) {
  return "0077 Power Meter Level At Off -- 4B -- RW<br>"
         "Version 2.1.X+<br>"
         "Power consumption of devices on DALI bus at light level off";
}

QStringList build_set_powerMeterLevelAtMin(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0078 %1").arg(argList.at(0));
}

QString help_powerMeterLevelAtMin(void) {
  return "0078 Power Meter Level At Min -- 4B -- RW<br>"
         "Version 2.1.X+<br>"
         "Power consumption of devices on DALI bus at light level min";
}

QStringList build_set_powerMeterLevelAtMax(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S0079 %1").arg(argList.at(0));
}

QString help_powerMeterLevelAtMax(void) {
  return "0079 Power Meter Level At Max -- 4B -- RW<br>"
         "Version 2.1.X+<br>"
         "Power consumption of devices on DALI bus at light level max";
}

QStringList build_set_powerMeterType(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007A %1").arg(argList.at(0));
}

QString help_powerMeterType(void) {
  return "007A Power Meter Type -- 1B -- RW<br>"
         "Version 2.1.X+<br>"
         "Method of Power Metering used by DLA (0 = DL, 1 = Philips, 2 = Estimate)";
}

QString help_get_powerMeterConfig(void) {
  QString help = help_powerMeterLevelAtOff() + "<br>";
  help += help_powerMeterLevelAtMin() + "<br>";
  help += help_powerMeterLevelAtMax() + "<br>";
  return help + help_powerMeterType();
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

QString help_DLAiSlaveMode(void) {
  return "007B DLAi Slave Mode -- 1B -- RW<br>"
         "Set for DALI slave operation, clear for normal operation";
}

QStringList build_get_DALIBootlodingActive(QStringList argList) {
  (void) argList;
  return QStringList() << "G007C";
}

QString help_DALIBootlodingActive(void) {
  return "007C DALI Bootloading Active -- 1B -- R<br>"
         "Version 2.1.X+<br>"
         "Set while bootload is in progress";
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

QString help_testingMode(void) {
  return "007D Testing Modes -- 1B -- RW<br>"
         "Version 2.1.2+<br>"
         "0 = None<br>"
         "1 = Occupancy Sensor Test.";
}

QStringList build_get_numberOfBatteriesSupported(QStringList argList) {
  (void) argList;
  return QStringList() << "G007E";
}

QStringList build_set_numberOfBatteriesSupported(QStringList argList) {
  if (argList.length() == 0) {
    return QStringList() << "ERROR: expected a value";
  }
  return QStringList() << QString("S007E %1").arg(argList.at(0));
}

QString help_numberOfBatteriesSupported(void) {
  return "007E Number of Batteries Supported -- 1B -- RW -- BAK<br>"
         "Version 2.1.10+<br>"
         "Only on Cowboy, Set at manufacturing time to 2 for 48K and 1 for other DLEs";
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

QString help_get_lbVersion(void) {
  return "Get Lightbar version information -- 1B -- R<br>"
         "Lightbar number 00 to 07 (no argument = 00), Only on Cowboy";
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
  parsedResponse << QString("String 1 current: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(2).toUShort(&ok, 16);
  parsedResponse << QString("String 2 current: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(3).toUShort(&ok, 16);
  parsedResponse << QString("String 3 current: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse << QString("String 4 current: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(5).toUShort(&ok, 16);
  parsedResponse << QString("String current minimum: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(6).toUShort(&ok, 16);
  parsedResponse << QString("Temperature: %1 C").arg(statusInt);
  statusInt = pmuResponse.at(7).toUShort(&ok, 16);
  parsedResponse << QString("String current sum: %1 mA").arg(statusInt);
  statusInt = pmuResponse.at(8).toUShort(&ok, 16);
  parsedResponse << QString("Voltage reference: %1 volts").arg(statusInt);
  parsedResponse << QString("Light level (0x029C = OFF): %1").arg(pmuResponse.at(9));
  parsedResponse << QString("Light active slew rate: %1").arg(pmuResponse.at(10));
  parsedResponse << QString("Light inactive slew rate: %1").arg(pmuResponse.at(11));
  return parsedResponse;
}

QString help_get_lbStatus(void) {
  return "Get Lightbar status information -- 1B -- R<br>"
         "Lightbar number 00 to 07 (no argument = 00), Only on Cowboy";
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

QString help_get_lbConfig(void) {
  return "Get Lightbar MFG configuration -- 1B -- R<br>"
         "Lightbar number 00 to 07 (no argument = 00), Only on Cowboy";
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

QString help_get_bbVersion(void) {
  return "Get Battery Backup version information -- 1B -- R<br>"
         "Battery number 00 to 01 (no argument = 00), Only on Cowboy";
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
  statusDict.insert(0, "42v active");
  statusDict.insert(1, "42v inactive");
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

QString help_get_bbStatus(void) {
  return "Get Battery Backup status information -- 1B -- R<br>"
         "Battery number 00 to 01 (no argument = 00), Only on Cowboy";
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

QString help_get_bbConfig(void) {
  return "Get Battery Backup MFG configuration -- 1B -- R<br>"
         "Battery number 00 to 01 (no argument = 00), Only on Cowboy";
}

/*** reset commands ***/
QStringList build_reset_usage(QStringList argList) {
  (void) argList;
  return QStringList() << "!U";
}

QString help_reset_usage(void) {
  return "!U -- Reset Usage (non Permanent)";
}

QStringList build_reset_oldLog(QStringList argList) {
  (void) argList;
  return QStringList() << "!L";
}

QString help_reset_oldLog(void) {
  return "!L -- Reset Log";
}

QStringList build_reset_log(QStringList argList) {
  (void) argList;
  return QStringList() << "!K";
}

QString help_reset_log(void) {
  return "!K -- Reset New Log (Do not use in normal operation!) (2.0 command)";
}

QStringList build_reset_logIndex(QStringList argList) {
  return QStringList() << QString("J%1").arg(argList.at(0));
}

QString help_reset_logIndex(void) {
  return "J Seen up to command for new logging (2.0 command)<br>"
         "JIIII\r -- Tell the PMU that you have seen up to I index, so that it can clean up log space. It does not guarantee removal of seen log entries. It will return an Error unless the index is between the Tail and the Head as returned by the K command.";
}

QStringList build_reset_eeprom(QStringList argList) {
  (void) argList;
  return QStringList() << "!Z";
}

QString help_reset_eeprom(void) {
  return "!Z -- Zaps entire EEPROM (1.0.2 command)";
}

QStringList build_reset_eepromToDefault(QStringList argList) {
  (void) argList;
  return QStringList() << "!C";
}

QString help_reset_eepromToDefault(void) {
  return "!C -- Initializes EEPROM (Resets everything to default)";
}

QStringList build_reset_eepromToLatestMapVersion(QStringList argList) {
  (void) argList;
  return QStringList() << "!E";
}

QString help_reset_eepromToLatestMapVersion(void) {
  return "!E -- Upgrades EEPROM to latest map version (1.0.7 command)";
}

QStringList build_reset_network(QStringList argList) {
  (void) argList;
  return QStringList() << "!N";
}

QString help_reset_network(void) {
  return "!E -- Upgrades EEPROM to latest map version (1.0.7 command)";
}

QStringList build_reset_networkWithoutChecking(QStringList argList) {
  (void) argList;
  return QStringList() << "!N1";
}

QString help_reset_networkWithoutChecking(void) {
  return "!N1 -- Changes the network to the network specified in registers 003B to 003E, immediately (1.0.10 command)";
}

QStringList build_reset_daliCommissioning(QStringList argList) {
  (void) argList;
  return QStringList() << "!Y";
}

QString help_reset_daliCommissioning(void) {
  return "!Y -- Commission DALI devices. Search and assign devices starting at addr 0x0";
}

QStringList build_reset_daliPowerMetering(QStringList argList) {
  (void) argList;
  return QStringList() << "!A";
}

QString help_reset_daliPowerMetering(void) {
  return "!A -- Power Meter DALI devices. Will run DALI devices to three light levels, then record sum of power meter values 0x8001, 0x8000, 0x0000";
}

/*** reboot commands ***/
QStringList build_reboot_pmu(QStringList argList) {
  (void) argList;
  return QStringList() << "!R";
}

QString help_reboot_pmu(void) {
  return "!R -- Reboot";
}

QStringList build_reboot_wirelessCard(QStringList argList) {
  (void) argList;
  return QStringList() << "!W";
}

QString help_reboot_wirelessCard(void) {
  return "!W -- Restarts Wireless Card (1.0 command)";
}

QStringList build_reboot_i2cDevices(QStringList argList) {
  (void) argList;
  return QStringList() << "!X";
}

QString help_reboot_i2cDevices(void) {
  return "!X -- Reset all I2C devices (Cowboy only, Version 2.1.10 and later)";
}

/*** reload commands ***/
QStringList build_reload_dlaFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!B";
}

QString help_reload_dlaFirmware(void) {
  return "!B -- Bootload PIC from stored image";
}

QStringList build_reload_wirelessModuleFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!M";
}

QString help_reload_wirelessModuleFirmware(void) {
  return "!M -- Bootload Wireless Module from stored image";
}

QStringList build_reload_powerboardFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QString help_reload_powerboardFirmware(void) {
  return "!P -- Bootload Power Board from stored image";
}

QStringList build_reload_lightbarFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QString help_reload_lightbarFirmware(void) {
  return "PAA -- Bootload DLE light bar or battery backup unit, where AA is the driver address (Version 2.1.0 and later)";
}

QStringList build_reload_batteryBackupFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QString help_reload_batteryBackupFirmware(void) {
  return "PAA -- Bootload DLE light bar or battery backup unit, where AA is the driver address (Version 2.1.0 and later)";
}

QStringList build_reload_motionSensorFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!V";
}

QString help_reload_motionSensorFirmware(void) {
  return "V -- Bootload Motion Sensor from stored image";
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

QString help_get_logIndex(void) {
  return "K New Logging Command (2.0 command)<br>"
         "K\r -- returns tail and head indices of the log (P values added in 2.0.2)<br>"
         "K: TTTTHHHHPPPP -- T is a 15 bit tail index inclusive. H is a 15 bit head exclusive. P is the index of the most recent power up, set to 0xFFFF if there isn't one.";
}

QStringList build_get_log(QStringList argList) {
  QString index;
  if (argList.length() == 0) {
    index = "0000";
  } else {
    index = argList.at(0);
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
    // parse log entry
    if (eventType == "00") {
      // build a dictionary of power events
      QMap <QString, QString> powerEventsMap;
      powerEventsMap.insert("00", "Power down");
      powerEventsMap.insert("01", "Power up");
      powerEventsMap.insert("02", "Power restored");
      powerEventsMap.insert("03", "Power soft reset");
      log += QString("%1 > %2").arg(uptime).arg(powerEventsMap[eventValue]);
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
      log += QString("%1 > %2").arg(uptime).arg(activityStateTransitionEventsMap[eventValue]);
    } else if (eventType == "02") {
      // type 2 events are not implemented
      log += QString("%1 > Type 2 event: %2").arg(uptime).arg(eventValue);
    } else if (eventType == "03") {
      log += QString("%1 > Sensor off: %2").arg(uptime).arg(eventValue);
    } else if (eventType == "04") {
      // unspecified value
      log += QString("%1 > SerialNet watchdog tripped").arg(uptime);
    } else if (eventType == "05") {
      log += QString("%1 > Temperature state change: %2").arg(uptime).arg(eventValue);
    } else if (eventType == "06") {
      log += QString("%1 > Lightbar error: %2").arg(uptime).arg(eventValue);
    } else if (eventType == "07") {
      log += QString("%1 > RTC set event: %2").arg(uptime).arg(eventValue);
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
      log += QString("%1 > Battery backup %2 event: %3").arg(uptime).arg(batteryNumber).arg(batteryBackupEventsMap[eventValue]);
    } else if (eventType == "09") {
      log += QString("%1 > I2C watchdog reset event: %2").arg(uptime).arg(eventValue);
    } else if (eventType == "0A") {
      // unspecified value
      log += QString("%1 > Registers restored from backup").arg(uptime);
    } else {
      // unknown event
      log += QString("%1 > Type %2 event: %3").arg(uptime).arg(eventType).arg(eventValue);
    }
    // insert a break
    log += "<br>";
  } while (arg1.isEmpty() != true);
  // notify the user that more logs are available
  if (isLastEntry == true) {
    log += "[End of log]";
  } else {
    log += "[More events available...]";
  }
  return QStringList() << log;
}

QString help_get_log(void) {
  return "Get the fixture log starting at index IIII (no arguments = index 0000) -- 2 bytes";
}

QStringList build_insert_logEntry(QStringList argList) {
  return QStringList() << QString("E%1").arg(argList.at(0));
}

QString help_insert_logEntry(void) {
  return "E Insert Event into logger (2.0.1 command)<br>"
         "ETTVVVVVVVV\r -- Force the PMU to insert an event into the log with type T (1 byte) and V (4 byte). V is big endian and will be converted to correct size for type T."
         "ETTVVVVVVVVSSSSSSSS\r -- Same as above but adds S (4 bytes) seconds to the uptime before inserting the event.";
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_cmdTable.insert("get firmwareVersion", new cmdEntry(build_get_firmwareVersion, parse_get_firmwareVersion, help_firmwareVersion));
  m_cmdTable.insert("get productCode", new cmdEntry(build_get_productCode, help_productCode));
  m_cmdTable.insert("set productCode", new cmdEntry(build_set_productCode, help_productCode));
  m_cmdTable.insert("get serialNumber", new cmdEntry(build_get_serialNumber, help_serialNumber));
  m_cmdTable.insert("set serialNumber", new cmdEntry(build_set_serialNumber, help_serialNumber));
  m_cmdTable.insert("get unixTime", new cmdEntry(build_get_unixTime, help_unixTime));
  m_cmdTable.insert("set unixTime", new cmdEntry(build_set_unixTime, help_unixTime));
  m_cmdTable.insert("get temperature", new cmdEntry(build_get_temperature, parse_get_temperature, help_temperature));
  m_cmdTable.insert("get lightLevel", new cmdEntry(build_get_lightLevel, parse_get_lightLevel, help_get_lightLevel));
  m_cmdTable.insert("set lightManualLevel", new cmdEntry(build_set_lightManualLevel, help_lightManualLevel));
  m_cmdTable.insert("set lightOverrideActiveLevel", new cmdEntry(build_set_lightOverrideActiveLevel, help_lightOverrideActiveLevel));
  m_cmdTable.insert("set lightOverrideInactiveLevel", new cmdEntry(build_set_lightOverrideInactiveLevel, help_lightOverrideInactiveLevel));
  m_cmdTable.insert("get sensorDelayTime", new cmdEntry(build_get_sensorDelayTime, help_sensorDelayTime));
  m_cmdTable.insert("get sensorOverrideDelayTime", new cmdEntry(build_get_sensorOverrideDelayTime, help_sensorOverrideDelayTime));
  m_cmdTable.insert("set sensorOverrideDelayTime", new cmdEntry(build_set_sensorOverrideDelayTime, help_sensorOverrideDelayTime));
  m_cmdTable.insert("get upTime", new cmdEntry(build_get_upTime, parse_get_upTime, help_upTime));
  m_cmdTable.insert("get usage", new cmdEntry(build_get_usage, parse_get_usage, help_get_usage));
  m_cmdTable.insert("get numberOfLogEntries", new cmdEntry(build_get_numberOfLogEntries, help_numberOfLogEntries));
  m_cmdTable.insert("get configCalibration", new cmdEntry(build_get_configCalibration, parse_get_configCalibration, help_configCalibration));
  m_cmdTable.insert("set configCalibrationP0", new cmdEntry(build_set_configCalibrationP0, help_configCalibrationP0));
  m_cmdTable.insert("set configCalibrationP1", new cmdEntry(build_set_configCalibrationP1, help_configCalibrationP1));
  m_cmdTable.insert("set configCalibrationP2", new cmdEntry(build_set_configCalibrationP2, help_configCalibrationP2));
  m_cmdTable.insert("set configCalibrationP3", new cmdEntry(build_set_configCalibrationP3, help_configCalibrationP3));
  m_cmdTable.insert("get buildTime", new cmdEntry(build_get_buildTime, help_buildTime));
  m_cmdTable.insert("set buildTime", new cmdEntry(build_set_buildTime, help_buildTime));
  m_cmdTable.insert("get sensorTimeoutCountdown", new cmdEntry(build_get_sensorTimeoutCountdown, help_sensorTimeoutCountdown));
  m_cmdTable.insert("get currentLightLevel", new cmdEntry(build_get_currentLightLevel, help_currentLightLevel));
  m_cmdTable.insert("get safeMode", new cmdEntry(build_get_safeMode, help_safeMode));
  m_cmdTable.insert("get lightBarSelect", new cmdEntry(build_get_lightBarSelect, help_lightBarSelect));
  m_cmdTable.insert("set lightBarSelect", new cmdEntry(build_set_lightBarSelect, help_lightBarSelect));
  m_cmdTable.insert("get currentPowerConsumption", new cmdEntry(build_get_currentPowerConsumption, help_currentPowerConsumption));
  m_cmdTable.insert("get wirelessDataAggregator", new cmdEntry(build_get_wirelessDataAggregator, help_wirelessDataAggregator));
  m_cmdTable.insert("set wirelessDataAggregator", new cmdEntry(build_set_wirelessDataAggregator, help_wirelessDataAggregator));
  m_cmdTable.insert("get resetUsageTimestamp", new cmdEntry(build_get_resetUsageTimestamp, help_resetUsageTimestamp));
  m_cmdTable.insert("get pwmPeriodRegister", new cmdEntry(build_get_pwmPeriodRegister, help_pwmPeriodRegister));
  m_cmdTable.insert("set pwmPeriodRegister", new cmdEntry(build_set_pwmPeriodRegister, help_pwmPeriodRegister));
  m_cmdTable.insert("get analogSensorValue", new cmdEntry(build_get_analogSensorValue, help_analogSensorValue));
  m_cmdTable.insert("get analogReportingHysteresis", new cmdEntry(build_get_analogReportingHysteresis, help_analogReportingHysteresis));
  m_cmdTable.insert("get zone", new cmdEntry(build_get_zone, help_zone));
  m_cmdTable.insert("set zone", new cmdEntry(build_set_zone, help_zone));
  m_cmdTable.insert("get lightTemporaryActiveLevel", new cmdEntry(build_get_lightTemporaryActiveLevel, help_lightTemporaryActiveLevel));
  m_cmdTable.insert("set lightTemporaryActiveLevel", new cmdEntry(build_set_lightTemporaryActiveLevel, help_lightTemporaryActiveLevel));
  m_cmdTable.insert("get lightTemporaryInactiveLevel", new cmdEntry(build_get_lightTemporaryInactiveLevel, help_lightTemporaryInactiveLevel));
  m_cmdTable.insert("set lightTemporaryInactiveLevel", new cmdEntry(build_set_lightTemporaryInactiveLevel, help_lightTemporaryInactiveLevel));
  m_cmdTable.insert("get sensorTemporaryDelayTime", new cmdEntry(build_get_sensorTemporaryDelayTime, help_sensorTemporaryDelayTime));
  m_cmdTable.insert("set sensorTemporaryDelayTime", new cmdEntry(build_set_sensorTemporaryDealyTime, help_sensorTemporaryDelayTime));
  m_cmdTable.insert("get temporaryOverrideTimeout", new cmdEntry(build_get_temporaryOverrideTimeout, help_temporaryOverrideTimeout));
  m_cmdTable.insert("set temporaryOverrideTimeout", new cmdEntry(build_set_temporaryOverrideTiemout, help_temporaryOverrideTimeout));
  m_cmdTable.insert("get setRemoteState", new cmdEntry(build_get_setRemoteState, help_setRemoteState));
  m_cmdTable.insert("set setRemoteState", new cmdEntry(build_set_setRemoteState, help_setRemoteState));
  m_cmdTable.insert("get remoteSetDelayTime", new cmdEntry(build_get_remoteStateDelayTime, help_remoteStateDelayTime));
  m_cmdTable.insert("set remoteSetDelayTime", new cmdEntry(build_set_remoteStateDelayTime, help_remoteStateDelayTime));
  m_cmdTable.insert("get remoteSecondsCountdown", new cmdEntry(build_get_remoteSecondsCountdown, help_remoteSecondsCountdown));
  m_cmdTable.insert("get minimumDimmingValue", new cmdEntry(build_get_minimumDimmingValue, help_minimumDimmingValue));
  m_cmdTable.insert("get powerCalibration", new cmdEntry(build_get_powerCalibration, parse_get_powerCalibration, help_get_powerCalibration));
  m_cmdTable.insert("set powerCalibrationA0", new cmdEntry(build_set_powerCalibrationA0, help_powerCalibrationA0));
  m_cmdTable.insert("set powerCalibrationB0", new cmdEntry(build_set_powerCalibrationB0, help_powerCalibrationB0));
  m_cmdTable.insert("set powerCalibrationC0", new cmdEntry(build_set_powerCalibrationC0, help_powerCalibrationC0));
  m_cmdTable.insert("set powerCalibrationMA", new cmdEntry(build_set_powerCalibrationMA, help_powerCalibrationMA));
  m_cmdTable.insert("set powerCalibrationMB", new cmdEntry(build_set_powerCalibrationMB, help_powerCalibrationMB));
  m_cmdTable.insert("set powerCalibrationMC", new cmdEntry(build_set_powerCalibrationMC, help_powerCalibrationMC));
  m_cmdTable.insert("set powerCalibrationPOff", new cmdEntry(build_set_powerCalibrationPOff, help_powerCalibrationPOff));
  m_cmdTable.insert("set powerCalibrationPOn", new cmdEntry(build_set_powerCalibrationPOn, help_powerCalibrationPOn));
  m_cmdTable.insert("set powerCalibrationT0", new cmdEntry(build_set_powerCalibrationT0, help_powerCalibrationT0));
  m_cmdTable.insert("get powerEstimatorTemperatureOverride", new cmdEntry(build_get_powerEstimatorTemperatureOverride, help_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("set powerEstimatorTemperatureOverride", new cmdEntry(build_set_powerEstimatorTemperatureOverride, help_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("get cachedTemperatureValue", new cmdEntry(build_get_cachedTemperatureValue, help_cachedTemperatureValue));
  m_cmdTable.insert("get eepromSize", new cmdEntry(build_get_eepromSize, help_eepromSize));
  m_cmdTable.insert("get hardwareRevision", new cmdEntry(build_get_hardwareRevision, help_hardwareRevision));
  m_cmdTable.insert("get wirelessConfig", new cmdEntry(build_get_wirelessConfig, parse_get_wirelessConfig, help_get_wirelessConfig));
  m_cmdTable.insert("set wirelessPanId", new cmdEntry(build_set_wirelessPanId, help_wirelessPanId));
  m_cmdTable.insert("set wirelessChannelMask", new cmdEntry(build_set_wirelessChannelMask, help_wirelessChannelMask));
  m_cmdTable.insert("set wirelessShortAddress", new cmdEntry(build_set_wirelessShortAddress, help_wirelessShortAddress));
  m_cmdTable.insert("set wirelessRole", new cmdEntry(build_set_wirelessRole, help_wirelessRole));
  m_cmdTable.insert("set wirelessWatchdogHold", new cmdEntry(build_set_wirelessWatchdogHold, help_wirelessWatchdogHold));
  m_cmdTable.insert("set wirelessWatchdogPeriod", new cmdEntry(build_set_wirelessWatchdogPeriod, help_wirelessWatchdogPeriod));
  m_cmdTable.insert("set wirelessNetworkKey", new cmdEntry(build_set_wirelessNetworkKey, help_wirelessNetworkKey));
  m_cmdTable.insert("get firmwareCode", new cmdEntry(build_get_firmwareCode, help_firmwareCode));
  m_cmdTable.insert("get moduleFirmwareCode", new cmdEntry(build_get_moduleFirmwareCode, help_moduleFirmwareCode));
  m_cmdTable.insert("get maxTemperature", new cmdEntry(build_get_maxTemperature, parse_get_maxTemperature, help_get_maxTemperature));
  m_cmdTable.insert("get overTemperatureConfig", new cmdEntry(build_get_overTemperatureConfig, parse_get_overTemperatureConfig, help_get_overTemperatureConfig));
  m_cmdTable.insert("set overTemperatureThresholdLow", new cmdEntry(build_set_overTemperatureThresholdLow, help_overTemperatureThresholdLow));
  m_cmdTable.insert("set overTemperatureThresholdHigh", new cmdEntry(build_get_overTemperatureThresholdHigh, help_overTemperatureThresholdHigh));
  m_cmdTable.insert("set overTemperatureDimmingLimit", new cmdEntry(build_set_overTemperatureDimmingLimit, help_overTemperatureDimmingLimit));
  m_cmdTable.insert("get analogDimmingMode", new cmdEntry(build_get_analogDimmingMode, help_analogDimmingMode));
  m_cmdTable.insert("set analogDimmingMode", new cmdEntry(build_set_analogDimmingMode, help_analogDimmingMode));
  m_cmdTable.insert("get fixtureIdMode", new cmdEntry(build_get_fixtureIdMode, help_fixtureIdMode));
  m_cmdTable.insert("set fixtureIdMode", new cmdEntry(build_set_fixtureIdMode, help_fixtureIdMode));
  m_cmdTable.insert("get acFrequency", new cmdEntry(build_get_acFrequency, help_acFrequency));
  m_cmdTable.insert("get sensorBits", new cmdEntry(build_get_sensorBits, help_sensorBits));
  m_cmdTable.insert("get powerMeterCommand", new cmdEntry(build_get_powerMeterCommand, help_powerMeterCommand));
  m_cmdTable.insert("set powerMeterCommand", new cmdEntry(build_set_powerMeterCommand, help_powerMeterCommand));
  m_cmdTable.insert("get powerMeterRegister", new cmdEntry(build_get_powerMeterRegister, help_powerMeterRegister));
  m_cmdTable.insert("set powerMeterRegister", new cmdEntry(build_set_powerMeterRegister, help_powerMeterRegister));
  m_cmdTable.insert("get ambientTemperature", new cmdEntry(build_get_ambientTemperature, help_ambientTemperature));
  m_cmdTable.insert("get lightSensorLevel", new cmdEntry(build_get_lightSensorLevel, help_lightSensorLevel));
  m_cmdTable.insert("get sensorConfig", new cmdEntry(build_get_sensorConfig, parse_get_sensorConfig, help_get_sensorConfig));
  m_cmdTable.insert("set sensor0Timeout", new cmdEntry(build_set_sensor0Timeout, help_sensor0Timeout));
  m_cmdTable.insert("set sensor0Offset", new cmdEntry(build_set_sensor0Offset, help_sensor0Offset));
  m_cmdTable.insert("set sensor1Timeout", new cmdEntry(build_set_sensor1Timeout, help_sensor1Timeout));
  m_cmdTable.insert("set sensor1Offset", new cmdEntry(build_set_sensor1Offset, help_sensor1Offset));
  m_cmdTable.insert("get analogDimmingConfig", new cmdEntry(build_get_analogDimmingConfig, parse_get_analogDimmingConfig, help_get_analogDimmingConfig));
  m_cmdTable.insert("set analogDimmingLowValue", new cmdEntry(build_set_analogDimmingLowValue, help_analogDimmingLowValue));
  m_cmdTable.insert("set analogDimmingHighValue", new cmdEntry(build_set_analogDimmingHighValue, help_analogDimmingHighValue));
  m_cmdTable.insert("set analogDimmingOffValue", new cmdEntry(build_set_analogDimmingOffValue, help_analogDimmingOffValue));
  m_cmdTable.insert("get powerMeasurementMode", new cmdEntry(build_get_powerMeasurementMode, help_powerMeasurementMode));
  m_cmdTable.insert("set powerMeasurementMode", new cmdEntry(build_set_powerMeasurementMode, help_powerMeasurementMode));
  m_cmdTable.insert("get externalPowerMeter", new cmdEntry(build_get_externalPowerMeter, help_externalPowerMeter));
  m_cmdTable.insert("set externalPowerMeter", new cmdEntry(build_set_externalPowerMeter, help_externalPowerMeter));
  m_cmdTable.insert("get ambientSensorValue", new cmdEntry(build_get_ambientSensorValue, help_ambientSensorValue));
  m_cmdTable.insert("get ambientConfig", new cmdEntry(build_get_ambientConfig, parse_get_ambientConfig, help_get_ambientConfig));
  m_cmdTable.insert("set ambientActiveLevel", new cmdEntry(build_set_ambientActiveLevel, help_ambientActiveLevel));
  m_cmdTable.insert("set ambientInactiveLevel", new cmdEntry(build_set_ambientInactiveLevel, help_ambientInactiveLevel));
  m_cmdTable.insert("set ambientEnvironmentalGain", new cmdEntry(build_set_ambientEnvironmentalGain, help_ambientEnvironmentalGain));
  m_cmdTable.insert("set ambientOffHysteresis", new cmdEntry(build_set_ambientOffHysteresis, help_ambientOffHysteresis));
  m_cmdTable.insert("set ambientOnHysteresis", new cmdEntry(build_set_ambientOnHysteresis, help_ambientOnHysteresis));
  m_cmdTable.insert("get powerboardProtocol", new cmdEntry(build_get_powerboardProtocol, help_powerboardProtocol));
  m_cmdTable.insert("get ledOverride", new cmdEntry(build_get_ledOverride, help_ledOverride));
  m_cmdTable.insert("set ledOverride", new cmdEntry(build_set_ledOverride, help_ledOverride));
  m_cmdTable.insert("get fadeUpStep", new cmdEntry(build_get_fadeUpStep, help_fadeUpStep));
  m_cmdTable.insert("set fadeUpStep", new cmdEntry(build_set_fadeUpStep, help_fadeUpStep));
  m_cmdTable.insert("get fadeDownStep", new cmdEntry(build_get_fadeDownStep, help_fadeDownStep));
  m_cmdTable.insert("set fadeDownStep", new cmdEntry(build_set_fadeDownStep, help_fadeDownStep));
  m_cmdTable.insert("get maxBrightness", new cmdEntry(build_get_maxBrightness, help_maxBrightness));
  m_cmdTable.insert("set maxBrightness", new cmdEntry(build_set_maxBrightness, help_maxBrightness));
  m_cmdTable.insert("get i2cResets", new cmdEntry(build_get_i2cResets, help_i2cResets));
  m_cmdTable.insert("get sensorGuardTime", new cmdEntry(build_get_sensorGuardTime, help_sensorGuardTime));
  m_cmdTable.insert("set sensorGuardTime", new cmdEntry(build_set_sensorGuardTime, help_sensorGuardTime));
  m_cmdTable.insert("get inputVoltage", new cmdEntry(build_get_inputVoltage, help_inputVoltage));
  m_cmdTable.insert("get inputVoltageCalibration", new cmdEntry(build_get_inputVoltageCalibration, help_inputVoltageCalibration));
  m_cmdTable.insert("set inputVoltageCalibration", new cmdEntry(build_set_inputVoltageCalibration, help_inputVoltageCalibration));
  m_cmdTable.insert("get numberOfLightbars", new cmdEntry(build_get_numberOfLightbars, help_numberOfLightbars));
  m_cmdTable.insert("set numberOfLightbars", new cmdEntry(build_set_numberOfLightbars, help_numberOfLightbars));
  m_cmdTable.insert("get currentLimit", new cmdEntry(build_get_currentLimit, help_currentLimit));
  m_cmdTable.insert("set currentLimit", new cmdEntry(build_set_currentLimit, help_currentLimit));
  m_cmdTable.insert("get bootloaderCode", new cmdEntry(build_get_bootloaderCode, help_bootloaderCode));
  m_cmdTable.insert("get xpressMode", new cmdEntry(build_get_xpressMode, help_xpressMode));
  m_cmdTable.insert("set xpressMode", new cmdEntry(build_set_xpressMode, help_xpressMode));
  m_cmdTable.insert("get batteryBackupStatus", new cmdEntry(build_get_batteryBackupStatus, parse_get_batteryBackupStatus, help_batteryBackupStatus));
  m_cmdTable.insert("set batteryBackupStatus", new cmdEntry(build_set_batteryBackupStatus, help_batteryBackupStatus));
  m_cmdTable.insert("get sensorSeconds", new cmdEntry(build_get_sensorSeconds, help_sensorSeconds));
  m_cmdTable.insert("get inputVoltageTwo", new cmdEntry(build_get_inputVoltageTwo, help_inputVoltageTwo));
  m_cmdTable.insert("get inputVoltageTwoCalibration", new cmdEntry(build_get_inputVoltageTwoCalibration, help_inputVoltageTwoCalibration));
  m_cmdTable.insert("set inputVoltageTwoCalibration", new cmdEntry(build_set_inputVoltageTwoCalibration, help_inputVoltageTwoCalibration));
  m_cmdTable.insert("get maxRampUpSpeed", new cmdEntry(build_get_maxRampUpSpeed, help_maxRampUpSpeed));
  m_cmdTable.insert("set maxRampUpSpeed", new cmdEntry(build_set_maxRampUpSpeed, help_maxRampUpSpeed));
  m_cmdTable.insert("get maxRampDownSpeed", new cmdEntry(build_get_maxRampDownSpeed, help_maxRampDownSpeed));
  m_cmdTable.insert("set maxRampDownSpeed", new cmdEntry(build_set_maxRampDownSpeed, help_maxRampDownSpeed));
  m_cmdTable.insert("get emergencyLightLevel", new cmdEntry(build_get_emergencyLightLevel, help_emergencyLightLevel));
  m_cmdTable.insert("get batteryBackupPowerCalibration", new cmdEntry(build_get_batteryBackupPowerCalibration, help_batteryBackupPowerCalibration));
  m_cmdTable.insert("set batteryBackupPowerCalibration", new cmdEntry(build_set_batteryBackupPowerCalibration, help_batteryBackupPowerCalibration));
  m_cmdTable.insert("get motionSensorProfile", new cmdEntry(build_get_motionSensorProfile, help_motionSensorProfile));
  m_cmdTable.insert("set motionSensorProfile", new cmdEntry(build_set_motionSensorProfile, help_motionSensorProfile));
  m_cmdTable.insert("get powerMeterConfig", new cmdEntry(build_get_powerMeterConfig, parse_get_powerMeterConfig, help_get_powerMeterConfig));
  m_cmdTable.insert("set powerMeterLevelAtOff", new cmdEntry(build_set_powerMeterLevelAtOff, help_powerMeterLevelAtOff));
  m_cmdTable.insert("set powerMeterLevelAtMin", new cmdEntry(build_set_powerMeterLevelAtMin, help_powerMeterLevelAtMin));
  m_cmdTable.insert("set powerMeterLevelAtMax", new cmdEntry(build_set_powerMeterLevelAtMax, help_powerMeterLevelAtMax));
  m_cmdTable.insert("set powerMeterType", new cmdEntry(build_set_powerMeterType, help_powerMeterType));
  m_cmdTable.insert("get DLAiSlaveMode", new cmdEntry(build_get_DLAiSlaveMode, help_DLAiSlaveMode));
  m_cmdTable.insert("set DLAiSlaveMode", new cmdEntry(build_set_DLAiSlaveMode, help_DLAiSlaveMode));
  m_cmdTable.insert("get DALIBootloadingActive", new cmdEntry(build_get_DALIBootlodingActive, help_DALIBootlodingActive));
  m_cmdTable.insert("get testingMode", new cmdEntry(build_get_testingMode, help_testingMode));
  m_cmdTable.insert("set testingMode", new cmdEntry(build_set_testingMode, help_testingMode));
  m_cmdTable.insert("get numberOfBatteriesSupported", new cmdEntry(build_get_numberOfBatteriesSupported, help_numberOfBatteriesSupported));
  m_cmdTable.insert("set numberOfBatteriesSupported", new cmdEntry(build_set_numberOfBatteriesSupported, help_numberOfBatteriesSupported));
  // get & set lightbar commands
  m_cmdTable.insert("get lbVersion", new cmdEntry(build_get_lbVersion, parse_get_lbVersion, help_get_lbVersion));
  m_cmdTable.insert("get lbStatus", new cmdEntry(build_get_lbStatus, parse_get_lbStatus, help_get_lbStatus));
  m_cmdTable.insert("get lbConfig", new cmdEntry(build_get_lbConfig, parse_get_lbConfig, help_get_lbConfig));
  // get & set battery backup commands
  m_cmdTable.insert("get bbVersion", new cmdEntry(build_get_bbVersion, parse_get_bbVersion, help_get_bbVersion));
  m_cmdTable.insert("get bbStatus", new cmdEntry(build_get_bbStatus, parse_get_bbStatus, help_get_bbStatus));
  m_cmdTable.insert("get bbConfig", new cmdEntry(build_get_bbConfig, parse_get_bbConfig, help_get_bbConfig));
  // reset commands
  m_cmdTable.insert("reset usage", new cmdEntry(build_reset_usage, help_reset_usage));
  m_cmdTable.insert("reset oldLog", new cmdEntry(build_reset_oldLog, help_reset_oldLog));
  m_cmdTable.insert("reset log", new cmdEntry(build_reset_log, help_reset_log));
  m_cmdTable.insert("reset logIndex", new cmdEntry(build_reset_logIndex, help_reset_logIndex));
  m_cmdTable.insert("reset eeprom", new cmdEntry(build_reset_eeprom, help_reset_eeprom));
  m_cmdTable.insert("reset eepromToDefault", new cmdEntry(build_reset_eepromToDefault, help_reset_eepromToDefault));
  m_cmdTable.insert("reset eepromToLatestMapVersion", new cmdEntry(build_reset_eepromToLatestMapVersion, help_reset_eepromToLatestMapVersion));
  m_cmdTable.insert("reset network", new cmdEntry(build_reset_network, help_reset_network));
  m_cmdTable.insert("reset networkWithoutChecking", new cmdEntry(build_reset_networkWithoutChecking, help_reset_networkWithoutChecking));
  m_cmdTable.insert("reset daliCommissioning", new cmdEntry(build_reset_daliCommissioning, help_reset_daliCommissioning));
  m_cmdTable.insert("reset daliPowerMetering", new cmdEntry(build_reset_daliPowerMetering, help_reset_daliPowerMetering));
  // reboot commands
  m_cmdTable.insert("reboot pmu", new cmdEntry(build_reboot_pmu, help_reboot_pmu));
  m_cmdTable.insert("reboot wirelessCard", new cmdEntry(build_reboot_wirelessCard, help_reboot_wirelessCard));
  m_cmdTable.insert("reboot i2cDevices", new cmdEntry(build_reboot_i2cDevices, help_reboot_i2cDevices));
  // reload commands
  m_cmdTable.insert("reload dlaFirmware", new cmdEntry(build_reload_dlaFirmware, help_reload_dlaFirmware));
  m_cmdTable.insert("reload wirelessModuleFirmware", new cmdEntry(build_reload_wirelessModuleFirmware, help_reload_wirelessModuleFirmware));
  m_cmdTable.insert("reload powerboardFirmware", new cmdEntry(build_reload_powerboardFirmware, help_reload_powerboardFirmware));
  m_cmdTable.insert("reload lightbarFirmware", new cmdEntry(build_reload_lightbarFirmware, help_reload_lightbarFirmware));
  m_cmdTable.insert("reload batteryBackupFirmware", new cmdEntry(build_reload_batteryBackupFirmware, help_reload_batteryBackupFirmware));
  m_cmdTable.insert("reload motionSensorFirmware", new cmdEntry(build_reload_motionSensorFirmware, help_reload_motionSensorFirmware));
  // log commands
  m_cmdTable.insert("get logIndex", new cmdEntry(build_get_logIndex, parse_get_logIndex, help_get_logIndex));
  m_cmdTable.insert("get log", new cmdEntry(build_get_log, parse_get_log, help_get_log));
  m_cmdTable.insert("insert logEntry", new cmdEntry(build_insert_logEntry, help_insert_logEntry));
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

QString cmdHelper::getNextCompletion(void) {
  m_cmdCompleter->setCurrentRow(m_cmdCompleter->currentRow() + 1);
  return m_cmdCompleter->currentCompletion();
}

int cmdHelper::getCurrentCompletionLength(void) {
  return m_cmdCompleter->currentCompletion().length();
}
