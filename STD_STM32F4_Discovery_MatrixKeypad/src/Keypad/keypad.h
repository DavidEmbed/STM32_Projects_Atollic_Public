/*
 * keypad.h
 *
 *  Created on: Aug 7, 2017
 *      Author: David Martinez
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef KEYPAD_H_
#define KEYPAD_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//////////////////////////////////////
// 	Matrix Keypad Pin Definitions	//
//									//
//      C1  C2  C3  C4				//
//      |   |   |   |				//
// R1 - 1   2   3   A				//
// R2 - 4   5   6   B				//
// R3 - 7   8   9   C				//
// R4 - *   0   #   D				//
//									//
// Columns							//
// Pin 1 keypad - PE7  - C4			//
// Pin 2 keypad - PE8  - C3			//
// Pin 3 keypad - PE9  - C2			//
// Pin 4 keypad - PE10 - C1			//
//									//
// Rows								//
// Pin 5 keypad - PE11 - R4			//
// Pin 6 keypad - PE12 - R3			//
// Pin 7 keypad - PE13 - R2			//
// Pin 8 keypad - PE14 - R1			//
//									//
//////////////////////////////////////

#define KEYPAD_NO_PRESSED		0xFF
#define KEYPAD_READ_INTERVAL	200

typedef enum {
	KEYPAD_Button_0 		= 0,    // Button 0 code
	KEYPAD_Button_1 		= 1,    // Button 1 code
	KEYPAD_Button_2 		= 2,    // Button 2 code
	KEYPAD_Button_3 		= 3,    // Button 3 code
	KEYPAD_Button_4 		= 4,    // Button 4 code
	KEYPAD_Button_5 		= 5,    // Button 5 code
	KEYPAD_Button_6 		= 6,    // Button 6 code
	KEYPAD_Button_7 		= 7,    // Button 7 code
	KEYPAD_Button_8 		= 8,    // Button 8 code
	KEYPAD_Button_9 		= 9,    // Button 9 code
	KEYPAD_Button_STAR 		= 10,   // Button START code
	KEYPAD_Button_HASH 		= 11,   // Button HASH code
	KEYPAD_Button_A 		= 12,	// Button A code
	KEYPAD_Button_B 		= 13,	// Button B code
	KEYPAD_Button_C 		= 14,	// Button C code
	KEYPAD_Button_D 		= 15,	// Button D code
	KEYPAD_Button_NOPRESSED = KEYPAD_NO_PRESSED 	// No button pressed
} Keypad_Button_TypeDef;


typedef enum{
	KEYPAD_COLUMN_1			= 0,
	KEYPAD_COLUMN_2			= 1,
	KEYPAD_COLUMN_3			= 2,
	KEYPAD_COLUMN_4			= 3
} Keypad_Column_TypeDef;


typedef enum{
	KEYPAD_ROW_1			= 0,
	KEYPAD_ROW_2			= 1,
	KEYPAD_ROW_3			= 2,
	KEYPAD_ROW_4			= 3
} Keypad_Row_TypeDef;


#define ColumnsN                            4  	// Number of Columns
#define RowsN								4	// Number of Rows


// Columns pin, port and clk
#define KEYPAD_COLUMN_4_PIN                 GPIO_Pin_7
#define KEYPAD_COLUMN_4_GPIO_PORT           GPIOE
#define KEYPAD_COLUMN_4_GPIO_CLK            RCC_AHB1Periph_GPIOE

#define KEYPAD_COLUMN_3_PIN                 GPIO_Pin_8
#define KEYPAD_COLUMN_3_GPIO_PORT           GPIOE
#define KEYPAD_COLUMN_3_GPIO_CLK            RCC_AHB1Periph_GPIOE

#define KEYPAD_COLUMN_2_PIN                 GPIO_Pin_9
#define KEYPAD_COLUMN_2_GPIO_PORT           GPIOE
#define KEYPAD_COLUMN_2_GPIO_CLK            RCC_AHB1Periph_GPIOE

#define KEYPAD_COLUMN_1_PIN                 GPIO_Pin_10
#define KEYPAD_COLUMN_1_GPIO_PORT           GPIOE
#define KEYPAD_COLUMN_1_GPIO_CLK            RCC_AHB1Periph_GPIOE


// Rows pin, port and clk
#define KEYPAD_ROW_4_PIN                    GPIO_Pin_11
#define KEYPAD_ROW_4_GPIO_PORT              GPIOE
#define KEYPAD_ROW_4_GPIO_CLK               RCC_AHB1Periph_GPIOE

#define KEYPAD_ROW_3_PIN                    GPIO_Pin_12
#define KEYPAD_ROW_3_GPIO_PORT              GPIOE
#define KEYPAD_ROW_3_GPIO_CLK               RCC_AHB1Periph_GPIOE

#define KEYPAD_ROW_2_PIN                    GPIO_Pin_13
#define KEYPAD_ROW_2_GPIO_PORT              GPIOE
#define KEYPAD_ROW_2_GPIO_CLK               RCC_AHB1Periph_GPIOE

#define KEYPAD_ROW_1_PIN                    GPIO_Pin_14
#define KEYPAD_ROW_1_GPIO_PORT              GPIOE
#define KEYPAD_ROW_1_GPIO_CLK               RCC_AHB1Periph_GPIOE


/* Functions ------------------------------------------------------------------*/
void ColumnsInit(Keypad_Column_TypeDef column);
void RowsInit(Keypad_Row_TypeDef row);
void KeypadInit(void);
void ColumnOn(Keypad_Column_TypeDef column);
void ColumnOff(Keypad_Row_TypeDef row);
uint8_t ReadRowState(Keypad_Row_TypeDef row);
uint8_t CheckRow(Keypad_Column_TypeDef column);
uint8_t ReadValueKeypad(void);
void Keypad_Update(uint32_t TimeMs);
Keypad_Button_TypeDef ReadButtonKeypad(void);

#ifdef __cplusplus
}
#endif

#endif /* KEYPAD_H_ */
