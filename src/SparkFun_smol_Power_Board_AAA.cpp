/*!
 * @file SparkFun_smol_Power_Board_AAA.cpp
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

#include "SparkFun_smol_Power_Board_AAA.h"

/**************************************************************************/
/*!
    @brief  Begin communication with the SparkFun smôl Power Board AAA
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
bool smolPowerAAA::begin(byte deviceAddress, TwoWire &wirePort)
{
  return (smolPowerAAA_io.begin(deviceAddress, wirePort));
}

/**************************************************************************/
/*!
    @brief  Check that the SparkFun smôl Power Board AAA is awake and communicating.
            This function is called by smolPowerAAA::begin
    @return True if communication with the Power Board was successful, otherwise false.
*/
/**************************************************************************/
bool smolPowerAAA::isConnected()
{
  return (smolPowerAAA_io.isConnected());
}

/**************************************************************************/
/*!
    @brief  Change the smôl Power Board AAA's I2C address.
            You will need to call .begin again using the new address to
            re-establish communication with the Power Board.
    @param  address
            The new I2C address.
    @return True if the I2C address change was requested successfully, otherwise false.
*/
/**************************************************************************/
bool smolPowerAAA::setI2CAddress(byte address)
{
  /** To change the address, we need to write two bytes to SFE_AAA_REGISTER_I2C_ADDRESS
      The first is the new address. The second is a one byte CRC of the address. */
  byte bytesToSend[2];
  bytesToSend[0] = address;
  bytesToSend[1] = computeCRC8(bytesToSend, 1);
  return (smolPowerAAA_io.writeMultipleBytes(SFE_AAA_REGISTER_I2C_ADDRESS, bytesToSend, 2));
}

/**************************************************************************/
/*!
    @brief  Just for fun, read back the Power Board's I2C address.
            This proves that I2C communication is possible and that the Power Board
            has the correct address stored in eeprom.
    @return The Power Board's I2C address.
*/
/**************************************************************************/
byte smolPowerAAA::getI2CAddress()
{
  return (smolPowerAAA_io.readSingleByte(SFE_AAA_REGISTER_I2C_ADDRESS);
}

/**************************************************************************/
/*!
    @brief  Read the reason for the ATtiny43U's most recent reset.
            The reset reason is updated as soon as the code starts.
            The four MCU STatus Register Flags are read.
            If the ATtiny43U found that its eeprom was corrupt when the code started,
            SFE_AAA_EEPROM_CORRUPT_ON_RESET will be set indicating that the eeprom
            settings have been reset to the default values.
    @return The reset reason: the logical OR of:
            SFE_AAA_RESET_REASON_PORF
            SFE_AAA_RESET_REASON_EXTRF
            SFE_AAA_RESET_REASON_BORF
            SFE_AAA_RESET_REASON_WDRF
            SFE_AAA_EEPROM_CORRUPT_ON_RESET
*/
/**************************************************************************/
byte smolPowerAAA::getResetReason()
{
  return (smolPowerAAA_io.readSingleByte(SFE_AAA_REGISTER_RESET_REASON);
}

/**************************************************************************/
/*!
    @brief  Read the ATtiny43U's internal temperature
    @return The temperature in Degrees Centigrade / Celcius or -273.15 if an error occured.
*/
/**************************************************************************/
float smolPowerAAA::getTemperature()
{
  /** To read the approximate temperature, we need to read two bytes (uint16_t, little endian)
      from SFE_AAA_REGISTER_TEMPERATURE. These will be the raw ADC reading which we
      need to convert to Degrees C. The ATtiny43U will automatically use the 1.1V
      internal reference for the conversion. There is no need to select it here.
      The sensitivity is approximately 1 LSB/°C with 25°C reading as 300 ADU. */
  byte theBytes[2];
  float result = -273.15; // Return -273.15 if readMultipleBytes fails
  if (smolPowerAAA_io.readMultipleBytes(SFE_AAA_REGISTER_TEMPERATURE, theBytes, 2))
  {
    uint16_t rawTemp = (((uint16_t)theBytes[1]) << 8) | theBytes[0]; // Little endian
    result = 25.0 + ((float)rawTemp) - 300.0; // Convert to °C
  }
  return (result);
}

/**************************************************************************/
/*!
    @brief  Read the ATtiny43U's battery voltage (VBAT).
            We need to read two bytes (uint16_t, little endian) from SFE_AAA_REGISTER_VBAT.
            This will be the raw 10-bit ADC reading. We need to manually convert this to
            voltage using the selected voltage reference.
    @return The battery voltage in Volts or -99.0 if an error occurred.
*/
/**************************************************************************/
float smolPowerAAA::getBatteryVoltage()
{
  float result = -99.0; // Return -99.0V if something bad happened.
  sfe_power_board_aaa_ADC_ref_e ref = getBatteryVoltageReference(); // Read which voltage reference is being used
  if (ref == SFE_AAA_USE_ADC_REF_UNDEFINED)
    return (result); // Return now if getBatteryVoltageReference failed
  byte theBytes[2];
  if (smolPowerAAA_io.readMultipleBytes(SFE_AAA_REGISTER_TEMPERATURE, theBytes, 2))
  {
    uint16_t rawVolts = (((uint16_t)theBytes[1]) << 8) | theBytes[0]; // Little endian
    result = (float)rawVolts) / 1023.0; // Convert 10-bit ADC result to float
    if (ref == SFE_AAA_USE_ADC_REF_VCC)
  }
  return (result);
}

/**************************************************************************/
/*!
    @brief  Read the ATtiny43U's 1.1V internal reference. This allows us to work out what VCC is.
            We need to read two bytes (uint16_t, little endian) from SFE_AAA_REGISTER_VBAT.
            This will be the raw 10-bit ADC reading. We need to manually convert this to
            voltage using the selected voltage reference.
    @return The battery voltage in Volts or -99.0 if an error occurred.
*/
/**************************************************************************/
float smolPowerAAA::getReferenceVoltage()
{

}

  bool smolPowerAAA::setBatteryVoltageReference(sfe_power_board_aaa_ADC_ref_e ref);
  sfe_power_board_aaa_ADC_ref_e smolPowerAAA::getBatteryVoltageReference();
  bool smolPowerAAA::setWatchdogTimerPrescaler(byte prescaler);
  byte smolPowerAAA::getWatchdogTimerPrescaler();
  bool smolPowerAAA::setPowerdownDurationWDTInts(uin16_t duration);
  bool smolPowerAAA::getPowerDownDurationWDTInts(uin16_t *duration);
  void smolPowerAAA::powerDownNow();

/**************************************************************************/
/*!
    @brief  Given an array of bytes, this calculates the CRC8 for those bytes.
            From: http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
            Tested with: http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
            x^8+x^5+x^4+1 = 0x31
    @param  data
            The array of bytes
    @param  len
            The length of the array of bytes
    @return The CRC
*/
/**************************************************************************/
byte smolPowerAAA::computeCRC8(byte data[], byte len)
{
  byte crc = 0xFF; //Init with 0xFF

  for (byte x = 0; x < len; x++)
  {
    crc ^= data[x]; // XOR-in the next input byte

    for (byte i = 0; i < 8; i++)
    {
      if ((crc & 0x80) != 0)
        crc = (byte)((crc << 1) ^ 0x31);
      else
        crc <<= 1;
    }
  }

  return crc; //No output reflection
}
