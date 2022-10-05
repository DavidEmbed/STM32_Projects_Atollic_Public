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
#include "arm_math.h"

// Clock defines
#define APB1_TIMERS_FREQUENCY				84000000													// Hz
#define PRESCALER 							199
#define CLOCK_DIVISON						1
#define APB1_TIMER_TICK_FREQUENCY			APB1_TIMERS_FREQUENCY / ((PRESCALER + 1) * CLOCK_DIVISON)	// Hz
#define PWM_FREQ			 				1000	 														// Hz
#define APB1_PWM_PERIOD						(APB1_TIMER_TICK_FREQUENCY / PWM_FREQ) - 1					// Ticks


// TypeDef of number of PWM signals
typedef enum  {
	TIM5_CH1	= 0,
} Tim_TypeDef;


// Number of PWM signals
#define TIMn                             	1

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


// Functions
void PWM_GPIO_INIT(Tim_TypeDef Tim);
void PWM_INIT(Tim_TypeDef Tim);
void PWM_INIT_ALL(void);
uint8_t PWM_SERVOMOTOR(Tim_TypeDef Tim, uint16_t degree);
uint8_t PWM_SetPercentage(Tim_TypeDef Tim, float32_t pwm_percen);

#ifdef __cplusplus
}
#endif

#endif /* PWM_H_ */
