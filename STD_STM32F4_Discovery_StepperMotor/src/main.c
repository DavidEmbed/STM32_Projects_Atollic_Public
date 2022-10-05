/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for control the 4 wires of a stepper motor
**  (PE3, PE4, PE5, PE6).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\Stepper\stepper.h"
#include ".\Encoder\encoder.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);


uint8_t Paso[8][4] = {  {1, 0, 0, 0},
						{1, 1, 0, 0},
						{0, 1, 0, 0},
						{0, 1, 1, 0},
						{0, 0, 1, 0},
						{0, 0, 1, 1},
						{0, 0, 0, 1},
						{1, 0, 0, 1}};


// Variables stepper motor
uint16_t steps_left = 4095;
uint8_t Direction = SET;
uint8_t Steps = 0;


// Variables encoder
extern uint8_t TurnDetected;
extern uint8_t direction;
extern uint8_t encoderPosCount;


void stepper(void);
void SetDirection(void);


// Main program
int main(void) {

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// Initialize Discovery button board
	PushButtonInit(BUTTON_USER, BUTTON_MODE_GPIO);

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Init all wires for step motor
	WireInitAll();

	// Init encoder pins
	EncoderPinInit(CLK_PIN);
	EncoderPinInit(DT_PIN);

	//pinCLKLast = EncoderPinGetState(CLK_PIN);

	// Initialization is okey
	printf("Init Ok!\r");


	while (1) {
// Example for stepper motor
/*
		while (steps_left > 0){
			stepper();
			steps_left--;
			Delay_ms(1);
		}
		Delay_ms(300);
		Direction = !Direction;
		steps_left = 4095;
	}
*/

// Example for encoder
/*
		CLKval = EncoderPinGetState(CLK_PIN);
		if (CLKval != pinCLKLast){

			DTval = EncoderPinGetState(DT_PIN);

			if (DTval != CLKval){
				encoderPosCount++;
				direction = SET;
			}
			else{
				encoderPosCount--;
				direction = RESET;
			}

			if (direction){
				printf("Rotate counterclockwise\r");
			}
			else{
				printf("Rotate clockwise\r");
			}
			printf("Encoder position: %u\r", encoderPosCount);
		}
		pinCLKLast = CLKval;

*/
	}
}


// Realitzar un pas del motor
void stepper(void){

	WireValue(WIRE_ORANGE, Paso[Steps][0]);
	WireValue(WIRE_YELLOW, Paso[Steps][1]);
	WireValue(WIRE_PINK, Paso[Steps][2]);
	WireValue(WIRE_BLUE, Paso[Steps][3]);

	SetDirection();
}

// Incrementar o disminuir la variable per recorrer les bobines en un sentit o en el contrari
void SetDirection(void){
	if (Direction) Steps++;
	else Steps--;

	// Asegurar variable Steps esta entre 0 i 7
	Steps = (Steps + 8) % 8;
}

