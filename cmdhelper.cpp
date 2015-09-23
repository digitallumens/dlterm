#include "cmdhelper.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

/*** PMU register commands ***/
QStringList cmd_get_firmwareVersion(QStringList argList) {
  (void) argList;
  return QStringList() << "G0000";
}

QString parse_get_firmwareVersion(QStringList pmuResponse) {
  bool ok;
  qulonglong verInt = pmuResponse.at(0).toULongLong(&ok, 16);
  // format verMajor.verMinor.verBuild (buildMonth/buildDay/BuildYear)
  return QString("%1.%2.%3 (%5/%6/%4)").arg((verInt >> 40) & 0xFF).arg((verInt >> 32) & 0xFF).arg((verInt >> 24) & 0xFF).arg((verInt >> 16) & 0xFF).arg((verInt >> 8) & 0xFF).arg(verInt & 0xFF);
}

QStringList cmd_get_productCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0001";
}

QStringList cmd_set_productCode(QStringList argList) {
  return QStringList() << QString("S0001 %1").arg(argList.at(0));
}

QStringList cmd_get_serialNumber(QStringList argList) {
  (void) argList;
  return QStringList() << "G0002";
}

QStringList cmd_set_serialNumber(QStringList argList) {
  return QStringList() << QString("S0002 %1").arg(argList.at(0));
}

QStringList cmd_get_unixTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0003";
}

QStringList cmd_set_unixTime(QStringList argList) {
  return QStringList() << QString("S0003 %1").arg(argList.at(0));
}

QStringList cmd_get_temperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G0004";
}

QString parse_get_temperature(QStringList pmuResponse) {
  return "todo";
}

QStringList cmd_get_lightManualLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0005";
}

QStringList cmd_set_lightManualLevel(QStringList argList) {
  return QStringList() << QString("S0005 %1").arg(argList.at(0));
}

QStringList cmd_get_lightActiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0006";
}

QStringList cmd_get_lightInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0007";
}

QStringList cmd_get_lightOverrideActiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0008";
}

QStringList cmd_set_lightOverrideActiveLevel(QStringList argList) {
  return QStringList() << QString("S0008 %1").arg(argList.at(0));
}

QStringList cmd_get_lightOverrideInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0009";
}

QStringList cmd_set_lightOverrideInactiveLevel(QStringList argList) {
  return QStringList() << QString("S0009 %1").arg(argList.at(0));
}

QStringList cmd_get_sensorDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000A";
}

QStringList cmd_get_sensorOverrideDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000B";
}

QStringList cmd_set_sensorOverrideDelayTime(QStringList argList) {
  return QStringList() << QString("S000B %1").arg(argList.at(0));
}

QStringList cmd_get_powerupTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000C";
}

QStringList cmd_get_usageActiveTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000D";
}

QStringList cmd_get_usageInactiveTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000E";
}

QStringList cmd_get_usagePermActiveTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G000F";
}

QStringList cmd_get_usagePermInactiveTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0010";
}

QStringList cmd_get_usageWh(QStringList argList) {
  (void) argList;
  return QStringList() << "G0011";
}

QStringList cmd_get_usagePerm(QStringList argList) {
  (void) argList;
  return QStringList() << "G0012";
}

QStringList cmd_get_usageSensorEvents(QStringList argList) {
  (void) argList;
  return QStringList() << "G0013";
}

QStringList cmd_get_usagePermSensorEvents(QStringList argList) {
  (void) argList;
  return QStringList() << "G0014";
}

QStringList cmd_get_numberOfLogEntries(QStringList argList) {
  (void) argList;
  return QStringList() << "G0015";
}

QStringList cmd_get_configCalibrationP0(QStringList argList) {
  (void) argList;
  return QStringList() << "G0016";
}

QStringList cmd_set_configCalibrationP0(QStringList argList) {
  return QStringList() << QString("S0016 %1").arg(argList.at(0));
}

QStringList cmd_get_configCalibrationP1(QStringList argList) {
  (void) argList;
  return QStringList() << "G0017";
}

QStringList cmd_set_configCalibrationP1(QStringList argList) {
  return QStringList() << QString("S0017 %1").arg(argList.at(0));
}

QStringList cmd_get_configCalibrationP2(QStringList argList) {
  (void) argList;
  return QStringList() << "G0018";
}

QStringList cmd_set_configCalibrationP2(QStringList argList) {
  return QStringList() << QString("S0018 %1").arg(argList.at(0));
}

QStringList cmd_get_configCalibrationP3(QStringList argList) {
  (void) argList;
  return QStringList() << "G0019";
}

QStringList cmd_set_configCalibrationP3(QStringList argList) {
  return QStringList() << QString("S0019 %1").arg(argList.at(0));
}

QStringList cmd_get_buildTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G001A";
}

QStringList cmd_set_buildTime(QStringList argList) {
  return QStringList() << QString("S001A %1").arg(argList.at(0));
}

QStringList cmd_get_sensorTimeoutCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G001B";
}

QStringList cmd_get_currentLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G001C";
}

QStringList cmd_get_safeMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G001D";
}

QStringList cmd_get_lightBarSelect(QStringList argList) {
  (void) argList;
  return QStringList() << "G001E";
}

QStringList cmd_set_lightBarSelect(QStringList argList) {
  return QStringList() << QString("S001E %1").arg(argList.at(0));
}

QStringList cmd_get_currentPowerConsumption(QStringList argList) {
  (void) argList;
  return QStringList() << "G001F";
}

QStringList cmd_get_wirelessDataAggregator(QStringList argList) {
  (void) argList;
  return QStringList() << "G0020";
}

QStringList cmd_set_wirelessDataAggregator(QStringList argList) {
  return QStringList() << QString("S0020 %1").arg(argList.at(0));
}

QStringList cmd_get_resetUsageTimestamp(QStringList argList) {
  (void) argList;
  return QStringList() << "G0021";
}

QStringList cmd_get_pwmPeriodRegister(QStringList argList) {
  (void) argList;
  return QStringList() << "G0022";
}

QStringList cmd_set_pwmPeriodRegister(QStringList argList) {
  return QStringList() << QString("S0022 %1").arg(argList.at(0));
}

QStringList cmd_get_analogSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0023";
}

QStringList cmd_get_analogReportingHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G0024";
}

QStringList cmd_get_zone(QStringList argList) {
  (void) argList;
  return QStringList() << "G0025";
}

QStringList cmd_set_zone(QStringList argList) {
  return QStringList() << QString("S0025 %1").arg(argList.at(0));
}

QStringList cmd_get_lightTemporaryActiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0026";
}

QStringList cmd_set_lightTemporaryActiveLevel(QStringList argList) {
  return QStringList() << QString("S0026 %1").arg(argList.at(0));
}

QStringList cmd_get_lightTemporaryInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0027";
}

QStringList cmd_set_lightTemporaryInactiveLevel(QStringList argList) {
  return QStringList() << QString("S0027 %1").arg(argList.at(0));
}

QStringList cmd_get_sensorTemporaryDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0028";
}

QStringList cmd_set_sensorTemporaryDealyTime(QStringList argList) {
  return QStringList() << QString("S0028 %1").arg(argList.at(0));
}

QStringList cmd_get_temporaryOverrideTimeout(QStringList argList) {
  (void) argList;
  return QStringList() << "G0029";
}

QStringList cmd_set_temporaryOverrideTiemout(QStringList argList) {
  return QStringList() << QString("S0029 %1").arg(argList.at(0));
}

QStringList cmd_get_setRemoteState(QStringList argList) {
  (void) argList;
  return QStringList() << "G002A";
}

QStringList cmd_set_setRemoteState(QStringList argList) {
  return QStringList() << QString("S002A %1").arg(argList.at(0));
}

QStringList cmd_get_remoteStateDelayTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G002B";
}

QStringList cmd_set_remoteStateDelayTime(QStringList argList) {
  return QStringList() << QString("S002B %1").arg(argList.at(0));
}

QStringList cmd_get_remoteSecondsCountdown(QStringList argList) {
  (void) argList;
  return QStringList() << "G002C";
}

QStringList cmd_get_minimumDimmingValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G002D";
}

QStringList cmd_get_powerCalibrationA0(QStringList argList) {
  (void) argList;
  return QStringList() << "G002E";
}

QStringList cmd_set_powerCalibrationA0(QStringList argList) {
  return QStringList() << QString("S002E %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationB0(QStringList argList) {
  (void) argList;
  return QStringList() << "G002F";
}

QStringList cmd_set_powerCalibrationB0(QStringList argList) {
  return QStringList() << QString("S002F %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationC0(QStringList argList) {
  (void) argList;
  return QStringList() << "G0030";
}

QStringList cmd_set_powerCalibrationC0(QStringList argList) {
  return QStringList() << QString("S0030 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationMA(QStringList argList) {
  (void) argList;
  return QStringList() << "G0031";
}

QStringList cmd_set_powerCalibrationMA(QStringList argList) {
  return QStringList() << QString("S0031 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationMB(QStringList argList) {
  (void) argList;
  return QStringList() << "G0032";
}

QStringList cmd_set_powerCalibrationMB(QStringList argList) {
  return QStringList() << QString("S0032 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationMC(QStringList argList) {
  (void) argList;
  return QStringList() << "G0033";
}

QStringList cmd_set_powerCalibrationMC(QStringList argList) {
  return QStringList() << QString("S0033 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationPOff(QStringList argList) {
  (void) argList;
  return QStringList() << "G0034";
}

QStringList cmd_set_powerCalibrationPOff(QStringList argList) {
  return QStringList() << QString("S0034 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationPOn(QStringList argList) {
  (void) argList;
  return QStringList() << "G0035";
}

QStringList cmd_set_powerCalibrationPOn(QStringList argList) {
  return QStringList() << QString("S0035 %1").arg(argList.at(0));
}

QStringList cmd_get_powerCalibrationT0(QStringList argList) {
  (void) argList;
  return QStringList() << "G0036";
}

QStringList cmd_set_powerCalibrationT0(QStringList argList) {
  return QStringList() << QString("S0036 %1").arg(argList.at(0));
}

QStringList cmd_get_powerEstimatorTemperatureOverride(QStringList argList) {
  (void) argList;
  return QStringList() << "G0037";
}

QStringList cmd_set_powerEstimatorTemperatureOverride(QStringList argList) {
  return QStringList() << QString("S0037 %1").arg(argList.at(0));
}

QStringList cmd_get_cachedTemperatureValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0038";
}

QStringList cmd_get_eepromSize(QStringList argList) {
  (void) argList;
  return QStringList() << "G0039";
}

QStringList cmd_get_hardwareRevision(QStringList argList) {
  (void) argList;
  return QStringList() << "G003A";
}

QStringList cmd_get_wirelessPanId(QStringList argList) {
  (void) argList;
  return QStringList() << "G003B";
}

QStringList cmd_set_wirelessPanId(QStringList argList) {
  return QStringList() << QString("S003B %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessChannelMask(QStringList argList) {
  (void) argList;
  return QStringList() << "G003C";
}

QStringList cmd_set_wirelessChannelMask(QStringList argList) {
  return QStringList() << QString("S003C %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessShortAddress(QStringList argList) {
  (void) argList;
  return QStringList() << "G003D";
}

QStringList cmd_set_wirelessShortAddress(QStringList argList) {
  return QStringList() << QString("S003D %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessRole(QStringList argList) {
  (void) argList;
  return QStringList() << "G003E";
}

QStringList cmd_set_wirelessRole(QStringList argList) {
  return QStringList() << QString("S003E %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessWatchdogHold(QStringList argList) {
  (void) argList;
  return QStringList() << "G003F";
}

QStringList cmd_set_wirelessWatchdogHold(QStringList argList) {
  return QStringList() << QString("S003F %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessWatchdogPeriod(QStringList argList) {
  (void) argList;
  return QStringList() << "G0040";
}

QStringList cmd_set_wirelessWatchdogPeriod(QStringList argList) {
  return QStringList() << QString("S0040 %1").arg(argList.at(0));
}

QStringList cmd_get_firmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0041";
}

QStringList cmd_get_moduleFirmwareCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0042";
}

QStringList cmd_get_maxTemperatureObserved(QStringList argList) {
  (void) argList;
  return QStringList() << "G0043";
}

QStringList cmd_get_maxTemperatureObservedTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0044";
}

QStringList cmd_get_overTemperatureThresholdLow(QStringList argList) {
  (void) argList;
  return QStringList() << "G0045";
}

QStringList cmd_set_overTemperatureThresholdLow(QStringList argList) {
  return QStringList() << QString("S0045 %1").arg(argList.at(0));
}

QStringList cmd_get_overTemperatureThresholdHigh(QStringList argList) {
  (void) argList;
  return QStringList() << "G0046";
}

QStringList cmd_set_overTemperatureThresholdHigh(QStringList argList) {
  return QStringList() << QString("S0046 %1").arg(argList.at(0));
}

QStringList cmd_get_overTemperatureDimmingLimit(QStringList argList) {
  (void) argList;
  return QStringList() << "G0047";
}

QStringList cmd_set_overTemperatureDimmingLimit(QStringList argList) {
  return QStringList() << QString("S0047 %1").arg(argList.at(0));
}

QStringList cmd_get_analogDimmingMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0048";
}

QStringList cmd_set_analogDimmingMode(QStringList argList) {
  return QStringList() << QString("S0048 %1").arg(argList.at(0));
}

QStringList cmd_get_fixtureIdMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0049";
}

QStringList cmd_set_fixtureIdMode(QStringList argList) {
  return QStringList() << QString("S0049 %1").arg(argList.at(0));
}

QStringList cmd_get_acFrequency(QStringList argList) {
  (void) argList;
  return QStringList() << "G004A";
}

QStringList cmd_get_sensorBits(QStringList argList) {
  (void) argList;
  return QStringList() << "G004B";
}

QStringList cmd_get_powerMeterCommand(QStringList argList) {
  (void) argList;
  return QStringList() << "G004C";
}

QStringList cmd_set_powerMeterCommond(QStringList argList) {
  return QStringList() << QString("S004C %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeterRegister(QStringList argList) {
  (void) argList;
  return QStringList() << "G004D";
}

QStringList cmd_set_powerMetereRegister(QStringList argList) {
  return QStringList() << QString("S004D %1").arg(argList.at(0));
}

QStringList cmd_get_ambientTemperature(QStringList argList) {
  (void) argList;
  return QStringList() << "G004E";
}

QStringList cmd_get_lightSensorLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G004F";
}

QStringList cmd_get_sensor0Timeout(QStringList argList) {
  (void) argList;
  return QStringList() << "G0050";
}

QStringList cmd_set_sensor0Timeout(QStringList argList) {
  return QStringList() << QString("S0050 %1").arg(argList.at(0));
}

QStringList cmd_get_sensor0Offset(QStringList argList) {
  (void) argList;
  return QStringList() << "G0051";
}

QStringList cmd_set_sensor0Offset(QStringList argList) {
  return QStringList() << QString("S0051 %1").arg(argList.at(0));
}

QStringList cmd_get_sensor1Timeout(QStringList argList) {
  (void) argList;
  return QStringList() << "G0052";
}

QStringList cmd_set_sensor1Timeout(QStringList argList) {
  return QStringList() << QString("S0052 %1").arg(argList.at(0));
}

QStringList cmd_get_sensor1Offset(QStringList argList) {
  (void) argList;
  return QStringList() << "G0053";
}

QStringList cmd_set_sensor1Offset(QStringList argList) {
  return QStringList() << QString("S0053 %1").arg(argList.at(0));
}

QStringList cmd_get_analogDimmingLowValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0054";
}

QStringList cmd_set_analogDimmingLowValue(QStringList argList) {
  return QStringList() << QString("S0054 %1").arg(argList.at(0));
}

QStringList cmd_get_analogDimmingHighValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0055";
}

QStringList cmd_set_analogDimmingHighValue(QStringList argList) {
  return QStringList() << QString("S0055 %1").arg(argList.at(0));
}

QStringList cmd_get_analogDimmingOffValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0056";
}

QStringList cmd_set_analogDimmingOffValue(QStringList argList) {
  return QStringList() << QString("S0056 %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeasurementMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G0057";
}

QStringList cmd_set_powerMeasurementMode(QStringList argList) {
  return QStringList() << QString("S0057 %1").arg(argList.at(0));
}

QStringList cmd_get_externalPowerMeter(QStringList argList) {
  (void) argList;
  return QStringList() << "G0058";
}

QStringList cmd_set_externalPowerMeter(QStringList argList) {
  return QStringList() << QString("S0058 %1").arg(argList.at(0));
}

QStringList cmd_get_ambientSensorValue(QStringList argList) {
  (void) argList;
  return QStringList() << "G0059";
}

QStringList cmd_get_ambientActiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G005A";
}

QStringList cmd_set_ambientActiveLevel(QStringList argList) {
  return QStringList() << QString("S005A %1").arg(argList.at(0));
}

QStringList cmd_get_ambientInactiveLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G005B";
}

QStringList cmd_set_ambientInactiveLevel(QStringList argList) {
  return QStringList() << QString("S005B %1").arg(argList.at(0));
}

QStringList cmd_get_ambientEnvironmentalGain(QStringList argList) {
  (void) argList;
  return QStringList() << "G005C";
}

QStringList cmd_set_ambientEnvironmentalGain(QStringList argList) {
  return QStringList() << QString("S005C %1").arg(argList.at(0));
}

QStringList cmd_get_ambientOffHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G005D";
}

QStringList cmd_set_ambientOffHysteresis(QStringList argList) {
  return QStringList() << QString("S005D %1").arg(argList.at(0));
}

QStringList cmd_get_ambientOnHysteresis(QStringList argList) {
  (void) argList;
  return QStringList() << "G005E";
}

QStringList cmd_set_ambientOnHystereis(QStringList argList) {
  return QStringList() << QString("S005E %1").arg(argList.at(0));
}

QStringList cmd_get_powerboardProtocol(QStringList argList) {
  (void) argList;
  return QStringList() << "G005F";
}

QStringList cmd_get_ledOverride(QStringList argList) {
  (void) argList;
  return QStringList() << "G0060";
}

QStringList cmd_set_ledOverride(QStringList argList) {
  return QStringList() << QString("S0060 %1").arg(argList.at(0));
}

QStringList cmd_get_fadeUpStep(QStringList argList) {
  (void) argList;
  return QStringList() << "G0061";
}

QStringList cmd_set_fadeUpStep(QStringList argList) {
  return QStringList() << QString("S0061 %1").arg(argList.at(0));
}

QStringList cmd_get_fadeDownStep(QStringList argList) {
  (void) argList;
  return QStringList() << "G0062";
}

QStringList cmd_set_fadeDownStep(QStringList argList) {
  return QStringList() << QString("S0062 %1").arg(argList.at(0));
}

QStringList cmd_get_maxBrightness(QStringList argList) {
  (void) argList;
  return QStringList() << "G0063";
}

QStringList cmd_set_maxBrightness(QStringList argList) {
  return QStringList() << QString("S0063 %1").arg(argList.at(0));
}

QStringList cmd_get_i2cResets(QStringList argList) {
  (void) argList;
  return QStringList() << "G0064";
}

QStringList cmd_get_sensorGuardTime(QStringList argList) {
  (void) argList;
  return QStringList() << "G0065";
}

QStringList cmd_set_sensorGuardTime(QStringList argList) {
  return QStringList() << QString("S0065 %1").arg(argList.at(0));
}

QStringList cmd_get_inputVoltage(QStringList argList) {
  (void) argList;
  return QStringList() << "G0066";
}

QStringList cmd_get_inputVoltageCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0067";
}

QStringList cmd_set_inputVoltageCalibration(QStringList argList) {
  return QStringList() << QString("S0067 %1").arg(argList.at(0));
}

QStringList cmd_get_numberOfLightbars(QStringList argList) {
  (void) argList;
  return QStringList() << "G0068";
}

QStringList cmd_set_numberOfLightbars(QStringList argList) {
  return QStringList() << QString("S0068 %1").arg(argList.at(0));
}

QStringList cmd_get_ambientDivisor(QStringList argList) {
  (void) argList;
  return QStringList() << "G0069";
}

QStringList cmd_get_currentLimit(QStringList argList) {
  (void) argList;
  return QStringList() << "G006A";
}

QStringList cmd_set_currentLimit(QStringList argList) {
  return QStringList() << QString("S006A %1").arg(argList.at(0));
}

QStringList cmd_get_bootloaderCode(QStringList argList) {
  (void) argList;
  return QStringList() << "G006B";
}

QStringList cmd_get_xpressMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G006C";
}

QStringList cmd_set_xpressMode(QStringList argList) {
  return QStringList() << QString("S006C %1").arg(argList.at(0));
}

QStringList cmd_get_batteryBackupStatus(QStringList argList) {
  (void) argList;
  return QStringList() << "G006D";
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

QStringList cmd_set_batteryBackupStatus(QStringList argList) {
  return QStringList() << QString("S006D %1").arg(argList.at(0));
}

QStringList cmd_get_sensorSeconds(QStringList argList) {
  (void) argList;
  return QStringList() << "G006E";
}

QStringList cmd_get_inputVoltageTwo(QStringList argList) {
  (void) argList;
  return QStringList() << "G006F";
}

QStringList cmd_get_inputVoltageTwoCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0070";
}

QStringList cmd_set_inputVoltageTwoCalibration(QStringList argList) {
  return QStringList() << QString("S0070 %1").arg(argList.at(0));
}

QStringList cmd_get_maxRampUpSpeed(QStringList argList) {
  (void) argList;
  return QStringList() << "G0071";
}

QStringList cmd_set_maxRampUpSpeed(QStringList argList) {
  return QStringList() << QString("S0071 %1").arg(argList.at(0));
}

QStringList cmd_get_maxRampDownSpeed(QStringList argList) {
  (void) argList;
  return QStringList() << "G0072";
}

QStringList cmd_set_maxRampDownSpeed(QStringList argList) {
  return QStringList() << QString("S0072 %1").arg(argList.at(0));
}

QStringList cmd_get_wirelessNetworkKey(QStringList argList) {
  (void) argList;
  return QStringList() << "G0073";
}

QStringList cmd_set_wirelessNetworkKey(QStringList argList) {
  return QStringList() << QString("S0073 %1").arg(argList.at(0));
}

QStringList cmd_get_emergentLightLevel(QStringList argList) {
  (void) argList;
  return QStringList() << "G0074";
}

QStringList cmd_get_batteryBackupPowerCalibration(QStringList argList) {
  (void) argList;
  return QStringList() << "G0075";
}

QStringList cmd_set_batteryBackupPowerCalibration(QStringList argList) {
  return QStringList() << QString("S0075 %1").arg(argList.at(0));
}

QStringList cmd_get_motionSensorProfile(QStringList argList) {
  (void) argList;
  return QStringList() << "G0076";
}

QStringList cmd_set_motionSensorProfile(QStringList argList) {
  return QStringList() << QString("S0076 %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeterLevelAtOff(QStringList argList) {
  (void) argList;
  return QStringList() << "G0077";
}

QStringList cmd_set_powerMeterLevelAtOff(QStringList argList) {
  return QStringList() << QString("S0077 %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeterLevelAtMin(QStringList argList) {
  (void) argList;
  return QStringList() << "G0078";
}

QStringList cmd_set_powerMeterLevelAtMin(QStringList argList) {
  return QStringList() << QString("S0078 %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeterLevelAtMax(QStringList argList) {
  (void) argList;
  return QStringList() << "G0079";
}

QStringList cmd_set_powerMeterLevelAtMax(QStringList argList) {
  return QStringList() << QString("S0079 %1").arg(argList.at(0));
}

QStringList cmd_get_powerMeterType(QStringList argList) {
  (void) argList;
  return QStringList() << "G007A";
}

QStringList cmd_set_powerMeterType(QStringList argList) {
  return QStringList() << QString("S007C %1").arg(argList.at(0));
}

QStringList cmd_get_DLAiSlaveMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G007B";
}

QStringList cmd_set_DLAiSlaveMode(QStringList argList) {
  return QStringList() << QString("S007B %1").arg(argList.at(0));
}

QStringList cmd_get_DALIBootlodingActive(QStringList argList) {
  (void) argList;
  return QStringList() << "G007C";
}

QStringList cmd_get_testingMode(QStringList argList) {
  (void) argList;
  return QStringList() << "G007D";
}

QStringList cmd_set_testingMode(QStringList argList) {
  return QStringList() << QString("S007D %1").arg(argList.at(0));
}

QStringList cmd_get_numberOfBatteriesSupported(QStringList argList) {
  (void) argList;
  return QStringList() << "G007E";
}

QStringList cmd_set_numberOfBatteriesSupported(QStringList argList) {
  return QStringList() << QString("S007E %1").arg(argList.at(0));
}

/*** lightbar register commands ***/
QStringList cmd_get_lbVersion(QStringList argList) {
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

QString parse_get_lbVersion(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  quint16 verHiInt = pmuResponse.at(3).toUShort(&ok, 16);
  quint16 verLoInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse += QString("Firmware version: %1.%2.%3<br>").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  parsedResponse += QString("Firmware code: %1%2<br>").arg(pmuResponse.at(1)).arg(pmuResponse.at(2));
  parsedResponse += QString("Protocol version: %1").arg(pmuResponse.at(0));
  return parsedResponse;
}

QStringList cmd_get_lbStatus(QStringList argList) {
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

QString parse_get_lbStatus(QStringList pmuResponse) {
  bool ok;
  quint16 statusInt = pmuResponse.at(0).toUShort(&ok, 16);
  QString parsedResponse;
  if (statusInt & 4) {
    parsedResponse += "Bypass: activate<br>";
  } else {
    parsedResponse += "Bypass: inactive<br>";
  }
  statusInt = pmuResponse.at(1).toUShort(&ok, 16);
  parsedResponse += QString("String 1 current: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(2).toUShort(&ok, 16);
  parsedResponse += QString("String 2 current: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(3).toUShort(&ok, 16);
  parsedResponse += QString("String 3 current: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse += QString("String 4 current: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(5).toUShort(&ok, 16);
  parsedResponse += QString("String current minimum: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(6).toUShort(&ok, 16);
  parsedResponse += QString("Temperature: %1 C<br>").arg(statusInt);
  statusInt = pmuResponse.at(7).toUShort(&ok, 16);
  parsedResponse += QString("String current sum: %1 mA<br>").arg(statusInt);
  statusInt = pmuResponse.at(8).toUShort(&ok, 16);
  parsedResponse += QString("Voltage reference: %1 volts<br>").arg(statusInt);
  parsedResponse += QString("Light level (0x029C = OFF): %1<br>").arg(pmuResponse.at(9));
  parsedResponse += QString("Light active slew rate: %1<br>").arg(pmuResponse.at(10));
  parsedResponse += QString("Light inactive slew rate: %1").arg(pmuResponse.at(11));
  return parsedResponse;
}

QStringList cmd_get_lbConfig(QStringList argList) {
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

QString parse_get_lbConfig(QStringList pmuResponse) {
  QString parsedResponse;
  parsedResponse += QString("Hardware revision: %1<br>").arg(pmuResponse.at(0));
  parsedResponse += QString("Temperature calibration: %1<br>").arg(pmuResponse.at(1));
  parsedResponse += QString("LED device type: %1<br>").arg(pmuResponse.at(2));
  parsedResponse += QString("Serial number: %1%2<br>").arg(pmuResponse.at(3)).arg(pmuResponse.at(4));
  parsedResponse += QString("Current sense bypass threshold: %1<br>").arg(pmuResponse.at(5));
  parsedResponse += QString("Current sense bypass hysteresis: %1<br>").arg(pmuResponse.at(6));
  parsedResponse += QString("Estimator current sense coefficient: %1<br>").arg(pmuResponse.at(7));
  parsedResponse += QString("Estimator current sense exponent: %1<br>").arg(pmuResponse.at(8));
  parsedResponse += QString("Bypass override temperature: %1<br>").arg(pmuResponse.at(9));
  parsedResponse += QString("Temperature throttle limit: %1").arg(pmuResponse.at(10));
  return parsedResponse;
}

/*** battery backup register commands ***/
QStringList cmd_get_bbVersion(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum == "C2";
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

QString parse_get_bbVersion(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  quint16 verHiInt = pmuResponse.at(3).toUShort(&ok, 16);
  quint16 verLoInt = pmuResponse.at(4).toUShort(&ok, 16);
  parsedResponse += QString("Firmware version: %1.%2.%3<br>").arg((verHiInt >> 8) & 0xFF).arg(verHiInt & 0xFF).arg((verLoInt >> 8) & 0xFF);
  parsedResponse += QString("Firmware code: %1%2<br>").arg(pmuResponse.at(1)).arg(pmuResponse.at(2));
  parsedResponse += QString("Protocol version: %1").arg(pmuResponse.at(0));
  return parsedResponse;
}

QStringList cmd_get_bbStatus(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum == "C2";
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

QString parse_get_bbStatus(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  QMap <int, QString> statusDict;
  quint32 statusInt = pmuResponse.at(0).toLong(&ok, 16);
  // parse mode bits
  statusDict.insert(0, "Invalid");
  statusDict.insert(1, "Charging");
  statusDict.insert(2, "Standby");
  statusDict.insert(3, "Error");
  statusDict.insert(4, "Emergency");
  statusDict.insert(5, "Test");
  statusDict.insert(6, "Powerdown");
  statusDict.insert(7, "Shutdown");
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
  statusDict.insert(0, "Good");
  statusDict.insert(1, "Failed");
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
  return parsedResponse;
}

QStringList cmd_get_bbConfig(QStringList argList) {
  QString battNum;
  QStringList cmdList;
  if (argList.length() == 0) {
    battNum = "C0";
  } else if (argList.length() == 1) {
    // map battery number to I2C address
    if (argList.at(0) == "00") {
      battNum = "C0";
    } else {
      battNum == "C2";
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

QString parse_get_bbConfig(QStringList pmuResponse) {
  bool ok;
  QString parsedResponse;
  parsedResponse += QString("Hardware revision: %1<br>").arg(pmuResponse.at(0));
  parsedResponse += QString("Temperature calibration: %1<br>").arg(pmuResponse.at(1));
  parsedResponse += QString("Serial number: %1%2<br>").arg(pmuResponse.at(2)).arg(pmuResponse.at(3));
  parsedResponse += QString("Charge time: %1 minutes<br>").arg(pmuResponse.at(4).toUShort(&ok, 16));
  parsedResponse += QString("Trickle time: %1 minutes<br>").arg(pmuResponse.at(5).toUShort(&ok, 16));
  parsedResponse += QString("Standby time: %1 minutes<br>").arg(pmuResponse.at(6).toUShort(&ok, 16));
  parsedResponse += QString("Shutdown time: %1 minutes<br>").arg(pmuResponse.at(16).toUShort(&ok, 16));
  parsedResponse += QString("Max battery voltage: %1 volts<br>").arg(pmuResponse.at(7).toUShort(&ok, 16) * 0.04);
  parsedResponse += QString("Min battery voltage: %1 volts<br>").arg(pmuResponse.at(8).toUShort(&ok, 16) * 0.04);
  parsedResponse += QString("Recharge battery voltage: %1 volts<br>").arg(pmuResponse.at(9).toUShort(&ok, 16) * 0.04);
  parsedResponse += QString("Max charge temperature: %1 C<br>").arg((pmuResponse.at(10).toUShort(&ok, 16) - 164) * 0.125);
  parsedResponse += QString("Max emergency temperature: %1 C<br>").arg((pmuResponse.at(11).toUShort(&ok, 16) - 164) * 0.125);
  parsedResponse += QString("Min emergency verify voltage: %1 volts<br>").arg(pmuResponse.at(12).toUShort(&ok, 16) * 0.05);
  parsedResponse += QString("Max emergency verify voltage: %1 volts<br>").arg(pmuResponse.at(13).toUShort(&ok, 16) * 0.05);
  parsedResponse += QString("Max lightbar PSU current: %1 mA<br>").arg(pmuResponse.at(14).toUShort(&ok, 16) * 2.44);
  parsedResponse += QString("Certification mark: %1<br>").arg((pmuResponse.at(15) == "0000") ? "UL" : "CE");
  parsedResponse += QString("Product code: %1%2").arg(pmuResponse.at(17)).arg(pmuResponse.at(18));
  return parsedResponse;
}

/*** reset commands ***/
QStringList cmd_reset_usage(QStringList argList) {
  (void) argList;
  return QStringList() << "!U";
}

QStringList cmd_reset_oldLog(QStringList argList) {
  (void) argList;
  return QStringList() << "!L";
}

QStringList cmd_reset_log(QStringList argList) {
  (void) argList;
  return QStringList() << "!K";
}

QStringList cmd_reset_logIndex(QStringList argList) {
  return QStringList() << QString("J%1").arg(argList.at(0));
}

QStringList cmd_reset_eeprom(QStringList argList) {
  (void) argList;
  return QStringList() << "!Z";
}

QStringList cmd_reset_eepromToDefault(QStringList argList) {
  (void) argList;
  return QStringList() << "!C";
}

QStringList cmd_reset_eepromToLatestMapVersion(QStringList argList) {
  (void) argList;
  return QStringList() << "!E";
}

QStringList cmd_reset_network(QStringList argList) {
  (void) argList;
  return QStringList() << "!N";
}

QStringList cmd_reset_networkWithoutChecking(QStringList argList) {
  (void) argList;
  return QStringList() << "!N1";
}

QStringList cmd_reset_daliCommissioning(QStringList argList) {
  (void) argList;
  return QStringList() << "!Y";
}

QStringList cmd_reset_daliPowerMetering(QStringList argList) {
  (void) argList;
  return QStringList() << "!A";
}

/*** reboot commands ***/
QStringList cmd_reboot_pmu(QStringList argList) {
  (void) argList;
  return QStringList() << "!R";
}

QStringList cmd_reboot_wirelessCard(QStringList argList) {
  (void) argList;
  return QStringList() << "!W";
}

QStringList cmd_reboot_i2cDevices(QStringList argList) {
  (void) argList;
  return QStringList() << "!X";
}

/*** reload commands ***/
QStringList cmd_reload_dlaFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!B";
}

QStringList cmd_reload_wirelessModuleFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!M";
}

QStringList cmd_reload_powerboardFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList cmd_reload_lightbarFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList cmd_reload_batteryBackupFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!P";
}

QStringList cmd_reload_motionSensorFirmware(QStringList argList) {
  (void) argList;
  return QStringList() << "!V";
}

/*** log commands ***/
QStringList cmd_get_logIndex(QStringList argList) {
  (void) argList;
  return QStringList() << "K";
}

QString parse_get_logIndex(QStringList pmuResponse) {
  QString arg1 = pmuResponse.at(0);
  QString head = arg1.left(4);
  arg1.remove(0, 4);
  QString tail = arg1.left(4);
  arg1.remove(0, 4);
  QString first = arg1.left(4);
  if (first == "FFFF") {
    first = "none";
  }
  return QString("head: %1<br>tail: %2<br>first recent: %3").arg(head).arg(tail).arg(first);
}

QStringList cmd_get_log(QStringList argList) {
  QString index;
  if (argList.length() == 0) {
    index = "0000";
  } else {
    index = argList.at(0);
  }
  return QStringList() << QString("K%1").arg(index);
}

QString parse_get_log(QStringList pmuResponse) {
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
  return log;
}

QStringList cmd_insert_logEntry(QStringList argList) {
  return QStringList() << QString("E%1").arg(argList.at(0));
}

cmdHelper::cmdHelper(QObject *parent) : QObject(parent) {
  QStringList keywordList;
  // get & set PMU register commands
  m_cmdTable.insert("get firmwareVersion", new pmu(cmd_get_firmwareVersion, parse_get_firmwareVersion));
  m_cmdTable.insert("get productCode", new pmu(cmd_get_productCode));
  m_cmdTable.insert("set productCode", new pmu(cmd_set_productCode));
  m_cmdTable.insert("get serialNumber", new pmu(cmd_get_serialNumber));
  m_cmdTable.insert("set serialNumber", new pmu(cmd_set_serialNumber));
  m_cmdTable.insert("get unixTime", new pmu(cmd_get_unixTime));
  m_cmdTable.insert("set unixTime", new pmu(cmd_set_unixTime));
  m_cmdTable.insert("get temperature", new pmu(cmd_get_temperature, parse_get_temperature));
  m_cmdTable.insert("get lightManualLevel", new pmu(cmd_get_lightManualLevel));
  m_cmdTable.insert("set lightManualLevel", new pmu(cmd_set_lightManualLevel));
  m_cmdTable.insert("get lightActiveLevel", new pmu(cmd_get_lightActiveLevel));
  m_cmdTable.insert("get lightInactiveLevel", new pmu(cmd_get_lightInactiveLevel));
  m_cmdTable.insert("get lightOverrideActiveLevel", new pmu(cmd_get_lightOverrideActiveLevel));
  m_cmdTable.insert("set lightOverrideActiveLevel", new pmu(cmd_set_lightOverrideActiveLevel));
  m_cmdTable.insert("get lightOverrideInactiveLevel", new pmu(cmd_get_lightOverrideInactiveLevel));
  m_cmdTable.insert("set lightOverrideInactiveLevel", new pmu(cmd_set_lightOverrideInactiveLevel));
  m_cmdTable.insert("get sensorDelayTime", new pmu(cmd_get_sensorDelayTime));
  m_cmdTable.insert("get sensorOverrideDelayTime", new pmu(cmd_get_sensorOverrideDelayTime));
  m_cmdTable.insert("set sensorOverrideDelayTime", new pmu(cmd_set_sensorOverrideDelayTime));
  m_cmdTable.insert("get powerupTime", new pmu(cmd_get_powerupTime));
  m_cmdTable.insert("get usageActiveTime", new pmu(cmd_get_usageActiveTime));
  m_cmdTable.insert("get usageInactiveTime", new pmu(cmd_get_usageInactiveTime));
  m_cmdTable.insert("get usagePermActiveTime", new pmu(cmd_get_usagePermActiveTime));
  m_cmdTable.insert("get usagePermInactiveTime", new pmu(cmd_get_usagePermInactiveTime));
  m_cmdTable.insert("get usageWh", new pmu(cmd_get_usageWh));
  m_cmdTable.insert("get usagePerm", new pmu(cmd_get_usagePerm));
  m_cmdTable.insert("get usageSensorEvents", new pmu(cmd_get_usageSensorEvents));
  m_cmdTable.insert("get usagePermSensorEvents", new pmu(cmd_get_usagePermSensorEvents));
  m_cmdTable.insert("get numberOfLogEntries", new pmu(cmd_get_numberOfLogEntries));
  m_cmdTable.insert("get configCalibrationP0", new pmu(cmd_get_configCalibrationP0));
  m_cmdTable.insert("set configCalibrationP0", new pmu(cmd_set_configCalibrationP0));
  m_cmdTable.insert("get configCalibrationP1", new pmu(cmd_get_configCalibrationP1));
  m_cmdTable.insert("set configCalibrationP1", new pmu(cmd_set_configCalibrationP1));
  m_cmdTable.insert("get configCalibrationP2", new pmu(cmd_get_configCalibrationP2));
  m_cmdTable.insert("set configCalibrationP2", new pmu(cmd_set_configCalibrationP2));
  m_cmdTable.insert("get configCalibrationP3", new pmu(cmd_get_configCalibrationP3));
  m_cmdTable.insert("set configCalibrationP3", new pmu(cmd_set_configCalibrationP3));
  m_cmdTable.insert("get buildTime", new pmu(cmd_get_buildTime));
  m_cmdTable.insert("set buildTime", new pmu(cmd_set_buildTime));
  m_cmdTable.insert("get sensorTimeoutCountdown", new pmu(cmd_get_sensorTimeoutCountdown));
  m_cmdTable.insert("get currentLightLevel", new pmu(cmd_get_currentLightLevel));
  m_cmdTable.insert("get safeMode", new pmu(cmd_get_safeMode));
  m_cmdTable.insert("get lightBarSelect", new pmu(cmd_get_lightBarSelect));
  m_cmdTable.insert("set lightBarSelect", new pmu(cmd_set_lightBarSelect));
  m_cmdTable.insert("get currentPowerConsumption", new pmu(cmd_get_currentPowerConsumption));
  m_cmdTable.insert("get wirelessDataAggregator", new pmu(cmd_get_wirelessDataAggregator));
  m_cmdTable.insert("set wirelessDataAggregator", new pmu(cmd_set_wirelessDataAggregator));
  m_cmdTable.insert("get resetUsageTimestamp", new pmu(cmd_get_resetUsageTimestamp));
  m_cmdTable.insert("get pwmPeriodRegister", new pmu(cmd_get_pwmPeriodRegister));
  m_cmdTable.insert("set pwmPeriodRegister", new pmu(cmd_set_pwmPeriodRegister));
  m_cmdTable.insert("get analogSensorValue", new pmu(cmd_get_analogSensorValue));
  m_cmdTable.insert("get analogReportingHysteresis", new pmu(cmd_get_analogReportingHysteresis));
  m_cmdTable.insert("get zone", new pmu(cmd_get_zone));
  m_cmdTable.insert("set zone", new pmu(cmd_set_zone));
  m_cmdTable.insert("get lightTemporaryActiveLevel", new pmu(cmd_get_lightTemporaryActiveLevel));
  m_cmdTable.insert("set lightTemporaryActiveLevel", new pmu(cmd_set_lightTemporaryActiveLevel));
  m_cmdTable.insert("get lightTemporaryInactiveLevel", new pmu(cmd_get_lightTemporaryInactiveLevel));
  m_cmdTable.insert("set lightTemporaryInactiveLevel", new pmu(cmd_set_lightTemporaryInactiveLevel));
  m_cmdTable.insert("get sensorTemporaryDelayTime", new pmu(cmd_get_sensorTemporaryDelayTime));
  m_cmdTable.insert("set sensorTemporaryDelayTime", new pmu(cmd_set_sensorTemporaryDealyTime));
  m_cmdTable.insert("get temporaryOverrideTimeout", new pmu(cmd_get_temporaryOverrideTimeout));
  m_cmdTable.insert("set temporaryOverrideTimeout", new pmu(cmd_set_temporaryOverrideTiemout));
  m_cmdTable.insert("get setRemoteState", new pmu(cmd_get_setRemoteState));
  m_cmdTable.insert("set setRemoteState", new pmu(cmd_set_setRemoteState));
  m_cmdTable.insert("get remoteSetDelayTime", new pmu(cmd_get_remoteStateDelayTime));
  m_cmdTable.insert("set remoteSetDelayTime", new pmu(cmd_set_remoteStateDelayTime));
  m_cmdTable.insert("get remoteSecondsCountdown", new pmu(cmd_get_remoteSecondsCountdown));
  m_cmdTable.insert("get minimumDimmingValue", new pmu(cmd_get_minimumDimmingValue));
  m_cmdTable.insert("get powerCalibrationA0", new pmu(cmd_get_powerCalibrationA0));
  m_cmdTable.insert("set powerCalibrationA0", new pmu(cmd_set_powerCalibrationA0));
  m_cmdTable.insert("get powerCalibrationB0", new pmu(cmd_get_powerCalibrationB0));
  m_cmdTable.insert("set powerCalibrationB0", new pmu(cmd_set_powerCalibrationB0));
  m_cmdTable.insert("get powerCalibrationC0", new pmu(cmd_get_powerCalibrationC0));
  m_cmdTable.insert("set powerCalibrationC0", new pmu(cmd_set_powerCalibrationC0));
  m_cmdTable.insert("get powerCalibrationMA", new pmu(cmd_get_powerCalibrationMA));
  m_cmdTable.insert("set powerCalibrationMA", new pmu(cmd_set_powerCalibrationMA));
  m_cmdTable.insert("get powerCalibrationMB", new pmu(cmd_get_powerCalibrationMB));
  m_cmdTable.insert("set powerCalibrationMB", new pmu(cmd_set_powerCalibrationMB));
  m_cmdTable.insert("get powerCalibrationMC", new pmu(cmd_get_powerCalibrationMC));
  m_cmdTable.insert("set powerCalibrationMC", new pmu(cmd_set_powerCalibrationMC));
  m_cmdTable.insert("get powerCalibrationPOff", new pmu(cmd_get_powerCalibrationPOff));
  m_cmdTable.insert("set powerCalibrationPOff", new pmu(cmd_set_powerCalibrationPOff));
  m_cmdTable.insert("get powerCalibrationPOn", new pmu(cmd_get_powerCalibrationPOn));
  m_cmdTable.insert("set powerCalibrationPOn", new pmu(cmd_set_powerCalibrationPOn));
  m_cmdTable.insert("get powerCalibrationT0", new pmu(cmd_get_powerCalibrationT0));
  m_cmdTable.insert("set powerCalibrationT0", new pmu(cmd_set_powerCalibrationT0));
  m_cmdTable.insert("get powerEstimatorTemperatureOverride", new pmu(cmd_get_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("set powerEstimatorTemperatureOverride", new pmu(cmd_set_powerEstimatorTemperatureOverride));
  m_cmdTable.insert("get cachedTemperatureValue", new pmu(cmd_get_cachedTemperatureValue));
  m_cmdTable.insert("get eepromSize", new pmu(cmd_get_eepromSize));
  m_cmdTable.insert("get hardwareRevision", new pmu(cmd_get_hardwareRevision));
  m_cmdTable.insert("get wirelessPanId", new pmu(cmd_get_wirelessPanId));
  m_cmdTable.insert("set wirelessPanId", new pmu(cmd_set_wirelessPanId));
  m_cmdTable.insert("get wirelessChannelMask", new pmu(cmd_get_wirelessChannelMask));
  m_cmdTable.insert("set wirelessChannelMask", new pmu(cmd_set_wirelessChannelMask));
  m_cmdTable.insert("get wirelessShortAddress", new pmu(cmd_get_wirelessShortAddress));
  m_cmdTable.insert("set wirelessShortAddress", new pmu(cmd_set_wirelessShortAddress));
  m_cmdTable.insert("get wirelessRole", new pmu(cmd_get_wirelessRole));
  m_cmdTable.insert("set wirelessRole", new pmu(cmd_set_wirelessRole));
  m_cmdTable.insert("get wirelessWatchdogHold", new pmu(cmd_get_wirelessWatchdogHold));
  m_cmdTable.insert("set wirelessWatchdogHold", new pmu(cmd_set_wirelessWatchdogHold));
  m_cmdTable.insert("get wirelessWatchdogPeriod", new pmu(cmd_get_wirelessWatchdogPeriod));
  m_cmdTable.insert("set wirelessWatchdogPeriod", new pmu(cmd_set_wirelessWatchdogPeriod));
  m_cmdTable.insert("get firmwareCode", new pmu(cmd_get_firmwareCode));
  m_cmdTable.insert("get moduleFirmwareCode", new pmu(cmd_get_moduleFirmwareCode));
  m_cmdTable.insert("get maxTemperatureObserved", new pmu(cmd_get_maxTemperatureObserved));
  m_cmdTable.insert("get maxTemperatureObservedTime", new pmu(cmd_get_maxTemperatureObservedTime));
  m_cmdTable.insert("get overTemperatureThresholdLow", new pmu(cmd_get_overTemperatureThresholdLow));
  m_cmdTable.insert("set overTemperatureThresholdLow", new pmu(cmd_set_overTemperatureThresholdLow));
  m_cmdTable.insert("get overTemperatureThresholdHigh", new pmu(cmd_get_overTemperatureThresholdHigh));
  m_cmdTable.insert("set overTemperatureThresholdHigh", new pmu(cmd_get_overTemperatureThresholdHigh));
  m_cmdTable.insert("get overTemperatureDimmingLimit", new pmu(cmd_get_overTemperatureDimmingLimit));
  m_cmdTable.insert("set overTemperatureDimmingLimit", new pmu(cmd_set_overTemperatureDimmingLimit));
  m_cmdTable.insert("get analogDimmingMode", new pmu(cmd_get_analogDimmingMode));
  m_cmdTable.insert("set analogDimmingMode", new pmu(cmd_set_analogDimmingMode));
  m_cmdTable.insert("get fixtureIdMode", new pmu(cmd_get_fixtureIdMode));
  m_cmdTable.insert("set fixtureIdMode", new pmu(cmd_set_fixtureIdMode));
  m_cmdTable.insert("get acFrequency", new pmu(cmd_get_acFrequency));
  m_cmdTable.insert("get sensorBits", new pmu(cmd_get_sensorBits));
  m_cmdTable.insert("get powerMeterCommand", new pmu(cmd_get_powerMeterCommand));
  m_cmdTable.insert("set powerMeterCommand", new pmu(cmd_set_powerMeterCommond));
  m_cmdTable.insert("get powerMeterRegister", new pmu(cmd_get_powerMeterRegister));
  m_cmdTable.insert("set powerMeterRegister", new pmu(cmd_set_powerMetereRegister));
  m_cmdTable.insert("get ambientTemperature", new pmu(cmd_get_ambientTemperature));
  m_cmdTable.insert("get lightSensorLevel", new pmu(cmd_get_lightSensorLevel));
  m_cmdTable.insert("get sensor0Timeout", new pmu(cmd_get_sensor0Timeout));
  m_cmdTable.insert("set sensor0Timeout", new pmu(cmd_set_sensor0Timeout));
  m_cmdTable.insert("get sensor0Offset", new pmu(cmd_get_sensor0Offset));
  m_cmdTable.insert("set sensor0Offset", new pmu(cmd_set_sensor0Offset));
  m_cmdTable.insert("get sensor1Timeout", new pmu(cmd_get_sensor1Timeout));
  m_cmdTable.insert("set sensor1Timeout", new pmu(cmd_set_sensor1Timeout));
  m_cmdTable.insert("get sensor1Offset", new pmu(cmd_get_sensor1Offset));
  m_cmdTable.insert("set sensor1Offset", new pmu(cmd_set_sensor1Offset));
  m_cmdTable.insert("get analogDimmingLowValue", new pmu(cmd_get_analogDimmingLowValue));
  m_cmdTable.insert("set analogDimmingLowValue", new pmu(cmd_set_analogDimmingLowValue));
  m_cmdTable.insert("get analogDimmingHighValue", new pmu(cmd_get_analogDimmingHighValue));
  m_cmdTable.insert("set analogDimmingHighValue", new pmu(cmd_set_analogDimmingHighValue));
  m_cmdTable.insert("get analogDimmingOffValue", new pmu(cmd_get_analogDimmingOffValue));
  m_cmdTable.insert("set analogDimmingOffValue", new pmu(cmd_set_analogDimmingOffValue));
  m_cmdTable.insert("get powerMeasurementMode", new pmu(cmd_get_powerMeasurementMode));
  m_cmdTable.insert("set powerMeasurementMode", new pmu(cmd_set_powerMeasurementMode));
  m_cmdTable.insert("get externalPowerMeter", new pmu(cmd_get_externalPowerMeter));
  m_cmdTable.insert("set externalPowerMeter", new pmu(cmd_set_externalPowerMeter));
  m_cmdTable.insert("get ambientSensorValue", new pmu(cmd_get_ambientSensorValue));
  m_cmdTable.insert("get ambientActiveLevel", new pmu(cmd_get_ambientActiveLevel));
  m_cmdTable.insert("set ambientActiveLevel", new pmu(cmd_set_ambientActiveLevel));
  m_cmdTable.insert("get ambientInactiveLevel", new pmu(cmd_get_ambientInactiveLevel));
  m_cmdTable.insert("set ambientInactiveLevel", new pmu(cmd_set_ambientInactiveLevel));
  m_cmdTable.insert("get ambientEnvironmentalGain", new pmu(cmd_get_ambientEnvironmentalGain));
  m_cmdTable.insert("set ambientEnvironmentalGain", new pmu(cmd_set_ambientEnvironmentalGain));
  m_cmdTable.insert("get ambientOffHysteresis", new pmu(cmd_get_ambientOffHysteresis));
  m_cmdTable.insert("set ambientOffHysteresis", new pmu(cmd_set_ambientOffHysteresis));
  m_cmdTable.insert("get ambientOnHysteresis", new pmu(cmd_get_ambientOnHysteresis));
  m_cmdTable.insert("set ambientOnHysteresis", new pmu(cmd_set_ambientOnHystereis));
  m_cmdTable.insert("get powerboardProtocol", new pmu(cmd_get_powerboardProtocol));
  m_cmdTable.insert("get ledOverride", new pmu(cmd_get_ledOverride));
  m_cmdTable.insert("set ledOverride", new pmu(cmd_set_ledOverride));
  m_cmdTable.insert("get fadeUpStep", new pmu(cmd_get_fadeUpStep));
  m_cmdTable.insert("set fadeUpStep", new pmu(cmd_set_fadeUpStep));
  m_cmdTable.insert("get fadeDownStep", new pmu(cmd_get_fadeDownStep));
  m_cmdTable.insert("set fadeDownStep", new pmu(cmd_set_fadeDownStep));
  m_cmdTable.insert("get maxBrightness", new pmu(cmd_get_maxBrightness));
  m_cmdTable.insert("set maxBrightness", new pmu(cmd_set_maxBrightness));
  m_cmdTable.insert("get i2cResets", new pmu(cmd_get_i2cResets));
  m_cmdTable.insert("get sensorGuardTime", new pmu(cmd_get_sensorGuardTime));
  m_cmdTable.insert("set sensorGuardTime", new pmu(cmd_set_sensorGuardTime));
  m_cmdTable.insert("get inputVoltage", new pmu(cmd_get_inputVoltage));
  m_cmdTable.insert("get inputVoltageCalibration", new pmu(cmd_get_inputVoltageCalibration));
  m_cmdTable.insert("set inputVoltageCalibration", new pmu(cmd_set_inputVoltageCalibration));
  m_cmdTable.insert("get numberOfLightbars", new pmu(cmd_get_numberOfLightbars));
  m_cmdTable.insert("set numberOfLightbars", new pmu(cmd_set_numberOfLightbars));
  m_cmdTable.insert("get ambientDivisor", new pmu(cmd_get_ambientDivisor));
  m_cmdTable.insert("get currentLimit", new pmu(cmd_get_currentLimit));
  m_cmdTable.insert("set currentLimit", new pmu(cmd_set_currentLimit));
  m_cmdTable.insert("get bootloaderCode", new pmu(cmd_get_bootloaderCode));
  m_cmdTable.insert("get xpressMode", new pmu(cmd_get_xpressMode));
  m_cmdTable.insert("set xpressMode", new pmu(cmd_set_xpressMode));
  m_cmdTable.insert("get batteryBackupStatus", new pmu(cmd_get_batteryBackupStatus, parse_get_batteryBackupStatus));
  m_cmdTable.insert("set batteryBackupStatus", new pmu(cmd_set_batteryBackupStatus));
  m_cmdTable.insert("get sensorSeconds", new pmu(cmd_get_sensorSeconds));
  m_cmdTable.insert("get inputVoltageTwo", new pmu(cmd_get_inputVoltageTwo));
  m_cmdTable.insert("get inputVoltageTwoCalibration", new pmu(cmd_get_inputVoltageTwoCalibration));
  m_cmdTable.insert("set inputVoltageTwoCalibration", new pmu(cmd_set_inputVoltageTwoCalibration));
  m_cmdTable.insert("get maxRampUpSpeed", new pmu(cmd_get_maxRampUpSpeed));
  m_cmdTable.insert("set maxRampUpSpeed", new pmu(cmd_set_maxRampUpSpeed));
  m_cmdTable.insert("get maxRampDownSpeed", new pmu(cmd_get_maxRampDownSpeed));
  m_cmdTable.insert("set maxRampDownSpeed", new pmu(cmd_set_maxRampDownSpeed));
  m_cmdTable.insert("get wirelessNetworkKey", new pmu(cmd_get_wirelessNetworkKey));
  m_cmdTable.insert("set wirelessNetworkKey", new pmu(cmd_set_wirelessNetworkKey));
  m_cmdTable.insert("get emergencyLightLevel", new pmu(cmd_get_emergentLightLevel));
  m_cmdTable.insert("get batteryBackupPowerCalibration", new pmu(cmd_get_batteryBackupPowerCalibration));
  m_cmdTable.insert("set batteryBackupPowerCalibration", new pmu(cmd_set_batteryBackupPowerCalibration));
  m_cmdTable.insert("get motionSensorProfile", new pmu(cmd_get_motionSensorProfile));
  m_cmdTable.insert("set motionSensorProfile", new pmu(cmd_set_motionSensorProfile));
  m_cmdTable.insert("get powerMeterLevelAtOff", new pmu(cmd_get_powerMeterLevelAtOff));
  m_cmdTable.insert("set powerMeterLevelAtOff", new pmu(cmd_set_powerMeterLevelAtOff));
  m_cmdTable.insert("get powerMeterLevelAtMin", new pmu(cmd_get_powerMeterLevelAtMin));
  m_cmdTable.insert("set powerMeterLevelAtMin", new pmu(cmd_set_powerMeterLevelAtMin));
  m_cmdTable.insert("get powerMeterLevelAtMax", new pmu(cmd_get_powerMeterLevelAtMax));
  m_cmdTable.insert("set powerMeterLevelAtMax", new pmu(cmd_set_powerMeterLevelAtMax));
  m_cmdTable.insert("get powerMeterType", new pmu(cmd_get_powerMeterType));
  m_cmdTable.insert("set powerMeterType", new pmu(cmd_set_powerMeterType));
  m_cmdTable.insert("get DLAiSlaveMode", new pmu(cmd_get_DLAiSlaveMode));
  m_cmdTable.insert("set DLAiSlaveMode", new pmu(cmd_set_DLAiSlaveMode));
  m_cmdTable.insert("get DALIBootloadingActive", new pmu(cmd_get_DALIBootlodingActive));
  m_cmdTable.insert("get testingMode", new pmu(cmd_get_testingMode));
  m_cmdTable.insert("set testingMode", new pmu(cmd_set_testingMode));
  m_cmdTable.insert("get numberOfBatteriesSupported", new pmu(cmd_get_numberOfBatteriesSupported));
  m_cmdTable.insert("set numberOfBatteriesSupported", new pmu(cmd_set_numberOfBatteriesSupported));
  // get & set lightbar commands
  m_cmdTable.insert("get lbVersion", new pmu(cmd_get_lbVersion, parse_get_lbVersion));
  m_cmdTable.insert("get lbStatus", new pmu(cmd_get_lbStatus, parse_get_lbStatus));
  m_cmdTable.insert("get lbConfig", new pmu(cmd_get_lbConfig, parse_get_lbConfig));
  // get & set battery backup commands
  m_cmdTable.insert("get bbVersion", new pmu(cmd_get_bbVersion, parse_get_bbVersion));
  m_cmdTable.insert("get bbStatus", new pmu(cmd_get_bbStatus, parse_get_bbStatus));
  m_cmdTable.insert("get bbConfig", new pmu(cmd_get_bbConfig, parse_get_bbConfig));
  // reset commands
  m_cmdTable.insert("reset usage", new pmu(cmd_reset_usage));
  m_cmdTable.insert("reset oldLog", new pmu(cmd_reset_oldLog));
  m_cmdTable.insert("reset log", new pmu(cmd_reset_log));
  m_cmdTable.insert("reset logIndex", new pmu(cmd_reset_logIndex));
  m_cmdTable.insert("reset eeprom", new pmu(cmd_reset_eeprom));
  m_cmdTable.insert("reset eepromToDefault", new pmu(cmd_reset_eepromToDefault));
  m_cmdTable.insert("reset eepromToLatestMapVersion", new pmu(cmd_reset_eepromToLatestMapVersion));
  m_cmdTable.insert("reset network", new pmu(cmd_reset_network));
  m_cmdTable.insert("reset networkWithoutChecking", new pmu(cmd_reset_networkWithoutChecking));
  m_cmdTable.insert("reset daliCommissioning", new pmu(cmd_reset_daliCommissioning));
  m_cmdTable.insert("reset daliPowerMetering", new pmu(cmd_reset_daliPowerMetering));
  // reboot commands
  m_cmdTable.insert("reboot pmu", new pmu(cmd_reboot_pmu));
  m_cmdTable.insert("reboot wirelessCard", new pmu(cmd_reboot_wirelessCard));
  m_cmdTable.insert("reboot i2cDevices", new pmu(cmd_reboot_i2cDevices));
  // reload commands
  m_cmdTable.insert("reload dlaFirmware", new pmu(cmd_reload_dlaFirmware));
  m_cmdTable.insert("reload wirelessModuleFirmware", new pmu(cmd_reload_wirelessModuleFirmware));
  m_cmdTable.insert("reload powerboardFirmware", new pmu(cmd_reload_powerboardFirmware));
  m_cmdTable.insert("reload lightbarFirmware", new pmu(cmd_reload_lightbarFirmware));
  m_cmdTable.insert("reload batteryBackupFirmware", new pmu(cmd_reload_batteryBackupFirmware));
  m_cmdTable.insert("reload motionSensorFirmware", new pmu(cmd_reload_motionSensorFirmware));
  // log commands
  m_cmdTable.insert("get logIndex", new pmu(cmd_get_logIndex, parse_get_logIndex));
  m_cmdTable.insert("get log", new pmu(cmd_get_log, parse_get_log));
  m_cmdTable.insert("insert logEntry", new pmu(cmd_insert_logEntry));
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
