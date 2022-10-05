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
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Defines
#define MAX_LEN_CMD 			150

// Functions
void doTask(char cmd[]);

// Main program
int main(void) {
	char buffer[MAX_LEN_CMD];
	uint8_t state;
	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Get state of pushbutton
	state = PushButtonGetState(BUTTON_USER);

	// Initialization is okey
	printf("Init Ok!\r");

	if (state == 0){
		while (1) {

			// Mirem si hi ha dada pel port Serie
			if (USARTx_isDataRecived() == SET) {

				// Get USART data:
				USARTx_GetStr(buffer);

				// El contingut del paquet es un string acabat amb \0
				printf("Data: --%s--\r", buffer);

				// Executar comanda
				doTask(&buffer[0]);
			}
		}
	}

	else if (state == 1){

		while (1){
			printf("One by One LED\r");
			// On led one by one
			LEDOn(LED_GREEN);
			Delay_ms(1000);

			LEDOn(LED_ORANGE);
			Delay_ms(1000);

			LEDOn(LED_RED);
			Delay_ms(1000);

			LEDOn(LED_BLUE);
			Delay_ms(1000);

			printf("ON all LED\r");
			// Off led one by one
			LEDOff(LED_GREEN);
			Delay_ms(1000);

			LEDOff(LED_ORANGE);
			Delay_ms(1000);

			LEDOff(LED_RED);
			Delay_ms(1000);

			LEDOff(LED_BLUE);
			Delay_ms(1000);

			printf("Off all LED\r");
			// On led all
			LEDOnAll();
			Delay_ms(1000);

			// Off led all
			LEDOffAll();

			printf("Reset led...\r");
			Delay_ms(3000);
		}
	}
}


// Function to execute the commands
void doTask(char cmd[]) {
	uint8_t numLed;

	// LEDOX\0 - Encendre led
	if(cmd[0] == 'L' && cmd[1] == 'E' && cmd[2] == 'D' && cmd[3] == 'O' && AreNumbers(&cmd[4], 1) && cmd[5] == '\0') {
		numLed = 1*(cmd[4] - '0');
		LEDOn(numLed);
		printf("Led On. Ok!\r");
	}

	// LEDOX\0 - Apagar led
	else if (cmd[0] == 'L' && cmd[1] == 'E' && cmd[2] == 'D' && cmd[3] == 'F' && AreNumbers(&cmd[4], 1) && cmd[5] == '\0'){
		numLed = 1*(cmd[4] - '0');
		LEDOff(numLed);
		printf("Led Off. Ok!\r");
	}

	// Comanda no valida
	else{
		printf("No valid command!\r");
	}
}


