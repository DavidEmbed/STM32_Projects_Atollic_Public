/*
 * __74hc595.h
 *
 *  Created on: Aug 19, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef __74HC595_H_
#define __74HC595_H_


#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"

////////// 74HC595 //////////// 7 Segment ////////
// Pin 15 	- Q0 			| 	Pin 7 - A		//
// Pin 1 	- Q1 			|	Pin 6 - B		//
// Pin 2 	- Q2 			|	Pin 4 - C		//
// Pin 3 	- Q3 			|	Pin 2 - D		//
// Pin 4 	- Q4 			|   Pin 1 - E		//
// Pin 5 	- Q5 			|   Pin 9 - F		//
// Pin 6 	- Q6 			|   Pin 10 - G		//
// Pin 7 	- Q7  			|   Pin 5 - DP		//
// Pin 8 	- GND 			|   Pin 3 - GND		//
// Pin 9 	- Q7S 			|   Pin 8 - GND		//
// Pin 10 	- MR 	- VCC	|   				//
// Pin 11 	- SHCP 	- PD7	|					//
// Pin 12 	- STCP 	- PD6	|					//
// Pin 13 	- OE 	- GND	|					//
// Pin 14 	- DS 	- PD3	|					//
// Pin 16 	- VCC 			|					//
//////////////////////////////////////////////////

typedef enum  {
	__74HC595_DS	= 0,
	__74HC595_STCP	= 1,
	__74HC595_SHCP	= 2
} __74HC595_TypeDef;


#define LSBFIRST	0
#define MSBFIRST	1

#define __74HC595n                          3  // Number of pins

#define __74HC595_DS_PIN                    GPIO_Pin_3
#define __74HC595_DS_GPIO_PORT              GPIOD
#define __74HC595_DS_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define __74HC595_STCP_PIN                  GPIO_Pin_6
#define __74HC595_STCP_GPIO_PORT            GPIOD
#define __74HC595_STCP_GPIO_CLK             RCC_AHB1Periph_GPIOD

#define __74HC595_SHCP_PIN                  GPIO_Pin_7
#define __74HC595_SHCP_GPIO_PORT            GPIOD
#define __74HC595_SHCP_GPIO_CLK             RCC_AHB1Periph_GPIOD


// Four 7Segment Anode Control to turn off/on the digit
typedef enum {
	__7Segment_Digit1	= 0,
	__7Segment_Digit2	= 1,
	__7Segment_Digit3	= 2,
	__7Segment_Digit4	= 3
} __7SegmentDigit_TypeDef;


#define __7Segmentn                    4  // Number of pins

#define __7Segment_Digit1_PIN                    GPIO_Pin_8
#define __7Segment_Digit1_GPIO_PORT              GPIOD
#define __7Segment_Digit1_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define __7Segment_Digit2_PIN                    GPIO_Pin_9
#define __7Segment_Digit2_GPIO_PORT              GPIOD
#define __7Segment_Digit2_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define __7Segment_Digit3_PIN                    GPIO_Pin_10
#define __7Segment_Digit3_GPIO_PORT              GPIOD
#define __7Segment_Digit3_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define __7Segment_Digit4_PIN                    GPIO_Pin_11
#define __7Segment_Digit4_GPIO_PORT              GPIOD
#define __7Segment_Digit4_GPIO_CLK               RCC_AHB1Periph_GPIOD


// Functions
void __74HC595Init(__74HC595_TypeDef Pin);
void __74HC595High(__74HC595_TypeDef Pin);
void __74HC595Low(__74HC595_TypeDef Pin);
void __74HC595WriteValue(__74HC595_TypeDef Pin, uint8_t value);
void __74HC595InitAll(void);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
void sevenSegWrite(uint8_t digit);
void Display(unsigned char num);

// Functions
void __AnodeFour7SegmentInit(__7SegmentDigit_TypeDef Pin) ;
void __AnodeFour7SegmentOff(__74HC595_TypeDef Pin);
void __AnodeFour7SegmentOn(__74HC595_TypeDef Pin);
void __AnodeFour7SegmentAllOff(void);
void __AnodeFour7SegmentAllOn(void);
void __AnodeFour7SegmentInitAll(void);

#ifdef __cplusplus
}
#endif

#endif /* __74HC595_H_ */
