/****************************************************************************
**
**  File        : main.c
**
**
**  This program configures four channels of Timer5 as PWM output signal
**  with a PWM_FREQ frequency to control the brightness of a RGB LED
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\PWM\pwm.h"
#include "arm_math.h"

/********************************************************************************************************************************************************
**  TIMX - Channel X Configuration (PXX)
**
**		Configuration System Clock -> HCLK = max 168 MHz
**
**		PCLK1 = HCLK/4 -> APB1
**		PCLK2 = HCLK/2 -> APB2
**
**		TIMXCLK = (PCLK1)x2 = (168 MHz/4)x2 = 84 MHz -> max APB1
** 		TIMXCLK = (PCLK2)x2 = (168 MHz/2)x2 = 168 MHz -> max APB2
**
**		Timers
**		APB1 -> TIM2,3,4,5,6,7,12,13,14
**		APB2 -> TIM1,8,9,10,11
**		TIM6,7 not valid timer to generate a PWM signal
**
**		Calculate PWM Frequency
**		Timer tick frequency = TIMXCLK / ((Prescaler + 1) * Clock_Division)
**		PWM_PERIOD = (Timer tick frequency / PWM_FREQ) - 1
**
**		TIMX update frequency = Timer tick frequency / (PWM_PERIOD + 1)
**		TIMX update time = 1 / TIMX update frequency
**
**
**			Own Configuration:
**			-----------
**			TIM 5:
**			CH1: PA0
**			CH2: PA1
**			CH3: PA2
**
**			For APB1:
**			Prescaler = 199, Clock_Division = 1 => TIMXCLK = 84 MHz
**          Timer tick frequency = 84 MHz / ((199 + 1) * 1) = 420 KHz
**			PWM_PERIOD = (420 KHz / 500 Hz) - 1 = 839 ticks
**
**          ChannelxPulse (CCx register) = [% DUTY] * PWM_PERIOD / 100;
**
**          When CCx register = PWM_PERIOD  => Duty = 100%
**          When CCx register = 0       	=> Duty = 0%
**
**
********************************************************************************************************************************************************/

// Defines
#define delayTime 50

// Variables
uint8_t i;
float32_t redValue;
float32_t greenValue;
float32_t blueValue;
uint32_t pulse;

// Main program
int main(void) {

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize TIM5: CH1 CH2 CH3
	PWM_INIT_ALL();

	while (1) {

		redValue = 100;
		greenValue = 0;
		blueValue = 0;

		for(i = 0; i <= 100; i++){
			redValue--;
			greenValue++;
			pulse = (uint32_t)(redValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare1(TIM5, pulse);
			pulse = (uint32_t)(greenValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare2(TIM5, pulse);
			Delay_ms(delayTime);
		}

		redValue = 0;
		greenValue = 100;
		blueValue = 0;

		for(i = 0; i <= 100; i++){
			greenValue--;
			blueValue++;
			pulse = (uint32_t)(greenValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare2(TIM5, pulse);
			pulse = (uint32_t)(blueValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare3(TIM5, pulse);
			Delay_ms(delayTime);
		}

		redValue = 0;
		greenValue = 0;
		blueValue = 100;

		for(i = 0; i <= 100; i++){
			blueValue--;
			redValue++;
			pulse = (uint32_t)(blueValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare3(TIM5, pulse);
			pulse = (uint32_t)(redValue * (float32_t)(APB1_PWM_PERIOD) / 100.0f);
			TIM_SetCompare1(TIM5, pulse);
			Delay_ms(delayTime);
		}
	}
}


