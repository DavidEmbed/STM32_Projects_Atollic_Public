/****************************************************************************
**
**  File        : main.c
**
**
**  This program configures 1 channels of Timer5 as PWM output signal
**  with a PWM_FREQ frequency to control a passive buzzer to generate notes
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
#include ".\PWM\pitches.h"

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

// notes in the melody:
//uint16_t melody[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

//Mario main theme melody
uint16_t melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

// Main program
int main(void) {
	uint8_t note;

	// Configures SysTick for delay functions
	Delay_Init();

	PWM_GPIO_INIT(TIM5_CH1);

	while (1) {
		for (note = 0; note < 16*5; note++){
			tone(melody[note]);
			Delay_ms(150);
		}
	}
}


