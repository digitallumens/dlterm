#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

QString parse_getFirmwareVersion(QString pmuResponse) {
  return pmuResponse;
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set commands
  m_cmds << new cmd(this, "get firmwareVersion", "G0000", parse_getFirmwareVersion);
  m_cmds << new cmd(this, "get productCode", "G0001");
  m_cmds << new cmd(this, "set productCode", "S0001");
  m_cmds << new cmd(this, "get serialNumber", "G0002");
  m_cmds << new cmd(this, "set serialNumber", "S0002");
  m_cmds << new cmd(this, "get unixTime", "G0003");
  m_cmds << new cmd(this, "set unixTime", "S0003");
  m_cmds << new cmd(this, "get temperature", "G0004");
  m_cmds << new cmd(this, "get lightManualLevel", "G0005");
  m_cmds << new cmd(this, "set lightManualLevel", "S0005");
  m_cmds << new cmd(this, "get lightActiveLevel", "G0006");
  m_cmds << new cmd(this, "get lightInactiveLevel", "G0007");
  m_cmds << new cmd(this, "get lightOverrideActiveLevel", "G0008");
  m_cmds << new cmd(this, "set lightOverrideActiveLevel", "S0008");
  m_cmds << new cmd(this, "get lightOverrideInactiveLevel", "G0009");
  m_cmds << new cmd(this, "set lightOverrideInactiveLevel", "S0009");
  m_cmds << new cmd(this, "get sensorDelayTime", "G000A");
  m_cmds << new cmd(this, "get sensorOverrideDelayTime", "G000B");
  m_cmds << new cmd(this, "set sensorOverrideDelayTime", "S000B");
  m_cmds << new cmd(this, "get powerupTime", "G000C");
  m_cmds << new cmd(this, "get usageActiveTime", "G000D");
  m_cmds << new cmd(this, "get usageInactiveTime", "G000E");
  m_cmds << new cmd(this, "get usagePermActiveTime", "G000F");
  m_cmds << new cmd(this, "get usagePermInactiveTime", "G0010");
  m_cmds << new cmd(this, "get usageWh", "G0011");
  m_cmds << new cmd(this, "get usagePerm", "G0012");
  m_cmds << new cmd(this, "get usageSenorEvents", "G0013");
  m_cmds << new cmd(this, "get usagePermSensorEvents", "G0014");
  m_cmds << new cmd(this, "get numberOfLogEntries", "G0015");
  m_cmds << new cmd(this, "get configCalibrationP0", "G0016");
  m_cmds << new cmd(this, "set configCalibrationP0", "S0016");
  m_cmds << new cmd(this, "get configCalibrationP1", "G0017");
  m_cmds << new cmd(this, "set configCalibrationP1", "S0017");
  m_cmds << new cmd(this, "get configCalibrationP2", "G0018");
  m_cmds << new cmd(this, "set configCalibrationP2", "S0018");
  m_cmds << new cmd(this, "get configCalibrationP3", "G0019");
  m_cmds << new cmd(this, "set configCalibrationP3", "S0019");
  m_cmds << new cmd(this, "get buildTime", "G001A");
  m_cmds << new cmd(this, "set buildTime", "S001A");
  m_cmds << new cmd(this, "get sensorTimeoutCountdown", "G001B");
  m_cmds << new cmd(this, "get currentLightLevel", "G001C");
  m_cmds << new cmd(this, "get safeMode", "G001D");
  m_cmds << new cmd(this, "get lightBarSelect", "G001E");
  m_cmds << new cmd(this, "set lightBarSelect", "S001E");
  m_cmds << new cmd(this, "get currentPowerConsumption", "G001F");
  m_cmds << new cmd(this, "get wirelessDataAggregator", "G0020");
  m_cmds << new cmd(this, "set wirelessDataAggregator", "S0020");
  m_cmds << new cmd(this, "get resetUsageTimestamp", "G0021");
  m_cmds << new cmd(this, "get pwmPeriodRegister", "G0022");
  m_cmds << new cmd(this, "set pwmPeriodRegister", "S0022");
  m_cmds << new cmd(this, "get analogSensorValue", "G0023");
  m_cmds << new cmd(this, "get analogReportingHysteresis", "G0024");
  m_cmds << new cmd(this, "get zone", "G0025");
  m_cmds << new cmd(this, "set zone", "S0025");
  m_cmds << new cmd(this, "get lightTemporaryActiveLevel", "G0026");
  m_cmds << new cmd(this, "set lightTemporaryActiveLevel", "S0026");
  m_cmds << new cmd(this, "get lightTemporaryInactiveLevel", "G0027");
  m_cmds << new cmd(this, "set lightTemporaryInactiveLevel", "S0027");
  m_cmds << new cmd(this, "get sensorTemporaryDelayTime", "G0028");
  m_cmds << new cmd(this, "set sensorTemporaryDelayTime", "S0028");
  m_cmds << new cmd(this, "get temporaryOverrideTimeout", "G0029");
  m_cmds << new cmd(this, "set temporaryOverrideTimeout", "S0029");
  m_cmds << new cmd(this, "get setRemoteState", "G002A");
  m_cmds << new cmd(this, "set setRemoteState", "S002A");
  m_cmds << new cmd(this, "get remoteSetDelayTime", "G002B");
  m_cmds << new cmd(this, "set remoteSetDelayTime", "S002B");
  m_cmds << new cmd(this, "get remoteSecondsCountdown", "G002C");
  m_cmds << new cmd(this, "get minimumDimmingValue", "G002D");
  m_cmds << new cmd(this, "get powerCalibrationA0", "G002E");
  m_cmds << new cmd(this, "set powerCalibrationA0", "S002E");
  m_cmds << new cmd(this, "get powerCalibrationB0", "G002F");
  m_cmds << new cmd(this, "set powerCalibrationB0", "S002F");
  m_cmds << new cmd(this, "get powerCalibrationC0", "G0030");
  m_cmds << new cmd(this, "set powerCalibrationC0", "S0030");
  m_cmds << new cmd(this, "get powerCalibrationMA", "G0031");
  m_cmds << new cmd(this, "set powerCalibrationMA", "S0031");
  m_cmds << new cmd(this, "get powerCalibrationMB", "G0032");
  m_cmds << new cmd(this, "set powerCalibrationMB", "S0032");
  m_cmds << new cmd(this, "get powerCalibrationMC", "G0033");
  m_cmds << new cmd(this, "set powerCalibrationMC", "S0033");
  m_cmds << new cmd(this, "get powerCalibrationPOff", "G0034");
  m_cmds << new cmd(this, "set powerCalibrationPOff", "S0034");
  m_cmds << new cmd(this, "get powerCalibrationPOn", "G0035");
  m_cmds << new cmd(this, "set powerCalibrationPOn", "S0035");
  m_cmds << new cmd(this, "get powerCalibrationT0", "G0036");
  m_cmds << new cmd(this, "set powerCalibrationT0", "S0036");
  m_cmds << new cmd(this, "get powerEstimatorTemperatureOverride", "G0037");
  m_cmds << new cmd(this, "set powerEstimatorTemperatureOverride", "S0037");
  m_cmds << new cmd(this, "get cachedTemperatureValue", "G0038");
  m_cmds << new cmd(this, "get eepromSize", "G0039");
  m_cmds << new cmd(this, "get hardwareRevision", "G003A");
  m_cmds << new cmd(this, "get wirelessPanId", "G003B");
  m_cmds << new cmd(this, "set wirelessPanId", "S003B");
  m_cmds << new cmd(this, "get wirelessChannelMask", "G003C");
  m_cmds << new cmd(this, "set wirelessChannelMask", "S003C");
  m_cmds << new cmd(this, "get wirelessShortAddress", "G003D");
  m_cmds << new cmd(this, "set wirelessShortAddress", "S003D");
  m_cmds << new cmd(this, "get wirelessRole", "G003E");
  m_cmds << new cmd(this, "set wirelessRole", "S003E");
  m_cmds << new cmd(this, "get wirelessWatchdogHold", "G003F");
  m_cmds << new cmd(this, "set wirelessWatchdogHold", "S003F");
  m_cmds << new cmd(this, "get wirelessWatchdogPeriod", "G0040");
  m_cmds << new cmd(this, "set wirelessWatchdogPeriod", "S0040");
  m_cmds << new cmd(this, "get firmwareCode", "G0041");
  m_cmds << new cmd(this, "get moduleFirmwareCode", "G0042");
  m_cmds << new cmd(this, "get maxTemperatureObserved", "G0043");
  m_cmds << new cmd(this, "get maxTemperatureObservedTime", "G0044");
  m_cmds << new cmd(this, "get overTemperatureThresholdLow", "G0045");
  m_cmds << new cmd(this, "set overTemperatureThresholdLow", "S0045");
  m_cmds << new cmd(this, "get overTemperatureThresholdHigh", "G0046");
  m_cmds << new cmd(this, "set overTemperatureThresholdHigh", "S0046");
  m_cmds << new cmd(this, "get overTemperatureDimmingLimit", "G0047");
  m_cmds << new cmd(this, "set overTemperatureDimmingLimit", "S0047");
  m_cmds << new cmd(this, "get analogDimmingMode", "G0048");
  m_cmds << new cmd(this, "set analogDimmingMode", "S0048");
  m_cmds << new cmd(this, "get fixtureIdMode", "G0049");
  m_cmds << new cmd(this, "set fixtureIdMode", "S0049");
  m_cmds << new cmd(this, "get acFrequency", "G004A");
  m_cmds << new cmd(this, "get sensorBits", "G004B");
  m_cmds << new cmd(this, "get powerMeterCommand", "G004C");
  m_cmds << new cmd(this, "set powerMeterCommand", "S004C");
  m_cmds << new cmd(this, "get powerMeterRegister", "G004D");
  m_cmds << new cmd(this, "set powerMeterRegister", "S004D");
  m_cmds << new cmd(this, "get ambientTemperature", "G004E");
  m_cmds << new cmd(this, "get lightSensorLevel", "G004F");
  m_cmds << new cmd(this, "get sensor0Timeout", "G0050");
  m_cmds << new cmd(this, "set sensor0Timeout", "S0050");
  m_cmds << new cmd(this, "get sensor0Offset", "G0051");
  m_cmds << new cmd(this, "set sensor0Offset", "S0051");
  m_cmds << new cmd(this, "get sensor1Timeout", "G0052");
  m_cmds << new cmd(this, "set sensor1Timeout", "S0052");
  m_cmds << new cmd(this, "get sensor1Offset", "G0053");
  m_cmds << new cmd(this, "set sensor1Offset", "S0053");
  m_cmds << new cmd(this, "get analogDimmingLowValue", "G0054");
  m_cmds << new cmd(this, "set analogDimmingLowValue", "S0054");
  m_cmds << new cmd(this, "get analogDimmingHighValue", "G0055");
  m_cmds << new cmd(this, "set analogDimmingHighValue", "S0055");
  m_cmds << new cmd(this, "get analogDimmingOffValue", "G0056");
  m_cmds << new cmd(this, "set analogDimmingOffValue", "S0056");
  m_cmds << new cmd(this, "get powerMeasurementMode", "G0057");
  m_cmds << new cmd(this, "set powerMeasurementMode", "S0057");
  m_cmds << new cmd(this, "get externalPowerMeter", "G0058");
  m_cmds << new cmd(this, "set externalPowerMeter", "S0058");
  m_cmds << new cmd(this, "get ambientSensorValue", "G0059");
  m_cmds << new cmd(this, "get ambientActiveLevel", "G005A");
  m_cmds << new cmd(this, "set ambientActiveLevel", "S005A");
  m_cmds << new cmd(this, "get ambientInactiveLevel", "G005B");
  m_cmds << new cmd(this, "set ambientInactiveLevel", "S005B");
  m_cmds << new cmd(this, "get ambientEnvironmentalGain", "G005C");
  m_cmds << new cmd(this, "set ambientEnvironmentalGain", "S005C");
  m_cmds << new cmd(this, "get ambientOffHysteresis", "G005D");
  m_cmds << new cmd(this, "set ambientOffHysteresis", "S005D");
  m_cmds << new cmd(this, "get ambientOnHysteresis", "G005E");
  m_cmds << new cmd(this, "set ambientOnHysteresis", "S005E");
  m_cmds << new cmd(this, "get powerboardProtocol", "G005F");
  m_cmds << new cmd(this, "get ledOverride", "G0060");
  m_cmds << new cmd(this, "set ledOverride", "S0060");
  m_cmds << new cmd(this, "get fadeUpStep", "G0061");
  m_cmds << new cmd(this, "set fadeUpStep", "S0061");
  m_cmds << new cmd(this, "get fadeDownStep", "G0062");
  m_cmds << new cmd(this, "set fadeDownStep", "S0062");
  m_cmds << new cmd(this, "get maxBrightness", "G0063");
  m_cmds << new cmd(this, "set maxBrightness", "S0063");
  m_cmds << new cmd(this, "get i2cResets", "G0064");
  m_cmds << new cmd(this, "get sensorGuardTime", "G0065");
  m_cmds << new cmd(this, "set sensorGuardTime", "S0065");
  m_cmds << new cmd(this, "get inputVoltage", "G0066");
  m_cmds << new cmd(this, "get inputVoltageCalibration", "G0067");
  m_cmds << new cmd(this, "set inputVoltageCalibration", "S0067");
  m_cmds << new cmd(this, "get numberOfLightbars", "G0068");
  m_cmds << new cmd(this, "set numberOfLightbars", "S0068");
  m_cmds << new cmd(this, "get ambientDivisor", "G0069");
  m_cmds << new cmd(this, "get currentLimit", "G006A");
  m_cmds << new cmd(this, "set currentLimit", "S006A");
  m_cmds << new cmd(this, "get bootloaderCode", "G006B");
  m_cmds << new cmd(this, "get xpressMode", "G006C");
  m_cmds << new cmd(this, "set xpressMode", "S006C");
  m_cmds << new cmd(this, "get batteryBackupStatus", "G006D");
  m_cmds << new cmd(this, "set batteryBackupStatus", "S006D");
  m_cmds << new cmd(this, "get sensorSeconds", "G006E");
  m_cmds << new cmd(this, "get inputVoltageTwo", "G006F");
  m_cmds << new cmd(this, "get inputVoltageTwoCalibration", "G0070");
  m_cmds << new cmd(this, "set inputVoltageTwoCalibration", "S0070");
  m_cmds << new cmd(this, "get maxRampUpSpeed", "G0071");
  m_cmds << new cmd(this, "set maxRampUpSpeed", "S0071");
  m_cmds << new cmd(this, "get maxRampDownSpeed", "G0072");
  m_cmds << new cmd(this, "set maxRampDownSpeed", "S0072");
  m_cmds << new cmd(this, "get wirelessNetworkKey", "G0073");
  m_cmds << new cmd(this, "set wirelessNetworkKey", "S0073");
  m_cmds << new cmd(this, "get emergencyLightLevel", "G0074");
  m_cmds << new cmd(this, "get batteryBackupPowerCalibration", "G0075");
  m_cmds << new cmd(this, "set batteryBackupPowerCalibration", "S0075");
  m_cmds << new cmd(this, "get motionSensorProfile", "G0076");
  m_cmds << new cmd(this, "set motionSensorProfile", "S0076");
  m_cmds << new cmd(this, "get powerMeterLevelAtOff", "G0077");
  m_cmds << new cmd(this, "set powerMeterLevelAtOff", "S0077");
  m_cmds << new cmd(this, "get powerMeterLevelAtMin", "G0078");
  m_cmds << new cmd(this, "set powerMeterLevelAtMin", "S0078");
  m_cmds << new cmd(this, "get powerMeterLevelAtMax", "G0079");
  m_cmds << new cmd(this, "set powerMeterLevelAtMax", "S0079");
  m_cmds << new cmd(this, "get powerMeterType", "G007A");
  m_cmds << new cmd(this, "set powerMeterType", "S007A");
  m_cmds << new cmd(this, "get DLAiSlaveMode", "G007B");
  m_cmds << new cmd(this, "set DLAiSlaveMode", "S007B");
  m_cmds << new cmd(this, "get DALIBootloadingActive", "G007C");
  m_cmds << new cmd(this, "get testingMode", "G007D");
  m_cmds << new cmd(this, "set testingMode", "S007D");
  m_cmds << new cmd(this, "get numberOfBatteriesSupported", "G007E");
  m_cmds << new cmd(this, "set numberOfBatteriesSupported", "S007E");
  // reset commands
  m_cmds << new cmd(this, "reset usage", "!U");
  m_cmds << new cmd(this, "reset log", "!L");
  m_cmds << new cmd(this, "reset newLog", "!K");
  m_cmds << new cmd(this, "reset eeprom", "!Z");
  m_cmds << new cmd(this, "reset eepromToDefault", "!C");
  m_cmds << new cmd(this, "reset eepromToLatestMapVersion", "!E");
  m_cmds << new cmd(this, "reset network", "!N");
  m_cmds << new cmd(this, "reset networkWithoutChecking", "!N1");
  m_cmds << new cmd(this, "reset daliCommissioning", "!Y");
  m_cmds << new cmd(this, "reset daliPowerMetering", "!A");
  // reboot commands
  m_cmds << new cmd(this, "reboot", "!R");
  m_cmds << new cmd(this, "reboot wirelessCard", "!W");
  m_cmds << new cmd(this, "reboot i2cDevices", "!X");
  // reload commands
  m_cmds << new cmd(this, "reload dlaFirmware", "!B");
  m_cmds << new cmd(this, "reload wirelessModuleFirmware", "!M");
  m_cmds << new cmd(this, "reload powerboardFirmware", "!P");
  m_cmds << new cmd(this, "reload lightbarFirmware", "!P");
  m_cmds << new cmd(this, "reload batteryBackupFirmware", "!P");
  m_cmds << new cmd(this, "reload motionSensorFirmware", "!V");
  // build the dictionary of helper commands
  foreach(cmd *cmd, m_cmds) {
    keywordList << cmd->m_helperCmd;
  }
  cmdCompleter = new QCompleter(keywordList, this);
  cmdCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  cmdCompleter->popup()->installEventFilter(this);
}

bool cmdHelper::eventFilter(QObject *target, QEvent *event) {
  // just close the completer list when enter/return is pressed
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter)) {
      cmdCompleter->popup()->close();
      return true;
    }
  }
  return QObject::eventFilter(target, event);
}
