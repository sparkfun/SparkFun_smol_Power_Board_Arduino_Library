/*!
 * @file Example3_PowerDown.ino
 * 
 * @mainpage SparkFun smôl Power Board AAA Arduino Library
 * 
 * @section intro_sec Examples
 * 
 * This example shows how to power-down the smôl Power Board AAA and check it wakes up correctly.
 * Don't run this code on a smôl processor as the smôl power will be disabled during power-down.
 * Instead, run it on a Qwiic-compatible board attached to the smôl header.
 * Be aware that any I2C pull-ups will back-feed the smôl 3.3V rail.
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

#include <Wire.h>

#include <SparkFun_smol_Power_Board_AAA.h> //Click here to get the library:  http://librarymanager/All#SparkFun_smol_Power_Board_AAA

smolPowerAAA myPowerBoard;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for the user to open the Serial console
  Serial.println(F("smôl Power Board AAA example"));
  Serial.println();

  Wire.begin();

  if (myPowerBoard.begin() == false) // Begin communication with the power board using the default I2C address (0x50) and the Wire port
  {
    Serial.println(F("Could not communicate with the power board. Please check the I2C connections. Freezing..."));
    while (1)
      ;
  }

  myPowerBoard.setADCVoltageReference(SFE_AAA_USE_ADC_REF_VCC); // Select VCC as the voltage reference

  Serial.println(F("To set the power-down duration, we need to do two things:"));
  Serial.println(F(" Set the ATtiny43U's Watchdog Timer prescaler to set a useful interrupt rate"));
  Serial.println(F(" Set the power-down duration in Watchdog Timer interrupts"));
  Serial.println();
  Serial.println(F("To power down for 10 seconds, we can:"));
  Serial.println(F(" Set the prescaler to 256K cycles = 2 seconds"));
  Serial.println(F(" Set the power-down duration to 5"));
  Serial.println();

  myPowerBoard.setWatchdogTimerPrescaler(SFE_AAA_WDT_PRESCALE_256K); // Set the WDT prescaler to 256K cycles = 2.0 seconds

  myPowerBoard.setPowerdownDurationWDTInts(5); // Set the sleep durtion to 5 WDT interrupts
}

void loop()
{
  Serial.println();
  Serial.println(F("Power down now..."));
  Serial.println();
  
  myPowerBoard.powerDownNow();

  delay(15000); // Wait 15 seconds before attempting to communicate with the smôl Power Board AAA
  
  float voltage = myPowerBoard.getBatteryVoltage();
  Serial.print(F("Using VCC as the voltage reference, the battery voltage reads as: "));
  Serial.println(voltage);

  if (voltage == -99.0)
  {
    Serial.println(F("A voltage of -99.0V indicates an error."));
  }
}
