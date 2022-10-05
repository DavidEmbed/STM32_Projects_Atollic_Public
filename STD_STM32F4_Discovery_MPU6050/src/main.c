/****************************************************************************
**
**  File        : main.c
**
**
**  This example configures and initializes GPIO and I2C communication for control
**  read values from a accelerometer, gyroscope and temperature sensor.
**
**
*****************************************************************************/

// Includes
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"
#include ".\MPU6050\mpu6050.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Main program
int main(void) {
	int16_t AcX, AcY, AcZ;
	int16_t GyX, GyY, GyZ;
	float32_t temp;
	uint16_t data;
	uint8_t result1, result2, result3, result4;

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// MPU6050 Sensor Init
	MPU6050_Init();

	// Wake up MPU6050 Sensor
	MPU6050_WakeUp();

	printf("Init Ok!\r");

	// Infinite loop
	while (1) {
		result1 = MPU6050_WHOIAM(&data);
		result2 = MPU6050_ReadAccelerometer(&AcX, &AcY, &AcZ);
		result3 = MPU6050_ReadGyroscope(&GyX, &GyY, &GyZ);
		result4 = MPU6050_ReadTemperature(&temp);

		printf("\rWho i am: %u, Result: %u\r", data, result1);
		printf("AcX: %d, AcY: %d, AxZ: %d, Result: %u\r", AcX, AcY, AcZ, result2);
		printf("GyX: %d, GyY: %d, GyZ: %d, Result: %u\r", GyX, GyY, GyZ, result3);
		printf("Temp: %f, Result: %u\r", temp, result4);

		Delay_ms(300);
	}
}
