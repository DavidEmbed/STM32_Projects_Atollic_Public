/*
 * max7219.c
 *
 *  Created on: Aug 12, 2017
 *      Author: David Martínez
 */

/* Includes ------------------------------------------------------------------*/
#include <MAX7219/max7219.h>

uint8_t status[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Initialize the MAX7219
void MAX7219_Init(void) {

	// GPIO initialize
	MAX7219_GPIO_Init();

	// SPI1 initialize
	MAX7219_SPI_Init();
}


// Initialize the GPIO of the SPI1
// SCLK = PA5, MOSI = PA7, CS = PE3
void MAX7219_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable SCK, MOSI and GPIO clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Enable CS GPIO clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// Mapping SPI1 to the specific pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	// Configure SCLK, MISO and MOSI GPIO pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure SS (Chip select) GPIO pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	// Deselect : Chip Select high
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

// Initializes the SPI1 peripheral
void MAX7219_SPI_Init(void) {
	SPI_InitTypeDef  SPI_InitStructure;

	// Enable the SPI peripheral
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	// SPI1 configuration:
	//	- Mode Master
	//	- Full-Duplex
	//	- 16 bits for data
	//	- CPOL = Low polarity
	//	- CPHA = Rising Edge
	//	- NSS (Slave select by software)
	//	- BautRate Preescaler = 4 => PCLK2/4 = 84/4 = 21MHz => 21Mbit/s
	//	- BautRate Preescaler = 8 => PCLK2/8 = 84/8 = 10MHz => 10Mbit/s
	//	- Frist byte is the MSB
	//	- CRC Polyniomial = 7 => X^2 + X + 1
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);

	// Enable SPI1
	SPI_Cmd(SPI1, ENABLE);
}


// Writes a register of the MAX7219 through the SPI1
void MAX7219_Write(uint8_t writeAddr, uint8_t value) {
	uint16_t data;

	// Set SS Low at the start of the transmission
	GPIO_ResetBits(GPIOE, GPIO_Pin_3);

	data = (writeAddr << 8) | value;

	// Send the Address of the indexed register
	MAX7219_SendByte(data);

	// Set SS High at the end of the transmission
	GPIO_SetBits(GPIOE, GPIO_Pin_3);
}


// Sends a Byte through the SPI interface and return the Byte received
uint8_t MAX7219_SendByte(uint16_t byte) {

	// Loop while Transmit buffer in not empty
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// Send a Byte through the SPI peripheral
	SPI_I2S_SendData(SPI1, byte);

	// Wait to receive a Byte
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	// Return the Byte read from the SPI bus
	return (uint8_t)SPI_I2S_ReceiveData(SPI1);
}


void MAX7219_PowerOn(void){
	MAX7219_Write(MAX7219_ADDR_SHUTDOWN, 1);
}


void MAX7219_PowerOff(void){
	MAX7219_Write(MAX7219_ADDR_SHUTDOWN, 0);
}

void MAX7219_DecodeMode(uint8_t mode){
	MAX7219_Write(MAX7219_ADDR_DECODE_MODE, mode);
}


void MAX7219_Intensity(uint8_t intensity) {
	MAX7219_Write(MAX7219_ADDR_INTENSITY, intensity & 0x0F);
}


void MAX7219_ScanLimit(uint8_t limit) {
	MAX7219_Write(MAX7219_ADDR_SCAN_LIMIT, limit & 0x07);
}


void MAX7219_DisplayTest(uint8_t mode){
	MAX7219_Write(MAX7219_ADDR_DISPLAY_TEST, mode);
}


void ClearDisplay(void){
	uint8_t i;

    for (i = 0; i < 8; i++) {
		status[i] = 0;
		MAX7219_Write(i+1, status[i]);
    }
}

void setRow(uint8_t row, uint8_t value) {

    if (row < 0 || row > 7) return;

    status[row] = value;
    MAX7219_Write(row+1, status[row]);
}

void setColumn(uint8_t col, uint8_t value) {
    uint8_t val, row;

    if (col < 0 || col > 7) return;

    for (row = 0; row < 8; row++){
		val = value >> (7 - row);
		val = val & 0x01;
		setLed(row, col, val);
    }
}

void setLed(uint8_t row, uint8_t column, uint8_t state){
    uint8_t val = 0;

    if (row < 0 || row > 7) return;
    if (column < 0 || column > 7) return;

    val = 0b10000000 >> column;

    if(state){
    	status[row] = status[row] | val;
    }
    else {
		val=~val;
		status[row]=status[row] & val;
    }

    MAX7219_Write(row+1, status[row]);
}
