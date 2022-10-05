/*
 * lcd1602.h
 *
 *  Created on: Aug 16, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef LCD1602_H_
#define LCD1602_H_


#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"
#include ".\SysTick\delay.h"

// Pin description
// 1 	GND		GND				Ground
// 2 	VCC		+5V				Power supply for LCD
// 3 	V0		Potentiometer	Contrast voltage. Connect to potentiometer
// 4 	RS		PB7				Register select, can be overwritten in your project's defines.h file
// 5 	RW		GND				Read/write
// 6 	E		PB5				Enable pin, can be overwritten in your project's defines.h file
// 7 	DB0		-				Data 0 - doesn't care
// 8 	DB1		-				Data 1 - doesn't care
// 9 	DB2		-				Data 2 - doesn't care
// 10 	DB3		-				Data 3 - doesn't  care
// 11 	DB4		PD2				Data 4, can be overwritten in your project's defines.h file
// 12 	DB5		PD0				Data 5, can be overwritten in your project's defines.h file
// 13 	DB6		PC11			Data 6, can be overwritten in your project's defines.h file
// 14 	DB7		PA15			Data 7, can be overwritten in your project's defines.h file
// 15 	A		+3V3			Back light positive power
// 16 	K		GND				Ground for back light


typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
} LCD1602_Options_TypeDef;


typedef enum  {
	LCD1602_DB7		= 0,
	LCD1602_DB6		= 1,
	LCD1602_DB5		= 2,
	LCD1602_DB4		= 3,
	LCD1602_E		= 4,
	LCD1602_RS		= 5
} LCD1602Pin_TypeDef;


#define PINn                             	6  // Number of pins

#define LCD1602_DB7_PIN						GPIO_Pin_15
#define LCD1602_DB7_GPIO_PORT               GPIOA
#define LCD1602_DB7_GPIO_CLK                RCC_AHB1Periph_GPIOA

#define LCD1602_DB6_PIN                     GPIO_Pin_11
#define LCD1602_DB6_GPIO_PORT               GPIOC
#define LCD1602_DB6_GPIO_CLK                RCC_AHB1Periph_GPIOC

#define LCD1602_DB5_PIN                     GPIO_Pin_0
#define LCD1602_DB5_GPIO_PORT               GPIOD
#define LCD1602_DB5_GPIO_CLK                RCC_AHB1Periph_GPIOD

#define LCD1602_DB4_PIN                     GPIO_Pin_2
#define LCD1602_DB4_GPIO_PORT               GPIOD
#define LCD1602_DB4_GPIO_CLK                RCC_AHB1Periph_GPIOD

#define LCD1602_E_PIN                       GPIO_Pin_5
#define LCD1602_E_GPIO_PORT                 GPIOB
#define LCD1602_E_GPIO_CLK                  RCC_AHB1Periph_GPIOB

#define LCD1602_RS_PIN                      GPIO_Pin_7
#define LCD1602_RS_GPIO_PORT                GPIOB
#define LCD1602_RS_GPIO_CLK                 RCC_AHB1Periph_GPIOB


// Commands
#define LCD1602_CLEARDISPLAY        0x01 // Bit d'activacio per la comanda de netejar la pantalla
#define LCD1602_RETURNHOME          0x02 // Bit d'activacio per la comanda de situar el punter escriptura a la posicio inicial
#define LCD1602_ENTRYMODESET        0x04 // Bit d'activacio per la comanda d'establir la direccio de desplazament de les adresses de la DDRAM
#define LCD1602_DISPLAYCONTROL      0x08 // Bit d'activacio per la comanda del control de la pantalla
#define LCD1602_CURSORSHIFT         0x10 // Bit d'activacio per la comanda d'establir la direccio de desplazament del cursor
#define LCD1602_FUNCTIONSET         0x20 // Bit d'activacio per la comanda d'establir el mode de 4 o 8 bits
#define LCD1602_SETCGRAMADDR        0x40 //
#define LCD1602_SETDDRAMADDR        0x80 // Bit d'activacio per la comanda per establir la adreça de la DDRAM

// Flags for display entry mode
#define LCD1602_ENTRYRIGHT          0x00 // Quan s'introdueix un caracter desplazar a la dreta automaticament el punter on escriure el seguent caracter
#define LCD1602_ENTRYLEFT           0x02 // Quan s'introdueix un caracter desplazar a la esquerra automaticament el punter on escriure el seguent caracter
#define LCD1602_ENTRYSHIFTINCREMENT 0x01 // Desplazar tota la pantalla al escriure un caracter (desplazar 16 vegades)
#define LCD1602_ENTRYSHIFTDECREMENT 0x00 // No desplazar tota la pantalla al escriure un caracter

// Flags for display on/off control
#define LCD1602_DISPLAYON           0x04 // Bit per activar el display // Per desactivar -> negar el bit
#define LCD1602_CURSORON            0x02 // Bit per activar el cursor // Per desactivar -> negar el bit
#define LCD1602_BLINKON             0x01 // Bit per activar el parpadeig del cursor // Per desactivar -> negar el bit

// Flags for display/cursor shift
#define LCD1602_DISPLAYMOVE         0x08
#define LCD1602_CURSORMOVE          0x00
#define LCD1602_MOVERIGHT           0x04
#define LCD1602_MOVELEFT            0x00

// Flags for function set
#define LCD1602_8BITMODE            0x10 // Escriure dades utilitzant 8 cables
#define LCD1602_4BITMODE            0x00 // Escriure dades utilitzant 4 cables
#define LCD1602_2LINE               0x08 // Habilitar 2 files d'escriptura
#define LCD1602_1LINE               0x00 // Habilitar 1 fila d'escriptura
#define LCD1602_5x10DOTS            0x04 // Habilitar escriptura 5x10 punts en cada posicio de fila x columna
#define LCD1602_5x8DOTS             0x00 // Habilitar escriptura 5x8 punts en cada posicio de fila x columna



// Functions
void LCD1602InitPins(LCD1602Pin_TypeDef Pin);
void LCD1602PinHigh(LCD1602Pin_TypeDef Pin);
void LCD1602PinLow(LCD1602Pin_TypeDef Pin);
void LCD1602SetPinValue(LCD1602Pin_TypeDef Pin, uint8_t value);
void LCD1602Init(uint8_t cols, uint8_t rows);
void LCD1602_Clear(void);
void LCD1602_DisplayOn(void);
void LCD1602_DisplayOff(void);
void LCD1602_BlinkOn(void);
void LCD1602_BlinkOff(void);
void LCD1602_CursorOn(void);
void LCD1602_CursorOff(void);
void LCD1602_ScrollLeft(void);
void LCD1602_ScrollRight(void);
void LCD1602_Cmd(uint8_t cmd);
void LCD1602_Data(uint8_t data);
void LCD1602_Cmd4bit(uint8_t cmd);
void LCD1602_CursorSet(uint8_t col, uint8_t row);
void LCD1602_Puts(uint8_t x, uint8_t y, char* str);
void LCD1602_CreateChar(uint8_t location, uint8_t *data);
void LCD1602_PutCustom(uint8_t x, uint8_t y, uint8_t location);


#ifdef __cplusplus
}
#endif

#endif /* LCD1602_H_ */
