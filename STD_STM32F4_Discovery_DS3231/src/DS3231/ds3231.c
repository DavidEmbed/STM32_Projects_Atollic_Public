/*
 * ds3231.c
 *
 *  Created on: Aug 14, 2017
 *      Author: David Martínez
 */

  
#include <DS3231/ds3231.h>

// External function prototypes (defined in tiny_printf.c)
extern int printf(const char *fmt, ...);
extern int sprintf(char* str, const char *fmt, ...);

// DS3231 Initialization
void DS3231_Init(void) {

	// Configure GPIO
	DS3231_GPIOConfig();

	// Configure I2Cx
	DS3231_InitI2C();
}


// I2Cx GIPO (SCL + SDA) Configuration
void DS3231_GPIOConfig(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO clock
	RCC_AHB1PeriphClockCmd(DS3231_I2Cx_SCL_GPIO_CLK | DS3231_I2Cx_SDA_GPIO_CLK, ENABLE);

	// Connect I2Cx pins to AFx
	GPIO_PinAFConfig(DS3231_I2Cx_SCL_GPIO_PORT, DS3231_I2Cx_SCL_SOURCE, DS3231_I2Cx_SCL_AF);
	GPIO_PinAFConfig(DS3231_I2Cx_SDA_GPIO_PORT, DS3231_I2Cx_SDA_SOURCE, DS3231_I2Cx_SDA_AF);

	// Setup GPIO SCL pin.
	GPIO_InitStructure.GPIO_Pin = DS3231_I2Cx_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DS3231_I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);

	// Setup GPIO SDA pin.
	GPIO_InitStructure.GPIO_Pin = DS3231_I2Cx_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DS3231_I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
}


// I2Cx Initialization
void DS3231_InitI2C(void) {
	I2C_InitTypeDef  I2C_InitStruct;

	// I2Cx clock enable
	RCC_APB1PeriphClockCmd(DS3231_I2Cx_CLK, ENABLE);

	// I2C DeInit
	I2C_DeInit(DS3231_I2Cx);

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
	I2C_Init(DS3231_I2Cx, &I2C_InitStruct);

	// Enable the I2C peripheral
	I2C_Cmd(DS3231_I2Cx, ENABLE);

	I2C_ClearFlag(DS3231_I2Cx, I2C_FLAG_AF);
}


// Read a 8-bit or 16-bit register through I2Cx, see datasheet
uint8_t DS3231_ReadRegister(DS3231_REG_TypeDef registerAddr, uint16_t *pRegisterValue, DS3231_DataLength_TypeDef nBytes) {
	uint32_t timeout;
	uint8_t data[2];

	// Wait while the bus is busy
	timeout = DS3231_I2C_TIMEOUT;
	while(I2C_GetFlagStatus(DS3231_I2Cx, I2C_FLAG_BUSY)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND START CONDITION
	I2C_GenerateSTART(DS3231_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while (!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND I2C ADDRESS + WRITE BIT + READ ACK (specify master as transmitter)
	I2C_Send7bitAddress(DS3231_I2Cx, DS3231_ADDR_WR, I2C_Direction_Transmitter);

	// Test on EV6 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while (!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND REGISTER + READ ACK
	I2C_SendData(DS3231_I2Cx, registerAddr);

	// Test on EV8 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}


	// SEND RE-START CONDITION
	I2C_GenerateSTART(DS3231_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while (!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}


	// SEND I2C ADDRESS + READ BIT + READ ACK (specify master as receiver)
	I2C_Send7bitAddress(DS3231_I2Cx, DS3231_ADDR_RD, I2C_Direction_Receiver);

	// Wait on ADDR flag to be set. Test on EV6 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	data[1] = 0;
	if (nBytes == DS3231_DATA_2Bytes) {

		// WAIT 8-bits + ACK
		while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

		// Read the received data (Most Significant Byte)
		data[1] = I2C_ReceiveData(DS3231_I2Cx);
	}


	// Disable Acknowledgment for next NACK
	I2C_AcknowledgeConfig(DS3231_I2Cx, DISABLE);

	// WAIT 8-bits + NACK (None ACK).
	while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

	// Read the received data (Least Significant Byte)
	data[0] = I2C_ReceiveData(DS3231_I2Cx);

	// SEND STOP CONDITION
	I2C_GenerateSTOP(DS3231_I2Cx, ENABLE);


	// Re-Enable Acknowledgment to be ready for another reception
	I2C_AcknowledgeConfig(DS3231_I2Cx, ENABLE);


	// Clear AF (Acknowledge failure flag) flag for next communication
	I2C_ClearFlag(DS3231_I2Cx, I2C_FLAG_AF);


	// Calculate the result
	*pRegisterValue =  (data[1] << 8) & 0xFF00;
	*pRegisterValue = *pRegisterValue + data[0];

	return 0;
}


// Write a 8-bit or 16-bit register through I2Cx, see datasheet
uint8_t DS3231_WriteRegister(DS3231_REG_TypeDef registerAddr, uint16_t RegisterValue, DS3231_DataLength_TypeDef nBytes) {
	uint32_t timeout;
	uint8_t data[2];

	// Wait while the bus is busy
	timeout = DS3231_I2C_TIMEOUT;
	while(I2C_GetFlagStatus(DS3231_I2Cx, I2C_FLAG_BUSY)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND START CONDITION
	I2C_GenerateSTART(DS3231_I2Cx, ENABLE);

	// Test on EV5 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while (!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_MODE_SELECT)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND I2C ADDRESS + WRITE BIT + READ ACK (specify master as transmitter)
	I2C_Send7bitAddress(DS3231_I2Cx, DS3231_ADDR_WR, I2C_Direction_Transmitter);

	// Test on EV6 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while (!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// SEND REGISTER + READ ACK
	I2C_SendData(DS3231_I2Cx, registerAddr);

	// Test on EV8 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}

	// Calculate the data bytes
	data[1] = (RegisterValue >> 8);
	data[0] = (RegisterValue & 0x00FF);

	if (nBytes == DS3231_DATA_2Bytes) {

		// SEND DATA (Most Significant Byte) + READ ACK
		I2C_SendData(DS3231_I2Cx, data[1]);

		// Test on EV8 and clear it
		timeout = DS3231_I2C_TIMEOUT;
		while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
			if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
		}
	}


	// SEND DATA (Least Significant Byte) + READ ACK
	I2C_SendData(DS3231_I2Cx, data[0]);

	// Test on EV8 and clear it
	timeout = DS3231_I2C_TIMEOUT;
	while(!I2C_CheckEvent(DS3231_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		if((timeout--) == 0) return DS3231_TIMEOUT_Callback();
	}


	// SEND STOP CONDITION
	I2C_GenerateSTOP(DS3231_I2Cx, ENABLE);


	// Re-Enable Acknowledgment to be ready for another reception
	I2C_AcknowledgeConfig(DS3231_I2Cx, ENABLE);


	// Clear AF (Acknowledge failure flag) flag for next communication
	I2C_ClearFlag(DS3231_I2Cx, I2C_FLAG_AF);

	return 0;
}


// Example: 2017, 8, 14, 1, 17, 36, 42 // TODO: Test
void DS3231_setDateTime(uint16_t year, uint8_t month, uint16_t date, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second){
	uint8_t result;

	second = DS3231_dec2bcd(second);
	minute = DS3231_dec2bcd(minute);
	hour = DS3231_dec2bcd(hour);
	day = DS3231_dec2bcd(day);
	date = DS3231_dec2bcd(date);
	month = DS3231_dec2bcd(month);
	year = year - 2000;
	year = DS3231_dec2bcd(year);

	result = DS3231_WriteRegister(DS3231_YEAR, year, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_MONTH_CENTURY, month, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_DATE, date, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_DAY, day, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_HOURS, hour, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_MINUTES, minute, DS3231_DATA_1Byte);
	result |= DS3231_WriteRegister(DS3231_SECONDS, second, DS3231_DATA_1Byte);
}


// Read second
uint8_t DS3231_ReadSecond(uint16_t *second){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_SECONDS, &data, DS3231_DATA_1Byte);
	*second = DS3231_bcd2dec(data);

	return result;
}


// Read minute
uint8_t DS3231_ReadMinute(uint16_t *minute){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_MINUTES, &data, DS3231_DATA_1Byte);
	*minute = DS3231_bcd2dec(data);

	return result;
}


// Read hour
uint8_t DS3231_ReadHour(uint16_t *hour){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_HOURS, &data, DS3231_DATA_1Byte);
	*hour = DS3231_bcd2dec(data);

	return result;
}


// Returns de number of day of week
uint8_t DS3231_ReadDay(uint16_t *day){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_DAY, &data, DS3231_DATA_1Byte);
	*day = DS3231_bcd2dec(data);

	return result;
}

// Returns the name of the day of week
char *DS3231_strDayOfWeek(uint8_t dayOfWeek)
{
    switch (dayOfWeek) {
        case 1:
            return "Monday";
            break;
        case 2:
            return "Tuesday";
            break;
        case 3:
            return "Wednesday";
            break;
        case 4:
            return "Thursday";
            break;
        case 5:
            return "Friday";
            break;
        case 6:
            return "Saturday";
            break;
        case 7:
            return "Sunday";
            break;
        default:
            return "Unknown";
    }
}


// Read day of month
uint8_t DS3231_ReadDate(uint16_t *date){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_DATE, &data, DS3231_DATA_1Byte);
	*date = DS3231_bcd2dec(data);

	return result;
}


// Read number of month
uint8_t DS3231_ReadMonth(uint16_t *month){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_MONTH_CENTURY, &data, DS3231_DATA_1Byte);
	*month = DS3231_bcd2dec(data);

	return result;
}


// Read name of month
char *DS3231_strMonth(uint8_t month)
{
    switch (month) {
        case 1:
            return "January";
            break;
        case 2:
            return "February";
            break;
        case 3:
            return "March";
            break;
        case 4:
            return "April";
            break;
        case 5:
            return "May";
            break;
        case 6:
            return "June";
            break;
        case 7:
            return "July";
            break;
        case 8:
            return "August";
            break;
        case 9:
            return "September";
            break;
        case 10:
            return "October";
            break;
        case 11:
            return "November";
            break;
        case 12:
            return "December";
            break;
        default:
            return "Unknown";
    }
}


// Read the last two digits of the year
uint8_t DS3231_ReadYear(uint16_t *year){
	uint16_t data;
	uint8_t result;

	result = DS3231_ReadRegister(DS3231_YEAR, &data, DS3231_DATA_1Byte);
	*year = DS3231_bcd2dec(data);

	// Convert in actual century year
	*year = 2000 + *year;

	return result;
}


// Decode from bcd values of registers to decimal values (Read register)
uint8_t DS3231_bcd2dec(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}


// Decode from decimal values to bcd values of registers (Write register)
uint8_t DS3231_dec2bcd(uint8_t dec)
{
    return ((dec / 10) * 16) + (dec % 10);
}


// Allows I2C error recovery and return to normal communication
uint8_t DS3231_TIMEOUT_Callback(void) {

	I2C_GenerateSTOP(DS3231_I2Cx, ENABLE);
	I2C_SoftwareResetCmd(DS3231_I2Cx, ENABLE);
	I2C_SoftwareResetCmd(DS3231_I2Cx, DISABLE);

	// I2Cx peripheral re-configuration
	DS3231_InitI2C();

	/* At this stage the I2C error should be recovered and device can communicate
	   again (except if the error source still exist).
	   User can implement mechanism (ex. test on max trial number) to manage situation
	   when the I2C can't recover from current error. */

	// Indicate Error
	return 1;
}

