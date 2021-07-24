/*!
 * @file SparkFun_smol_Power_Board_AAA_Constants.h
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

#ifndef __SFE_SMOL_POWER_BOARD_AAA_CONSTANTS__
#define __SFE_SMOL_POWER_BOARD_AAA_CONSTANTS__

//Platform specific configurations

#if !defined(I2C_BUFFER_LENGTH) && !defined(ARDUINO_ARCH_ESP32)

//The catch-all default is 32
#define I2C_BUFFER_LENGTH 32

#endif

#define SFE_AAA_DEFAULT_I2C_ADDRESS 0x50

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/** getResetReason will return these bits */
#define SFE_AAA_RESET_REASON_PORF_BIT       0                                           ///< Bit position of the Power-on Reset Flag
#define SFE_AAA_RESET_REASON_PORF           (1 << SFE_AAA_RESET_REASON_PORF_BIT)        ///< Flag to indicate if the MCUSR PORF Flag was set when the ATtiny43U came out of reset
#define SFE_AAA_RESET_REASON_EXTRF_BIT      1                                           ///< Bit position of the External Reset Flag
#define SFE_AAA_RESET_REASON_EXTRF          (1 << SFE_AAA_RESET_REASON_EXTRF_BIT)       ///< Flag to indicate if the MCUSR EXTRF Flag was set when the ATtiny43U came out of reset
#define SFE_AAA_RESET_REASON_BORF_BIT       2                                           ///< Bit position of the Brown-out Reset Flag
#define SFE_AAA_RESET_REASON_BORF           (1 << SFE_AAA_RESET_REASON_BORF_BIT)        ///< Flag to indicate if the MCUSR BORF Flag was set when the ATtiny43U came out of reset
#define SFE_AAA_RESET_REASON_WDRF_BIT       3                                           ///< Bit position of the Watchdog Reset Flag
#define SFE_AAA_RESET_REASON_WDRF           (1 << SFE_AAA_RESET_REASON_WDRF_BIT)        ///< Flag to indicate if the MCUSR WDRF Flag was set when the ATtiny43U came out of reset
#define SFE_AAA_EEPROM_CORRUPT_ON_RESET_BIT 4                                           ///< Bit position of the eeprom memory corrupt on reset flag
#define SFE_AAA_EEPROM_CORRUPT_ON_RESET     (1 << SFE_AAA_EEPROM_CORRUPT_ON_RESET_BIT)  ///< Flag to indicate if the ATtiny43U's eeprom memory was corrupt on reset and has been overwritten with the default settings
#define SFE_AAA_COMM_ERROR_BIT              7                                           ///< Bit position of the communication error flag
#define SFE_AAA_COMM_ERROR                  (1 << SFE_AAA_COMM_ERROR_BIT)               ///< Flag to indicate if a communication error occurred while fetching the reset reason

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/** The addresses of the registers within the ATtiny43U's memory */
typedef enum 
{
  SFE_AAA_REGISTER_I2C_ADDRESS = 0x00,
  SFE_AAA_REGISTER_RESET_REASON,
  SFE_AAA_REGISTER_TEMPERATURE,
  SFE_AAA_REGISTER_VBAT,
  SFE_AAA_REGISTER_VCC_VOLTAGE,
  SFE_AAA_REGISTER_ADC_REFERENCE,
  SFE_AAA_REGISTER_WDT_PRESCALER,
  SFE_AAA_REGISTER_POWERDOWN_DURATION,
  SFE_AAA_REGISTER_POWERDOWN_NOW
} sfe_power_board_aaa_registers_e;

/** Allow the user to select either VCC or the internal 1.1V reference as the reference for VBAT ADC measurements */
typedef enum 
{
  SFE_AAA_USE_ADC_REF_VCC = 0,  //Use the ATtiny43U's VCC rail as the ADC reference. Noisy, but allows a full scale of 2 * VCC.
  SFE_AAA_USE_ADC_REF_1V1,      //Use the ATtiny43U's built-in 1.1V ADC reference. Quiet, but limits the full scale to 2.2V.
  SFE_AAA_USE_ADC_REF_UNDEFINED //Something bad has happened...
} sfe_power_board_aaa_ADC_ref_e;

/** The Watchdog Timer Prescale select */
typedef enum 
{
  SFE_AAA_WDT_PRESCALE_2K = 0,    //2K cycles = 16ms timeout
  SFE_AAA_WDT_PRESCALE_4K,        //4K cycles = 32ms timeout
  SFE_AAA_WDT_PRESCALE_8K,        //8K cycles = 64ms timeout
  SFE_AAA_WDT_PRESCALE_16K,       //16K cycles = 0.125s timeout
  SFE_AAA_WDT_PRESCALE_32K,       //32K cycles = 0.25s timeout
  SFE_AAA_WDT_PRESCALE_64K,       //64K cycles = 0.5s timeout
  SFE_AAA_WDT_PRESCALE_128K,      //128K cycles = 1.0s timeout
  SFE_AAA_WDT_PRESCALE_256K,      //256K cycles = 2.0s timeout
  SFE_AAA_WDT_PRESCALE_512K,      //512K cycles = 4.0s timeout
  SFE_AAA_WDT_PRESCALE_1024K,     //1024K cycles = 8.0s timeout
  SFE_AAA_WDT_PRESCALE_UNDEFINED  //Something bad has happened...
} sfe_power_board_aaa_WDT_prescale_e;

#endif // /__SFE_SMOL_POWER_BOARD_AAA_CONSTANTS__
