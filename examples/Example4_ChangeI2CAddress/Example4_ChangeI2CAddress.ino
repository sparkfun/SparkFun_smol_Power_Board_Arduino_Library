/*!
 * @file Example4_ChangeI2CAddress.ino
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

  if (myPowerBoard.begin(0x50) == false) // Begin communication with the power board using the default I2C address (0x50) and the Wire port
  {
    Serial.println(F("Could not communicate with the power board on 0x50. Please check the I2C connections. Freezing..."));
    while (1)
      ;
  }

  Serial.println(F("Changing the I2C address to 0x51"));

  myPowerBoard.setI2CAddress(0x51);

  // -----
  
  if (myPowerBoard.begin(0x51) == false) // Re-start communication with the power board using I2C address 0x51 and the Wire port
  {
    Serial.println(F("Could not communicate with the power board on 0x51. Please check the I2C connections. Freezing..."));
    while (1)
      ;
  }

  Serial.println(F("Changing the I2C address back to 0x50"));

  myPowerBoard.setI2CAddress(0x50);

  // -----
  
  if (myPowerBoard.begin(0x50) == false) // Re-start communication with the power board using I2C address 0x50 and the Wire port
  {
    Serial.println(F("Could not communicate with the power board on 0x50. Please check the I2C connections. Freezing..."));
    while (1)
      ;
  }

  Serial.println(F("All done!"));
}

void loop()
{
  // Nothing to do here
}
