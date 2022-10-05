/****************************************************************************
**
**  File        : main.c
**
**  This example configures and initializes GPIO and SPI communication for control
**  a matrix LED with MAX7219
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
#include ".\MAX7219\max7219.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

void writeArduinoOnMatrix(void);
void rows(void);
void columns(void);
void single(void);

// Main program
int main(void) {

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// MAX7219 Initialization (GPIO and SPI1)
	MAX7219_Init();

	// Init matrix led
	MAX7219_PowerOff();
	MAX7219_PowerOn();
	MAX7219_DecodeMode(MAX7219_NoDecode);
	MAX7219_Intensity(DutyCicle31);
	MAX7219_ScanLimit(DisplayDigit0_1_2_3_4_5_6_7);
	MAX7219_DisplayTest(NormalOperation);
	ClearDisplay();

	// Infinite loop 1
	while (1) {
		ClearDisplay();
		writeArduinoOnMatrix();
		rows();
		columns();
		single();
	}
}


void writeArduinoOnMatrix(void){
  /* here is the data for the characters */
  uint8_t a[5]={0b01111110, 0b10001000, 0b10001000, 0b10001000, 0b01111110};
  uint8_t r[5]={0b00010000, 0b00100000, 0b00100000, 0b00010000, 0b00111110};
  uint8_t d[5]={0b11111110, 0b00010010, 0b00100010, 0b00100010, 0b00011100};
  uint8_t u[5]={0b00111110, 0b00000100, 0b00000010, 0b00000010, 0b00111100};
  uint8_t i[5]={0b00000000, 0b00000010, 0b10111110, 0b00100010, 0b00000000};
  uint8_t n[5]={0b00011110, 0b00100000, 0b00100000, 0b00010000, 0b00111110};
  uint8_t o[5]={0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b00011100};

  /* now display them one by one with a small delay */
  setRow(0,a[0]);
  setRow(1,a[1]);
  setRow(2,a[2]);
  setRow(3,a[3]);
  setRow(4,a[4]);
  Delay_ms(500);
  setRow(0,r[0]);
  setRow(1,r[1]);
  setRow(2,r[2]);
  setRow(3,r[3]);
  setRow(4,r[4]);
  Delay_ms(500);
  setRow(0,d[0]);
  setRow(1,d[1]);
  setRow(2,d[2]);
  setRow(3,d[3]);
  setRow(4,d[4]);
  Delay_ms(500);
  setRow(0,u[0]);
  setRow(1,u[1]);
  setRow(2,u[2]);
  setRow(3,u[3]);
  setRow(4,u[4]);
  Delay_ms(500);
  setRow(0,i[0]);
  setRow(1,i[1]);
  setRow(2,i[2]);
  setRow(3,i[3]);
  setRow(4,i[4]);
  Delay_ms(500);
  setRow(0,n[0]);
  setRow(1,n[1]);
  setRow(2,n[2]);
  setRow(3,n[3]);
  setRow(4,n[4]);
  Delay_ms(500);
  setRow(0,o[0]);
  setRow(1,o[1]);
  setRow(2,o[2]);
  setRow(3,o[3]);
  setRow(4,o[4]);
  Delay_ms(500);
  setRow(0,0);
  setRow(1,0);
  setRow(2,0);
  setRow(3,0);
  setRow(4,0);
  Delay_ms(500);
}


void rows(void){
	uint8_t row, i;

	for(row = 0; row < 8; row++) {
		Delay_ms(50);
		setRow(row,0b10100000);
		Delay_ms(50);
		setRow(row,0b00000000);

		for(i = 0; i < row; i++) {
			Delay_ms(50);
			setRow(row,0b10100000);
			Delay_ms(50);
			setRow(row, 0b00000000);
		}
	}
}


void columns(void){
	uint8_t col, i;

	for(col = 0; col < 8; col++) {
		Delay_ms(50);
		setColumn(col, 0b10100000);
		Delay_ms(50);
		setColumn(col, 0b00000000);

		for(i = 0; i < col; i++) {
			Delay_ms(50);
			setColumn(col, 0b10100000);
			Delay_ms(50);
			setColumn(col, 0b00000000);
		}
	}
}


void single(void){
	uint8_t row, col, i;

	for (row = 0; row < 8; row++){
		for (col = 0; col < 8; col++){
			Delay_ms(50);
			setLed(row, col, SET);
			Delay_ms(50);

			for(i = 0; i < col; i++) {
				setLed(row, col, RESET);
				Delay_ms(50);
				setLed(row, col, SET);
				Delay_ms(50);
			}
		}
	}
}

