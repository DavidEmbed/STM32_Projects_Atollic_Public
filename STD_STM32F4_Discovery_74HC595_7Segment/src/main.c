/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for control a 7 segment display thought the chip 74HC595
**  (PD3, PD6, PD7).
**
**  For Four 7segment control anode lines: PD8, PD9, PD10, PD11
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\74HC595\74hc595.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Define function
void TIM12_Config(void);
void CallbackEverySecond(void);

// Temps acumulat
uint32_t localTimeMs = 0;

// Global variables
uint8_t digit1 = 0, digit2 = 0, digit3 = 0, digit4 = 0;

// Main program
int main(void) {
	//uint8_t digit;

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

	// Init 74HC595
	__74HC595InitAll();

	// Init anode control of four 7segment
	__AnodeFour7SegmentInitAll();

	sevenSegWrite(0);

	// Initialization is okey
	printf("Init Ok!\r");


	while (1) {
		/*
		 * Example for one 7segment
		// Count from 9 to 0
		for (digit = 10; digit > 0; digit--){
			Delay_ms(1000);
			sevenSegWrite(digit - 1);
		}

		Delay_ms(3000);
		*/

		/*
		 * Example for four 7 segment
		  Display(1);
		  Delay_ms(500);
		  Display(2);
		  Delay_ms(500);
		  Display(3);
		  Delay_ms(500);
		  Display(4);
		  Delay_ms(500);
		  Display(5);
		  Delay_ms(500);
		  Display(6);
		  Delay_ms(500);
		  Display(7);
		  Delay_ms(500);
		  Display(8);
		  Delay_ms(500);
		  Display(9);
		  Delay_ms(500);
		  Display(10);
		  Delay_ms(500);
		  Display(11);
		  Delay_ms(500);
		  Display(12);
		  Delay_ms(500);
		  Display(13);
		  Delay_ms(500);
		  Display(14);
		  Delay_ms(500);
		  Display(15);
		  Delay_ms(500);
		*/

		// Example four 7segment counter:
		//__AnodeFour7SegmentOff(__7Segment_Digit1);
		//__AnodeFour7SegmentOn(__7Segment_Digit4);
		sevenSegWrite(digit1);
		__AnodeFour7SegmentOff(__7Segment_Digit1);
		__AnodeFour7SegmentOn(__7Segment_Digit4);

		//__AnodeFour7SegmentAllOff();
		//__AnodeFour7SegmentOn(__7Segment_Digit3);
		//__AnodeFour7SegmentOff(__7Segment_Digit4);
		sevenSegWrite(digit2);
		__AnodeFour7SegmentOff(__7Segment_Digit4);
		__AnodeFour7SegmentOn(__7Segment_Digit3);

		//__AnodeFour7SegmentAllOff();
		//__AnodeFour7SegmentOn(__7Segment_Digit2);
		//__AnodeFour7SegmentOff(__7Segment_Digit3);
		sevenSegWrite(digit3);
		__AnodeFour7SegmentOff(__7Segment_Digit3);
		__AnodeFour7SegmentOn(__7Segment_Digit2);

		//__AnodeFour7SegmentAllOff();
		//__AnodeFour7SegmentOn(__7Segment_Digit1);
		//__AnodeFour7SegmentOff(__7Segment_Digit2);
		sevenSegWrite(digit4);
		__AnodeFour7SegmentOff(__7Segment_Digit2);
		__AnodeFour7SegmentOn(__7Segment_Digit1);
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

		if (localTimeMs >= 1000)
		{
			localTimeMs = 0;
			CallbackEverySecond();
		}


		// We need to clear line pending bit manually
		TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
	}
}

void CallbackEverySecond(void){

	digit1++;

	if (digit1 >= 10){
		digit1 = 0;
		digit2++;
	}
	if (digit2 >= 10){
		digit2 = 0;
		digit3++;
	}
	if (digit3 >= 10){
		digit3 = 0;
		digit4++;
	}
	if (digit4 >= 10){
		digit1 = 0;
		digit2 = 0;
		digit3 = 0;
		digit4 = 0;
	}
}

