#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

QMap <QString, QString> errorResponses;

QString parse_getFirmwareVersion(QString pmuResponse) {
  bool ok;
  if (pmuResponse.startsWith("G")) {
    qulonglong verInt = pmuResponse.split(" ").at(1).toULongLong(&ok, 16);
    // format verMajor.verMinor.verBuild (buildMonth/buildDay/BuildYear)
    return QString("%1.%2.%3 (%5/%6/%4)").
        arg((verInt >> 40) & 0xFF).
        arg((verInt >> 32) & 0xFF).
        arg((verInt >> 24) & 0xFF).
        arg((verInt >> 16) & 0xFF).
        arg((verInt >> 8) & 0xFF).
        arg(verInt & 0xFF);
  } else if (pmuResponse.startsWith("F")) {
    return errorResponses.value(pmuResponse);
  } else {
    return "Could not parse response";
  }
}

QString parse_getTemperature(QString pmuResponse) {
  if (pmuResponse.startsWith("G")) {
    // todo
    return pmuResponse;
  } else if (pmuResponse.startsWith("F")) {
    return errorResponses.value(pmuResponse);
  } else {
    return "Could not parse response";
  }
}

QString parse_getBatteryBackupStatus(QString pmuResponse) {
  bool ok;
  QString parsedResponse;
  QMap <int, QString> battDetectedDict;
  QMap <int, QString> testReportDict;
  QMap <int, QString> testRunningDict;
  if (pmuResponse.startsWith("G")) {
    quint32 status = pmuResponse.split(" ").at(1).toLong(&ok, 16);
    // parse batteries detected bits
    battDetectedDict.insert(0, "No batteries detected");
    battDetectedDict.insert(1, "Battery 1 detected");
    battDetectedDict.insert(2, "Battery 2 detected");
    battDetectedDict.insert(3, "Batteries 1 & 2 detected");
    parsedResponse = (battDetectedDict[status & 0x3] + "\r");
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
    parsedResponse += (testReportDict[(status >> 2) & 0xF] + "\r");
    parsedResponse += "Battery 2 test report: ";
    parsedResponse += (testReportDict[(status >> 6) & 0xF] + "\r");
    // parse test running bits
    testRunningDict.insert(0, "No tests running");
    testRunningDict.insert(1, "Short test running");
    testRunningDict.insert(2, "Long test running");
    testRunningDict.insert(4, "Push button test running");
    parsedResponse += (testRunningDict[(status >> 10) & 0x3] + "\r");
    // parse test time
    parsedResponse += "Test time: ";
    parsedResponse += QString("%1 seconds").arg(status >> 16);
    return parsedResponse;
  } else if (pmuResponse.startsWith("F")) {
    return errorResponses.value(pmuResponse);
  } else {
    return "Could not parse response";
  }
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_cmdTable.insert("get firmwareVersion", new pmu("G0000", parse_getFirmwareVersion));
  m_cmdTable.insert("get productCode", new pmu("G0001"));
  m_cmdTable.insert("set productCode", new pmu("S0001"));
  m_cmdTable.insert("get serialNumber", new pmu("G0002"));
  m_cmdTable.insert("set serialNumber", new pmu("S0002"));
  m_cmdTable.insert("get unixTime", new pmu("G0003"));
  m_cmdTable.insert("set unixTime", new pmu("S0003"));
  m_cmdTable.insert("get temperature", new pmu("G0004", parse_getTemperature));
  m_cmdTable.insert("get lightManualLevel", new pmu("G0005"));
  m_cmdTable.insert("set lightManualLevel", new pmu("S0005"));
  m_cmdTable.insert("get lightActiveLevel", new pmu("G0006"));
  m_cmdTable.insert("get lightInactiveLevel", new pmu("G0007"));
  m_cmdTable.insert("get lightOverrideActiveLevel", new pmu("G0008"));
  m_cmdTable.insert("set lightOverrideActiveLevel", new pmu("S0008"));
  m_cmdTable.insert("get lightOverrideInactiveLevel", new pmu("G0009"));
  m_cmdTable.insert("set lightOverrideInactiveLevel", new pmu("S0009"));
  m_cmdTable.insert("get sensorDelayTime", new pmu("G000A"));
  m_cmdTable.insert("get sensorOverrideDelayTime", new pmu("G000B"));
  m_cmdTable.insert("set sensorOverrideDelayTime", new pmu("S000B"));
  m_cmdTable.insert("get powerupTime", new pmu("G000C"));
  m_cmdTable.insert("get usageActiveTime", new pmu("G000D"));
  m_cmdTable.insert("get usageInactiveTime", new pmu("G000E"));
  m_cmdTable.insert("get usagePermActiveTime", new pmu("G000F"));
  m_cmdTable.insert("get usagePermInactiveTime", new pmu("G0010"));
  m_cmdTable.insert("get usageWh", new pmu("G0011"));
  m_cmdTable.insert("get usagePerm", new pmu("G0012"));
  m_cmdTable.insert("get usageSenorEvents", new pmu("G0013"));
  m_cmdTable.insert("get usagePermSensorEvents", new pmu("G0014"));
  m_cmdTable.insert("get numberOfLogEntries", new pmu("G0015"));
  m_cmdTable.insert("get configCalibrationP0", new pmu("G0016"));
  m_cmdTable.insert("set configCalibrationP0", new pmu("S0016"));
  m_cmdTable.insert("get configCalibrationP1", new pmu("G0017"));
  m_cmdTable.insert("set configCalibrationP1", new pmu("S0017"));
  m_cmdTable.insert("get configCalibrationP2", new pmu("G0018"));
  m_cmdTable.insert("set configCalibrationP2", new pmu("S0018"));
  m_cmdTable.insert("get configCalibrationP3", new pmu("G0019"));
  m_cmdTable.insert("set configCalibrationP3", new pmu("S0019"));
  m_cmdTable.insert("get buildTime", new pmu("G001A"));
  m_cmdTable.insert("set buildTime", new pmu("S001A"));
  m_cmdTable.insert("get sensorTimeoutCountdown", new pmu("G001B"));
  m_cmdTable.insert("get currentLightLevel", new pmu("G001C"));
  m_cmdTable.insert("get safeMode", new pmu("G001D"));
  m_cmdTable.insert("get lightBarSelect", new pmu("G001E"));
  m_cmdTable.insert("set lightBarSelect", new pmu("S001E"));
  m_cmdTable.insert("get currentPowerConsumption", new pmu("G001F"));
  m_cmdTable.insert("get wirelessDataAggregator", new pmu("G0020"));
  m_cmdTable.insert("set wirelessDataAggregator", new pmu("S0020"));
  m_cmdTable.insert("get resetUsageTimestamp", new pmu("G0021"));
  m_cmdTable.insert("get pwmPeriodRegister", new pmu("G0022"));
  m_cmdTable.insert("set pwmPeriodRegister", new pmu("S0022"));
  m_cmdTable.insert("get analogSensorValue", new pmu("G0023"));
  m_cmdTable.insert("get analogReportingHysteresis", new pmu("G0024"));
  m_cmdTable.insert("get zone", new pmu("G0025"));
  m_cmdTable.insert("set zone", new pmu("S0025"));
  m_cmdTable.insert("get lightTemporaryActiveLevel", new pmu("G0026"));
  m_cmdTable.insert("set lightTemporaryActiveLevel", new pmu("S0026"));
  m_cmdTable.insert("get lightTemporaryInactiveLevel", new pmu("G0027"));
  m_cmdTable.insert("set lightTemporaryInactiveLevel", new pmu("S0027"));
  m_cmdTable.insert("get sensorTemporaryDelayTime", new pmu("G0028"));
  m_cmdTable.insert("set sensorTemporaryDelayTime", new pmu("S0028"));
  m_cmdTable.insert("get temporaryOverrideTimeout", new pmu("G0029"));
  m_cmdTable.insert("set temporaryOverrideTimeout", new pmu("S0029"));
  m_cmdTable.insert("get setRemoteState", new pmu("G002A"));
  m_cmdTable.insert("set setRemoteState", new pmu("S002A"));
  m_cmdTable.insert("get remoteSetDelayTime", new pmu("G002B"));
  m_cmdTable.insert("set remoteSetDelayTime", new pmu("S002B"));
  m_cmdTable.insert("get remoteSecondsCountdown", new pmu("G002C"));
  m_cmdTable.insert("get minimumDimmingValue", new pmu("G002D"));
  m_cmdTable.insert("get powerCalibrationA0", new pmu("G002E"));
  m_cmdTable.insert("set powerCalibrationA0", new pmu("S002E"));
  m_cmdTable.insert("get powerCalibrationB0", new pmu("G002F"));
  m_cmdTable.insert("set powerCalibrationB0", new pmu("S002F"));
  m_cmdTable.insert("get powerCalibrationC0", new pmu("G0030"));
  m_cmdTable.insert("set powerCalibrationC0", new pmu("S0030"));
  m_cmdTable.insert("get powerCalibrationMA", new pmu("G0031"));
  m_cmdTable.insert("set powerCalibrationMA", new pmu("S0031"));
  m_cmdTable.insert("get powerCalibrationMB", new pmu("G0032"));
  m_cmdTable.insert("set powerCalibrationMB", new pmu("S0032"));
  m_cmdTable.insert("get powerCalibrationMC", new pmu("G0033"));
  m_cmdTable.insert("set powerCalibrationMC", new pmu("S0033"));
  m_cmdTable.insert("get powerCalibrationPOff", new pmu("G0034"));
  m_cmdTable.insert("set powerCalibrationPOff", new pmu("S0034"));
  m_cmdTable.insert("get powerCalibrationPOn", new pmu("G0035"));
  m_cmdTable.insert("set powerCalibrationPOn", new pmu("S0035"));
  m_cmdTable.insert("get powerCalibrationT0", new pmu("G0036"));
  m_cmdTable.insert("set powerCalibrationT0", new pmu("S0036"));
  m_cmdTable.insert("get powerEstimatorTemperatureOverride", new pmu("G0037"));
  m_cmdTable.insert("set powerEstimatorTemperatureOverride", new pmu("S0037"));
  m_cmdTable.insert("get cachedTemperatureValue", new pmu("G0038"));
  m_cmdTable.insert("get eepromSize", new pmu("G0039"));
  m_cmdTable.insert("get hardwareRevision", new pmu("G003A"));
  m_cmdTable.insert("get wirelessPanId", new pmu("G003B"));
  m_cmdTable.insert("set wirelessPanId", new pmu("S003B"));
  m_cmdTable.insert("get wirelessChannelMask", new pmu("G003C"));
  m_cmdTable.insert("set wirelessChannelMask", new pmu("S003C"));
  m_cmdTable.insert("get wirelessShortAddress", new pmu("G003D"));
  m_cmdTable.insert("set wirelessShortAddress", new pmu("S003D"));
  m_cmdTable.insert("get wirelessRole", new pmu("G003E"));
  m_cmdTable.insert("set wirelessRole", new pmu("S003E"));
  m_cmdTable.insert("get wirelessWatchdogHold", new pmu("G003F"));
  m_cmdTable.insert("set wirelessWatchdogHold", new pmu("S003F"));
  m_cmdTable.insert("get wirelessWatchdogPeriod", new pmu("G0040"));
  m_cmdTable.insert("set wirelessWatchdogPeriod", new pmu("S0040"));
  m_cmdTable.insert("get firmwareCode", new pmu("G0041"));
  m_cmdTable.insert("get moduleFirmwareCode", new pmu("G0042"));
  m_cmdTable.insert("get maxTemperatureObserved", new pmu("G0043"));
  m_cmdTable.insert("get maxTemperatureObservedTime", new pmu("G0044"));
  m_cmdTable.insert("get overTemperatureThresholdLow", new pmu("G0045"));
  m_cmdTable.insert("set overTemperatureThresholdLow", new pmu("S0045"));
  m_cmdTable.insert("get overTemperatureThresholdHigh", new pmu("G0046"));
  m_cmdTable.insert("set overTemperatureThresholdHigh", new pmu("S0046"));
  m_cmdTable.insert("get overTemperatureDimmingLimit", new pmu("G0047"));
  m_cmdTable.insert("set overTemperatureDimmingLimit", new pmu("S0047"));
  m_cmdTable.insert("get analogDimmingMode", new pmu("G0048"));
  m_cmdTable.insert("set analogDimmingMode", new pmu("S0048"));
  m_cmdTable.insert("get fixtureIdMode", new pmu("G0049"));
  m_cmdTable.insert("set fixtureIdMode", new pmu("S0049"));
  m_cmdTable.insert("get acFrequency", new pmu("G004A"));
  m_cmdTable.insert("get sensorBits", new pmu("G004B"));
  m_cmdTable.insert("get powerMeterCommand", new pmu("G004C"));
  m_cmdTable.insert("set powerMeterCommand", new pmu("S004C"));
  m_cmdTable.insert("get powerMeterRegister", new pmu("G004D"));
  m_cmdTable.insert("set powerMeterRegister", new pmu("S004D"));
  m_cmdTable.insert("get ambientTemperature", new pmu("G004E"));
  m_cmdTable.insert("get lightSensorLevel", new pmu("G004F"));
  m_cmdTable.insert("get sensor0Timeout", new pmu("G0050"));
  m_cmdTable.insert("set sensor0Timeout", new pmu("S0050"));
  m_cmdTable.insert("get sensor0Offset", new pmu("G0051"));
  m_cmdTable.insert("set sensor0Offset", new pmu("S0051"));
  m_cmdTable.insert("get sensor1Timeout", new pmu("G0052"));
  m_cmdTable.insert("set sensor1Timeout", new pmu("S0052"));
  m_cmdTable.insert("get sensor1Offset", new pmu("G0053"));
  m_cmdTable.insert("set sensor1Offset", new pmu("S0053"));
  m_cmdTable.insert("get analogDimmingLowValue", new pmu("G0054"));
  m_cmdTable.insert("set analogDimmingLowValue", new pmu("S0054"));
  m_cmdTable.insert("get analogDimmingHighValue", new pmu("G0055"));
  m_cmdTable.insert("set analogDimmingHighValue", new pmu("S0055"));
  m_cmdTable.insert("get analogDimmingOffValue", new pmu("G0056"));
  m_cmdTable.insert("set analogDimmingOffValue", new pmu("S0056"));
  m_cmdTable.insert("get powerMeasurementMode", new pmu("G0057"));
  m_cmdTable.insert("set powerMeasurementMode", new pmu("S0057"));
  m_cmdTable.insert("get externalPowerMeter", new pmu("G0058"));
  m_cmdTable.insert("set externalPowerMeter", new pmu("S0058"));
  m_cmdTable.insert("get ambientSensorValue", new pmu("G0059"));
  m_cmdTable.insert("get ambientActiveLevel", new pmu("G005A"));
  m_cmdTable.insert("set ambientActiveLevel", new pmu("S005A"));
  m_cmdTable.insert("get ambientInactiveLevel", new pmu("G005B"));
  m_cmdTable.insert("set ambientInactiveLevel", new pmu("S005B"));
  m_cmdTable.insert("get ambientEnvironmentalGain", new pmu("G005C"));
  m_cmdTable.insert("set ambientEnvironmentalGain", new pmu("S005C"));
  m_cmdTable.insert("get ambientOffHysteresis", new pmu("G005D"));
  m_cmdTable.insert("set ambientOffHysteresis", new pmu("S005D"));
  m_cmdTable.insert("get ambientOnHysteresis", new pmu("G005E"));
  m_cmdTable.insert("set ambientOnHysteresis", new pmu("S005E"));
  m_cmdTable.insert("get powerboardProtocol", new pmu("G005F"));
  m_cmdTable.insert("get ledOverride", new pmu("G0060"));
  m_cmdTable.insert("set ledOverride", new pmu("S0060"));
  m_cmdTable.insert("get fadeUpStep", new pmu("G0061"));
  m_cmdTable.insert("set fadeUpStep", new pmu("S0061"));
  m_cmdTable.insert("get fadeDownStep", new pmu("G0062"));
  m_cmdTable.insert("set fadeDownStep", new pmu("S0062"));
  m_cmdTable.insert("get maxBrightness", new pmu("G0063"));
  m_cmdTable.insert("set maxBrightness", new pmu("S0063"));
  m_cmdTable.insert("get i2cResets", new pmu("G0064"));
  m_cmdTable.insert("get sensorGuardTime", new pmu("G0065"));
  m_cmdTable.insert("set sensorGuardTime", new pmu("S0065"));
  m_cmdTable.insert("get inputVoltage", new pmu("G0066"));
  m_cmdTable.insert("get inputVoltageCalibration", new pmu("G0067"));
  m_cmdTable.insert("set inputVoltageCalibration", new pmu("S0067"));
  m_cmdTable.insert("get numberOfLightbars", new pmu("G0068"));
  m_cmdTable.insert("set numberOfLightbars", new pmu("S0068"));
  m_cmdTable.insert("get ambientDivisor", new pmu("G0069"));
  m_cmdTable.insert("get currentLimit", new pmu("G006A"));
  m_cmdTable.insert("set currentLimit", new pmu("S006A"));
  m_cmdTable.insert("get bootloaderCode", new pmu("G006B"));
  m_cmdTable.insert("get xpressMode", new pmu("G006C"));
  m_cmdTable.insert("set xpressMode", new pmu("S006C"));
  m_cmdTable.insert("get batteryBackupStatus", new pmu("G006D", parse_getBatteryBackupStatus));
  m_cmdTable.insert("set batteryBackupStatus", new pmu("S006D"));
  m_cmdTable.insert("get sensorSeconds", new pmu("G006E"));
  m_cmdTable.insert("get inputVoltageTwo", new pmu("G006F"));
  m_cmdTable.insert("get inputVoltageTwoCalibration", new pmu("G0070"));
  m_cmdTable.insert("set inputVoltageTwoCalibration", new pmu("S0070"));
  m_cmdTable.insert("get maxRampUpSpeed", new pmu("G0071"));
  m_cmdTable.insert("set maxRampUpSpeed", new pmu("S0071"));
  m_cmdTable.insert("get maxRampDownSpeed", new pmu("G0072"));
  m_cmdTable.insert("set maxRampDownSpeed", new pmu("S0072"));
  m_cmdTable.insert("get wirelessNetworkKey", new pmu("G0073"));
  m_cmdTable.insert("set wirelessNetworkKey", new pmu("S0073"));
  m_cmdTable.insert("get emergencyLightLevel", new pmu("G0074"));
  m_cmdTable.insert("get batteryBackupPowerCalibration", new pmu("G0075"));
  m_cmdTable.insert("set batteryBackupPowerCalibration", new pmu("S0075"));
  m_cmdTable.insert("get motionSensorProfile", new pmu("G0076"));
  m_cmdTable.insert("set motionSensorProfile", new pmu("S0076"));
  m_cmdTable.insert("get powerMeterLevelAtOff", new pmu("G0077"));
  m_cmdTable.insert("set powerMeterLevelAtOff", new pmu("S0077"));
  m_cmdTable.insert("get powerMeterLevelAtMin", new pmu("G0078"));
  m_cmdTable.insert("set powerMeterLevelAtMin", new pmu("S0078"));
  m_cmdTable.insert("get powerMeterLevelAtMax", new pmu("G0079"));
  m_cmdTable.insert("set powerMeterLevelAtMax", new pmu("S0079"));
  m_cmdTable.insert("get powerMeterType", new pmu("G007A"));
  m_cmdTable.insert("set powerMeterType", new pmu("S007A"));
  m_cmdTable.insert("get DLAiSlaveMode", new pmu("G007B"));
  m_cmdTable.insert("set DLAiSlaveMode", new pmu("S007B"));
  m_cmdTable.insert("get DALIBootloadingActive", new pmu("G007C"));
  m_cmdTable.insert("get testingMode", new pmu("G007D"));
  m_cmdTable.insert("set testingMode", new pmu("S007D"));
  m_cmdTable.insert("get numberOfBatteriesSupported", new pmu("G007E"));
  m_cmdTable.insert("set numberOfBatteriesSupported", new pmu("S007E"));
  // reset commands
  m_cmdTable.insert("reset usage", new pmu("!U"));
  m_cmdTable.insert("reset log", new pmu("!L"));
  m_cmdTable.insert("reset newLog", new pmu("!K"));
  m_cmdTable.insert("reset eeprom", new pmu("!Z"));
  m_cmdTable.insert("reset eepromToDefault", new pmu("!C"));
  m_cmdTable.insert("reset eepromToLatestMapVersion", new pmu("!E"));
  m_cmdTable.insert("reset network", new pmu("!N"));
  m_cmdTable.insert("reset networkWithoutChecking", new pmu("!N1"));
  m_cmdTable.insert("reset daliCommissioning", new pmu("!Y"));
  m_cmdTable.insert("reset daliPowerMetering", new pmu("!A"));
  // reboot commands
  m_cmdTable.insert("reboot", new pmu("!R"));
  m_cmdTable.insert("reboot wirelessCard", new pmu("!W"));
  m_cmdTable.insert("reboot i2cDevices", new pmu("!X"));
  // reload commands
  m_cmdTable.insert("reload dlaFirmware", new pmu("!B"));
  m_cmdTable.insert("reload wirelessModuleFirmware", new pmu("!M"));
  m_cmdTable.insert("reload powerboardFirmware", new pmu("!P"));
  m_cmdTable.insert("reload lightbarFirmware", new pmu("!P"));
  m_cmdTable.insert("reload batteryBackupFirmware", new pmu("!P"));
  m_cmdTable.insert("reload motionSensorFirmware", new pmu("!V"));
  // build the dictionary of helper commands
  m_cmdCompleter = new QCompleter(m_cmdTable.keys(), this);
  m_cmdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  m_cmdCompleter->setCompletionMode(QCompleter::InlineCompletion);
  // build a dictionary of error responses
  errorResponses.insert("FFFF", "Invalid opcode");
  errorResponses.insert("FFFE", "Syntax error");
  errorResponses.insert("FFFD", "Invalid register");
  errorResponses.insert("FFFC", "Register is read only");
  errorResponses.insert("FFFB", "Invalid register length");
  errorResponses.insert("FFFA", "ARP not addressed");
  errorResponses.insert("FFF9", "Flash error");
  errorResponses.insert("FFF8", "Storage out of bounds");
  errorResponses.insert("FFF7", "Storage unaligned");
  errorResponses.insert("FFF6", "Message queue full");
  errorResponses.insert("FFF5", "I2C error");
  errorResponses.insert("FFF4", "Internal error");
  errorResponses.insert("FFF3", "Insufficient free buffers");
  errorResponses.insert("FFF2", "Bad image");
  errorResponses.insert("FFF1", "Remote install fail");
  errorResponses.insert("FFF0", "Bus error");
  errorResponses.insert("FFEF", "Bus busy");
  errorResponses.insert("FFEE", "Resource busy");
}

QString cmdHelper::getNextCompletion(void) {
  m_cmdCompleter->setCurrentRow(m_cmdCompleter->currentRow() + 1);
  return m_cmdCompleter->currentCompletion();
}

int cmdHelper::getCurrentCompletionLength(void) {
  return m_cmdCompleter->currentCompletion().length();
}
