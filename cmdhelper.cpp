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

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  cmdMap.insert("get firmwareVersion", new pmu("G0000", parse_getFirmwareVersion));
  cmdMap.insert("get productCode", new pmu("G0001"));
  cmdMap.insert("set productCode", new pmu("S0001"));
  cmdMap.insert("get serialNumber", new pmu("G0002"));
  cmdMap.insert("set serialNumber", new pmu("S0002"));
  cmdMap.insert("get unixTime", new pmu("G0003"));
  cmdMap.insert("set unixTime", new pmu("S0003"));
  cmdMap.insert("get temperature", new pmu("G0004", parse_getTemperature));
  cmdMap.insert("get lightManualLevel", new pmu("G0005"));
  cmdMap.insert("set lightManualLevel", new pmu("S0005"));
  cmdMap.insert("get lightActiveLevel", new pmu("G0006"));
  cmdMap.insert("get lightInactiveLevel", new pmu("G0007"));
  cmdMap.insert("get lightOverrideActiveLevel", new pmu("G0008"));
  cmdMap.insert("set lightOverrideActiveLevel", new pmu("S0008"));
  cmdMap.insert("get lightOverrideInactiveLevel", new pmu("G0009"));
  cmdMap.insert("set lightOverrideInactiveLevel", new pmu("S0009"));
  cmdMap.insert("get sensorDelayTime", new pmu("G000A"));
  cmdMap.insert("get sensorOverrideDelayTime", new pmu("G000B"));
  cmdMap.insert("set sensorOverrideDelayTime", new pmu("S000B"));
  cmdMap.insert("get powerupTime", new pmu("G000C"));
  cmdMap.insert("get usageActiveTime", new pmu("G000D"));
  cmdMap.insert("get usageInactiveTime", new pmu("G000E"));
  cmdMap.insert("get usagePermActiveTime", new pmu("G000F"));
  cmdMap.insert("get usagePermInactiveTime", new pmu("G0010"));
  cmdMap.insert("get usageWh", new pmu("G0011"));
  cmdMap.insert("get usagePerm", new pmu("G0012"));
  cmdMap.insert("get usageSenorEvents", new pmu("G0013"));
  cmdMap.insert("get usagePermSensorEvents", new pmu("G0014"));
  cmdMap.insert("get numberOfLogEntries", new pmu("G0015"));
  cmdMap.insert("get configCalibrationP0", new pmu("G0016"));
  cmdMap.insert("set configCalibrationP0", new pmu("S0016"));
  cmdMap.insert("get configCalibrationP1", new pmu("G0017"));
  cmdMap.insert("set configCalibrationP1", new pmu("S0017"));
  cmdMap.insert("get configCalibrationP2", new pmu("G0018"));
  cmdMap.insert("set configCalibrationP2", new pmu("S0018"));
  cmdMap.insert("get configCalibrationP3", new pmu("G0019"));
  cmdMap.insert("set configCalibrationP3", new pmu("S0019"));
  cmdMap.insert("get buildTime", new pmu("G001A"));
  cmdMap.insert("set buildTime", new pmu("S001A"));
  cmdMap.insert("get sensorTimeoutCountdown", new pmu("G001B"));
  cmdMap.insert("get currentLightLevel", new pmu("G001C"));
  cmdMap.insert("get safeMode", new pmu("G001D"));
  cmdMap.insert("get lightBarSelect", new pmu("G001E"));
  cmdMap.insert("set lightBarSelect", new pmu("S001E"));
  cmdMap.insert("get currentPowerConsumption", new pmu("G001F"));
  cmdMap.insert("get wirelessDataAggregator", new pmu("G0020"));
  cmdMap.insert("set wirelessDataAggregator", new pmu("S0020"));
  cmdMap.insert("get resetUsageTimestamp", new pmu("G0021"));
  cmdMap.insert("get pwmPeriodRegister", new pmu("G0022"));
  cmdMap.insert("set pwmPeriodRegister", new pmu("S0022"));
  cmdMap.insert("get analogSensorValue", new pmu("G0023"));
  cmdMap.insert("get analogReportingHysteresis", new pmu("G0024"));
  cmdMap.insert("get zone", new pmu("G0025"));
  cmdMap.insert("set zone", new pmu("S0025"));
  cmdMap.insert("get lightTemporaryActiveLevel", new pmu("G0026"));
  cmdMap.insert("set lightTemporaryActiveLevel", new pmu("S0026"));
  cmdMap.insert("get lightTemporaryInactiveLevel", new pmu("G0027"));
  cmdMap.insert("set lightTemporaryInactiveLevel", new pmu("S0027"));
  cmdMap.insert("get sensorTemporaryDelayTime", new pmu("G0028"));
  cmdMap.insert("set sensorTemporaryDelayTime", new pmu("S0028"));
  cmdMap.insert("get temporaryOverrideTimeout", new pmu("G0029"));
  cmdMap.insert("set temporaryOverrideTimeout", new pmu("S0029"));
  cmdMap.insert("get setRemoteState", new pmu("G002A"));
  cmdMap.insert("set setRemoteState", new pmu("S002A"));
  cmdMap.insert("get remoteSetDelayTime", new pmu("G002B"));
  cmdMap.insert("set remoteSetDelayTime", new pmu("S002B"));
  cmdMap.insert("get remoteSecondsCountdown", new pmu("G002C"));
  cmdMap.insert("get minimumDimmingValue", new pmu("G002D"));
  cmdMap.insert("get powerCalibrationA0", new pmu("G002E"));
  cmdMap.insert("set powerCalibrationA0", new pmu("S002E"));
  cmdMap.insert("get powerCalibrationB0", new pmu("G002F"));
  cmdMap.insert("set powerCalibrationB0", new pmu("S002F"));
  cmdMap.insert("get powerCalibrationC0", new pmu("G0030"));
  cmdMap.insert("set powerCalibrationC0", new pmu("S0030"));
  cmdMap.insert("get powerCalibrationMA", new pmu("G0031"));
  cmdMap.insert("set powerCalibrationMA", new pmu("S0031"));
  cmdMap.insert("get powerCalibrationMB", new pmu("G0032"));
  cmdMap.insert("set powerCalibrationMB", new pmu("S0032"));
  cmdMap.insert("get powerCalibrationMC", new pmu("G0033"));
  cmdMap.insert("set powerCalibrationMC", new pmu("S0033"));
  cmdMap.insert("get powerCalibrationPOff", new pmu("G0034"));
  cmdMap.insert("set powerCalibrationPOff", new pmu("S0034"));
  cmdMap.insert("get powerCalibrationPOn", new pmu("G0035"));
  cmdMap.insert("set powerCalibrationPOn", new pmu("S0035"));
  cmdMap.insert("get powerCalibrationT0", new pmu("G0036"));
  cmdMap.insert("set powerCalibrationT0", new pmu("S0036"));
  cmdMap.insert("get powerEstimatorTemperatureOverride", new pmu("G0037"));
  cmdMap.insert("set powerEstimatorTemperatureOverride", new pmu("S0037"));
  cmdMap.insert("get cachedTemperatureValue", new pmu("G0038"));
  cmdMap.insert("get eepromSize", new pmu("G0039"));
  cmdMap.insert("get hardwareRevision", new pmu("G003A"));
  cmdMap.insert("get wirelessPanId", new pmu("G003B"));
  cmdMap.insert("set wirelessPanId", new pmu("S003B"));
  cmdMap.insert("get wirelessChannelMask", new pmu("G003C"));
  cmdMap.insert("set wirelessChannelMask", new pmu("S003C"));
  cmdMap.insert("get wirelessShortAddress", new pmu("G003D"));
  cmdMap.insert("set wirelessShortAddress", new pmu("S003D"));
  cmdMap.insert("get wirelessRole", new pmu("G003E"));
  cmdMap.insert("set wirelessRole", new pmu("S003E"));
  cmdMap.insert("get wirelessWatchdogHold", new pmu("G003F"));
  cmdMap.insert("set wirelessWatchdogHold", new pmu("S003F"));
  cmdMap.insert("get wirelessWatchdogPeriod", new pmu("G0040"));
  cmdMap.insert("set wirelessWatchdogPeriod", new pmu("S0040"));
  cmdMap.insert("get firmwareCode", new pmu("G0041"));
  cmdMap.insert("get moduleFirmwareCode", new pmu("G0042"));
  cmdMap.insert("get maxTemperatureObserved", new pmu("G0043"));
  cmdMap.insert("get maxTemperatureObservedTime", new pmu("G0044"));
  cmdMap.insert("get overTemperatureThresholdLow", new pmu("G0045"));
  cmdMap.insert("set overTemperatureThresholdLow", new pmu("S0045"));
  cmdMap.insert("get overTemperatureThresholdHigh", new pmu("G0046"));
  cmdMap.insert("set overTemperatureThresholdHigh", new pmu("S0046"));
  cmdMap.insert("get overTemperatureDimmingLimit", new pmu("G0047"));
  cmdMap.insert("set overTemperatureDimmingLimit", new pmu("S0047"));
  cmdMap.insert("get analogDimmingMode", new pmu("G0048"));
  cmdMap.insert("set analogDimmingMode", new pmu("S0048"));
  cmdMap.insert("get fixtureIdMode", new pmu("G0049"));
  cmdMap.insert("set fixtureIdMode", new pmu("S0049"));
  cmdMap.insert("get acFrequency", new pmu("G004A"));
  cmdMap.insert("get sensorBits", new pmu("G004B"));
  cmdMap.insert("get powerMeterCommand", new pmu("G004C"));
  cmdMap.insert("set powerMeterCommand", new pmu("S004C"));
  cmdMap.insert("get powerMeterRegister", new pmu("G004D"));
  cmdMap.insert("set powerMeterRegister", new pmu("S004D"));
  cmdMap.insert("get ambientTemperature", new pmu("G004E"));
  cmdMap.insert("get lightSensorLevel", new pmu("G004F"));
  cmdMap.insert("get sensor0Timeout", new pmu("G0050"));
  cmdMap.insert("set sensor0Timeout", new pmu("S0050"));
  cmdMap.insert("get sensor0Offset", new pmu("G0051"));
  cmdMap.insert("set sensor0Offset", new pmu("S0051"));
  cmdMap.insert("get sensor1Timeout", new pmu("G0052"));
  cmdMap.insert("set sensor1Timeout", new pmu("S0052"));
  cmdMap.insert("get sensor1Offset", new pmu("G0053"));
  cmdMap.insert("set sensor1Offset", new pmu("S0053"));
  cmdMap.insert("get analogDimmingLowValue", new pmu("G0054"));
  cmdMap.insert("set analogDimmingLowValue", new pmu("S0054"));
  cmdMap.insert("get analogDimmingHighValue", new pmu("G0055"));
  cmdMap.insert("set analogDimmingHighValue", new pmu("S0055"));
  cmdMap.insert("get analogDimmingOffValue", new pmu("G0056"));
  cmdMap.insert("set analogDimmingOffValue", new pmu("S0056"));
  cmdMap.insert("get powerMeasurementMode", new pmu("G0057"));
  cmdMap.insert("set powerMeasurementMode", new pmu("S0057"));
  cmdMap.insert("get externalPowerMeter", new pmu("G0058"));
  cmdMap.insert("set externalPowerMeter", new pmu("S0058"));
  cmdMap.insert("get ambientSensorValue", new pmu("G0059"));
  cmdMap.insert("get ambientActiveLevel", new pmu("G005A"));
  cmdMap.insert("set ambientActiveLevel", new pmu("S005A"));
  cmdMap.insert("get ambientInactiveLevel", new pmu("G005B"));
  cmdMap.insert("set ambientInactiveLevel", new pmu("S005B"));
  cmdMap.insert("get ambientEnvironmentalGain", new pmu("G005C"));
  cmdMap.insert("set ambientEnvironmentalGain", new pmu("S005C"));
  cmdMap.insert("get ambientOffHysteresis", new pmu("G005D"));
  cmdMap.insert("set ambientOffHysteresis", new pmu("S005D"));
  cmdMap.insert("get ambientOnHysteresis", new pmu("G005E"));
  cmdMap.insert("set ambientOnHysteresis", new pmu("S005E"));
  cmdMap.insert("get powerboardProtocol", new pmu("G005F"));
  cmdMap.insert("get ledOverride", new pmu("G0060"));
  cmdMap.insert("set ledOverride", new pmu("S0060"));
  cmdMap.insert("get fadeUpStep", new pmu("G0061"));
  cmdMap.insert("set fadeUpStep", new pmu("S0061"));
  cmdMap.insert("get fadeDownStep", new pmu("G0062"));
  cmdMap.insert("set fadeDownStep", new pmu("S0062"));
  cmdMap.insert("get maxBrightness", new pmu("G0063"));
  cmdMap.insert("set maxBrightness", new pmu("S0063"));
  cmdMap.insert("get i2cResets", new pmu("G0064"));
  cmdMap.insert("get sensorGuardTime", new pmu("G0065"));
  cmdMap.insert("set sensorGuardTime", new pmu("S0065"));
  cmdMap.insert("get inputVoltage", new pmu("G0066"));
  cmdMap.insert("get inputVoltageCalibration", new pmu("G0067"));
  cmdMap.insert("set inputVoltageCalibration", new pmu("S0067"));
  cmdMap.insert("get numberOfLightbars", new pmu("G0068"));
  cmdMap.insert("set numberOfLightbars", new pmu("S0068"));
  cmdMap.insert("get ambientDivisor", new pmu("G0069"));
  cmdMap.insert("get currentLimit", new pmu("G006A"));
  cmdMap.insert("set currentLimit", new pmu("S006A"));
  cmdMap.insert("get bootloaderCode", new pmu("G006B"));
  cmdMap.insert("get xpressMode", new pmu("G006C"));
  cmdMap.insert("set xpressMode", new pmu("S006C"));
  cmdMap.insert("get batteryBackupStatus", new pmu("G006D"));
  cmdMap.insert("set batteryBackupStatus", new pmu("S006D"));
  cmdMap.insert("get sensorSeconds", new pmu("G006E"));
  cmdMap.insert("get inputVoltageTwo", new pmu("G006F"));
  cmdMap.insert("get inputVoltageTwoCalibration", new pmu("G0070"));
  cmdMap.insert("set inputVoltageTwoCalibration", new pmu("S0070"));
  cmdMap.insert("get maxRampUpSpeed", new pmu("G0071"));
  cmdMap.insert("set maxRampUpSpeed", new pmu("S0071"));
  cmdMap.insert("get maxRampDownSpeed", new pmu("G0072"));
  cmdMap.insert("set maxRampDownSpeed", new pmu("S0072"));
  cmdMap.insert("get wirelessNetworkKey", new pmu("G0073"));
  cmdMap.insert("set wirelessNetworkKey", new pmu("S0073"));
  cmdMap.insert("get emergencyLightLevel", new pmu("G0074"));
  cmdMap.insert("get batteryBackupPowerCalibration", new pmu("G0075"));
  cmdMap.insert("set batteryBackupPowerCalibration", new pmu("S0075"));
  cmdMap.insert("get motionSensorProfile", new pmu("G0076"));
  cmdMap.insert("set motionSensorProfile", new pmu("S0076"));
  cmdMap.insert("get powerMeterLevelAtOff", new pmu("G0077"));
  cmdMap.insert("set powerMeterLevelAtOff", new pmu("S0077"));
  cmdMap.insert("get powerMeterLevelAtMin", new pmu("G0078"));
  cmdMap.insert("set powerMeterLevelAtMin", new pmu("S0078"));
  cmdMap.insert("get powerMeterLevelAtMax", new pmu("G0079"));
  cmdMap.insert("set powerMeterLevelAtMax", new pmu("S0079"));
  cmdMap.insert("get powerMeterType", new pmu("G007A"));
  cmdMap.insert("set powerMeterType", new pmu("S007A"));
  cmdMap.insert("get DLAiSlaveMode", new pmu("G007B"));
  cmdMap.insert("set DLAiSlaveMode", new pmu("S007B"));
  cmdMap.insert("get DALIBootloadingActive", new pmu("G007C"));
  cmdMap.insert("get testingMode", new pmu("G007D"));
  cmdMap.insert("set testingMode", new pmu("S007D"));
  cmdMap.insert("get numberOfBatteriesSupported", new pmu("G007E"));
  cmdMap.insert("set numberOfBatteriesSupported", new pmu("S007E"));
  // reset commands
  cmdMap.insert("reset usage", new pmu("!U"));
  cmdMap.insert("reset log", new pmu("!L"));
  cmdMap.insert("reset newLog", new pmu("!K"));
  cmdMap.insert("reset eeprom", new pmu("!Z"));
  cmdMap.insert("reset eepromToDefault", new pmu("!C"));
  cmdMap.insert("reset eepromToLatestMapVersion", new pmu("!E"));
  cmdMap.insert("reset network", new pmu("!N"));
  cmdMap.insert("reset networkWithoutChecking", new pmu("!N1"));
  cmdMap.insert("reset daliCommissioning", new pmu("!Y"));
  cmdMap.insert("reset daliPowerMetering", new pmu("!A"));
  // reboot commands
  cmdMap.insert("reboot", new pmu("!R"));
  cmdMap.insert("reboot wirelessCard", new pmu("!W"));
  cmdMap.insert("reboot i2cDevices", new pmu("!X"));
  // reload commands
  cmdMap.insert("reload dlaFirmware", new pmu("!B"));
  cmdMap.insert("reload wirelessModuleFirmware", new pmu("!M"));
  cmdMap.insert("reload powerboardFirmware", new pmu("!P"));
  cmdMap.insert("reload lightbarFirmware", new pmu("!P"));
  cmdMap.insert("reload batteryBackupFirmware", new pmu("!P"));
  cmdMap.insert("reload motionSensorFirmware", new pmu("!V"));
  // build the dictionary of helper commands
  m_cmdCompleter = new QCompleter(cmdMap.keys(), this);
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
