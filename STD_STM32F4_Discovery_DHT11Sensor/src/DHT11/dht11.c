/*
 * dht11.c
 *
 *  Created on: Aug 8, 2017
 *      Author: David Martínez
 */

/* Includes ------------------------------------------------------------------*/
#include <DHT11/dht11.h>

/* Variables -----------------------------------------------------------------*/
GPIO_TypeDef*	DHT11_DATA_GPIO_PORT[DATAn] = {DATA_GPIO_PORT};
const uint16_t	DHT11_DATA_GPIO_PIN[DATAn]  = {DATA_PIN};
const uint32_t	DHT11_DATA_GPIO_CLK[DATAn]  = {DATA_GPIO_CLK};


// Initialization of the Dht11 data pin
void DHT11Init(Dht11DataPin_TypeDef Data, Dht11DataDirection_TypeDef direction) {
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  /* Enable the GPIO Clock */
	  RCC_AHB1PeriphClockCmd(DHT11_DATA_GPIO_CLK[Data], ENABLE);

	  if (direction == DHT11_OUTPUT){
		  /* Configure the GPIO pin as output */
		  GPIO_InitStructure.GPIO_Pin = DHT11_DATA_GPIO_PIN[Data];
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		  GPIO_Init(DHT11_DATA_GPIO_PORT[Data], &GPIO_InitStructure);
	  }

	  else if (direction == DHT11_INPUT){
		  /* Configure the GPIO pin as input */
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		  GPIO_InitStructure.GPIO_Pin = DHT11_DATA_GPIO_PIN[Data];
		  GPIO_Init(DHT11_DATA_GPIO_PORT[Data], &GPIO_InitStructure);
	  }
}


// Set data to 1
void DataOutputHigh(Dht11DataPin_TypeDef Data) {
	GPIO_SetBits(DHT11_DATA_GPIO_PORT[Data], DHT11_DATA_GPIO_PIN[Data]);
}


// Set data to 0 - Clear port
void DataOutputLow(Dht11DataPin_TypeDef Data) {
	GPIO_ResetBits(DHT11_DATA_GPIO_PORT[Data], DHT11_DATA_GPIO_PIN[Data]);
}


// Get data input state
uint8_t DataInputState(Dht11DataPin_TypeDef Data) {
  return GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT[Data], DHT11_DATA_GPIO_PIN[Data]);
}


Dht11ErrorCode_TypeDef DHT11_Read(uint8_t *pData)
{
	uint8_t numBytes, numBits;
	uint32_t timeout;
	Dht11ErrorCode_TypeDef errorCode = DHT11_OK;

	// Set I/O as output
	DHT11Init(DHT11_DATA, DHT11_OUTPUT);

	// Output to low
	DataOutputLow(DHT11_DATA);

	// MCU sends out start signal and pulls down voltage for at leat 18 ms to let DHT11 detect the signal
	Delay_ms(18);

	// Output to high
	DataOutputHigh(DHT11_DATA);

	// Wait 40us MCU pulls up voltage and waits for DHT response
	Delay_us(40);

	// Set I/O as input
	DHT11Init(DHT11_DATA, DHT11_INPUT);

	timeout = TIMEOUT;
	//DHT sends out response signal & keeps it for 80us on state '0'
	while (DataInputState(DHT11_DATA) == 0){
		timeout--;
		if (timeout <= 0)
		{
			errorCode = DHT11_TIMEOUT;
			break;
		}
	}

	if (errorCode == DHT11_OK){
		timeout = TIMEOUT;
		// DHT pulls up voltage and keeps it for 80us on state '1'
		while (DataInputState(DHT11_DATA) == 1){
			timeout--;
			if (timeout <= 0)
			{
				errorCode = DHT11_TIMEOUT;
				break;
			}
		}
	}

	// Read Sequence
	if (errorCode == DHT11_OK){
        for(numBytes = 0; numBytes < 5; numBytes++)
        {
            for(numBits = 0; numBits < 8; numBits++)
            {
            	timeout = TIMEOUT;
            	// Start to trasmit 1Bit data, 50us on '0'
                while(DataInputState(DHT11_DATA) == 0){
        			timeout--;
        			if (timeout <= 0)
        			{
        				errorCode = DHT11_TIMEOUT;
        				break;
        			}
                }

                if (DataInputState(DHT11_DATA) == 1)
                {
                    Delay_us(30); // wait only 30us means 1-bit data '0'
                }

                pData[numBytes] = pData[numBytes] << 1;

                if (DataInputState(DHT11_DATA) == 1)
                {
                    Delay_us(40); // wait 'till 70us means 1-bit data '1'
                    pData[numBytes] = pData[numBytes] | 1;
                }
                else
                {
                    pData[numBytes] = pData[numBytes] & 0xfe;
                }
            }
        }
	}

    // Checksum check
    if (errorCode == DHT11_OK)
    {
        if ((pData[0] + pData[1] + pData[2] + pData[3]) != pData[4])
        {
            errorCode = DHT11_WRONG_CHECKSUM;
        }
        else
        {
            errorCode = DHT11_OK;
        }
    }

    return errorCode;
}


