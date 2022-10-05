/*
 * stepper.c
 *
 *  Created on: Aug 21, 2017
 *      Author: David Martínez
 */

/* Includes ------------------------------------------------------------------*/
#include <Stepper/stepper.h>

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*	WIRE_GPIO_PORT[WIREn] = {WIRE_ORANGE_GPIO_PORT, WIRE_YELLOW_GPIO_PORT, 	WIRE_PINK_GPIO_PORT, 	WIRE_BLUE_GPIO_PORT};
const uint16_t	WIRE_GPIO_PIN[WIREn]  = {WIRE_ORANGE_PIN, 		WIRE_YELLOW_PIN, 		WIRE_PINK_PIN, 			WIRE_BLUE_PIN};
const uint32_t	WIRE_GPIO_CLK[WIREn]  = {WIRE_ORANGE_GPIO_CLK, 	WIRE_YELLOW_GPIO_CLK, 	WIRE_PINK_GPIO_CLK, 	WIRE_BLUE_GPIO_CLK};


// Initialization of the wires
void WireInit(Wire_TypeDef Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(WIRE_GPIO_CLK[Pin], ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = WIRE_GPIO_PIN[Pin];
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(WIRE_GPIO_PORT[Pin], &GPIO_InitStructure);
}


// Initialization of all wires
void WireInitAll(void) {
	uint8_t i;
	for (i = 0; i < WIREn; i++) WireInit(i);

}


// Wire On
void WireOn(Wire_TypeDef Pin) {
	GPIO_SetBits(WIRE_GPIO_PORT[Pin], WIRE_GPIO_PIN[Pin]);
}


// Wire Off
void WireOff(Wire_TypeDef Pin) {
	GPIO_ResetBits(WIRE_GPIO_PORT[Pin], WIRE_GPIO_PIN[Pin]);
}


// Write bit value
void WireValue(Wire_TypeDef Pin, uint8_t value) {
	GPIO_WriteBit(WIRE_GPIO_PORT[Pin], WIRE_GPIO_PIN[Pin], value);
}

