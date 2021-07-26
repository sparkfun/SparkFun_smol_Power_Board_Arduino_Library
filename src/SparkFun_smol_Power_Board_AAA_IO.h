/*!
 * @file SparkFun_smol_Power_Board_AAA_IO.h
 *
 * SparkFun smôl Power Board AAA Arduino Library
 * 
 * This library facilitates communication with the smôl Power Board AAA over I<sup>2</sup>C.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * <br>SparkX smôl Power Board AAA (SPX-18360): https://www.sparkfun.com/products/18360
 * <br>SparkX smôl ESP32 (SPX-18362): https://www.sparkfun.com/products/18362
 * 
 * This library was written by:
 * Paul Clark
 * July 23rd 2021
 * 
 * Please see LICENSE.md for the license information
 * 
 */

#ifndef __SFE_SMOL_POWER_BOARD_AAA_IO__
#define __SFE_SMOL_POWER_BOARD_AAA_IO__

#include <Arduino.h>
#include <Wire.h> // Needed for I2C communication

#include "SparkFun_smol_Power_Board_AAA_Constants.h"

/** Communication interface for the SparkFun smôl Power Board AAA */
class SMOL_POWER_AAA_IO
{
private:
  TwoWire* _i2cPort;
  byte _address;

public:
  /** @brief Create an object to communicate with the SparkFun smôl Power Board AAA over I2C. */
  SMOL_POWER_AAA_IO() {}

  /** Starts two wire interface. */
  bool begin(byte address, TwoWire& wirePort);

  /** Returns true if we get a reply from the I2C device. */
  bool isConnected();

  /** Read a single byte from a register. */
  bool readSingleByte(byte registerAddress, byte* buffer, byte waitMS = 0);

  /** Writes a single byte into a register. */
  bool writeSingleByte(byte registerAddress, byte value);

  /** Reads multiple bytes from a register into buffer byte array. */
  bool readMultipleBytes(byte registerAddress, byte* buffer, byte packetLength, byte waitMS = 0);

  /** Writes multiple bytes to register from buffer byte array. */
  bool writeMultipleBytes(byte registerAddress, const byte* buffer, byte packetLength);
};

#endif // /__SFE_SMOL_POWER_BOARD_AAA_IO__