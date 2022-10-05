/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes the ADC1 to obtain the
**  potentiometers value connected to the PC2 and PC4, and digital
**  pin PC5 to control a joystick
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

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Functions definitions
void Pot1_GPIO_Config(void);
void Pot2_GPIO_Config(void);
void Switch_GPIO_Config(void);
void Pot_ADC1_Ch12_Init(void);
uint16_t Pot1_ReadValue(void);
uint16_t Pot2_ReadValue(void);
uint8_t SwitchGetState(void);

// Main program
int main(void) {
	uint8_t State;
	uint16_t potValue1, potValue2;
	float32_t potVoltage1, potVoltage2;

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// Configures the PC2 pin as analog input
	Pot1_GPIO_Config();

	// Configures the PC4 pin as analog input
	Pot2_GPIO_Config();

	// Configures the PC5 pin as digital input
	Switch_GPIO_Config();

	// Initializes the ADC1-Channel12 as single regular conversion mode
	Pot_ADC1_Ch12_Init();

	while (1) {

		// Do a ADC1-CH12 conversion and get the value
		potValue1 = Pot1_ReadValue();

		// Calculate the voltage input considering VREF+ = 3.0V
		potVoltage1 = (float32_t)potValue1 * (3.0f/1023.0f);

		// Do a ADC1-CH14 conversion and get the value
		potValue2 = Pot2_ReadValue();

		// Calculate the voltage input considering VREF+ = 3.0V
		potVoltage2 = (float32_t)potValue2 * (3.0f/1023.0f);

		// Get value switch state
		State = !SwitchGetState();

		printf("Potentiometer1 ADC value: %4u, Input voltage: %.2f\r", potValue1, potVoltage1);
		printf("Potentiometer2 ADC value: %4u, Input voltage: %.2f\r", potValue2, potVoltage2);
		printf("State switch: %u\r", State);

		// Wait 200 milliseconds
		Delay_ms(200);
	}
}

// Configures the PC2 pin as analog input
void Pot1_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Setup GPIO potentiometer1 pin (PC2) as analog mode.
	// GPIO_Speed, GPIO_OType not used in AN mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// Configures the PC4 pin as analog input
void Pot2_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Setup GPIO potentiometer1 pin (PC2) as analog mode.
	// GPIO_Speed, GPIO_OType not used in AN mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


// Configures the PC5 pin as digital input
void Switch_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	// Setup GPIO potentiometer1 pin (PC2) as analog mode.
	// GPIO_Speed, GPIO_OType not used in AN mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


// Get the switch state
uint8_t SwitchGetState(void) {
  return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}


// Initializes the ADC1-Channel12 as single regular conversion mode
void Pot_ADC1_Ch12_Init(void) {
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	//ADC1 Clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	// ADC Common Init
	//   - ADC1 and ADC2 operate independently
	//   - ADCCLK 84MHz/DIV2 = 42MHz
	//   - DMA dual/triple mode acces Disabled
	//   - 5 cycles delay between two diferent ADC samples (only in dual/triple mode)
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	// ADC1 Init
	//   - 12Bits Resolution,
	//   - Disable the multi-chanel (scan) conversion, Only one channel sequence
	//   - Don't do contimuous conversions - do them on demand
	//   - Start conversin by software, not an external trigger
	//   - Data align riht -> put 12-bit result in the lower variable bits
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_10b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);
}


// Get the ADC1-CH12 conversion value
uint16_t Pot1_ReadValue(void) {
	uint16_t value, i;

	// Select the channel 12 of the ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

	// Do an ADC-CH12 regular conversion
	// Average of 10 conversions
	value = 0;
	for (i=0; i<10; i++) {
		// Start the conversion
		ADC_SoftwareStartConv(ADC1);

		// Wait until conversion completion
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

		// Get conversion result (shift 4 bits for 12-bits value alignment)
		value = value + (ADC_GetConversionValue(ADC1) >> 6);

	}

	return value / 10;
}


// Get the ADC1-CH14 conversion value
uint16_t Pot2_ReadValue(void) {
	uint16_t value, i;

	// Select the channel 12 of the ADC1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);

	// Do an ADC-CH12 regular conversion
	// Average of 10 conversions
	value = 0;
	for (i=0; i<10; i++) {
		// Start the conversion
		ADC_SoftwareStartConv(ADC1);

		// Wait until conversion completion
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

		// Get conversion result (shift 4 bits for 12-bits value alignment)
		value = value + (ADC_GetConversionValue(ADC1) >> 6);

	}

	return value / 10;
}
