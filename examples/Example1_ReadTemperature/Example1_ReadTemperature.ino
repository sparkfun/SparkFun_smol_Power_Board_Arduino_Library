/*!
 * @file Example1_ReadTemperature
 * 
 * @mainpage SparkFun smôl Power Board AAA Arduino Library
 * 
 * @section intro_sec Examples
 * 
 * This example shows how to read the temperature from the ATtiny43U on the smôl Power Board AAA.
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

  float temperature = myPowerBoard.getTemperature(); // Get the temperature

  Serial.print(F("The temperature in Degrees C is: "));
  Serial.println(temperature);

  if (temperature == -99.0)
  {
    Serial.println(F("-99.0C indicates an error. Please check the connections and I2C address."));
  }
}

void loop()
{
  //Nothing to do here
}
