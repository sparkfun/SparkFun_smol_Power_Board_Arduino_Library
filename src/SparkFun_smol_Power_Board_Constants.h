/*!
 * @file SparkFun_smol_Power_Board_Constants.h
 *
 * SparkFun smôl Power Board Arduino Library
 * 
 * This library facilitates communication with the smôl Power Board over I<sup>2</sup>C.
 * 
 * Want to support open source hardware? Buy a board from SparkFun!
 * <br>SparkX smôl Power Board LiPo (SPX-18622): https://www.sparkfun.com/products/18622
 * <br>SparkX smôl Power Board AAA (SPX-18621): https://www.sparkfun.com/products/18621
 * <br>SparkX smôl ESP32 (SPX-18619): https://www.sparkfun.com/products/18619
 * 
 * This library was written by:
 * Paul Clark
 * SparkFun Electronics
 * July 23rd 2021
 * 
 * Please see LICENSE.md for the license information
 * 
 */

#ifndef __SFE_SMOL_POWER_BOARD_CONSTANTS__
#define __SFE_SMOL_POWER_BOARD_CONSTANTS__

//Platform specific configurations

#if !defined(I2C_BUFFER_LENGTH) && !defined(ARDUINO_ARCH_ESP32)

//The catch-all default is 32
#define I2C_BUFFER_LENGTH 32

#endif

#define SFE_SMOL_POWER_DEFAULT_I2C_ADDRESS 0x50

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/** delay durations for the ADC read and eeprom update */
#define SFE_SMOL_POWER_ADC_READ_DELAY              15 ///< The ADC read (eight samples, averaged) takes ~11ms to complete at 4MHz. 15ms provides margin.
#define SFE_SMOL_POWER_EEPROM_UPDATE_DELAY         6  ///< The eeprom update takes ~4ms to complete at 4MHz. 6ms provides margin.


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/** getResetReason will return these bits */
#define SFE_SMOL_POWER_RESET_REASON_PORF_BIT       0                                                  ///< Bit position of the Power-on Reset Flag
#define SFE_SMOL_POWER_RESET_REASON_PORF           (1 << SFE_SMOL_POWER_RESET_REASON_PORF_BIT)        ///< Flag to indicate if the MCUSR PORF Flag was set when the ATtiny43U came out of reset
#define SFE_SMOL_POWER_RESET_REASON_EXTRF_BIT      1                                                  ///< Bit position of the External Reset Flag
#define SFE_SMOL_POWER_RESET_REASON_EXTRF          (1 << SFE_SMOL_POWER_RESET_REASON_EXTRF_BIT)       ///< Flag to indicate if the MCUSR EXTRF Flag was set when the ATtiny43U came out of reset
#define SFE_SMOL_POWER_RESET_REASON_BORF_BIT       2                                                  ///< Bit position of the Brown-out Reset Flag
#define SFE_SMOL_POWER_RESET_REASON_BORF           (1 << SFE_SMOL_POWER_RESET_REASON_BORF_BIT)        ///< Flag to indicate if the MCUSR BORF Flag was set when the ATtiny43U came out of reset
#define SFE_SMOL_POWER_RESET_REASON_WDRF_BIT       3                                                  ///< Bit position of the Watchdog Reset Flag
#define SFE_SMOL_POWER_RESET_REASON_WDRF           (1 << SFE_SMOL_POWER_RESET_REASON_WDRF_BIT)        ///< Flag to indicate if the MCUSR WDRF Flag was set when the ATtiny43U came out of reset
#define SFE_SMOL_POWER_EEPROM_CORRUPT_ON_RESET_BIT 4                                                  ///< Bit position of the eeprom memory corrupt on reset flag
#define SFE_SMOL_POWER_EEPROM_CORRUPT_ON_RESET     (1 << SFE_SMOL_POWER_EEPROM_CORRUPT_ON_RESET_BIT)  ///< Flag to indicate if the ATtiny43U's eeprom memory was corrupt on reset and has been overwritten with the default settings
#define SFE_SMOL_POWER_COMM_ERROR_BIT              7                                                  ///< Bit position of the communication error flag
#define SFE_SMOL_POWER_COMM_ERROR                  (1 << SFE_SMOL_POWER_COMM_ERROR_BIT)               ///< Flag to indicate if a communication error occurred while fetching the reset reason

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/** The addresses of the registers within the ATtiny43U's memory */
typedef enum 
{
  SFE_SMOL_POWER_REGISTER_I2C_ADDRESS = 0x00,
  SFE_SMOL_POWER_REGISTER_RESET_REASON,
  SFE_SMOL_POWER_REGISTER_TEMPERATURE,
  SFE_SMOL_POWER_REGISTER_VBAT,
  SFE_SMOL_POWER_REGISTER_1V1,
  SFE_SMOL_POWER_REGISTER_ADC_REFERENCE,
  SFE_SMOL_POWER_REGISTER_WDT_PRESCALER,
  SFE_SMOL_POWER_REGISTER_POWERDOWN_DURATION,
  SFE_SMOL_POWER_REGISTER_POWERDOWN_NOW,
  SFE_SMOL_POWER_REGISTER_FIRMWARE_VERSION
} sfe_power_board_registers_e;

/** Allow the user to select either VCC or the internal 1.1V reference as the reference for VBAT ADC measurements */
typedef enum 
{
  SFE_SMOL_POWER_USE_ADC_REF_VCC = 0,  //Use the ATtiny43U's VCC rail as the ADC reference. Noisy, but allows a full scale of 2 * VCC.
  SFE_SMOL_POWER_USE_ADC_REF_1V1,      //Use the ATtiny43U's built-in 1.1V ADC reference. Quiet, but limits the full scale to 2.2V.
  SFE_SMOL_POWER_USE_ADC_REF_UNDEFINED //Something bad has happened...
} sfe_power_board_ADC_ref_e;

/** The Watchdog Timer Prescale select */
typedef enum 
{
  SFE_SMOL_POWER_WDT_TIMEOUT_16ms = 0,    //16ms timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_32ms,        //32ms timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_64ms,        //64ms timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_125ms,       //0.125s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_250ms,       //0.25s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_500ms,       //0.5s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_1s,          //1.0s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_2s,          //2.0s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_4s,          //4.0s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_8s,          //8.0s timeout
  SFE_SMOL_POWER_WDT_TIMEOUT_UNDEFINED   //Something bad has happened...
} sfe_power_board_WDT_prescale_e;

#endif // /__SFE_SMOL_POWER_BOARD_CONSTANTS__
