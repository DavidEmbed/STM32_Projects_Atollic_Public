/*
 * encoder.h
 *
 *  Created on: Aug 21, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef ENCODER_H_
#define ENCODER_H_


#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"
#include ".\SysTick\delay.h"

// Pin Description
// PB7 - CLK
// PB8 - DT

typedef enum {
	CLK_PIN = 0,
	DT_PIN = 1
} Pin_TypeDef;


#define PINn                           	2  // Number of pins

#define CLK_PIN_PIN                		GPIO_Pin_7
#define CLK_PIN_GPIO_PORT          		GPIOB
#define CLK_PIN_GPIO_CLK           		RCC_AHB1Periph_GPIOB

#define DT_PIN_PIN                		GPIO_Pin_8
#define DT_PIN_GPIO_PORT         	 	GPIOB
#define DT_PIN_GPIO_CLK           		RCC_AHB1Periph_GPIOB

// Functions
void EncoderPinInit(Pin_TypeDef Button);
uint8_t EncoderPinGetState(Pin_TypeDef Button);

#ifdef __cplusplus
}
#endif

#endif /* ENCODER_H_ */
