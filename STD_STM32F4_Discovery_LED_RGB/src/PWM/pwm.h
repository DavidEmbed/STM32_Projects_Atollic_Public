/*
 * pwm.h
 *
 *  Created on: Feb 25, 2015
 *      Author: David Martínez Piqué
 */

// Define to prevent recursive inclusion
#ifndef PWM_H_
#define PWM_H_


#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"

// Clock defines
#define APB1_TIMERS_FREQUENCY				84000000													// Hz
#define PRESCALER 							199
#define CLOCK_DIVISON						1
#define APB1_TIMER_TICK_FREQUENCY			APB1_TIMERS_FREQUENCY / ((PRESCALER + 1) * CLOCK_DIVISON)	// Hz
#define PWM_FREQ			 				500 														// Hz
#define APB1_PWM_PERIOD						(APB1_TIMER_TICK_FREQUENCY / PWM_FREQ) - 1					// Ticks


// TypeDef of number of PWM signals
typedef enum  {
	TIM5_CH1	= 0,
	TIM5_CH2	= 1,
	TIM5_CH3	= 2,
} Tim_TypeDef;


// Number of PWM signals
#define TIMn                             	3

// Defines TIM5 CH1 - PWM_GPIO_INIT()
#define TIM5_CH1_PIN                       	GPIO_Pin_0
#define TIM5_CH1_GPIO_PORT                 	GPIOA
#define TIM5_CH1_GPIO_CLK                  	RCC_AHB1Periph_GPIOA
#define TIM5_CH1_PINSOURCE					GPIO_PinSource0
#define TIM5_CH1_AF							GPIO_AF_TIM5

// Defines TIM5 CH1 - PWM_INIT()
#define TIM5_CH1_TIM_CLK					RCC_APB1Periph_TIM5
#define TIM5_CH1_TIM						TIM5
#define TIM5_CH1_CHANNEL					TIM_Channel_1
#define TIM5_CH1_PWM_PERIOD					APB1_PWM_PERIOD


// Defines TIM5 CH2 - PWM_GPIO_INIT()
#define TIM5_CH2_PIN                       	GPIO_Pin_1
#define TIM5_CH2_GPIO_PORT                 	GPIOA
#define TIM5_CH2_GPIO_CLK                  	RCC_AHB1Periph_GPIOA
#define TIM5_CH2_PINSOURCE					GPIO_PinSource1
#define TIM5_CH2_AF							GPIO_AF_TIM5

// Defines TIM5 CH2 - PWM_INIT()
#define TIM5_CH2_TIM_CLK					RCC_APB1Periph_TIM5
#define TIM5_CH2_TIM						TIM5
#define TIM5_CH2_CHANNEL					TIM_Channel_2
#define TIM5_CH2_PWM_PERIOD					APB1_PWM_PERIOD


// Defines TIM5 CH3 - PWM_GPIO_INIT()
#define TIM5_CH3_PIN                       	GPIO_Pin_2
#define TIM5_CH3_GPIO_PORT                 	GPIOA
#define TIM5_CH3_GPIO_CLK                  	RCC_AHB1Periph_GPIOA
#define TIM5_CH3_PINSOURCE					GPIO_PinSource2
#define TIM5_CH3_AF							GPIO_AF_TIM5

// Defines TIM5 CH3 - PWM_INIT()
#define TIM5_CH3_TIM_CLK					RCC_APB1Periph_TIM5
#define TIM5_CH3_TIM						TIM5
#define TIM5_CH3_CHANNEL					TIM_Channel_3
#define TIM5_CH3_PWM_PERIOD					APB1_PWM_PERIOD


// Functions
void PWM_GPIO_INIT(Tim_TypeDef Tim);
void PWM_INIT(Tim_TypeDef Tim);
void PWM_INIT_ALL(void);


#ifdef __cplusplus
}
#endif

#endif /* PWM_H_ */
