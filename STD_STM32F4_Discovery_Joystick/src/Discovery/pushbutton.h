/*
 * pushbutton.h
 *
 *  Created on: Oct 1, 2013
 *      Author: UdL
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include ".\led.h" // LED functions used in the EXTI0 interrupt handle

typedef enum {
	BUTTON_USER = 0,
} Button_TypeDef;

typedef enum {
	BUTTON_MODE_GPIO = 0,
	BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;


#define BUTTONn                          1  // Number of user pushbuttons

#define USER_BUTTON_PIN                GPIO_Pin_0
#define USER_BUTTON_GPIO_PORT          GPIOA
#define USER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define USER_BUTTON_EXTI_LINE          EXTI_Line0
#define USER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define USER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define USER_BUTTON_EXTI_IRQn          EXTI0_IRQn
#define USER_BUTTON_EXTI_IRQHandler    EXTI0_IRQHandler

/* Functions ------------------------------------------------------------------*/
void PushButtonInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint8_t PushButtonGetState(Button_TypeDef Button);

#ifdef __cplusplus
}
#endif

#endif /* PUSHBUTTON_H_ */
