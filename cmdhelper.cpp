#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

QString parse_get_firmwareVersion(QStringList pmuResponse) {
  bool ok;
  qulonglong verInt = pmuResponse.at(0).toULongLong(&ok, 16);
  // format verMajor.verMinor.verBuild (buildMonth/buildDay/BuildYear)
  return QString("%1.%2.%3 (%5/%6/%4)").arg((verInt >> 40) & 0xFF).arg((verInt >> 32) & 0xFF).arg((verInt >> 24) & 0xFF).arg((verInt >> 16) & 0xFF).arg((verInt >> 8) & 0xFF).arg(verInt & 0xFF);
}

QString parse_get_temperature(QStringList pmuResponse) {
  return "todo";
}

QString parse_get_batteryBackupStatus(QStringList pmuResponse) {
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
  parsedResponse = (battDetectedDict[status & 0x3] + "<br>");
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
  return parsedResponse;
}

QString parse_get_lbFirmwareVersion(QStringList pmuResponse) {
  bool ok;
  quint16 verHiInt = pmuResponse.at(0).toUShort(&ok, 16);
  quint16 verLoInt = pmuResponse.at(1).toUShort(&ok, 16);
  return QString("%1.%2.%3").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_cmdTable.insert("get firmwareVersion", new pmu(QStringList("G0000"), parse_get_firmwareVersion));
  m_cmdTable.insert("get productCode", new pmu(QStringList("G0001")));
  m_cmdTable.insert("set productCode", new pmu(QStringList("S0001")));
  m_cmdTable.insert("get serialNumber", new pmu(QStringList("G0002")));
  m_cmdTable.insert("set serialNumber", new pmu(QStringList("S0002")));
  m_cmdTable.insert("get unixTime", new pmu(QStringList("G0003")));
  m_cmdTable.insert("set unixTime", new pmu(QStringList("S0003")));
  m_cmdTable.insert("get temperature", new pmu(QStringList("G0004"), parse_get_temperature));
  m_cmdTable.insert("get lightManualLevel", new pmu(QStringList("G0005")));
  m_cmdTable.insert("set lightManualLevel", new pmu(QStringList("S0005")));
  m_cmdTable.insert("get lightActiveLevel", new pmu(QStringList("G0006")));
  m_cmdTable.insert("get lightInactiveLevel", new pmu(QStringList("G0007")));
  m_cmdTable.insert("get lightOverrideActiveLevel", new pmu(QStringList("G0008")));
  m_cmdTable.insert("set lightOverrideActiveLevel", new pmu(QStringList("S0008")));
  m_cmdTable.insert("get lightOverrideInactiveLevel", new pmu(QStringList("G0009")));
  m_cmdTable.insert("set lightOverrideInactiveLevel", new pmu(QStringList("S0009")));
  m_cmdTable.insert("get sensorDelayTime", new pmu(QStringList("G000A")));
  m_cmdTable.insert("get sensorOverrideDelayTime", new pmu(QStringList("G000B")));
  m_cmdTable.insert("set sensorOverrideDelayTime", new pmu(QStringList("S000B")));
  m_cmdTable.insert("get powerupTime", new pmu(QStringList("G000C")));
  m_cmdTable.insert("get usageActiveTime", new pmu(QStringList("G000D")));
  m_cmdTable.insert("get usageInactiveTime", new pmu(QStringList("G000E")));
  m_cmdTable.insert("get usagePermActiveTime", new pmu(QStringList("G000F")));
  m_cmdTable.insert("get usagePermInactiveTime", new pmu(QStringList("G0010")));
  m_cmdTable.insert("get usageWh", new pmu(QStringList("G0011")));
  m_cmdTable.insert("get usagePerm", new pmu(QStringList("G0012")));
  m_cmdTable.insert("get usageSenorEvents", new pmu(QStringList("G0013")));
  m_cmdTable.insert("get usagePermSensorEvents", new pmu(QStringList("G0014")));
  m_cmdTable.insert("get numberOfLogEntries", new pmu(QStringList("G0015")));
  m_cmdTable.insert("get configCalibrationP0", new pmu(QStringList("G0016")));
  m_cmdTable.insert("set configCalibrationP0", new pmu(QStringList("S0016")));
  m_cmdTable.insert("get configCalibrationP1", new pmu(QStringList("G0017")));
  m_cmdTable.insert("set configCalibrationP1", new pmu(QStringList("S0017")));
  m_cmdTable.insert("get configCalibrationP2", new pmu(QStringList("G0018")));
  m_cmdTable.insert("set configCalibrationP2", new pmu(QStringList("S0018")));
  m_cmdTable.insert("get configCalibrationP3", new pmu(QStringList("G0019")));
  m_cmdTable.insert("set configCalibrationP3", new pmu(QStringList("S0019")));
  m_cmdTable.insert("get buildTime", new pmu(QStringList("G001A")));
  m_cmdTable.insert("set buildTime", new pmu(QStringList("S001A")));
  m_cmdTable.insert("get sensorTimeoutCountdown", new pmu(QStringList("G001B")));
  m_cmdTable.insert("get currentLightLevel", new pmu(QStringList("G001C")));
  m_cmdTable.insert("get safeMode", new pmu(QStringList("G001D")));
  m_cmdTable.insert("get lightBarSelect", new pmu(QStringList("G001E")));
  m_cmdTable.insert("set lightBarSelect", new pmu(QStringList("S001E")));
  m_cmdTable.insert("get currentPowerConsumption", new pmu(QStringList("G001F")));
  m_cmdTable.insert("get wirelessDataAggregator", new pmu(QStringList("G0020")));
  m_cmdTable.insert("set wirelessDataAggregator", new pmu(QStringList("S0020")));
  m_cmdTable.insert("get resetUsageTimestamp", new pmu(QStringList("G0021")));
  m_cmdTable.insert("get pwmPeriodRegister", new pmu(QStringList("G0022")));
  m_cmdTable.insert("set pwmPeriodRegister", new pmu(QStringList("S0022")));
  m_cmdTable.insert("get analogSensorValue", new pmu(QStringList("G0023")));
  m_cmdTable.insert("get analogReportingHysteresis", new pmu(QStringList("G0024")));
  m_cmdTable.insert("get zone", new pmu(QStringList("G0025")));
  m_cmdTable.insert("set zone", new pmu(QStringList("S0025")));
  m_cmdTable.insert("get lightTemporaryActiveLevel", new pmu(QStringList("G0026")));
  m_cmdTable.insert("set lightTemporaryActiveLevel", new pmu(QStringList("S0026")));
  m_cmdTable.insert("get lightTemporaryInactiveLevel", new pmu(QStringList("G0027")));
  m_cmdTable.insert("set lightTemporaryInactiveLevel", new pmu(QStringList("S0027")));
  m_cmdTable.insert("get sensorTemporaryDelayTime", new pmu(QStringList("G0028")));
  m_cmdTable.insert("set sensorTemporaryDelayTime", new pmu(QStringList("S0028")));
  m_cmdTable.insert("get temporaryOverrideTimeout", new pmu(QStringList("G0029")));
  m_cmdTable.insert("set temporaryOverrideTimeout", new pmu(QStringList("S0029")));
  m_cmdTable.insert("get setRemoteState", new pmu(QStringList("G002A")));
  m_cmdTable.insert("set setRemoteState", new pmu(QStringList("S002A")));
  m_cmdTable.insert("get remoteSetDelayTime", new pmu(QStringList("G002B")));
  m_cmdTable.insert("set remoteSetDelayTime", new pmu(QStringList("S002B")));
  m_cmdTable.insert("get remoteSecondsCountdown", new pmu(QStringList("G002C")));
  m_cmdTable.insert("get minimumDimmingValue", new pmu(QStringList("G002D")));
  m_cmdTable.insert("get powerCalibrationA0", new pmu(QStringList("G002E")));
  m_cmdTable.insert("set powerCalibrationA0", new pmu(QStringList("S002E")));
  m_cmdTable.insert("get powerCalibrationB0", new pmu(QStringList("G002F")));
  m_cmdTable.insert("set powerCalibrationB0", new pmu(QStringList("S002F")));
  m_cmdTable.insert("get powerCalibrationC0", new pmu(QStringList("G0030")));
  m_cmdTable.insert("set powerCalibrationC0", new pmu(QStringList("S0030")));
  m_cmdTable.insert("get powerCalibrationMA", new pmu(QStringList("G0031")));
  m_cmdTable.insert("set powerCalibrationMA", new pmu(QStringList("S0031")));
  m_cmdTable.insert("get powerCalibrationMB", new pmu(QStringList("G0032")));
  m_cmdTable.insert("set powerCalibrationMB", new pmu(QStringList("S0032")));
  m_cmdTable.insert("get powerCalibrationMC", new pmu(QStringList("G0033")));
  m_cmdTable.insert("set powerCalibrationMC", new pmu(QStringList("S0033")));
  m_cmdTable.insert("get powerCalibrationPOff", new pmu(QStringList("G0034")));
  m_cmdTable.insert("set powerCalibrationPOff", new pmu(QStringList("S0034")));
  m_cmdTable.insert("get powerCalibrationPOn", new pmu(QStringList("G0035")));
  m_cmdTable.insert("set powerCalibrationPOn", new pmu(QStringList("S0035")));
  m_cmdTable.insert("get powerCalibrationT0", new pmu(QStringList("G0036")));
  m_cmdTable.insert("set powerCalibrationT0", new pmu(QStringList("S0036")));
  m_cmdTable.insert("get powerEstimatorTemperatureOverride", new pmu(QStringList("G0037")));
  m_cmdTable.insert("set powerEstimatorTemperatureOverride", new pmu(QStringList("S0037")));
  m_cmdTable.insert("get cachedTemperatureValue", new pmu(QStringList("G0038")));
  m_cmdTable.insert("get eepromSize", new pmu(QStringList("G0039")));
  m_cmdTable.insert("get hardwareRevision", new pmu(QStringList("G003A")));
  m_cmdTable.insert("get wirelessPanId", new pmu(QStringList("G003B")));
  m_cmdTable.insert("set wirelessPanId", new pmu(QStringList("S003B")));
  m_cmdTable.insert("get wirelessChannelMask", new pmu(QStringList("G003C")));
  m_cmdTable.insert("set wirelessChannelMask", new pmu(QStringList("S003C")));
  m_cmdTable.insert("get wirelessShortAddress", new pmu(QStringList("G003D")));
  m_cmdTable.insert("set wirelessShortAddress", new pmu(QStringList("S003D")));
  m_cmdTable.insert("get wirelessRole", new pmu(QStringList("G003E")));
  m_cmdTable.insert("set wirelessRole", new pmu(QStringList("S003E")));
  m_cmdTable.insert("get wirelessWatchdogHold", new pmu(QStringList("G003F")));
  m_cmdTable.insert("set wirelessWatchdogHold", new pmu(QStringList("S003F")));
  m_cmdTable.insert("get wirelessWatchdogPeriod", new pmu(QStringList("G0040")));
  m_cmdTable.insert("set wirelessWatchdogPeriod", new pmu(QStringList("S0040")));
  m_cmdTable.insert("get firmwareCode", new pmu(QStringList("G0041")));
  m_cmdTable.insert("get moduleFirmwareCode", new pmu(QStringList("G0042")));
  m_cmdTable.insert("get maxTemperatureObserved", new pmu(QStringList("G0043")));
  m_cmdTable.insert("get maxTemperatureObservedTime", new pmu(QStringList("G0044")));
  m_cmdTable.insert("get overTemperatureThresholdLow", new pmu(QStringList("G0045")));
  m_cmdTable.insert("set overTemperatureThresholdLow", new pmu(QStringList("S0045")));
  m_cmdTable.insert("get overTemperatureThresholdHigh", new pmu(QStringList("G0046")));
  m_cmdTable.insert("set overTemperatureThresholdHigh", new pmu(QStringList("S0046")));
  m_cmdTable.insert("get overTemperatureDimmingLimit", new pmu(QStringList("G0047")));
  m_cmdTable.insert("set overTemperatureDimmingLimit", new pmu(QStringList("S0047")));
  m_cmdTable.insert("get analogDimmingMode", new pmu(QStringList("G0048")));
  m_cmdTable.insert("set analogDimmingMode", new pmu(QStringList("S0048")));
  m_cmdTable.insert("get fixtureIdMode", new pmu(QStringList("G0049")));
  m_cmdTable.insert("set fixtureIdMode", new pmu(QStringList("S0049")));
  m_cmdTable.insert("get acFrequency", new pmu(QStringList("G004A")));
  m_cmdTable.insert("get sensorBits", new pmu(QStringList("G004B")));
  m_cmdTable.insert("get powerMeterCommand", new pmu(QStringList("G004C")));
  m_cmdTable.insert("set powerMeterCommand", new pmu(QStringList("S004C")));
  m_cmdTable.insert("get powerMeterRegister", new pmu(QStringList("G004D")));
  m_cmdTable.insert("set powerMeterRegister", new pmu(QStringList("S004D")));
  m_cmdTable.insert("get ambientTemperature", new pmu(QStringList("G004E")));
  m_cmdTable.insert("get lightSensorLevel", new pmu(QStringList("G004F")));
  m_cmdTable.insert("get sensor0Timeout", new pmu(QStringList("G0050")));
  m_cmdTable.insert("set sensor0Timeout", new pmu(QStringList("S0050")));
  m_cmdTable.insert("get sensor0Offset", new pmu(QStringList("G0051")));
  m_cmdTable.insert("set sensor0Offset", new pmu(QStringList("S0051")));
  m_cmdTable.insert("get sensor1Timeout", new pmu(QStringList("G0052")));
  m_cmdTable.insert("set sensor1Timeout", new pmu(QStringList("S0052")));
  m_cmdTable.insert("get sensor1Offset", new pmu(QStringList("G0053")));
  m_cmdTable.insert("set sensor1Offset", new pmu(QStringList("S0053")));
  m_cmdTable.insert("get analogDimmingLowValue", new pmu(QStringList("G0054")));
  m_cmdTable.insert("set analogDimmingLowValue", new pmu(QStringList("S0054")));
  m_cmdTable.insert("get analogDimmingHighValue", new pmu(QStringList("G0055")));
  m_cmdTable.insert("set analogDimmingHighValue", new pmu(QStringList("S0055")));
  m_cmdTable.insert("get analogDimmingOffValue", new pmu(QStringList("G0056")));
  m_cmdTable.insert("set analogDimmingOffValue", new pmu(QStringList("S0056")));
  m_cmdTable.insert("get powerMeasurementMode", new pmu(QStringList("G0057")));
  m_cmdTable.insert("set powerMeasurementMode", new pmu(QStringList("S0057")));
  m_cmdTable.insert("get externalPowerMeter", new pmu(QStringList("G0058")));
  m_cmdTable.insert("set externalPowerMeter", new pmu(QStringList("S0058")));
  m_cmdTable.insert("get ambientSensorValue", new pmu(QStringList("G0059")));
  m_cmdTable.insert("get ambientActiveLevel", new pmu(QStringList("G005A")));
  m_cmdTable.insert("set ambientActiveLevel", new pmu(QStringList("S005A")));
  m_cmdTable.insert("get ambientInactiveLevel", new pmu(QStringList("G005B")));
  m_cmdTable.insert("set ambientInactiveLevel", new pmu(QStringList("S005B")));
  m_cmdTable.insert("get ambientEnvironmentalGain", new pmu(QStringList("G005C")));
  m_cmdTable.insert("set ambientEnvironmentalGain", new pmu(QStringList("S005C")));
  m_cmdTable.insert("get ambientOffHysteresis", new pmu(QStringList("G005D")));
  m_cmdTable.insert("set ambientOffHysteresis", new pmu(QStringList("S005D")));
  m_cmdTable.insert("get ambientOnHysteresis", new pmu(QStringList("G005E")));
  m_cmdTable.insert("set ambientOnHysteresis", new pmu(QStringList("S005E")));
  m_cmdTable.insert("get powerboardProtocol", new pmu(QStringList("G005F")));
  m_cmdTable.insert("get ledOverride", new pmu(QStringList("G0060")));
  m_cmdTable.insert("set ledOverride", new pmu(QStringList("S0060")));
  m_cmdTable.insert("get fadeUpStep", new pmu(QStringList("G0061")));
  m_cmdTable.insert("set fadeUpStep", new pmu(QStringList("S0061")));
  m_cmdTable.insert("get fadeDownStep", new pmu(QStringList("G0062")));
  m_cmdTable.insert("set fadeDownStep", new pmu(QStringList("S0062")));
  m_cmdTable.insert("get maxBrightness", new pmu(QStringList("G0063")));
  m_cmdTable.insert("set maxBrightness", new pmu(QStringList("S0063")));
  m_cmdTable.insert("get i2cResets", new pmu(QStringList("G0064")));
  m_cmdTable.insert("get sensorGuardTime", new pmu(QStringList("G0065")));
  m_cmdTable.insert("set sensorGuardTime", new pmu(QStringList("S0065")));
  m_cmdTable.insert("get inputVoltage", new pmu(QStringList("G0066")));
  m_cmdTable.insert("get inputVoltageCalibration", new pmu(QStringList("G0067")));
  m_cmdTable.insert("set inputVoltageCalibration", new pmu(QStringList("S0067")));
  m_cmdTable.insert("get numberOfLightbars", new pmu(QStringList("G0068")));
  m_cmdTable.insert("set numberOfLightbars", new pmu(QStringList("S0068")));
  m_cmdTable.insert("get ambientDivisor", new pmu(QStringList("G0069")));
  m_cmdTable.insert("get currentLimit", new pmu(QStringList("G006A")));
  m_cmdTable.insert("set currentLimit", new pmu(QStringList("S006A")));
  m_cmdTable.insert("get bootloaderCode", new pmu(QStringList("G006B")));
  m_cmdTable.insert("get xpressMode", new pmu(QStringList("G006C")));
  m_cmdTable.insert("set xpressMode", new pmu(QStringList("S006C")));
  m_cmdTable.insert("get batteryBackupStatus", new pmu(QStringList("G006D"), parse_get_batteryBackupStatus));
  m_cmdTable.insert("set batteryBackupStatus", new pmu(QStringList("S006D")));
  m_cmdTable.insert("get sensorSeconds", new pmu(QStringList("G006E")));
  m_cmdTable.insert("get inputVoltageTwo", new pmu(QStringList("G006F")));
  m_cmdTable.insert("get inputVoltageTwoCalibration", new pmu(QStringList("G0070")));
  m_cmdTable.insert("set inputVoltageTwoCalibration", new pmu(QStringList("S0070")));
  m_cmdTable.insert("get maxRampUpSpeed", new pmu(QStringList("G0071")));
  m_cmdTable.insert("set maxRampUpSpeed", new pmu(QStringList("S0071")));
  m_cmdTable.insert("get maxRampDownSpeed", new pmu(QStringList("G0072")));
  m_cmdTable.insert("set maxRampDownSpeed", new pmu(QStringList("S0072")));
  m_cmdTable.insert("get wirelessNetworkKey", new pmu(QStringList("G0073")));
  m_cmdTable.insert("set wirelessNetworkKey", new pmu(QStringList("S0073")));
  m_cmdTable.insert("get emergencyLightLevel", new pmu(QStringList("G0074")));
  m_cmdTable.insert("get batteryBackupPowerCalibration", new pmu(QStringList("G0075")));
  m_cmdTable.insert("set batteryBackupPowerCalibration", new pmu(QStringList("S0075")));
  m_cmdTable.insert("get motionSensorProfile", new pmu(QStringList("G0076")));
  m_cmdTable.insert("set motionSensorProfile", new pmu(QStringList("S0076")));
  m_cmdTable.insert("get powerMeterLevelAtOff", new pmu(QStringList("G0077")));
  m_cmdTable.insert("set powerMeterLevelAtOff", new pmu(QStringList("S0077")));
  m_cmdTable.insert("get powerMeterLevelAtMin", new pmu(QStringList("G0078")));
  m_cmdTable.insert("set powerMeterLevelAtMin", new pmu(QStringList("S0078")));
  m_cmdTable.insert("get powerMeterLevelAtMax", new pmu(QStringList("G0079")));
  m_cmdTable.insert("set powerMeterLevelAtMax", new pmu(QStringList("S0079")));
  m_cmdTable.insert("get powerMeterType", new pmu(QStringList("G007A")));
  m_cmdTable.insert("set powerMeterType", new pmu(QStringList("S007A")));
  m_cmdTable.insert("get DLAiSlaveMode", new pmu(QStringList("G007B")));
  m_cmdTable.insert("set DLAiSlaveMode", new pmu(QStringList("S007B")));
  m_cmdTable.insert("get DALIBootloadingActive", new pmu(QStringList("G007C")));
  m_cmdTable.insert("get testingMode", new pmu(QStringList("G007D")));
  m_cmdTable.insert("set testingMode", new pmu(QStringList("S007D")));
  m_cmdTable.insert("get numberOfBatteriesSupported", new pmu(QStringList("G007E")));
  m_cmdTable.insert("set numberOfBatteriesSupported", new pmu(QStringList("S007E")));
  // get & set lightbar commands
  m_cmdTable.insert("get lbProtocolVersion", new pmu(QStringList("R0000")));
  m_cmdTable.insert("get lbFirmwareVersion", new pmu(QStringList() << "R0003" << "R0004", parse_get_lbFirmwareVersion));
  // reset commands
  m_cmdTable.insert("reset usage", new pmu(QStringList("!U")));
  m_cmdTable.insert("reset log", new pmu(QStringList("!L")));
  m_cmdTable.insert("reset newLog", new pmu(QStringList("!K")));
  m_cmdTable.insert("reset eeprom", new pmu(QStringList("!Z")));
  m_cmdTable.insert("reset eepromToDefault", new pmu(QStringList("!C")));
  m_cmdTable.insert("reset eepromToLatestMapVersion", new pmu(QStringList("!E")));
  m_cmdTable.insert("reset network", new pmu(QStringList("!N")));
  m_cmdTable.insert("reset networkWithoutChecking", new pmu(QStringList("!N1")));
  m_cmdTable.insert("reset daliCommissioning", new pmu(QStringList("!Y")));
  m_cmdTable.insert("reset daliPowerMetering", new pmu(QStringList("!A")));
  // reboot commands
  m_cmdTable.insert("reboot", new pmu(QStringList("!R")));
  m_cmdTable.insert("reboot wirelessCard", new pmu(QStringList("!W")));
  m_cmdTable.insert("reboot i2cDevices", new pmu(QStringList("!X")));
  // reload commands
  m_cmdTable.insert("reload dlaFirmware", new pmu(QStringList("!B")));
  m_cmdTable.insert("reload wirelessModuleFirmware", new pmu(QStringList("!M")));
  m_cmdTable.insert("reload powerboardFirmware", new pmu(QStringList("!P")));
  m_cmdTable.insert("reload lightbarFirmware", new pmu(QStringList("!P")));
  m_cmdTable.insert("reload batteryBackupFirmware", new pmu(QStringList("!P")));
  m_cmdTable.insert("reload motionSensorFirmware", new pmu(QStringList("!V")));
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
