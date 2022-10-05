/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO pin for detect movement with a motion sensor
**  (PB7).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"


// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

void HCSR501PinInit(void);
uint8_t HCSR501GetState(void);

// Main program
int main(void) {
	uint8_t state;

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Pin HCSR501 sensor initialization
	HCSR501PinInit() ;
	LEDOff(LED_RED);

	// Initialization is okey
	printf("Init Ok!\r");

	while (1) {
		state = HCSR501GetState();
		if (state == RESET){
			LEDOff(LED_RED);
		}
		else if (state == SET){
			LEDOn(LED_RED);
		}
	}
}


// Initialization of the PB7 pin
void HCSR501PinInit(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// Get the PB7 pin state
uint8_t HCSR501GetState(void) {
  return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
}
