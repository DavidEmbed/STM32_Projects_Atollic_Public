/*
 * pwm.c
 *
 *  Created on: Feb 25, 2015
 *      Author: David Martínez Piqué
 */

// Includes
#include "pwm.h"

// Variables PWM_GPIO_INIT
uint16_t		TIM_PIN[TIMn] 		= {TIM5_CH1_PIN, 		TIM5_CH2_PIN, 		TIM5_CH3_PIN};
GPIO_TypeDef*	TIM_GPIO_PORT[TIMn] = {TIM5_CH1_GPIO_PORT, 	TIM5_CH2_GPIO_PORT, TIM5_CH3_GPIO_PORT};
uint32_t		TIM_GPIO_CLK[TIMn] 	= {TIM5_CH1_GPIO_CLK, 	TIM5_CH2_GPIO_CLK, 	TIM5_CH3_GPIO_CLK};
uint8_t			TIM_PINSOURCE[TIMn]	= {TIM5_CH1_PINSOURCE, 	TIM5_CH2_PINSOURCE, TIM5_CH3_PINSOURCE};
uint8_t 		TIM_AF[TIMn] 		= {TIM5_CH1_AF, 		TIM5_CH2_AF, 		TIM5_CH3_AF};

// Variables PWM_INIT
uint32_t		TIM_CLK[TIMn] 		 = {TIM5_CH1_TIM_CLK, 	 	TIM5_CH2_TIM_CLK, 		TIM5_CH3_TIM_CLK};
TIM_TypeDef*	TIM[TIMn] 			 = {TIM5_CH1_TIM, 		 	TIM5_CH2_TIM, 			TIM5_CH3_TIM};
uint16_t		TIM_CHANNEL[TIMn] 	 = {TIM5_CH1_CHANNEL, 	 	TIM5_CH2_CHANNEL, 		TIM5_CH3_CHANNEL};
uint32_t		TIM_PWM_PERIOD[TIMn] = {TIM5_CH1_PWM_PERIOD,	TIM5_CH2_PWM_PERIOD, 	TIM5_CH3_PWM_PERIOD};

// Variables
uint32_t pulse = 0;

// Configure the GPIOx PIN
void PWM_GPIO_INIT(Tim_TypeDef Tim)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// GPIOx Clocks enable
	RCC_AHB1PeriphClockCmd(TIM_GPIO_CLK[Tim], ENABLE);

	// GPIOx Configuration
	GPIO_InitStructure.GPIO_Pin = TIM_PIN[Tim];
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(TIM_GPIO_PORT[Tim], &GPIO_InitStructure);

	// Mapping the PINx to the TIMx
	GPIO_PinAFConfig(TIM_GPIO_PORT[Tim], TIM_PINSOURCE[Tim], TIM_AF[Tim]);
}


// Initialization of TimeBase and Output Compare for PWM output
void PWM_INIT(Tim_TypeDef Tim)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// TIMx Clock APB1 enable
	RCC_APB1PeriphClockCmd(TIM_CLK[Tim], ENABLE);

	// Time Base configuration
	TIM_TimeBaseStructure.TIM_Prescaler = PRESCALER;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = TIM_PWM_PERIOD[Tim];
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM[Tim], &TIM_TimeBaseStructure);

	// Output Compare configuration for PWM mode
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;

	// If timer comes from Channel 1, do:
	if (TIM_CHANNEL[Tim] == TIM_Channel_1)
	{
		// Initializes TIMx Channel 1 and enable Preload register
		TIM_OC1Init(TIM[Tim], &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM[Tim], TIM_OCPreload_Enable);
	}

	// If timer comes from Channel 2, do:
	else if (TIM_CHANNEL[Tim] == TIM_Channel_2)
	{
		// Initializes TIMx Channel 2 and enable Preload register
		TIM_OC2Init(TIM[Tim], &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM[Tim], TIM_OCPreload_Enable);
	}

	// If timer comes from Channel 3, do:
	else if (TIM_CHANNEL[Tim] == TIM_Channel_3)
	{
		// Initializes TIMx Channel 3 and enable Preload register
		TIM_OC3Init(TIM[Tim], &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM[Tim], TIM_OCPreload_Enable);
	}

	// If timer comes from Channel 4, do:
	else
	{
		// Initializes TIMx Channel 4 and enable Preload register
		TIM_OC4Init(TIM[Tim], &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM[Tim], TIM_OCPreload_Enable);
	}

	// Enable TIMx peripheral
	TIM_Cmd(TIM[Tim], ENABLE);

	// Enables Capture/Compare TIMx-CHx output
	TIM_CCxCmd(TIM[Tim], TIM_CHANNEL[Tim], TIM_CCx_Enable);
}


// Initialization of all timers
void PWM_INIT_ALL(void)
{
	uint8_t i;

	// Loop for initialize all timers and channels
	for (i = 0; i < TIMn; i++)
	{
		PWM_GPIO_INIT(i);
		PWM_INIT(i);
	}
}

