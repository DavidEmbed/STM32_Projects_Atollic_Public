/*
 * keypad.c
 *
 *  Created on: Aug 7, 2017
 *      Author: David Martinez
 */

/* Includes ------------------------------------------------------------------*/
#include <Keypad/keypad.h>

/* Variables -----------------------------------------------------------------*/
// Column
GPIO_TypeDef*	COLUMN_GPIO_PORT[ColumnsN] = {KEYPAD_COLUMN_1_GPIO_PORT, 	KEYPAD_COLUMN_2_GPIO_PORT, 	KEYPAD_COLUMN_3_GPIO_PORT, 	KEYPAD_COLUMN_4_GPIO_PORT};
const uint16_t	COLUMN_GPIO_PIN[ColumnsN]  = {KEYPAD_COLUMN_1_PIN, 			KEYPAD_COLUMN_2_PIN, 		KEYPAD_COLUMN_3_PIN, 		KEYPAD_COLUMN_4_PIN};
const uint32_t	COLUMN_GPIO_CLK[ColumnsN]  = {KEYPAD_COLUMN_1_GPIO_CLK,		KEYPAD_COLUMN_2_GPIO_CLK, 	KEYPAD_COLUMN_3_GPIO_CLK, 	KEYPAD_COLUMN_4_GPIO_CLK};

// Row
GPIO_TypeDef*	ROW_GPIO_PORT[RowsN] = {KEYPAD_ROW_1_GPIO_PORT, KEYPAD_ROW_2_GPIO_PORT, KEYPAD_ROW_3_GPIO_PORT, KEYPAD_ROW_4_GPIO_PORT};
const uint16_t	ROW_GPIO_PIN[RowsN]  = {KEYPAD_ROW_1_PIN, 		KEYPAD_ROW_2_PIN, 		KEYPAD_ROW_3_PIN, 		KEYPAD_ROW_4_PIN};
const uint32_t	ROW_GPIO_CLK[RowsN]  = {KEYPAD_ROW_1_GPIO_CLK, 	KEYPAD_ROW_2_GPIO_CLK, 	KEYPAD_ROW_3_GPIO_CLK, 	KEYPAD_ROW_4_GPIO_CLK};

uint8_t Keypad_Buttons[4][4] = {
	{1,  2,  3,  12},
	{4,  5,  6,  13},
	{7,  8,  9,  14},
	{10, 0,  11, 15},
};

Keypad_Button_TypeDef KeypadStatus = KEYPAD_Button_NOPRESSED;
uint32_t RefTimeMs = 0;

// Initialization columns pins of keypad as output
void ColumnsInit(Keypad_Column_TypeDef column) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO Clock */
	  RCC_AHB1PeriphClockCmd(COLUMN_GPIO_CLK[column], ENABLE);

	  /* Configure the GPIO pin */
	  GPIO_InitStructure.GPIO_Pin = COLUMN_GPIO_PIN[column];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	  GPIO_Init(COLUMN_GPIO_PORT[column], &GPIO_InitStructure);
}


// Initialization rows pins of keypad as input
void RowsInit(Keypad_Row_TypeDef row) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO Clock */
	  RCC_AHB1PeriphClockCmd(ROW_GPIO_CLK[row], ENABLE);

	  /* Configure the GPIO pin */
	  GPIO_InitStructure.GPIO_Pin = ROW_GPIO_PIN[row];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	  GPIO_Init(ROW_GPIO_PORT[row], &GPIO_InitStructure);
}


void KeypadInit(void){
	uint8_t i;

	// Initialize all columns pins
	for (i = 0; i < ColumnsN; i++){
		ColumnsInit(i);
	}

	// Initialize all rows pins
	for (i = 0; i < RowsN; i++){
		RowsInit(i);
	}

	for (i = 0; i < ColumnsN; i++){
		ColumnOn(i);
	}
}


// Switch the Column On
void ColumnOn(Keypad_Column_TypeDef column) {
	GPIO_SetBits(COLUMN_GPIO_PORT[column], COLUMN_GPIO_PIN[column]);
}

// Switch the Column Off
void ColumnOff(Keypad_Row_TypeDef row) {
	GPIO_ResetBits(COLUMN_GPIO_PORT[row], COLUMN_GPIO_PIN[row]);
}


// Read row state
uint8_t ReadRowState(Keypad_Row_TypeDef row) {
  return GPIO_ReadInputDataBit(ROW_GPIO_PORT[row], ROW_GPIO_PIN[row]);
}


uint8_t CheckRow(Keypad_Column_TypeDef column) {

	// Scan row 1
	if (!ReadRowState(KEYPAD_ROW_1)) {
		return Keypad_Buttons[KEYPAD_ROW_1][column];
	}
	// Scan row 2
	if (!ReadRowState(KEYPAD_ROW_2)) {
		return Keypad_Buttons[KEYPAD_ROW_2][column];
	}
	// Scan row 3
	if (!ReadRowState(KEYPAD_ROW_3)) {
		return Keypad_Buttons[KEYPAD_ROW_3][column];
	}
	// Scan row 4
	if (!ReadRowState(KEYPAD_ROW_4)) {
		return Keypad_Buttons[KEYPAD_ROW_4][column];
	}

	// Not pressed
	return KEYPAD_NO_PRESSED;
}


uint8_t ReadValueKeypad(void) {
	uint8_t check;

	// Set column 1 to LOW
	ColumnOff(KEYPAD_COLUMN_1);
	ColumnOn(KEYPAD_COLUMN_2);
	ColumnOn(KEYPAD_COLUMN_3);
	ColumnOn(KEYPAD_COLUMN_4);

	// Check rows
	check = CheckRow(KEYPAD_COLUMN_1);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}


	// Set column 2 to LOW
	ColumnOn(KEYPAD_COLUMN_1);
	ColumnOff(KEYPAD_COLUMN_2);
	ColumnOn(KEYPAD_COLUMN_3);
	ColumnOn(KEYPAD_COLUMN_4);

	// Check rows
	check = CheckRow(KEYPAD_COLUMN_2);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}


	// Set column 3 to LOW
	ColumnOn(KEYPAD_COLUMN_1);
	ColumnOn(KEYPAD_COLUMN_2);
	ColumnOff(KEYPAD_COLUMN_3);
	ColumnOn(KEYPAD_COLUMN_4);

	// Check rows
	check = CheckRow(KEYPAD_COLUMN_3);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}


	// Set column 4 to LOW
	ColumnOn(KEYPAD_COLUMN_1);
	ColumnOn(KEYPAD_COLUMN_2);
	ColumnOn(KEYPAD_COLUMN_3);
	ColumnOff(KEYPAD_COLUMN_4);

	// Check rows
	check = CheckRow(KEYPAD_COLUMN_4);
	if (check != KEYPAD_NO_PRESSED) {
		return check;
	}

	// Not pressed
	return KEYPAD_NO_PRESSED;
}


void Keypad_Update(uint32_t TimeMs) {

	// Every X ms read
	if ((TimeMs - RefTimeMs) >= KEYPAD_READ_INTERVAL && KeypadStatus == KEYPAD_Button_NOPRESSED) {

		// Read keyboard
		KeypadStatus = ReadValueKeypad();

		// Reset
		RefTimeMs = TimeMs;
	}
}


Keypad_Button_TypeDef ReadButtonKeypad(void){
	Keypad_Button_TypeDef temp;

	// Get keypad status
	temp = KeypadStatus;

	// Reset keypad status
	KeypadStatus = KEYPAD_Button_NOPRESSED;

	return temp;
}


