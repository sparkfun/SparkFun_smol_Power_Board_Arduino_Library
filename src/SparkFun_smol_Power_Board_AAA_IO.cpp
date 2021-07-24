/*!
 * @file SparkFun_smol_Power_Board_AAA_IO.cpp
 * 
 * @mainpage SparkFun smôl Power Board AAA Arduino Library
 * 
 * @section intro_sec Introduction
 * 
 * This library facilitates communication with the smôl Power Board AAA over I<sup>2</sup>C.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * SparkX smôl Power Board AAA (SPX-18360): https://www.sparkfun.com/products/18360
 * SparkX smôl ESP32 (SPX-18362): https://www.sparkfun.com/products/18362
 * 
 * @section author Author
 * 
 * This library was written by:
 * Paul Clark
 * July 23rd 2021
 * 
 * @section license License
 * 
 * MIT: please see LICENSE.md for the full license information
 * 
 */

#include "SparkFun_smol_Power_Board_AAA_IO.h"

/**************************************************************************/
/*!
    @brief  Begin communication with the SparkFun smôl Power Board AAA over I2C
    @param  deviceAddress
            The I2C address of the Power Board.
            Default is SFE_AAA_DEFAULT_I2C_ADDRESS 0x50.
            Can be changed with setI2CAddress.
    @param  wirePort
            The TwoWire (I2C) port used to communicate with the Power Board.
            Default is Wire.
    @return True if communication with the Power Board was successful, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::begin(byte address, TwoWire& wirePort)
{
  _i2cPort = &wirePort;
  _address = address;
  return isConnected();
}

/**************************************************************************/
/*!
    @brief  Reads the smôl Power Board AAA's I2C address and confirms it matches _address.
            In theory, it should be impossible for _address and the smôl Power Board AAA's I2C address to not be the same,
            but you never know...
    @return True if communication with the Power Board was successful, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::isConnected()
{
  _i2cPort->beginTransmission(_address);
  if (_i2cPort->endTransmission() == 0)
  {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(SFE_AAA_REGISTER_I2C_ADDRESS);
    _i2cPort->endTransmission(false); //Don't release bus
    byte bytesReturned = _i2cPort->requestFrom(_address, (byte)1);
    if (bytesReturned != 1)
        return (false);
    byte incomingByte = _i2cPort->read();
    if (incomingByte == _address)
      return (true);
  }
  return (false);
}

/**************************************************************************/
/*!
    @brief  Write multiple bytes to the SparkFun smôl Power Board AAA over I2C
    @param  registerAddress
            The (software) register address being written to.
    @param  buffer
            A pointer to the byte array to be written.
    @param  packetLength
            The number of bytes to be written.
    @return True if communication with the Power Board was successful, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::writeMultipleBytes(byte registerAddress, const byte* buffer, byte const packetLength)
{
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(registerAddress);

  for (byte i = 0; i < packetLength; i++)
    _i2cPort->write(buffer[i]);

  return (_i2cPort->endTransmission() == 0);
}

/**************************************************************************/
/*!
    @brief  Read multiple bytes from the SparkFun smôl Power Board AAA over I2C.
            First the register address is written, then the data bytes are read.
    @param  registerAddress
            The (software) register address being read from.
    @param  buffer
            A pointer to the byte array which will hold the read data.
    @param  packetLength
            The number of bytes to be read.
    @return True if the data was read successfully, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::readMultipleBytes(byte registerAddress, byte* buffer, byte const packetLength)
{
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(registerAddress);
  _i2cPort->endTransmission(false); //Don't release bus

  byte bytesReturned = _i2cPort->requestFrom(_address, packetLength);

  byte i;
  for (i = 0; (i < bytesReturned) && (i < packetLength) && _i2cPort->available(); i++)
    buffer[i] = _i2cPort->read();

  return ((bytesReturned == packetLength) && (i == packetLength));
}

/**************************************************************************/
/*!
    @brief  Read a single byte from the SparkFun smôl Power Board AAA over I2C.
            First the register address is written, then the data byte is read.
    @param  registerAddress
            The (software) register address being read from.
    @param  buffer
            A pointer to the byte which will hold the read data.
    @return True if the data byte was read successfully, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::readSingleByte(byte registerAddress, byte* buffer)
{
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(registerAddress);
  _i2cPort->endTransmission(false); //Don't release bus

  byte bytesReturned = _i2cPort->requestFrom(_address, (byte)1);

  *buffer = _i2cPort->read();

  return (bytesReturned == 1);
}

/**************************************************************************/
/*!
    @brief  Write a single byte to the SparkFun smôl Power Board AAA over I2C
    @param  registerAddress
            The (software) register address being written to.
    @param  value
            The data byte to be written.
    @return True if communication with the Power Board was successful, otherwise false.
*/
/**************************************************************************/
bool SMOL_POWER_AAA_IO::writeSingleByte(byte registerAddress, byte const value)
{
  _i2cPort->beginTransmission(_address);
  _i2cPort->write(registerAddress);
  _i2cPort->write(value);
  return (_i2cPort->endTransmission() == 0);
}