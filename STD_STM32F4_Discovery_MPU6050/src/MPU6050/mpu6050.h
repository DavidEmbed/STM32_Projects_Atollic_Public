/*
 * mpu6050.h
 *
 *  Created on: Aug 13, 2017
 *      Author: David Martínez
 */

// Define to prevent recursive inclusion
#ifndef __MPU6050_H
#define __MPU6050_H

#ifdef __cplusplus
 extern "C" {
#endif

// Includes
#include "stm32f4xx.h"
#include "arm_math.h"

// Pin Description
// MPU6050		STM32F4xx	Description
//
// SCL			PB6			Clock line for I2C
// SDA			PB9			Data line for I2C
// VCC			3.3V
// GND			GND
// AD0			-			If pin is low, I2C address is 0xD0, if pin is high, the address is 0xD2


// Configuration for I2C
#define MPU6050_I2Cx					I2C1
#define MPU6050_I2Cx_CLK				RCC_APB1Periph_I2C1

#define MPU6050_I2Cx_SCL_PIN           	GPIO_Pin_6
#define MPU6050_I2Cx_SCL_GPIO_PORT     	GPIOB
#define MPU6050_I2Cx_SCL_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define MPU6050_I2Cx_SCL_SOURCE        	GPIO_PinSource6
#define MPU6050_I2Cx_SCL_AF            	GPIO_AF_I2C1

#define MPU6050_I2Cx_SDA_PIN           	GPIO_Pin_9
#define MPU6050_I2Cx_SDA_GPIO_PORT     	GPIOB
#define MPU6050_I2Cx_SDA_GPIO_CLK      	RCC_AHB1Periph_GPIOB
#define MPU6050_I2Cx_SDA_SOURCE        	GPIO_PinSource9
#define MPU6050_I2Cx_SDA_AF            	GPIO_AF_I2C1


// I2Cx timeout waiting value (set for system running at 168 MHz)
#define MPU6050_I2C_TIMEOUT			10000

// Who I am register value
#define MPU6050_I_AM				0x68

/* MPU6050 registers */
 typedef enum {
	MPU6050_AUX_VDDIO			= 0x01,
	MPU6050_SMPLRT_DIV			= 0x19,
	MPU6050_CONFIG				= 0x1A,
	MPU6050_GYRO_CONFIG			= 0x1B,
	MPU6050_ACCEL_CONFIG		= 0x1C,
	MPU6050_MOTION_THRESH		= 0x1F,
	MPU6050_INT_PIN_CFG			= 0x37,
	MPU6050_INT_ENABLE			= 0x38,
	MPU6050_INT_STATUS			= 0x3A,
	MPU6050_ACCEL_XOUT_H		= 0x3B,
	MPU6050_ACCEL_XOUT_L		= 0x3C,
	MPU6050_ACCEL_YOUT_H		= 0x3D,
	MPU6050_ACCEL_YOUT_L		= 0x3E,
	MPU6050_ACCEL_ZOUT_H		= 0x3F,
	MPU6050_ACCEL_ZOUT_L		= 0x40,
	MPU6050_TEMP_OUT_H			= 0x41,
	MPU6050_TEMP_OUT_L			= 0x42,
	MPU6050_GYRO_XOUT_H			= 0x43,
	MPU6050_GYRO_XOUT_L			= 0x44,
	MPU6050_GYRO_YOUT_H			= 0x45,
	MPU6050_GYRO_YOUT_L			= 0x46,
	MPU6050_GYRO_ZOUT_H			= 0x47,
	MPU6050_GYRO_ZOUT_L			= 0x48,
	MPU6050_MOT_DETECT_STATUS	= 0x61,
	MPU6050_SIGNAL_PATH_RESET	= 0x68,
	MPU6050_MOT_DETECT_CTRL		= 0x69,
	MPU6050_USER_CTRL			= 0x6A,
	MPU6050_PWR_MGMT_1			= 0x6B,
	MPU6050_PWR_MGMT_2			= 0x6C,
	MPU6050_FIFO_COUNTH			= 0x72,
	MPU6050_FIFO_COUNTL			= 0x73,
	MPU6050_FIFO_R_W			= 0x74,
	MPU6050_WHO_AM_I			= 0x75
} MPU6050_REG_TypeDef;


// MPU6050 slave Address
typedef enum{
	MPU6050_ADDR_WR    = 0b11010000, // LSB to '0' indicating write operation
	MPU6050_ADDR_RD    = 0b11010001  // LSB to '1' indicating read operation
}MPU6050_Address_TypeDef;


// MPU6050 data length types
typedef enum{
	MPU6050_DATA_1Byte     = 0,
	MPU6050_DATA_2Bytes    = 1
}MPU6050_DataLength_TypeDef;


// Parameters for accelerometer range
typedef enum {
	MPU6050_Accelerometer_2G 	= 0x00,
	MPU6050_Accelerometer_4G 	= 0x01,
	MPU6050_Accelerometer_8G 	= 0x02,
	MPU6050_Accelerometer_16G 	= 0x03
} MPU6050_Accelerometer_TypeDef;

// Parameters for gyroscope range
typedef enum {
	TM_MPU6050_Gyroscope_250s 	= 0x00,
	TM_MPU6050_Gyroscope_500s 	= 0x01,
	TM_MPU6050_Gyroscope_1000s 	= 0x02,
	TM_MPU6050_Gyroscope_2000s 	= 0x03
} MPU6050_Gyroscope_TypeDef;


// Functions
void MPU6050_Init(void);
void MPU6050_GPIOConfig(void);
void MPU6050_InitI2C(void);

// I2C Communication functions
uint8_t MPU6050_ReadRegister(MPU6050_REG_TypeDef registerAddr, uint16_t *pRegisterValue, MPU6050_DataLength_TypeDef nBytes);
uint8_t MPU6050_WriteRegister(MPU6050_REG_TypeDef registerAddr, uint16_t RegisterValue, MPU6050_DataLength_TypeDef nBytes);

// Get user functions
uint8_t MPU6050_ReadAccelerometer(int16_t *AccelValueX, int16_t *AccelValueY, int16_t *AccelValueZ);
uint8_t MPU6050_ReadGyroscope(int16_t *GyroValueX, int16_t *GyroValueY, int16_t *GyroValueZ);
uint8_t MPU6050_ReadTemperature(float32_t *temp);

// Set user functions


// Power control
uint8_t MPU6050_WakeUp(void);

// I2C TimeOut function
uint8_t MPU6050_TIMEOUT_Callback(void);

// Test function
uint8_t MPU6050_WHOIAM(uint16_t *value);


#ifdef __cplusplus
}
#endif

#endif /* __MPU6050_H */
