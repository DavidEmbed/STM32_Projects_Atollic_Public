/*
 * stepper.h
 *
 *  Created on: Aug 21, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef STEPPER_H_
#define STEPPER_H_


#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"

// Pin Description
// PE3 - Orange
// PE4 - yellow
// PE5 - Pink
// PE6 - Blue

typedef enum  {
	WIRE_ORANGE	= 0,
	WIRE_YELLOW	= 1,
	WIRE_PINK	= 2,
	WIRE_BLUE	= 3
} Wire_TypeDef;

#define WIREn                             	4  // Number of wires

#define WIRE_ORANGE_PIN                     GPIO_Pin_3
#define WIRE_ORANGE_GPIO_PORT               GPIOE
#define WIRE_ORANGE_GPIO_CLK                RCC_AHB1Periph_GPIOE

#define WIRE_YELLOW_PIN                     GPIO_Pin_4
#define WIRE_YELLOW_GPIO_PORT               GPIOE
#define WIRE_YELLOW_GPIO_CLK                RCC_AHB1Periph_GPIOE

#define WIRE_PINK_PIN                       GPIO_Pin_5
#define WIRE_PINK_GPIO_PORT                 GPIOE
#define WIRE_PINK_GPIO_CLK                  RCC_AHB1Periph_GPIOE

#define WIRE_BLUE_PIN                       GPIO_Pin_6
#define WIRE_BLUE_GPIO_PORT                 GPIOE
#define WIRE_BLUE_GPIO_CLK                  RCC_AHB1Periph_GPIOE


// Functions
void WireInit(Wire_TypeDef Pin);
void WireInitAll(void);
void WireOn(Wire_TypeDef Pin);
void WireOff(Wire_TypeDef Pin);
void WireValue(Wire_TypeDef Pin, uint8_t value);


#ifdef __cplusplus
}
#endif

#endif /* STEPPER_H_ */
