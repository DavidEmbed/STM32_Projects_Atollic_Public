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
#include <DS3231/ds3231.h>
#include "stm32f4xx.h"
#include ".\Discovery\led.h"
#include ".\Discovery\pushbutton.h"
#include ".\SysTick\delay.h"
#include ".\USART\usart.h"
#include "stringcmd.h"

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// Main program
int main(void) {
	uint16_t sec, min, hour, day, date, month, year;
	char *weekdayName, *monthName;

	// Configures SysTick for delay functions
	Delay_Init();

	// Initialize all LEDs
	LEDInitAll();

	// USART2 Initialization
	USARTx_Init(USART_MODE_IRQ);

	// DS3231 Sensor Init
	DS3231_Init();

	printf("Init Ok!\r");

	// Infinite loop
	while (1) {

		DS3231_ReadSecond(&sec);
		DS3231_ReadMinute(&min);
		DS3231_ReadHour(&hour);
		DS3231_ReadDay(&day);
		weekdayName = DS3231_strDayOfWeek(day);
		DS3231_ReadDate(&date);
		DS3231_ReadMonth(&month);
		monthName = DS3231_strMonth(month);
		DS3231_ReadYear(&year);

		printf("Year: %u, Month: %s, Date: %u, Day: %s, Hour: %u, Min: %u, Sec: %u\r", year, monthName, date, weekdayName, hour, min, sec);

		Delay_ms(1000);
	}
}
