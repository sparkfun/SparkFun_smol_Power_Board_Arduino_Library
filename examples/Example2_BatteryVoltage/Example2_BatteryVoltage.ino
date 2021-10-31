/*!
 * @file Example2_BatteryVoltage.ino
 * 
 * @mainpage SparkFun smôl Power Board Arduino Library
 * 
 * @section intro_sec Examples
 * 
 * This example shows how to read the battery voltage from the smôl Power Board.
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
 * July 23rd 2021
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

  Serial.println(F("The ATtiny43U on the smôl Power Board AAA can read the battery voltage via the ADC."));
  Serial.println(F("Setting the ADC Voltage Reference to VCC allows the ADC to read voltages of up to 2 * VCC"));
  Serial.println(F("thanks to the ATtiny's internal resistor divider circuit. However the reading may be noisy"));
  Serial.println(F("if the on-board boost regulator is running. We can get a less noisy reading by selecting"));
  Serial.println(F("the internal 1.1V reference instead, but then we are limited to a maximum battery voltage"));
  Serial.println(F("of 2.2V. If you are using a single AA or AAA battery to power the board, the 1.1V reference"));
  Serial.println(F("will give best results. For 2 * AA/AAA you will need to use VCC as the reference."));
  Serial.println();

  Serial.println(F("On the smôl Power Board LiPo, the battery voltage is read from the MAX17048 fuel gauge."));
  Serial.println(F("setADCVoltageReference has no effect on the Power Board LiPo."));
  Serial.println();

  Serial.println(F("Selecting the 1.1V reference."));

  myPowerBoard.setADCVoltageReference(SFE_SMOL_POWER_USE_ADC_REF_1V1); // Select the 1.1V internal voltage reference

  float voltage = myPowerBoard.getBatteryVoltage();
  Serial.print(F("The battery voltage reads as: "));
  Serial.println(voltage);

  if (voltage == -99.0)
  {
    Serial.println(F("A voltage of -99.0V indicates an error."));
  }

  Serial.println();
  
  Serial.println(F("Selecting VCC as the reference."));

  myPowerBoard.setADCVoltageReference(SFE_SMOL_POWER_USE_ADC_REF_VCC); // Select VCC as the voltage reference

  voltage = myPowerBoard.getBatteryVoltage();
  Serial.print(F("The battery voltage reads as: "));
  Serial.println(voltage);

  if (voltage == -99.0)
  {
    Serial.println(F("A voltage of -99.0V indicates an error."));
  }

}

void loop()
{
  //Nothing to do here
}
