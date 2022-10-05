/*
 * sr04.c
 *
 *  Created on: Aug 5, 2017
 *      Author: David Martinez
 */

/* Includes ------------------------------------------------------------------*/
#include <SR04/sr04.h>

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*	SR04_GPIO_PORT[SR04n] = {TRIG_GPIO_PORT, ECHO_GPIO_PORT};
const uint16_t	SR04_GPIO_PIN[SR04n]  = {TRIG_PIN, ECHO_PIN};
const uint32_t	SR04_GPIO_CLK[SR04n]  = {TRIG_GPIO_CLK, ECHO_GPIO_CLK};


// Initialization of the SR04 pin
void SR04Init(SR04Pin_TypeDef Sr04Pin) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_SR04 Clock */
	  RCC_AHB1PeriphClockCmd(SR04_GPIO_CLK[Sr04Pin], ENABLE);

	  /* Configure the GPIO_SR04 pin */
	  GPIO_InitStructure.GPIO_Pin = SR04_GPIO_PIN[Sr04Pin];

	  if (Sr04Pin == TRIG){
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  }
	  else if (Sr04Pin == ECHO){
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  }
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	  GPIO_Init(SR04_GPIO_PORT[Sr04Pin], &GPIO_InitStructure);
}


// SR04 Trig or Echo pin high
void SR04High(SR04Pin_TypeDef Sr04Pin) {
	GPIO_SetBits(SR04_GPIO_PORT[Sr04Pin], SR04_GPIO_PIN[Sr04Pin]);
}

// SR04 Trig or Echo pin low
void Sr04Low(SR04Pin_TypeDef Sr04Pin) {
	GPIO_ResetBits(SR04_GPIO_PORT[Sr04Pin], SR04_GPIO_PIN[Sr04Pin]);
}


// Get the Echo pin state
uint8_t EchoGetState(SR04Pin_TypeDef Sr04Pin) {
  return GPIO_ReadInputDataBit(SR04_GPIO_PORT[Sr04Pin], SR04_GPIO_PIN[Sr04Pin]);
}


float32_t ReadDistance(){
	uint32_t time, timeout;
	float32_t distance;

	// Trigger low
	Sr04Low(TRIG);
	Delay_us(2);
	// Trigger high for 10us
	SR04High(TRIG);
	Delay_us(10);
	// Trigger low
	Sr04Low(TRIG);

	// Give some time for response
	timeout = HCSR04_TIMEOUT;
	while (!EchoGetState(ECHO)){
		timeout--;
		if (timeout == 0){
			return -1;
		}
	}

	// Start time
	time = 0;
	while (EchoGetState(ECHO)){
		time++;
		Delay_us(1);
	}

	// Convert us to cm
	distance = (float)time * HCSR04_NUMBER * 2;

	// Return distance in cm
	return distance;
}
