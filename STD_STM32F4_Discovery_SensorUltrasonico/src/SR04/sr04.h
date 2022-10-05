/*
 * sr04.h
 *
 *  Created on: Aug 5, 2017
 *      Author: David Martinez
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SR04_H_
#define SR04_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "arm_math.h"
#include ".\SysTick\delay.h"

typedef enum  {
	TRIG	= 0,
	ECHO	= 1,
} SR04Pin_TypeDef;

#define SR04n                       2  // Number of pins: PD6 PD7

#define TRIG_PIN             		GPIO_Pin_6
#define TRIG_GPIO_PORT              GPIOD
#define TRIG_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define ECHO_PIN                    GPIO_Pin_7
#define ECHO_GPIO_PORT              GPIOD
#define ECHO_GPIO_CLK               RCC_AHB1Periph_GPIOD

#define HCSR04_TIMEOUT			1000000
#define HCSR04_NUMBER			((float)0.0171821)

/* Functions ------------------------------------------------------------------*/
void SR04Init(SR04Pin_TypeDef Sr04Pin);
void SR04High(SR04Pin_TypeDef Sr04Pin);
void Sr04Low(SR04Pin_TypeDef Sr04Pin);
uint8_t EchoGetState(SR04Pin_TypeDef Sr04Pin);
float32_t ReadDistance();

#ifdef __cplusplus
}
#endif

#endif /* SR04_H_ */
