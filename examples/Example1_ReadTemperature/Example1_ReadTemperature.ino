/*!
 * @file Example1_ReadTemperature.ino
 * 
 * @mainpage SparkFun smôl Power Board Arduino Library
 * 
 * @section intro_sec Examples
 * 
 * This example shows how to read the firmware version and the temperature from the smôl Power Board.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * SparkX smôl Power Board LiPo (SPX-18622): https://www.sparkfun.com/products/18622
 * SparkX smôl Power Board AAA (SPX-18621): https://www.sparkfun.com/products/18621
 * SparkX smôl ESP32 (SPX-18619): https://www.sparkfun.com/products/18619
 * 
 * @section author Author
 * 
 * This library was written by:
 * Paul Clark
 * SparkFun Electronics
 * July 29th 2021
 * 
 * @section license License
 * 
 * MIT: please see LICENSE.md for the full license information
 * 
 */

#include <Wire.h>

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library
#include <SparkFun_smol_Power_Board.h> //Click here to get the library:  http://librarymanager/All#SparkFun_smol_Power_Board

smolPowerAAA myPowerBoard; // Uncomment this line if you are using the smôl Power Board AAA
//smolPowerLiPo myPowerBoard; // Uncomment this line if you are using the smôl Power Board LiPo

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for the user to open the Serial console
  Serial.println(F("smôl Power Board example"));
  Serial.println();

  Wire.begin();

  if (myPowerBoard.begin() == false) // Begin communication with the power board using the default I2C address (0x50) and the Wire port
  {
    Serial.println(F("Could not communicate with the power board. Please check the I2C connections. Freezing..."));
    while (1)
      ;
  }

  byte firmwareVersion = myPowerBoard.getFirmwareVersion(); // Get the firmware version

  Serial.print(F("The firmware version is v"));
  Serial.print(firmwareVersion >> 4); // Major version is in the MS nibble
  Serial.print(F("."));
  Serial.println(firmwareVersion & 0x0F); // Minor version is in the LS nibble

  float temperature = myPowerBoard.getTemperature(); // Get the temperature

  Serial.print(F("The temperature in Degrees C is: "));
  Serial.println(temperature, 0);

  if (temperature == -99.0)
  {
    Serial.println(F("-99.0C indicates an error. Please check the connections and I2C address."));
  }
}

void loop()
{
  //Nothing to do here
}
