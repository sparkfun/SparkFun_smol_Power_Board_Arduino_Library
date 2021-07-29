/*!
 * @file SparkFun_smol_Power_Board.h
 *
 * SparkFun smôl Power Board Arduino Library
 * 
 * This library facilitates communication with the smôl Power Boards over I<sup>2</sup>C.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * <br>SparkX smôl Power Board AAA (SPX-18360): https://www.sparkfun.com/products/18360
 * <br>SparkX smôl Power Board LiPo (SPX-18359): https://www.sparkfun.com/products/18359
 * <br>SparkX smôl ESP32 (SPX-18362): https://www.sparkfun.com/products/18362
 * 
 * This library was written by:
 * Paul Clark
 * SparkFun Electronics
 * July 23rd 2021
 * 
 * Please see LICENSE.md for the license information
 * 
 */

#ifndef __SFE_SMOL_POWER_BOARD__
#define __SFE_SMOL_POWER_BOARD__

#include <Arduino.h>

#include "SparkFun_smol_Power_Board_Constants.h"
#include "SparkFun_smol_Power_Board_IO.h"
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>

/** Communication interface for the SparkFun smôl Power Boards */
class sfeSmolPowerBoard
{
public:
  /** @brief Class to communicate with the SparkFun smôl Power Boards */
  bool isConnected();
  bool setI2CAddress(byte address);
  byte getI2CAddress();
  byte getResetReason();
  float getTemperature();
  float measureVCC();
  bool setADCVoltageReference(sfe_power_board_ADC_ref_e ref);
  sfe_power_board_ADC_ref_e getADCVoltageReference();
  bool setWatchdogTimerPrescaler(sfe_power_board_WDT_prescale_e prescaler);
  sfe_power_board_WDT_prescale_e getWatchdogTimerPrescaler();
  bool setPowerdownDurationWDTInts(uint16_t duration);
  bool getPowerDownDurationWDTInts(uint16_t *duration);
  bool powerDownNow();
  byte getFirmwareVersion();

  // I2C communication object instance
  SMOL_POWER_BOARD_IO smolPowerBoard_io;
  
  byte computeCRC8(byte data[], byte len);
};

/** Communication interface for the SparkFun smôl Power Board AAA */
class smolPowerAAA : public sfeSmolPowerBoard
{
public:
  /** @brief Create an object to communicate with the SparkFun smôl Power Board AAA */
  smolPowerAAA() {}

  bool begin(byte deviceAddress = SFE_SMOL_POWER_DEFAULT_I2C_ADDRESS, TwoWire &wirePort = Wire);
  float getBatteryVoltage(); // Measure the battery voltage via the ATtiny43U ADC

};

/** Communication interface for the SparkFun smôl Power Board LiPo */
class smolPowerLiPo : public sfeSmolPowerBoard
{
public:
  /** @brief Create an object to communicate with the SparkFun smôl Power Board LiPo */
  smolPowerLiPo() {}

  bool begin(byte deviceAddress = SFE_SMOL_POWER_DEFAULT_I2C_ADDRESS, TwoWire &wirePort = Wire);
  float getBatteryVoltage(); // Measure the battery voltage via the MAX17048 fuel gauge

private:
  // MAX17048 fuel gauge instance
  SFE_MAX1704X powerBoardFuelGauge = SFE_MAX1704X(MAX1704X_MAX17048);
};

#endif // /__SFE_SMOL_POWER_BOARD__
