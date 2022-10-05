/****************************************************************************
**
**  File        : main.c
**
**
**  The USART2 Configuration for the debug is:
**
**  	- BaudRate = 115200 baud
**  	- Word Length = 8 Bits
**  	- One Stop Bit
**  	- No parity
**  	- Hardware flow control disabled (RTS and CTS signals)
**
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include "arm_math.h"
#include ".\MFRC522\mfrc522.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// keys uid 65 37 F9 C2
// card uid 40 79 64 19

// Main program
int main(void) {
	// Recognized card ID
	uint8_t CardID[4];
	// My card id
	// I read them with program below, and write this here
	uint8_t MyID[4] = {
		0x65, 0x37, 0xF9, 0xC2	//My card on my keys
	};


	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// LIS302DL Sensor Initialization (GPIO and SPI1)
	MFRC522_Init();

	// Infinite loop 1
	while (1) {
		//If any card detected
		if (TM_MFRC522_Check(CardID) == MI_OK) {
			//CardID is valid

			//Check if this is my card
			if (TM_MFRC522_Compare(CardID, MyID) == MI_OK) {
				printf("Hello man!\r");
			} else {
				printf("Goob Bye man!\r");
			}

			//Print on LCD
			printf("Card detected\r");
			// Show ID card
			printf("0x%02x\n0x%02x\n0x%02x\n0x%02x\n0x%02x", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
		} else {
			printf("Card NOT detected\r");
		}
	}
}


