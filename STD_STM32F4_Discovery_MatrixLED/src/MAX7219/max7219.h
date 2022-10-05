/*
 * max7219.h
 *
 *  Created on: Aug 12, 2017
 *      Author: David Martínez
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAX7219_H_
#define MAX7219_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

// Pin Description
// Vcc 			- 5V
// GND 			- GND
// DIN (MOSI) 	- PA7
// CS (CC) 		- PE3
// CLK (SCLK)	- PA5


// Serial Data Format (16 bits)
// D15 D14 D13 D12 	|	D11 D10 D9 D8 	|	D7 D6 D5 D4 D3 D2 D1 D0	|
// 	X   X	x	x	|		ADRESS		|	MSB		DATA		LSB |

// Register Address Map of MAX7219
typedef enum {
	MAX7219_ADDR_NO_OP				= 0x00,
	MAX7219_ADDR_DIGIT0				= 0x01,
	MAX7219_ADDR_DIGIT1				= 0x02,
	MAX7219_ADDR_DIGIT2				= 0x03,
	MAX7219_ADDR_DIGIT3				= 0x04,
	MAX7219_ADDR_DIGIT4				= 0x05,
	MAX7219_ADDR_DIGIT5				= 0x06,
	MAX7219_ADDR_DIGIT6				= 0x07,
	MAX7219_ADDR_DIGIT7				= 0x08,
	MAX7219_ADDR_DECODE_MODE 		= 0x09,
	MAX7219_ADDR_INTENSITY 			= 0x0A,
	MAX7219_ADDR_SCAN_LIMIT 		= 0x0B,
	MAX7219_ADDR_SHUTDOWN 			= 0x0C,
	MAX7219_ADDR_DISPLAY_TEST 		= 0x0F
} RegistersAdress_TypeDef;


// Shutdown Register Format (0x0C)
typedef enum {
	MAX7219_ShutdownMode 	= 0x00,
	MAX7219_NormalOperation = 0x01
} ShutdownMode_TypeDef;


// Decode-Mode Register (0x09)
typedef enum {
	MAX7219_NoDecode		= 0x00,
	MAX7219_CodeBDigit0		= 0x01,
	MAX7219_COdeBDigit3_0	= 0x0F,
	MAX7219_CodeBDigit7_0	= 0xFF
}DecodeMode_TypeDef;


// Code B Font * Decimal point is set by bit D7 = 1 *
typedef enum {
	zero 	= 0x00,
	one 	= 0x01,
	two 	= 0x02,
	three 	= 0x03,
	four 	= 0x04,
	five 	= 0x05,
	six 	= 0x06,
	seven 	= 0x07,
	eigth 	= 0x08,
	nine 	= 0x09,
	line 	= 0x0A,
	Eletter = 0x0B,
	Hletter = 0x0C,
	Lletter = 0x0D,
	Pletter = 0x0E,
	blank 	= 0x0F,
} Character7Segment_TypeDef;


// Intensity Register Format (0x0A)
typedef enum {
	DutyCicle1 	= 0x00,
	DutyCicle3 	= 0x01,
	DutyCicle5 	= 0x02,
	DutyCicle7 	= 0x03,
	DutyCicle9 	= 0x04,
	DutyCucle13 = 0x05,
	DutyCicle11 = 0x06,
	DutyCicle15 = 0x07,
	DutyCicle17 = 0x08,
	DutyCicle19 = 0x09,
	DutyCicle21 = 0x0A,
	DutyCicle23 = 0x0B,
	DutyCicle25 = 0x0C,
	DutyCicle27 = 0x0D,
	DutyCicle29 = 0x0E,
	DutyCicle31 = 0x0F,
} IntensityLevel_TyepDef;


// Scan-Limit Register Format (0x0B)
typedef enum {
	DisplayDigit0				= 0x00,
	DisplayDigit0_1				= 0x01,
	DisplayDigit0_1_2			= 0x02,
	DisplayDigit0_1_2_3			= 0x03,
	DisplayDigit0_1_2_3_4		= 0x04,
	DisplayDigit0_1_2_3_4_5		= 0x05,
	DisplayDigit0_1_2_3_4_5_6	= 0x06,
	DisplayDigit0_1_2_3_4_5_6_7	= 0x07
} ScanLimit_TypeDef;


// Display Test Register Format
typedef enum {
	NormalOperation	= 0x00,
	DisplayTestMode	= 0x01
}DisplayTestMode_TypeDef;


// Functions definitions
void MAX7219_Init(void);
void MAX7219_GPIO_Init(void);
void MAX7219_SPI_Init(void);
void MAX7219_Write(uint8_t writeAddr, uint8_t value);
uint8_t MAX7219_SendByte(uint16_t byte);
void MAX7219_PowerOn(void);
void MAX7219_PowerOff(void);
void MAX7219_DecodeMode(uint8_t mode);
void MAX7219_Intensity(uint8_t intensity);
void MAX7219_ScanLimit(uint8_t limit);
void MAX7219_DisplayTest(uint8_t mode);
void ClearDisplay(void);
void setRow(uint8_t row, uint8_t value);
void setColumn(uint8_t col, uint8_t value);
void setLed(uint8_t row, uint8_t column, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* MAX7219_H_ */
