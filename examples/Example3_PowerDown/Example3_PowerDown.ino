/*!
 * @file Example3_PowerDown
 * 
 * @mainpage SparkFun smôl Power Board AAA Arduino Library
 * 
 * @section intro_sec Examples
 * 
 * This example shows how to power-down the smôl Power Board AAA.
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

#include <SparkFun_smol_Power_Board_AAA.h> //Click here to get the library:  http://librarymanager/All#SparkFun_smol_Power_Board_AAA

smolPowerAAA myPowerBoard;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for the user to open the Serial console
  Serial.println(F("smôl Power Board AAA example"));
  Serial.println();

  if (myPowerBoard.begin() == false) // Begin communication with the power board using the default I2C address (0x50) and the Wire port
  {
    Serial.println(F("Could not communicate with the power board. Please check the I2C connections. Freezing..."));
  }

  Serial.println(F("To set the power-down duration, we need to do two things:"));
  Serial.println(F(" Set the ATtiny43U's Watchdog Timer prescaler to set a useful interrupt rate"));
  Serial.println(F(" Set the power-down duration in Watchdog Timer interrupts"));
  Serial.println();
  Serial.println(F("To power down for 30 seconds, we can:"));
  Serial.println(F(" Set the prescaler to 256K cycles = 2 seconds"));
  Serial.println(F(" Set the power-down duration to 15"));
  Serial.println();

  myPowerBoard.setWatchdogTimerPrescaler(SFE_AAA_WDT_PRESCALE_256K); // Set the WDT prescaler to 256K cycles = 2.0 seconds

  myPowerBoard.setPowerdownDurationWDTInts(15); // Set the sleep durtion to 15 WDT interrupts

  Serial.println(F("Power down now..."));
  Serial.println();
  Serial.flush();
  
  myPowerBoard.powerDownNow();
}

void loop()
{
  delay(1);
  Serial.print(F(".")); // The number of periods will give an indication of how quickly the board powered down
}
