/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for generate sound with a active buzzer
**  (PB13).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"

void Ext_LEDInit();

// Main program
int main(void) {
	uint8_t i;

	// Initialize gpio led
	Ext_LEDInit();

	while(1){

	   //output an frequency
	   for(i = 0; i < 10; i++)
	   {
		   GPIO_SetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(1);//wait for 1ms
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(1);//wait for 1ms
		}

		//output another frequency
		for(i = 0; i < 10; i++)
		{
		   GPIO_SetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(5);//wait for 1ms
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(5);//wait for 1ms
		}

		//output another frequency
		for(i = 0; i < 10; i++)
		{
		   GPIO_SetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(10);//wait for 1ms
		   GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		   Delay_ms(10);//wait for 1ms
		}
	}
}

// Initialization of the LED
void Ext_LEDInit() {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
