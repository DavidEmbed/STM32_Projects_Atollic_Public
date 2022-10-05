/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO reading distance from a ultrasonic sensor
**  (PD6, PD7).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\SR04\sr04.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Main program
int main(void) {
	float32_t dist;

	// Configures SysTick for delay functions
	Delay_Init();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Initialize SR04 Pins
	SR04Init(TRIG);
	SR04Init(ECHO);

	printf("Init Ok!\r");

	while(1){
		dist = ReadDistance();
		printf("Dist = %.2f cm\r", dist);
		// Suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
		Delay_ms(1000);
	}
}
