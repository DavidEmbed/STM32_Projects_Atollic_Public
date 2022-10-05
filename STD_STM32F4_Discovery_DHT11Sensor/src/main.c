/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for read temperature and humidity sensor
**  (PB8).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\DHT11\dht11.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Main program
int main(void) {

	uint8_t data[5] = {0};

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Initialize DHT11 Sensor and wait 1 sec for pass the unstable state
	DHT11Init(DHT11_DATA, DHT11_INPUT);
	Delay_ms(1000);

	// Initialization is okey
	printf("Init Ok!\r");


	while (1) {
		if (DHT11_Read(data) == DHT11_OK){
			printf("DHT11 data: %u.%u%%, %u.%uC\r", data[0], data[1], data[2], data[3]);
		}
		else{
			printf("DHT11: Error on read\r");
		}

		Delay_ms(1000);
	}
}





