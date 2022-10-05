/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for control a matrix keypad
**  (PE7, PE8, PE9, PE10, PE11, PE12 , PE13, PE14).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\Keypad\keypad.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Define function
void TIM12_Config(void);

// Temps acumulat
uint32_t localTimeMs = 0;

// Main program
int main(void) {
	Keypad_Button_TypeDef keypadButton;

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// TIM12 Initialization
	TIM12_Config();

	// Initialize keypad
	KeypadInit();

	// Initialization is okey
	printf("Init Ok!\r");

	while (1){

		keypadButton = ReadButtonKeypad();

		if (keypadButton != KEYPAD_Button_NOPRESSED){
			switch(keypadButton){
				case KEYPAD_Button_0:
					printf("0\r");
					break;
				case KEYPAD_Button_1:
					printf("1\r");
					break;
				case KEYPAD_Button_2:
					printf("2\r");
					break;
				case KEYPAD_Button_3:
					printf("3\r");
					break;
				case KEYPAD_Button_4:
					printf("4\r");
					break;
				case KEYPAD_Button_5:
					printf("5\r");
					break;
				case KEYPAD_Button_6:
					printf("6\r");
					break;
				case KEYPAD_Button_7:
					printf("7\r");
					break;
				case KEYPAD_Button_8:
					printf("8\r");
					break;
				case KEYPAD_Button_9:
					printf("9\r");
					break;
				case KEYPAD_Button_STAR:
					printf("*\r");
					break;
				case KEYPAD_Button_HASH:
					printf("#\r");
					break;
				case KEYPAD_Button_A:
					printf("A\r");
					break;
				case KEYPAD_Button_B:
					printf("B\r");
					break;
				case KEYPAD_Button_C:
					printf("C\r");
					break;
				case KEYPAD_Button_D:
					printf("D\r");
					break;
				default:
					break;
			}
		}
	}
}


/*
* @note PCLK1 = HCLK/4
* 		TIM12CLK = (PCLK1)x2 = (168 MHz/4)x2 = 84 MHz
*
* 		so CLK_INT = TIM12CLK/((Preescaler+1) * Clock_Division)
*				TIM12 update time = (1/CLK_INT) * (Period+1)
*				TIM12 update freq = CLK_INT / (Period+1)
*
*			Own Config:
*			-----------
*			Precaler = 199
*			Period = 41999 => 42000 ticks (from 0 to 41999)
*			CLK_INT = 84MHz/((199+1) * 1) = 420KHz
*         	TIM12 update freq = 420KHz / (41999 + 1) = 10 Hz => 100 ms
*         	TIM12 update freq = 420KHz / (4199 + 1) = 100 Hz => 10 ms
*         	TIM12 update freq = 420KHz / (419 + 1) = 1000 Hz => 1 ms
*/

void TIM12_Config(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// TIM12 clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	// NVIC Configuration
	// Enable the TIM12 global Interrupt with lower priority
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xFF;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xFF;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Time base configuration
	TIM_TimeBaseStructure.TIM_Period = 419;
	TIM_TimeBaseStructure.TIM_Prescaler = 199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

	// Set Counter to 0
	TIM_SetCounter(TIM12, 0);

	// Clear Interrupt pending bit
	TIM_ClearITPendingBit(TIM12, TIM_IT_Update);

	// Enable Interruption of peripheral TIM12 Update
	TIM_ITConfig(TIM12, TIM_IT_Update, ENABLE);

	// Enable Timer 12
	TIM_Cmd(TIM12, ENABLE);
}

// This function handles TIM12 global interrupt request.
void TIM8_BRK_TIM12_IRQHandler(void) {

	// Check if TIM12 Interrupt update is asserted
	if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET) {

		// This code is executed when TIM12 counter has an overflow
		// (the counter value rise the Auto-reload specified value)

		// Temps acumulat general
		localTimeMs = localTimeMs + 1;

		Keypad_Update(localTimeMs);

		// We need to clear line pending bit manually
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
	}
}
