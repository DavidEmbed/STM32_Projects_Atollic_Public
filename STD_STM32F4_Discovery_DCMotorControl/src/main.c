/****************************************************************************
**
**  File        : main.c
**
**
**  This program configures 1 channels of Timer5 as PWM output signal
**  with a PWM_FREQ frequency to control the speed of a DC Motor, also there are two digital pins
**  for control the direction of the rotation.
**
**  //TODO: Senyal pwm 3V no suficient
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
**			PWM_PERIOD = (420 KHz / 50 Hz) - 1 = 8399 ticks
**
**          ChannelxPulse (CCx register) = [% DUTY] * PWM_PERIOD / 100;
**
**          When CCx register = PWM_PERIOD  => Duty = 100%
**          When CCx register = 0       	=> Duty = 0%
**
**
********************************************************************************************************************************************************/

typedef enum {
	DirectionA = 0,
	DirectionB = 1
} DirectionsPins_TypeDef;

#define PINn	2

#define DIRECTION_A_PIN                      GPIO_Pin_7
#define DIRECTION_A_GPIO_PORT                GPIOE
#define DIRECTION_A_GPIO_CLK                 RCC_AHB1Periph_GPIOE

#define DIRECTION_B_PIN                      GPIO_Pin_8
#define DIRECTION_B_GPIO_PORT                GPIOE
#define DIRECTION_B_GPIO_CLK                 RCC_AHB1Periph_GPIOE

// Variables
GPIO_TypeDef*	DIRECTION_GPIO_PORT[LEDn] = {DIRECTION_A_GPIO_PORT, DIRECTION_B_GPIO_PORT};
const uint16_t	DIRECTION_GPIO_PIN[LEDn]  = {DIRECTION_A_PIN, 		DIRECTION_B_PIN};
const uint32_t	DIRECTION_GPIO_CLK[LEDn]  = {DIRECTION_A_GPIO_CLK, 	DIRECTION_B_GPIO_CLK};

// Functions
void DirectionPinInit(DirectionsPins_TypeDef Pin);
void SetDirection(DirectionsPins_TypeDef Pin);
void ResetDirection(DirectionsPins_TypeDef Pin);


// Main program
int main(void) {

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize TIM5: CH1 CH2 CH3
	PWM_INIT_ALL();

	//Initialize two digital pins
	DirectionPinInit(DirectionA);
	DirectionPinInit(DirectionB);

	while (1) {

		PWM_SetPercentage(TIM5_CH1, 25);
		SetDirection(DirectionA);
		ResetDirection(DirectionB);
		Delay_ms(2000);

		PWM_SetPercentage(TIM5_CH1, 25);
		SetDirection(DirectionB);
		ResetDirection(DirectionA);
		Delay_ms(2000);

		PWM_SetPercentage(TIM5_CH1, 50);
		SetDirection(DirectionA);
		ResetDirection(DirectionB);
		Delay_ms(2000);

		PWM_SetPercentage(TIM5_CH1, 50);
		SetDirection(DirectionB);
		ResetDirection(DirectionA);
		Delay_ms(2000);

		PWM_SetPercentage(TIM5_CH1, 75);
		SetDirection(DirectionA);
		ResetDirection(DirectionB);
		Delay_ms(2000);

		PWM_SetPercentage(TIM5_CH1, 75);
		SetDirection(DirectionB);
		ResetDirection(DirectionA);
		Delay_ms(2000);
	}
}


// Initialization of pins to control the direction of DC Motor
void DirectionPinInit(DirectionsPins_TypeDef Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(DIRECTION_GPIO_CLK[Pin], ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = DIRECTION_GPIO_PIN[Pin];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(DIRECTION_GPIO_PORT[Pin], &GPIO_InitStructure);
}


// Switch the LED On
void SetDirection(DirectionsPins_TypeDef Pin) {
	GPIO_SetBits(DIRECTION_GPIO_PORT[Pin], DIRECTION_GPIO_PIN[Pin]);
}


// Switch the LED Off
void ResetDirection(DirectionsPins_TypeDef Pin) {
	GPIO_ResetBits(DIRECTION_GPIO_PORT[Pin], DIRECTION_GPIO_PIN[Pin]);
}

