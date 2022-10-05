/*
 * encoder.c
 *
 *  Created on: Aug 21, 2017
 *      Author: David Martínez
 */

/* Includes ------------------------------------------------------------------*/
#include <Encoder/encoder.h>

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*  PIN_PORT[PINn] 	= {CLK_PIN_GPIO_PORT, 	DT_PIN_GPIO_PORT };
const uint16_t PIN_PIN[PINn] 	= {CLK_PIN_PIN, 		DT_PIN_PIN };
const uint32_t PIN_CLK[PINn] 	= {CLK_PIN_GPIO_CLK, 	DT_PIN_GPIO_CLK };


// Initialization of Encoder pins
void EncoderPinInit(Pin_TypeDef Pin) {
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(PIN_CLK[Pin], ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = PIN_PIN[Pin];
	GPIO_Init(PIN_PORT[Pin], &GPIO_InitStructure);

	if (Pin == CLK_PIN){
		/* Enable the SYSCFG Clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

		/* Connect Button EXTI Line to Button GPIO Pin */
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);

		/* Configure Button EXTI line */
		EXTI_InitStructure.EXTI_Line = EXTI_Line7;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		/* Enable and set Button EXTI Interrupt to the lowest priority */
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

		NVIC_Init(&NVIC_InitStructure);
	}
}


// Get the encoder state
uint8_t EncoderPinGetState(Pin_TypeDef Pin) {
  return GPIO_ReadInputDataBit(PIN_PORT[Pin], PIN_PIN[Pin]);
}


// This function handles EXTI0 interrupt request.
void EXTI9_5_IRQHandler(void) {

	//Check if EXTI_Line0 is asserted
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) {

		// Do code here
		// ...

		//TODO: Get pulses from encoder

		// We need to clear line pending bit manually
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
