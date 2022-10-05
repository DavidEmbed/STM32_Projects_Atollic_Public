/*
 * dht11.h
 *
 *  Created on: Aug 8, 2017
 *      Author: David Martínez
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DHT11_H_
#define DHT11_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include ".\SysTick\delay.h"

#define TIMEOUT				100000


typedef enum  {
	DHT11_DATA				= 0,
} Dht11DataPin_TypeDef;

typedef enum{
	DHT11_OUTPUT 			= 0,
	DHT11_INPUT 			= 1
}Dht11DataDirection_TypeDef;

typedef enum{
	DHT11_OK 				= 0,
	DHT11_TIMEOUT 			= 1,
	DHT11_WRONG_CHECKSUM 	= 2
}Dht11ErrorCode_TypeDef;

#define DATAn                          1

#define DATA_PIN                       GPIO_Pin_8
#define DATA_GPIO_PORT                 GPIOB
#define DATA_GPIO_CLK                  RCC_AHB1Periph_GPIOB


/* Functions ------------------------------------------------------------------*/
void DHT11Init(Dht11DataPin_TypeDef Data, Dht11DataDirection_TypeDef direction);
void DataOutputHigh(Dht11DataPin_TypeDef Data);
void DataOutputLow(Dht11DataPin_TypeDef Data);
uint8_t DataInputState(Dht11DataPin_TypeDef Data);
Dht11ErrorCode_TypeDef DHT11_Read(uint8_t *pData);

#ifdef __cplusplus
}
#endif

#endif /* DHT11_H_ */
