/*
 * __74hc595.c
 *
 *  Created on: Aug 19, 2017
 *      Author: David Martínez
 */

/* Includes ------------------------------------------------------------------*/
#include <74HC595/74hc595.h>

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*	__74HC595_GPIO_PORT[__7Segmentn] = {__74HC595_DS_GPIO_PORT, 	__74HC595_STCP_GPIO_PORT, 	__74HC595_SHCP_GPIO_PORT};
const uint16_t	__74HC595_GPIO_PIN[__7Segmentn]  = {__74HC595_DS_PIN, 		__74HC595_STCP_PIN, 		__74HC595_SHCP_PIN};
const uint32_t	__74HC595_GPIO_CLK[__7Segmentn]  = {__74HC595_DS_GPIO_CLK, 	__74HC595_STCP_GPIO_CLK, 	__74HC595_SHCP_GPIO_CLK};

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*	__7Segment_GPIO_PORT[__7Segmentn] = {__7Segment_Digit1_GPIO_PORT, 	__7Segment_Digit2_GPIO_PORT, 	__7Segment_Digit3_GPIO_PORT, 	__7Segment_Digit4_GPIO_PORT};
const uint16_t	__7Segment_GPIO_PIN[__7Segmentn]  = {__7Segment_Digit1_PIN, 		__7Segment_Digit2_PIN, 			__7Segment_Digit3_PIN, 			__7Segment_Digit4_PIN};
const uint32_t	__7Segment_GPIO_CLK[__7Segmentn]  = {__7Segment_Digit1_GPIO_CLK, 	__7Segment_Digit2_GPIO_CLK, 	__7Segment_Digit3_GPIO_CLK, 	__7Segment_Digit4_GPIO_CLK};

// Variable for turn on de leds of 7segment
uint8_t seven_seg_digits[10] = {0b11111100,  // = 0
                              	0b01100000,  // = 1
                                0b11011010,  // = 2
                                0b11110010,  // = 3
                                0b01100110,  // = 4
                                0b10110110,  // = 5
                                0b10111110,  // = 6
                                0b11100000,  // = 7
                                0b11111110,  // = 8
                                0b11100110}; // = 9


unsigned char table[]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};


// Initialization of the Pins
void __74HC595Init(__74HC595_TypeDef Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(__74HC595_GPIO_CLK[Pin], ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = __74HC595_GPIO_PIN[Pin];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(__74HC595_GPIO_PORT[Pin], &GPIO_InitStructure);
}


// High pin
void __74HC595High(__74HC595_TypeDef Pin) {
	GPIO_SetBits(__74HC595_GPIO_PORT[Pin], __74HC595_GPIO_PIN[Pin]);
}


// Low pin
void __74HC595Low(__74HC595_TypeDef Pin) {
	GPIO_ResetBits(__74HC595_GPIO_PORT[Pin], __74HC595_GPIO_PIN[Pin]);
}

void __74HC595WriteValue(__74HC595_TypeDef Pin, uint8_t value) {
	GPIO_WriteBit(__74HC595_GPIO_PORT[Pin], __74HC595_GPIO_PIN[Pin], value);
}


// Init all pins of 74HC595
void __74HC595InitAll(void){
	uint8_t i;

	for(i = 0; i < __74HC595n; i++){
		__74HC595Init(i);
	}
}


// Put data in registers bit per bit thought the clockPin
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST){
        	__74HC595WriteValue(dataPin, !!(val & (1 << i)));
        }

        else if (bitOrder == MSBFIRST) {
        	__74HC595WriteValue(dataPin, !!(val & (1 << (7 - i))));
        }

        __74HC595High(clockPin);
        __74HC595Low(clockPin);
    }
}


void sevenSegWrite(uint8_t digit){
	// Set the STCP pin to low, before sending data
	__74HC595Low(__74HC595_STCP);

	// Send data
	shiftOut(__74HC595_DS, __74HC595_SHCP, LSBFIRST, seven_seg_digits[digit]);

	// Set the STCP pin to high, after sending data
	__74HC595High(__74HC595_STCP);
}


void Display(unsigned char num){
	// Set the STCP pin to low, before sending data
	__74HC595Low(__74HC595_STCP);

	// Send data
	shiftOut(__74HC595_DS, __74HC595_SHCP, MSBFIRST, table[num]);

	// Set the STCP pin to high, after sending data
	__74HC595High(__74HC595_STCP);
}


// Initialization of the pins 7segment anode
void __AnodeFour7SegmentInit(__7SegmentDigit_TypeDef Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(__7Segment_GPIO_CLK[Pin], ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = __7Segment_GPIO_PIN[Pin];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(__7Segment_GPIO_PORT[Pin], &GPIO_InitStructure);
}


// High pin
void __AnodeFour7SegmentOff(__74HC595_TypeDef Pin) {
	GPIO_SetBits(__7Segment_GPIO_PORT[Pin], __7Segment_GPIO_PIN[Pin]);
}


// Low pin
void __AnodeFour7SegmentOn(__74HC595_TypeDef Pin) {
	GPIO_ResetBits(__7Segment_GPIO_PORT[Pin], __7Segment_GPIO_PIN[Pin]);
}

// All pins low
void __AnodeFour7SegmentAllOff(void) {
	uint8_t i;
	for (i = 0; i < __7Segmentn; i++) __AnodeFour7SegmentOff(i);
}

// All pins low
void __AnodeFour7SegmentAllOn(void) {
	uint8_t i;
	for (i = 0; i < __7Segmentn; i++) __AnodeFour7SegmentOn(i);
}


// Init all pins of 74HC595
void __AnodeFour7SegmentInitAll(void){
	uint8_t i;

	for(i = 0; i < __7Segmentn; i++){
		__AnodeFour7SegmentInit(i);
	}

	__AnodeFour7SegmentAllOn();
}
