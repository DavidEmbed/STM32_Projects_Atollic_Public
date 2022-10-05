/*
 * ds3231.h
 *
 *  Created on: Aug 14, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef __DS3231_H
#define __DS3231_H

#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"
#include "arm_math.h"

// Pin Description
// DS3231		STM32F4xx	Description
//
// SCL			PB6			Clock line for I2C
// SDA			PB9			Data line for I2C
// VCC			3.3V
// GND			GND


// Configuration for I2C
#define DS3231_I2Cx						I2C1
#define DS3231_I2Cx_CLK					RCC_APB1Periph_I2C1

#define DS3231_I2Cx_SCL_PIN           	GPIO_Pin_6
#define DS3231_I2Cx_SCL_GPIO_PORT     	GPIOB
#define DS3231_I2Cx_SCL_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define DS3231_I2Cx_SCL_SOURCE        	GPIO_PinSource6
#define DS3231_I2Cx_SCL_AF            	GPIO_AF_I2C1

#define DS3231_I2Cx_SDA_PIN           	GPIO_Pin_9
#define DS3231_I2Cx_SDA_GPIO_PORT     	GPIOB
#define DS3231_I2Cx_SDA_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define DS3231_I2Cx_SDA_SOURCE        	GPIO_PinSource9
#define DS3231_I2Cx_SDA_AF            	GPIO_AF_I2C1


// I2Cx timeout waiting value (set for system running at 168 MHz)
#define DS3231_I2C_TIMEOUT			10000

/* DS3231 registers */
 typedef enum {
	DS3231_SECONDS			= 0x00,
	DS3231_MINUTES			= 0x01,
	DS3231_HOURS			= 0x02,
	DS3231_DAY				= 0x03,
	DS3231_DATE				= 0x04,
	DS3231_MONTH_CENTURY	= 0x05,
	DS3231_YEAR				= 0x06,
	DS3231_ALARM_1SECOND	= 0x07,
	DS3231_ALARM_1MINUTE	= 0x08,
	DS3231_ALARM_1HOUR		= 0x09,
	DS3231_ALARM_1DAY		= 0x0A,
	DS3231_ALARM_1DATE		= 0x0A,
	DS3231_ALARM_2MINUTE	= 0x0B,
	DS3231_ALARM_2HOUR		= 0x0C,
	DS3231_ALARM_2DAY		= 0x0D,
	DS3231_ALARM_2DATE		= 0x0D,
	DS3231_CONTROL			= 0x0E,
	DS3231_CONTROL_STATUS	= 0x0F,
	DS3231_AGING_OFFSET		= 0x10,
	DS3231_MSB_TEMP			= 0x11,
	DS3231_LSB_TEMP			= 0x12
} DS3231_REG_TypeDef;


// DS3231 slave Address + Write/Read Bit
typedef enum{
	DS3231_ADDR_WR    = 0b11010000, // LSB to '0' indicating write operation
	DS3231_ADDR_RD    = 0b11010001  // LSB to '1' indicating read operation
}DS3231_Address_TypeDef;


// DS3231 data length types
typedef enum{
	DS3231_DATA_1Byte     = 0,
	DS3231_DATA_2Bytes    = 1
}DS3231_DataLength_TypeDef;


// Functions
void DS3231_Init(void);
void DS3231_GPIOConfig(void);
void DS3231_InitI2C(void);

// I2C Communication functions
uint8_t DS3231_ReadRegister(DS3231_REG_TypeDef registerAddr, uint16_t *pRegisterValue, DS3231_DataLength_TypeDef nBytes);
uint8_t DS3231_WriteRegister(DS3231_REG_TypeDef registerAddr, uint16_t RegisterValue, DS3231_DataLength_TypeDef nBytes);

//Write
void DS3231_setDateTime(uint16_t year, uint8_t month, uint16_t date, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

// Read
uint8_t DS3231_ReadSecond(uint16_t *second);
uint8_t DS3231_ReadMinute(uint16_t *minute);
uint8_t DS3231_ReadHour(uint16_t *hour);
uint8_t DS3231_ReadDay(uint16_t *day);
char *DS3231_strDayOfWeek(uint8_t dayOfWeek);
uint8_t DS3231_ReadDate(uint16_t *date);
uint8_t DS3231_ReadMonth(uint16_t *month);
char *DS3231_strMonth(uint8_t month);
uint8_t DS3231_ReadYear(uint16_t *year);


// Convert
uint8_t DS3231_bcd2dec(uint8_t bcd);
uint8_t DS3231_dec2bcd(uint8_t dec);

// I2C TimeOut function
uint8_t DS3231_TIMEOUT_Callback(void);


#ifdef __cplusplus
}
#endif

#endif /* __DS3231_H */
