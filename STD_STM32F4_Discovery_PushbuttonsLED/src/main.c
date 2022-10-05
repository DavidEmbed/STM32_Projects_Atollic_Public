/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO for turn on and turn off a led throught externals pushbuttons
**  (PB11, PB12, PB13).
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"

// Types of buttons
typedef enum {
	BUTTON_1 = 0,
	BUTTON_2 = 1,
} Ext_Button_TypeDef;

// Number of buttons
#define NumButton                   2  // Number of user pushbuttons

// Pins, ports and clk definitons
#define BUTTON_PIN_1                GPIO_Pin_11
#define BUTTON_GPIO_PORT_1          GPIOB
#define BUTTON_GPIO_CLK_1           RCC_AHB1Periph_GPIOB

#define BUTTON_PIN_2                GPIO_Pin_12
#define BUTTON_GPIO_PORT_2          GPIOB
#define BUTTON_GPIO_CLK_2           RCC_AHB1Periph_GPIOB

// Variables
GPIO_TypeDef*  EXT_BUTTON_PORT[NumButton] = {BUTTON_GPIO_PORT_1, BUTTON_GPIO_PORT_2};
const uint16_t EXT_BUTTON_PIN[NumButton] =  {BUTTON_PIN_1, BUTTON_PIN_2};
const uint32_t EXT_BUTTON_CLK[NumButton] = {BUTTON_GPIO_CLK_1, BUTTON_GPIO_CLK_2};

// Functions
void Ext_PushButtonInit(Ext_Button_TypeDef Button);
uint8_t Ext_PushButtonGetState(Ext_Button_TypeDef Button);
void Ext_LEDInit();

// Main program
int main(void) {

	// Initialize gpio pushbuttons
	Ext_PushButtonInit(BUTTON_1);
	Ext_PushButtonInit(BUTTON_2);

	// Initialize gpio led
	Ext_LEDInit();

	while(1){
		if (Ext_PushButtonGetState(BUTTON_1) == RESET){
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
		}

		else if (Ext_PushButtonGetState(BUTTON_2) == RESET){
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
		}
	}
}


// Initialization of PushButton
void Ext_PushButtonInit(Ext_Button_TypeDef Button) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(EXT_BUTTON_CLK[Button], ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = EXT_BUTTON_PIN[Button];
	GPIO_Init(EXT_BUTTON_PORT[Button], &GPIO_InitStructure);
}

// Get the Pushbutton state
uint8_t Ext_PushButtonGetState(Ext_Button_TypeDef Button) {
  return GPIO_ReadInputDataBit(EXT_BUTTON_PORT[Button], EXT_BUTTON_PIN[Button]);
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
