/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for control 4 led blink
**  (PD12, PD13, PD14, PD15).
**
**  Two modes:
**
**  - Pushbutton 0: Control leds throught commands to turn on or turn off the leds
**
**  - Pushbutton 1: Execute a sequence of blink leds
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\LCD1602\lcd1602.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Main program
int main(void) {

	// Custom character
    uint8_t customChar[] = {
        0x1F,   // xxx 11111
        0x11,   // xxx 10001
        0x11,   // xxx 10001
        0x11,   // xxx 10001
        0x11,   // xxx 10001
        0x11,   // xxx 10001
        0x11,   // xxx 10001
        0x1F	// xxx 11111
    };

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Init LCD
	LCD1602Init(16, 2);

	// Save custom character in location 0
	LCD1602_CreateChar(0, &customChar[0]);

	// Shows in display "Hola!"
	LCD1602_Puts(0,0, " La pantalla\n funciona!");

	// Wait 3 seconds
	Delay_ms(3000);

	// Show in display the custom character
	LCD1602_PutCustom(14, 1, 0);

	// Initialization is okey
	printf("Init Ok!\r");

	while (1) {

	}
}




