/*
 * ds3231.h
 *
 *  Created on: Jul 2, 2025
 *      Author: mrohi
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#include "main.h"

#define DS3231_ADDRESS										0b01101000

/****************************
 * Registers
 ****************************/

#define SECONDS_REG											0x00
#define MINUTES_REG											0x01
#define HOURS_REG												0x02
#define DAY_REG													0x03
#define DATE_REG													0x04
#define MONTH_CENTURY_REG							0x05
#define YEAR_REG													0x06
#define ALM1_SECONDS_REG								0x07
#define ALM1_MINUTES_REG								0x08
#define ALM1_HOURS_REG									0x09
#define ALM1_DAY_DATE_REG								0x0A
#define ALM2_MINUTES_REG								0x0B
#define ALM2_HOURS_REG									0x0C
#define ALM2_DAY_DATE_REG								0x0D
#define CONTROL													0x0E
#define CONTROL_STATUS										0x0F
#define AGING														0x10
#define TEMP_MSB												0x11
#define TEMP_LSB													0x12

/****************************
 * Bit Positions
 ****************************/

#define SECONDS_BIT_POS									0x00
#define SECONDS_BIT_MSK									0x0F
#define SECONDS_10_BIT_POS								0x04
#define SECONDS_10_BIT_MSK								(0x07 << SECONDS_10_BIT_POS)

#define MINUTES_BIT_POS									0x00
#define MINUTES_BIT_MSK									0x0F
#define MINUTES_10_BIT_POS								0x04
#define MINUTES_10_BIT_MSK								(0x07 << MINUTES_10_BIT_POS)

#define HOURS_BIT_POS										0x00
#define HOURS_BIT_MSK										0x0F
#define HOURS_10_BIT_POS									0x04
#define HOURS_10_BIT_MSK									(0x01 << HOURS_10_BIT_POS)
#define HOURS_20_BIT_POS									0x05
#define HOURS_20_BIT_MSK									(0x01 << HOURS_20_BIT_POS)
#define HOURS_AM_PM_POS								0x05
#define HOURS_AM_PM_MSK								(0x01 << HOURS_AM_PM_POS)
#define HOURS_12_24_POS									0x06
#define HOURS_12_24_MSK									(0x01 << HOURS_12_24_POS)

#define DAY_BIT_POS											0x00
#define DAY_BIT_MSK											(0x07 << DAY_BIT_POS)

#define DATE_BIT_POS											0x00
#define DATE_BIT_MSK											(0x0F << DATE_BIT_POS)
#define DATE_10_BIT_POS									0x04
#define DATE_10_BIT_MSK									(0x03 << DATE_10_BIT_POS)

#define MONTH_BIT_POS										0x00
#define MONTH_BIT_MSK										(0x0F << MONTH_BIT_POS)
#define MONTH_10_BIT_POS								0x04
#define MONTH_10_BIT_MSK								(0x01 << MONTH_10_BIT_POS)

#define CENTURY_BIT_POS									0x07
#define CENTURY_BIT_MSK									(0x01 << CENTURY_BIT_POS)

#define YEAR_BIT_POS											0x00
#define YEAR_BIT_MSK											(0x0F << YEAR_BIT_POS)
#define YEAR_10_BIT_POS									0x04
#define YEAR_10_BIT_MSK									(0x0F << YEAR_10_BIT_POS)

#define ALM1_SEC_BIT_POS									0x00
#define ALM1_SEC_BIT_MSK									(0x0F << ALM1_SEC_BIT_POS)
#define ALM1_SEC_10_BIT_POS							0X04
#define ALM1_SEC_10_BIT_MSK							(0x07 << ALM1_SEC_10_BIT_POS)
#define ALM1_SEC_A1M1_BIT_POS						0x07
#define ALM1_SEC_A1M1_BIT_MSK						(0x01 << ALM1_SEC_A1M1_BIT_POS)

#define ALM1_MIN_BIT_POS									0x00
#define ALM1_MIN_BIT_MSK								(0x0F << ALM1_MIN_BIT_POS)
#define ALM1_MIN_10_BIT_POS							0X04
#define ALM1_MIN_10_BIT_MSK							(0x07 << ALM1_MIN_10_BIT_POS)
#define ALM1_MIN_A1M2_BIT_POS						0x07
#define ALM1_MIN_A1M2_BIT_MSK						(0x01 << ALM1_MIN_A1M2_BIT_POS)

#define ALM1_HOURS_BIT_POS							0x00
#define ALM1_HOURS_BIT_MSK							(0x0F << ALM1_HOURS_BIT_POS)
#define ALM1_HOURS_10_BIT_POS						0x04
#define ALM1_HOURS_10_BIT_MSK						(0x01 << ALM1_HOURS_10_BIT_POS)
#define ALM1_HOURS_20_BIT_POS						0x05
#define ALM1_HOURS_20_BIT_MSK						(0x01 << ALM1_HOURS_20_BIT_POS)
#define ALM1_HOURS_AM_PM_BIT_POS				0x05
#define ALM1_HOURS_AM_PM_BIT_MSK				(0x01 << ALM1_HOURS_AM_PM_BIT_POS)
#define ALM1_HOURS_12_24_BIT_POS				0x06
#define ALM1_HOURS_12_24_BIT_MSK				(0x01 << ALM1_HOURS_12_24_BIT_POS)
#define ALM1_HOURS_A1M3_BIT_POS				0x07
#define ALM1_HOURS_A1M3_BIT_MSK				(0x01 << ALM1_HOURS_A1M3_BIT_POS)

#define ALM1_DAY_BIT_POS									0x00
#define ALM1_DAY_BIT_MSK									(0x0F << ALM1_DAY_BIT_POS)
#define ALM1_DATE_BIT_POS								0x00
#define ALM1_DATE_BIT_MSK								(0x0F << ALM1_DATE_BIT_POS)
#define ALM1_DATE_10_BIT_POS							0x04
#define ALM1_DATE_10_BIT_MSK							(0x03 << ALM1_DATE_10_BIT_POS)
#define ALM1_DY_DT_BIT_POS								0x06
#define ALM1_DY_DT_BIT_MSK							(0x01 << ALM1_DY_DT_BIT_POS)
#define ALM1_A1M4_BIT_POS								0x07
#define ALM1_A1M4_BIT_MSK								(0x01 << ALM1_A1M4_BIT_POS)

#define ALM2_MIN_BIT_POS									0x00
#define ALM2_MIN_BIT_MSK								(0x0F << ALM2_MIN_BIT_POS)
#define ALM2_MIN_10_BIT_POS							0X04
#define ALM2_MIN_10_BIT_MSK							(0x07 << ALM2_MIN_10_BIT_POS)
#define ALM2_MIN_A2M2_BIT_POS						0x07
#define ALM2_MIN_A2M2_BIT_MSK						(0x01 << ALM2_MIN_A2M2_BIT_POS)

#define ALM2_HOURS_BIT_POS							0x00
#define ALM2_HOURS_BIT_MSK							(0x0F << ALM2_HOURS_BIT_POS)
#define ALM2_HOURS_10_BIT_POS						0x04
#define ALM2_HOURS_10_BIT_MSK						(0x01 << ALM2_HOURS_10_BIT_POS)
#define ALM2_HOURS_20_BIT_POS						0x05
#define ALM2_HOURS_20_BIT_MSK						(0x01 << ALM2_HOURS_20_BIT_POS)
#define ALM2_HOURS_AM_PM_BIT_POS				0x05
#define ALM2_HOURS_AM_PM_BIT_MSK				(0x01 << ALM2_HOURS_AM_PM_BIT_POS)
#define ALM2_HOURS_12_24_BIT_POS				0x06
#define ALM2_HOURS_12_24_BIT_MSK				(0x01 << ALM2_HOURS_12_24_BIT_POS)
#define ALM2_HOURS_A2M2_BIT_POS				0x07
#define ALM2_HOURS_A2M2_BIT_MSK				(0x01 << ALM2_HOURS_A2M2_BIT_POS)

#define ALM2_DAY_BIT_POS									0x00
#define ALM2_DAY_BIT_MSK									(0x0F << ALM2_DAY_BIT_POS)
#define ALM2_DATE_BIT_POS								0x00
#define ALM2_DATE_BIT_MSK								(0x0F << ALM2_DATE_BIT_POS)
#define ALM2_DATE_10_BIT_POS							0x04
#define ALM2_DATE_10_BIT_MSK							(0x03 << ALM2_DATE_10_BIT_POS)
#define ALM2_DY_DT_BIT_POS								0x06
#define ALM2_DY_DT_BIT_MSK							(0x01 << ALM2_DY_DT_BIT_POS)
#define ALM2_A2M4_BIT_POS								0x07
#define ALM2_A2M4_BIT_MSK								(0x01 << ALM2_A2M4_BIT_POS)

#define CONTROL_A1IE_BIT_POS							0x00
#define CONTROL_A2IE_BIT_POS							0x01
#define CONTROL_INTCN_BIT_POS						0x02
#define CONTROL_RS1_BIT_POS							0x03
#define CONTROL_RS2_BIT_POS							0x04
#define CONTROL_CONV_BIT_POS						0x05
#define CONTROL_BBSQW_BIT_POS					0x06
#define CONTROL_EOSC_BIT_POS						0x07

#define CONTROL_STATUS_A1F_BIT_POS				0x00
#define CONTROL_STATUS_A2F_BIT_POS				0x01
#define CONTROL_STATUS_BSY_BIT_POS				0x02
#define CONTROL_STATUS_EN32KHZ_BIT_POS	0x03
#define CONTROL_STATUS_OSF_BIT_POS				0x07

#define AGING_DATA_BIT_POS								0x00
#define AGING_DATA_BIT_MSK								(0x7F << AGING_DATA_BIT_POS)
#define AGING_SIGN_BIT_POS								0x07
#define TEMP_MSB_DATA_BIT_POS						0x00
#define TEMP_MSB_DATA_BIT_MSK						(0x7F << TEMP_MSB_DATA_BIT_POS)
#define TEMP_MSB_SIGN_BIT_POS						0x07

#define TEMP_LSB_DATA_BIT_POS						0x06
#define TEMP_LSB_DATA_BIT_MSK						(0x03 << TEMP_LSB_DATA_BIT_POS)

/****************************
 * Struct Handler
 ****************************/

#define DS3231_BASIC_SET									15

//@ref_Clock_Mode_12_24
#define CLOCK_MODE_12										0x01
#define CLOCK_MODE_24										0x00

//@ref_Clock_Mode_AM_PM
#define CLOCK_MODE_PM									0x01
#define CLOCK_MODE_AM									0x00

//@ref_Alarm_select
#define ALARM_1													0x00
#define ALARM_2													0x01

//@ref_ALM_DY_DT
#define ALM_DY														0x01
#define ALM_DT														0x00

//@ref_SQW
#define SQW_1														0x00
#define SQW_1_4													0x01
#define SQW_4														0x02
#define SQW_8														0x03

//@ref_INTCN
#define INTCN_DI													0x00
#define INTCN_EN													0x01

//@ref_Alarm_rate

//For Alarm 1
#define ALM1_ONCE_PER_SECOND														0x00
#define ALM1_WHEN_SECOND_MATCH												0x01
#define ALM1_WHEN_SECOND_MINUTE_MATCH								0x02
#define ALM1_WHEN_SECOND_MINUTE_HOUR_MATCH					0x03
#define ALM1_WHEN_DATE_SECOND_MINUTE_HOUR_MATCH			0x04
#define ALM1_WHEN_DAY_SECOND_MINUTE_HOUR_MATCH			0x05

//For Alarm 2
#define ALM2_ONCE_PER_MINUTE														0x06
#define ALM2_WHEN_MINUTE_MATCH												0x07
#define ALM2_WHEN_MINUTE_HOUR_MATCH									0x08
#define ALM2_WHEN_DATE_MINUTE_HOUR_MATCH							0x09
#define ALM2_WHEN_DAY_MINUTE_HOUR_MATCH							0x0A

//@ref_TIM
#define TIM_REQ																					0x00
#define TIM_NOT_REQ																			0x01

//@ref_ALM
#define ALM_REQ																					0x00
#define ALM_NOT_REQ																			0x01

typedef struct{
	uint8_t DEV_ADD;												//Device Address
	uint8_t reg_address;											//The offset from where data should be written
	uint8_t Seconds; 													//In the range 0 - 59
	uint8_t Minutes; 													//In the range 0 - 59
	uint8_t Hours;														//In the range 0 - 23 / 1 - 12
	uint8_t Day;															//In the range 1 - 7
	uint8_t Date;														//In the range 1 - 31
	uint8_t Month_Century;										//In the range 1 - 12
	uint8_t Year;														//In the range 0 - 99
	uint8_t ALM1_Seconds;										//In the range 0 - 59
	uint8_t ALM1_Minutes;										//In the range 0 - 59
	uint8_t ALM1_Hours;											//In the range 0 - 23 / 1 - 12
	uint8_t ALM1_Day_Date;									//In the range 1 - 31
	uint8_t ALM2_Minutes;										//In the range 0 - 59
	uint8_t ALM2_Hours;											//In the range 0 - 23 / 1 - 12
	uint8_t ALM2_Day_Date;									//In the range 1 - 31
	uint8_t Control;
	uint8_t Control_Status;
	uint8_t Aging_Offset;
	uint8_t MSB_Temp;
	uint8_t LSB_Temp;
}ds3231_reg;

typedef struct{
	uint8_t seconds; 													//In the range 0 - 59
	uint8_t minutes; 													//In the range 0 - 59
	uint8_t Hour; 														//In the range 0 - 23 / 1 - 12
	uint8_t Clock_Mode_12_24;								//@ref_Clock_Mode_12_24
	uint8_t Clock_Mode_AM_PM;					       	//@ref_Clock_Mode_AM_PM
	uint8_t Day;															//In the range 1 - 7
	uint8_t Date;														//In the range 1 - 31
	uint8_t Month;													//In the range 1 - 12
	uint8_t Century;													//In the range 0 - 1
	uint8_t Year;														//In the range 0 - 99
}ds3231_time_handler_t;

typedef struct{
	uint8_t ALM_1_2;												//@ref_Alarm_select
	uint8_t ALM_Rate;												//@ref_Alarm_rate
	uint8_t ALM_Seconds;										//In the range 0 - 59
	uint8_t ALM_Minutes;										//In the range 0 - 59
	uint8_t ALM_Hours;											//In the range 0 - 23 / 1 - 12
	uint8_t ALM_Clock_Mode_12_24;						//@ref_Clock_Mode_12_24
	uint8_t ALM_Clock_AM_PM;								//@ref_Clock_Mode_AM_PM
	uint8_t ALM_DY_DT;											//@ref_ALM_DY_DT
	uint8_t ALM_Date;												//In the range 1 - 31
	uint8_t ALM_Day;												//In the range 1 - 7
	uint8_t ALM_A1M1;											//In the range 0 - 1
	uint8_t ALM_A1M2;											//In the range 0 - 1
	uint8_t ALM_A1M3;											//In the range 0 - 1
	uint8_t ALM_A1M4;											//In the range 0 - 1
	uint8_t ALM_A2M2;											//In the range 0 - 1
	uint8_t ALM_A2M3;											//In the range 0 - 1
	uint8_t ALM_A2M4;											//In the range 0 - 1
}ds3231_alarm_handler_t;

typedef struct{
	ds3231_time_handler_t* time;
	ds3231_alarm_handler_t* alarm;
	uint8_t ALM_SET;												//@ref_ALM
	uint8_t TIM_SET;													//@ref_TIM
	uint8_t INTCN;														//@ref_INTCN
	uint8_t SQW;														//@ref_SQW
	uint8_t aging_data;
}ds3231_handler_t;

typedef enum{
	OK,
	NOT_OK
}err_t;

/****************************
 * API Available
 ****************************/

err_t ds3231_set(ds3231_handler_t* myds);
err_t ds3231_time_set(ds3231_handler_t* myds);
err_t ds3231_alarm_set(ds3231_handler_t* myds);
uint8_t bcd2bin(uint8_t bcd);
uint8_t bin2bcd(uint8_t bin);
void ds3231_read_time(ds3231_time_handler_t* mytime);
void ds3231_clear_registers(void);

#endif /* INC_DS3231_H_ */
