/*
 * mpu6050.c
 *
 *  Created on: Aug 13, 2017
 *      Author: David Martínez
 */

  
#include "mpu6050.h"


// MPU6050 Sensor Initialization
void MPU6050_Init(void) {

	// Configure GPIO
	MPU6050_GPIOConfig();

	// Configure I2Cx
	MPU6050_InitI2C();
}


// I2Cx GIPO (SCL + SDA) Configuration
void MPU6050_GPIOConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(MPU6050_I2Cx_SCL_GPIO_CLK | MPU6050_I2Cx_SDA_GPIO_CLK, ENABLE);

	// Connect I2Cx pins to AFx
	GPIO_PinAFConfig(MPU6050_I2Cx_SCL_GPIO_PORT, MPU6050_I2Cx_SCL_SOURCE, MPU6050_I2Cx_SCL_AF);
	GPIO_PinAFConfig(MPU6050_I2Cx_SDA_GPIO_PORT, MPU6050_I2Cx_SDA_SOURCE, MPU6050_I2Cx_SDA_AF);

	// Setup GPIO SCL pin.
	GPIO_InitStructure.GPIO_Pin = MPU6050_I2Cx_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MPU6050_I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);

	// Setup GPIO SDA pin.
	GPIO_InitStructure.GPIO_Pin = MPU6050_I2Cx_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MPU6050_I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
}


// I2Cx Initialization
void MPU6050_InitI2C(void) {
	I2C_InitTypeDef  I2C_InitStruct;

	// I2Cx clock enable
	RCC_APB1PeriphClockCmd(MPU6050_I2Cx_CLK, ENABLE);

	// I2C DeInit
	I2C_DeInit(MPU6050_I2Cx);

	// Initialize the I2C peripheral with selected parameters
	//  - Mode: I2C
	//  - Duty Cycle = 2  (not used in standard speed)
	//  - Own address = 0xFE (not important, we are the master)
	//  - Acknowledge = ON
	//  - 7-bit for address
	//  - Clock Speed = 30000 Hz => Works in standard speed (tlow=thigh)
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_Init(MPU6050_I2Cx, &I2C_InitStruct);

	// Enable the I2C peripheral
	I2C_Cmd(MPU6050_I2Cx, ENABLE);

	I2C_ClearFlag(MPU6050_I2Cx, I2C_FLAG_AF);
}


// Read a 8-bit or 16-bit register through I2Cx, see datasheet
uint8_t MPU6050_ReadRegister(MPU6050_REG_TypeDef registerAddr, uint16_t *pRegisterValue, MPU6050_DataLength_TypeDef nBytes) {
	uint32_t timeout;
	uint8_t data[2];

	// Wait while the bus is busy
	timeout = MPU6050_I2C_TIMEOUT;
	while(I2C_GetFlagStatus(MPU6050_I2Cx, I2C_FLAG_BUSY)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND START CONDITION
	I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND I2C ADDRESS + WRITE BIT + READ ACK (specify master as transmitter)
	I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDR_WR, I2C_Direction_Transmitter);

	// Test on EV6 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND REGISTER + READ ACK
	I2C_SendData(MPU6050_I2Cx, registerAddr);

	// Test on EV8 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}


	// SEND RE-START CONDITION
	I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}


	// SEND I2C ADDRESS + READ BIT + READ ACK (specify master as receiver)
	I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDR_RD, I2C_Direction_Receiver);

	// Wait on ADDR flag to be set. Test on EV6 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	data[1] = 0;
	if (nBytes == MPU6050_DATA_2Bytes) {

		// WAIT 8-bits + ACK
		while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

		// Read the received data (Most Significant Byte)
		data[1] = I2C_ReceiveData(MPU6050_I2Cx);
	}


	// Disable Acknowledgment for next NACK
	I2C_AcknowledgeConfig(MPU6050_I2Cx, DISABLE);

	// WAIT 8-bits + NACK (None ACK).
	while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read the received data (Least Significant Byte)
	data[0] = I2C_ReceiveData(MPU6050_I2Cx);

	// SEND STOP CONDITION
	I2C_GenerateSTOP(MPU6050_I2Cx, ENABLE);


	// Re-Enable Acknowledgment to be ready for another reception
	I2C_AcknowledgeConfig(MPU6050_I2Cx, ENABLE);


	// Clear AF (Acknowledge failure flag) flag for next communication
	I2C_ClearFlag(MPU6050_I2Cx, I2C_FLAG_AF);


	// Calculate the result
	*pRegisterValue =  (data[1] << 8) & 0xFF00;
	*pRegisterValue = *pRegisterValue + data[0];

	return 0;
}


// Write a 8-bit or 16-bit register through I2Cx, see datasheet
uint8_t MPU6050_WriteRegister(MPU6050_REG_TypeDef registerAddr, uint16_t RegisterValue, MPU6050_DataLength_TypeDef nBytes) {
	uint32_t timeout;
	uint8_t data[2];

	// Wait while the bus is busy
	timeout = MPU6050_I2C_TIMEOUT;
	while(I2C_GetFlagStatus(MPU6050_I2Cx, I2C_FLAG_BUSY)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND START CONDITION
	I2C_GenerateSTART(MPU6050_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND I2C ADDRESS + WRITE BIT + READ ACK (specify master as transmitter)
	I2C_Send7bitAddress(MPU6050_I2Cx, MPU6050_ADDR_WR, I2C_Direction_Transmitter);

	// Test on EV6 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while (!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// SEND REGISTER + READ ACK
	I2C_SendData(MPU6050_I2Cx, registerAddr);

	// Test on EV8 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}

	// Calculate the data bytes
	data[1] = (RegisterValue >> 8);
	data[0] = (RegisterValue & 0x00FF);

	if (nBytes == MPU6050_DATA_2Bytes) {

		// SEND DATA (Most Significant Byte) + READ ACK
		I2C_SendData(MPU6050_I2Cx, data[1]);

		// Test on EV8 and clear it
		timeout = MPU6050_I2C_TIMEOUT;
		while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
			if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
		}
	}


	// SEND DATA (Least Significant Byte) + READ ACK
	I2C_SendData(MPU6050_I2Cx, data[0]);

	// Test on EV8 and clear it
	timeout = MPU6050_I2C_TIMEOUT;
	while(!I2C_CheckEvent(MPU6050_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return MPU6050_TIMEOUT_Callback();
	}


	// SEND STOP CONDITION
	I2C_GenerateSTOP(MPU6050_I2Cx, ENABLE);


	// Re-Enable Acknowledgment to be ready for another reception
	I2C_AcknowledgeConfig(MPU6050_I2Cx, ENABLE);


	// Clear AF (Acknowledge failure flag) flag for next communication
	I2C_ClearFlag(MPU6050_I2Cx, I2C_FLAG_AF);

	return 0;
}


uint8_t MPU6050_ReadAccelerometer(int16_t *AccelValueX, int16_t *AccelValueY, int16_t *AccelValueZ){
	uint16_t AccelXHigh, AccelXLow;
	uint16_t AccelYHigh, AccelYLow;
	uint16_t AccelZHigh, AccelZLow;
	uint8_t result = 0;

	result |= MPU6050_ReadRegister(MPU6050_ACCEL_XOUT_H, &AccelXHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_ACCEL_XOUT_L, &AccelXLow, MPU6050_DATA_1Byte);

	*AccelValueX = (int16_t)(((uint8_t)(AccelXHigh) << 8) + (uint8_t)(AccelXLow));

	result |= MPU6050_ReadRegister(MPU6050_ACCEL_YOUT_H, &AccelYHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_ACCEL_YOUT_L, &AccelYLow, MPU6050_DATA_1Byte);

	*AccelValueY = (int16_t)(((uint8_t)(AccelYHigh) << 8) + (uint8_t)(AccelYLow));

	result |= MPU6050_ReadRegister(MPU6050_ACCEL_ZOUT_H, &AccelZHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_ACCEL_ZOUT_L, &AccelZLow, MPU6050_DATA_1Byte);

	*AccelValueZ = (int16_t)(((uint8_t)(AccelZHigh) << 8) + (uint8_t)(AccelZLow));

	return result;
}

uint8_t MPU6050_ReadGyroscope(int16_t *GyroValueX, int16_t *GyroValueY, int16_t *GyroValueZ){
	uint16_t GyroXHigh, GyroXLow;
	uint16_t GyroYHigh, GyroYLow;
	uint16_t GyroZHigh, GyroZLow;
	uint8_t result = 0;

	result |= MPU6050_ReadRegister(MPU6050_GYRO_XOUT_H, &GyroXHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_GYRO_XOUT_L, &GyroXLow, MPU6050_DATA_1Byte);

	*GyroValueX = (int16_t)(((uint8_t)(GyroXHigh) << 8) + (uint8_t)(GyroXLow));

	result |= MPU6050_ReadRegister(MPU6050_GYRO_YOUT_H, &GyroYHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_GYRO_YOUT_L, &GyroYLow, MPU6050_DATA_1Byte);

	*GyroValueY = (int16_t)(((uint8_t)(GyroYHigh) << 8) + (uint8_t)(GyroYLow));

	result |= MPU6050_ReadRegister(MPU6050_GYRO_ZOUT_H, &GyroZHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_GYRO_ZOUT_L, &GyroZLow, MPU6050_DATA_1Byte);

	*GyroValueZ = (int16_t)(((uint8_t)(GyroZHigh) << 8) + (uint8_t)(GyroZLow));

	return result;
}

uint8_t MPU6050_ReadTemperature(float32_t *temp){
	uint16_t tempHigh, tempLow;
	int16_t data;
	uint8_t result = 0;

	result |= MPU6050_ReadRegister(MPU6050_TEMP_OUT_H, &tempHigh, MPU6050_DATA_1Byte);
	result |= MPU6050_ReadRegister(MPU6050_TEMP_OUT_L, &tempLow, MPU6050_DATA_1Byte);

	data = (int16_t)(((uint8_t)(tempHigh) << 8) + (uint8_t)(tempLow));

	*temp = (float32_t)((int16_t)data / (float32_t)340.0 + (float32_t)36.53);

	return result;
}


// WakeUp the MPU6050 sensor
uint8_t MPU6050_WakeUp(void){
	uint8_t result = 0;

	result |= MPU6050_WriteRegister(MPU6050_PWR_MGMT_1, 0, MPU6050_DATA_1Byte);

	return result;
}


// Who i am
uint8_t MPU6050_WHOIAM(uint16_t *value){
	uint16_t data;
	uint8_t result = 0;

	result |= MPU6050_ReadRegister(MPU6050_WHO_AM_I, &data, MPU6050_DATA_1Byte);
	*value = data;

	return result;
}

// Allows I2C error recovery and return to normal communication
uint8_t MPU6050_TIMEOUT_Callback(void) {

	I2C_GenerateSTOP(MPU6050_I2Cx, ENABLE);
	I2C_SoftwareResetCmd(MPU6050_I2Cx, ENABLE);
	I2C_SoftwareResetCmd(MPU6050_I2Cx, DISABLE);

	// I2Cx peripheral re-configuration
	MPU6050_InitI2C();

	/* At this stage the I2C error should be recovered and device can communicate
	   again (except if the error source still exist).
	   User can implement mechanism (ex. test on max trial number) to manage situation
	   when the I2C can't recover from current error. */

	// Indicate Error
	return 1;
}

