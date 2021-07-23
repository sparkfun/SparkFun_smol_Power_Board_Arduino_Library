/*!
 * @file SparkFun_smol_Power_Board_AAA.cpp
 * 
 * @mainpage SparkFun smôl Power Board AAA Arduino Library
 * 
 * @section intro_sec Introduction
 * 
 * This library facilitates communication with the smôl Power Board AAA.
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
*/
/**************************************************************************/
void smolPowerAAA::setI2CAddress(byte address)
{
  /** To change the address, we need to write two bytes to SFE_AAA_REGISTER_I2C_ADDRESS.
      The first is the new address. The second is a one byte CRC of the address. */
  byte bytesToSend[2];
  bytesToSend[0] = address;
  bytesToSend[1] = computeCRC8(bytesToSend, 1);
  smolPowerAAA_io.writeMultipleBytes(SFE_AAA_REGISTER_I2C_ADDRESS, bytesToSend, 2);
  //We can't return anything because: writeMultipleBytes returns void; and we can't
  //call getI2CAddress() unless we have called .begin first.
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
    @brief  Read the ATtiny43U's internal temperature.
            TO DO: Add temperature calibration / correction functionality.
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
            voltage using the selected voltage reference. The ADC has a built-in divide-by-2
            circuit, so we can measure up to 2*VCC or 2.2V depending on the reference.
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
  if (smolPowerAAA_io.readMultipleBytes(SFE_AAA_REGISTER_VCC_VOLTAGE, theBytes, 2))
  {
    uint16_t rawVolts = (((uint16_t)theBytes[1]) << 8) | theBytes[0]; // Little endian
    result = ((float)rawVolts) / 1023.0; // Convert 10-bit ADC result to the fraction of full range
    if (ref == SFE_AAA_USE_ADC_REF_VCC) // Are we using VCC as the reference?
    {
      float vcc = measureVCC(); // We need to measure VCC so we can scale the ADC reading correctly
      if (vcc > 0.0)
      {
        result = result * 2.0 * vcc; // Scale rawVolts to VCC
      }
      else
        return (-99.0); // measureVCC failed so return an error
    }
    else // We must be using the 1.1V reference
    {
      result = result * 2.0 * 1.1; // Scale rawVolts to 1.1V
    }
  }
  return (result);
}

/**************************************************************************/
/*!
    @brief  Measure the ATtiny43U's VCC by reading the 1.1V internal reference via the ADC.
            This allows us to work out what VCC is.
            We need to read two bytes (uint16_t, little endian) from SFE_AAA_REGISTER_VBAT.
            This will be the raw 10-bit ADC reading. We need to manually convert this to
            voltage. The ATtiny43U will automatically select VCC as the reference. There is
            no need to do it here.
    @return The battery voltage in Volts or -99.0 if an error occurred.
*/
/**************************************************************************/
float smolPowerAAA::measureVCC()
{
  float result = -99.0; // Return -99.0V if something bad happened.
  byte theBytes[2];
  if (smolPowerAAA_io.readMultipleBytes(SFE_AAA_REGISTER_VCC_VOLTAGE, theBytes, 2))
  {
    uint16_t rawVolts = (((uint16_t)theBytes[1]) << 8) | theBytes[0]; // Little endian
    float fractionFullRange = ((float)rawVolts) / 1023.0; // Convert 10-bit ADC result to the fraction of full range
    // We know that the ADC has measured the 1.1V internal reference using VCC as the full range.
    // Therefore we can calculate VCC from fractionFullRange.
    // fractionFullRange = 1.1V / VCC
    // VCC = 1.1 / fractionFullRange
    result = 1.1 / fractionFullRange;
  }
  return (result);
}

/**************************************************************************/
/*!
    @brief  Set the ATtiny43U's ADC voltage reference to VCC or the internal 1.1V reference.
    @param  ref
            The reference: SFE_AAA_USE_ADC_REF_VCC or SFE_AAA_USE_ADC_REF_1V1
    @return True if the reference is set successfuly, false if not.
*/
/**************************************************************************/
bool smolPowerAAA::setADCVoltageReference(sfe_power_board_aaa_ADC_ref_e ref)
{
  /** To change the voltage reference, we need to write two bytes to SFE_AAA_REGISTER_ADC_REFERENCE
      The first is the new address. The second is a one byte CRC of the address. */
  byte bytesToSend[2];
  bytesToSend[0] = (byte)ref;
  bytesToSend[1] = computeCRC8(bytesToSend, 1);
  smolPowerAAA_io.writeMultipleBytes(SFE_AAA_REGISTER_ADC_REFERENCE, bytesToSend, 2);
  return (getADCVoltageReference() == ref); //Check the reference was modified correctly by reading it back again
}

/**************************************************************************/
/*!
    @brief  Get the ATtiny43U's ADC voltage reference: VCC or the internal 1.1V reference.
    @return SFE_AAA_USE_ADC_REF_VCC or SFE_AAA_USE_ADC_REF_1V1 if the reference was read successfuly,
            SFE_AAA_USE_ADC_REF_UNDEFINED if not.
*/
/**************************************************************************/
sfe_power_board_aaa_ADC_ref_e smolPowerAAA::getADCVoltageReference()
{
  byte buffer;
  bool result = readSingleByte(SFE_AAA_REGISTER_ADC_REFERENCE, &buffer);
  if (!result)
    return (SFE_AAA_USE_ADC_REF_UNDEFINED);
  if ((sfe_power_board_aaa_ADC_ref_e)buffer == SFE_AAA_USE_ADC_REF_VCC)
    return (SFE_AAA_USE_ADC_REF_VCC);
  else if ((sfe_power_board_aaa_ADC_ref_e)buffer == SFE_AAA_USE_ADC_REF_1V1)
    return (SFE_AAA_USE_ADC_REF_1V1);
  else
    return (SFE_AAA_USE_ADC_REF_UNDEFINED);
}

/**************************************************************************/
/*!
    @brief  Set the ATtiny43U's Watchdog Timer prescaler to set the WDT interrupt rate.
    @param  prescaler
            The prescaler.
    @return True if the prescaler is set successfuly, false if not.
*/
/**************************************************************************/
bool smolPowerAAA::setWatchdogTimerPrescaler(sfe_power_board_aaa_WDT_prescale_e prescaler)
{
  /** To change the prescaler, we need to write two bytes to SFE_AAA_REGISTER_WDT_PRESCALER
      The first is the new prescaler. The second is a one byte CRC of the address. */
  byte bytesToSend[2];
  bytesToSend[0] = (byte)prescaler;
  bytesToSend[1] = computeCRC8(bytesToSend, 1);
  smolPowerAAA_io.writeMultipleBytes(SFE_AAA_REGISTER_WDT_PRESCALER, bytesToSend, 2);
  return (getWatchdogTimerPrescaler() == ref); //Check the prescaler was modified correctly by reading it back again
}


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
