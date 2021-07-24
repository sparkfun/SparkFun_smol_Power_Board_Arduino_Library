/*!
 * @file SparkFun_smol_Power_Board_AAA.h
 *
 * SparkFun smôl Power Board AAA Arduino Library
 * 
 * This library facilitates communication with the smôl Power Board AAA over I<sup>2</sup>C.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * SparkX smôl Power Board AAA (SPX-18360): https://www.sparkfun.com/products/18360
 * SparkX smôl ESP32 (SPX-18362): https://www.sparkfun.com/products/18362
 * 
 * This library was written by:
 * Paul Clark
 * July 23rd 2021
 * 
 * Please see LICENSE.md for the license information
 * 
 */

#ifndef __SFE_SMOL_POWER_BOARD_AAA__
#define __SFE_SMOL_POWER_BOARD_AAA__

#include <Arduino.h>

#include "SparkFun_smol_Power_Board_AAA_Constants.h"
#include "SparkFun_smol_Power_Board_AAA_IO.h"

/** Communication interface for the SparkFun smôl Power Board AAA */
class smolPowerAAA
{
public:
  /** @brief Create an object to communicate with the SparkFun smôl Power Board AAA */
  smolPowerAAA() {}

  bool begin(byte deviceAddress = SFE_AAA_DEFAULT_I2C_ADDRESS, TwoWire &wirePort = Wire);
  bool isConnected();
  bool setI2CAddress(byte address);
  byte getI2CAddress();
  byte getResetReason();
  float getTemperature();
  float getBatteryVoltage();
  float measureVCC();
  bool setADCVoltageReference(sfe_power_board_aaa_ADC_ref_e ref);
  sfe_power_board_aaa_ADC_ref_e getADCVoltageReference();
  bool setWatchdogTimerPrescaler(sfe_power_board_aaa_WDT_prescale_e prescaler);
  sfe_power_board_aaa_WDT_prescale_e getWatchdogTimerPrescaler();
  bool setPowerdownDurationWDTInts(uint16_t duration);
  bool getPowerDownDurationWDTInts(uint16_t *duration);
  bool powerDownNow();

private:
  // I2C communication object instance
  SMOL_POWER_AAA_IO smolPowerAAA_io;
  
  byte computeCRC8(byte data[], byte len);
};

#endif // /__SFE_SMOL_POWER_BOARD_AAA__
