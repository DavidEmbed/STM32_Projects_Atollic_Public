/*
 * lcd1602.c
 *
 *  Created on: Aug 16, 2017
 *      Author: David Martínez
 */

// Includes
#include <LCD1602/lcd1602.h>

// Variables
GPIO_TypeDef*	LCD1602_GPIO_PORT[PINn] = {LCD1602_DB7_GPIO_PORT, 	LCD1602_DB6_GPIO_PORT, 	LCD1602_DB5_GPIO_PORT, 	LCD1602_DB4_GPIO_PORT, 	LCD1602_E_GPIO_PORT, 	LCD1602_RS_GPIO_PORT};
const uint16_t	LCD1602_GPIO_PIN[PINn]  = {LCD1602_DB7_PIN, 		LCD1602_DB6_PIN, 		LCD1602_DB5_PIN, 		LCD1602_DB4_PIN, 		LCD1602_E_PIN, 			LCD1602_RS_PIN};
const uint32_t	LCD1602_GPIO_CLK[PINn]  = {LCD1602_DB7_GPIO_CLK, 	LCD1602_DB6_GPIO_CLK, 	LCD1602_DB5_GPIO_CLK, 	LCD1602_DB4_GPIO_CLK, 	LCD1602_E_GPIO_CLK, 	LCD1602_RS_GPIO_CLK};


// Variable struct
LCD1602_Options_TypeDef LCD1602_Options;


// Initialization of the LCD
void LCD1602InitPins(LCD1602Pin_TypeDef Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(LCD1602_GPIO_CLK[Pin], ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = LCD1602_GPIO_PIN[Pin];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(LCD1602_GPIO_PORT[Pin], &GPIO_InitStructure);
}


// Switch the Pin high
void LCD1602PinHigh(LCD1602Pin_TypeDef Pin) {
	GPIO_SetBits(LCD1602_GPIO_PORT[Pin], LCD1602_GPIO_PIN[Pin]);
}


// Switch the Pin low
void LCD1602PinLow(LCD1602Pin_TypeDef Pin) {
	GPIO_ResetBits(LCD1602_GPIO_PORT[Pin], LCD1602_GPIO_PIN[Pin]);
}


// Set pin value
void LCD1602SetPinValue(LCD1602Pin_TypeDef Pin, uint8_t value){
	GPIO_WriteBit(LCD1602_GPIO_PORT[Pin], LCD1602_GPIO_PIN[Pin], value);
}


// Init LCD
void LCD1602Init(uint8_t cols, uint8_t rows){
	uint8_t i;

	// Initialize all pins
	for(i = 0; i < PINn; i++){
		LCD1602InitPins(i);
	}

	// // Set all pins to low
	for(i = 0; i < PINn; i++){
		LCD1602PinLow(i);
	}

	// At least 40ms
	Delay_ms(45);

	// Set LCD width and height
	LCD1602_Options.Rows = rows;
	LCD1602_Options.Cols = cols;

	/* Set cursor pointer to beginning for LCD */
	LCD1602_Options.currentX = 0;
	LCD1602_Options.currentY = 0;

	// Joint bit options of display function
	LCD1602_Options.DisplayFunction = LCD1602_4BITMODE | LCD1602_5x8DOTS | LCD1602_1LINE;
	if (rows > 1) {
		LCD1602_Options.DisplayFunction |= LCD1602_2LINE;
	}

	// Try to set 4bit mode
	LCD1602_Cmd4bit(0x03);
	Delay_us(4500);

	// Second try
	LCD1602_Cmd4bit(0x03);
	Delay_us(4500);

	// Third goo!
	LCD1602_Cmd4bit(0x03);
	Delay_us(4500);

	// Set 4-bit interface
	LCD1602_Cmd4bit(0x02);
	Delay_us(100);

	// Set # lines, font size, etc.
	LCD1602_Cmd(LCD1602_FUNCTIONSET | LCD1602_Options.DisplayFunction);

	// Turn the display on with no cursor or blinking default
	LCD1602_Options.DisplayControl = LCD1602_DISPLAYON;
	LCD1602_DisplayOn();

	// Clear lcd
	LCD1602_Clear();

	// Default font directions
	LCD1602_Options.DisplayMode = LCD1602_ENTRYLEFT | LCD1602_ENTRYSHIFTDECREMENT;
	LCD1602_Cmd(LCD1602_ENTRYMODESET | LCD1602_Options.DisplayMode);

	// Delay
	Delay_us(4500);
}


void LCD1602_Clear(void){
	LCD1602_Cmd(LCD1602_CLEARDISPLAY);
	Delay_ms(3);
}


void LCD1602_DisplayOn(void) {
	LCD1602_Options.DisplayControl |= LCD1602_DISPLAYON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_DisplayOff(void) {
	LCD1602_Options.DisplayControl &= ~LCD1602_DISPLAYON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_BlinkOn(void) {
	LCD1602_Options.DisplayControl |= LCD1602_BLINKON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_BlinkOff(void) {
	LCD1602_Options.DisplayControl &= ~LCD1602_BLINKON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_CursorOn(void) {
	LCD1602_Options.DisplayControl |= LCD1602_CURSORON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_CursorOff(void) {
	LCD1602_Options.DisplayControl &= ~LCD1602_CURSORON;
	LCD1602_Cmd(LCD1602_DISPLAYCONTROL | LCD1602_Options.DisplayControl);
}


void LCD1602_ScrollLeft(void) {
	LCD1602_Cmd(LCD1602_CURSORSHIFT | LCD1602_DISPLAYMOVE | LCD1602_MOVELEFT);
}


void LCD1602_ScrollRight(void) {
	LCD1602_Cmd(LCD1602_CURSORSHIFT | LCD1602_DISPLAYMOVE | LCD1602_MOVERIGHT);
}


void LCD1602_Cmd(uint8_t cmd) {
	// Command mode
	LCD1602PinLow(LCD1602_RS);

	// High nibble
	LCD1602_Cmd4bit(cmd >> 4);

	// Low nibble
	LCD1602_Cmd4bit(cmd & 0x0F);
}


void LCD1602_Data(uint8_t data) {
	// Data mode
	LCD1602PinHigh(LCD1602_RS);

	// High nibble byte
	LCD1602_Cmd4bit(data >> 4);

	// Low nibble byte
	LCD1602_Cmd4bit(data & 0x0F);
}


void LCD1602_Cmd4bit(uint8_t cmd) {
	// Set output port
	LCD1602SetPinValue(LCD1602_DB7, (cmd & 0x08));
	LCD1602SetPinValue(LCD1602_DB6, (cmd & 0x04));
	LCD1602SetPinValue(LCD1602_DB5, (cmd & 0x02));
	LCD1602SetPinValue(LCD1602_DB4, (cmd & 0x01));

	LCD1602PinHigh(LCD1602_E);
	Delay_us(20);
	LCD1602PinLow(LCD1602_E);
	Delay_us(20);
}


void LCD1602_CursorSet(uint8_t col, uint8_t row) {
	uint8_t row_offsets[] = {0x00, 0x40};

	// Go to beginning
	if (row >= LCD1602_Options.Rows) {
		row = 0;
	}

	// Set current column and row
	LCD1602_Options.currentX = col;
	LCD1602_Options.currentY = row;

	// Set location address
	LCD1602_Cmd(LCD1602_SETDDRAMADDR | (col + row_offsets[row]));
}


void LCD1602_Puts(uint8_t x, uint8_t y, char* str) {
	LCD1602_CursorSet(x, y);
	while (*str) {
		if (LCD1602_Options.currentX >= LCD1602_Options.Cols) {
			LCD1602_Options.currentX = 0;
			LCD1602_Options.currentY++;
			LCD1602_CursorSet(LCD1602_Options.currentX, LCD1602_Options.currentY);
		}
		if (*str == '\n') {
			LCD1602_Options.currentY++;
			LCD1602_CursorSet(LCD1602_Options.currentX, LCD1602_Options.currentY);
		} else if (*str == '\r') {
			LCD1602_CursorSet(0, LCD1602_Options.currentY);
		} else {
			LCD1602_Data(*str);
			//LCD1602_Options.currentX++;
		}
		str++;
	}
}


// Create custom char. We have 8 locations available for custom characters
void LCD1602_CreateChar(uint8_t location, uint8_t *data) {
	uint8_t i;

	location &= 0x07;
	LCD1602_Cmd(LCD1602_SETCGRAMADDR | (location << 3));

	for (i = 0; i < 8; i++) {
		LCD1602_Data(data[i]);
	}
}


// Show the custom character
void LCD1602_PutCustom(uint8_t x, uint8_t y, uint8_t location) {
	LCD1602_CursorSet(x, y);
	LCD1602_Data(location);
}


