/*
 * ds3231.c
 *
 *  Created on: Jul 2, 2025
 *      Author: mrohi
 */


#include "ds3231.h"

extern  I2C_HandleTypeDef myi2c;

ds3231_reg txrx_reg = {0};

uint8_t bcd2bin(uint8_t bcd)
{
	int ret = 0;
	ret = (bcd >> 0x4) & 0xF;
	ret = (ret*10)+(bcd & 0xF);
	return ret;
}

uint8_t bin2bcd(uint8_t bin)
{
	int ret = 0;
	ret = bin%10;
	bin = bin/10;
	ret |= (bin%10 << 4);
	return ret;
}

static void my_memset(void* ptr, uint8_t size)
{
	if(ptr == NULL)
			return;

	for(int i = 0; i < size; i++)
	{
		*(uint8_t*)(ptr++) = 0;
	}
}

err_t ds3231_set(ds3231_handler_t* myds)
{
	my_memset(&txrx_reg, sizeof(txrx_reg));

	txrx_reg.DEV_ADD = DS3231_ADDRESS << 1;

	//check if time setting is required
	if((myds->TIM_SET == TIM_REQ))
	{
		if(ds3231_time_set(myds) != OK)
		{
			printf("Time Setting Failed\n");
			Error_Handler();
		}
	}

	//check if alarm setting is required
	if((myds->ALM_SET == ALM_REQ))
	{
		if(myds->alarm->ALM_Rate == ALM1_ONCE_PER_SECOND)
		{
			myds->alarm->ALM_A1M1 = 1;
			myds->alarm->ALM_A1M2 = 1;
			myds->alarm->ALM_A1M3 = 1;
			myds->alarm->ALM_A1M4 = 1;
		}
		else if(myds->alarm->ALM_Rate == ALM1_WHEN_SECOND_MATCH)
		{
			myds->alarm->ALM_A1M1 = 0;
			myds->alarm->ALM_A1M2 = 1;
			myds->alarm->ALM_A1M3 = 1;
			myds->alarm->ALM_A1M4 = 1;
		}
		else if(myds->alarm->ALM_Rate == ALM1_WHEN_SECOND_MINUTE_MATCH)
		{
			myds->alarm->ALM_A1M1 = 0;
			myds->alarm->ALM_A1M2 = 0;
			myds->alarm->ALM_A1M3 = 1;
			myds->alarm->ALM_A1M4 = 1;
		}
		else if(myds->alarm->ALM_Rate == ALM1_WHEN_SECOND_MINUTE_HOUR_MATCH)
		{
			myds->alarm->ALM_A1M1 = 0;
			myds->alarm->ALM_A1M2 = 0;
			myds->alarm->ALM_A1M3 = 0;
			myds->alarm->ALM_A1M4 = 1;
		}
		else
		{
			myds->alarm->ALM_A1M1 = 0;
			myds->alarm->ALM_A1M2 = 0;
			myds->alarm->ALM_A1M3 = 0;
			myds->alarm->ALM_A1M4 = 0;
		}

		if(ds3231_alarm_set(myds) != OK)
		{
			printf("Alarm Setting Failed\n");
			Error_Handler();
		}

		txrx_reg.Control |= (0x1 << CONTROL_A1IE_BIT_POS) | (0x1 << CONTROL_INTCN_BIT_POS);
	}

	txrx_reg.reg_address = SECONDS_REG;

	HAL_I2C_Master_Transmit(&myi2c, txrx_reg.DEV_ADD, &(txrx_reg.reg_address), DS3231_BASIC_SET, HAL_MAX_DELAY);

	return OK;
}

err_t ds3231_time_set(ds3231_handler_t* myds)
{
	uint8_t temp_val = 0;

	//Reset
	ds3231_clear_registers();

	//Seconds
	if(myds->time->seconds < 60)
		txrx_reg.Seconds = bin2bcd(myds->time->seconds);
	else
	{
		printf("Time Seconds is invalid...Setting to 0\n");
		txrx_reg.Seconds = 0;
	}

	//Minutes
	if(myds->time->minutes < 60)
		txrx_reg.Minutes = bin2bcd(myds->time->minutes);
	else
	{
		printf("Time Minutes is invalid...Setting to 0\n");
		txrx_reg.Minutes = 0;
	}

	//Hours
	if(myds->time->Clock_Mode_12_24 == CLOCK_MODE_12)
	{
		temp_val |= ((myds->time->Clock_Mode_12_24) << HOURS_12_24_POS) | ((myds->time->Clock_Mode_AM_PM) << HOURS_AM_PM_POS);
		if(myds->time->Hour > 20)
		{
			printf("Incorrect Time\n");
			return NOT_OK;
		}
		else if(myds->time->Hour > 10)
		{
			temp_val |= (0x1 << HOURS_10_BIT_POS);
		}
		temp_val |= (myds->time->Hour % 10) & 0xF;
	}
	else
	{
		temp_val = 0;
		temp_val |= ((myds->time->Clock_Mode_12_24) << HOURS_12_24_POS) | ((myds->time->Hour / 10) << HOURS_10_BIT_POS) | ((myds->time->Hour % 10) & 0xF);
	}
	txrx_reg.Hours = temp_val;

	//Day
	if(myds->time->Day <= 7)
		txrx_reg.Day = myds->time->Day;
	else
	{
		printf("Incorrect Day...Setting to Day 1\n");
		txrx_reg.Day = 1;
	}

	//Date
	if((myds->time->Date < 32))
	{
		txrx_reg.Date = bin2bcd(myds->time->Date);
	}
	else
	{
		printf("Incorrect Date...Setting to Date 1\n");
		txrx_reg.Date = 1;
	}

	//Month_Century
	if(myds->time->Month <=12)
	{
		txrx_reg.Month_Century = bin2bcd(myds->time->Month);
		txrx_reg.Month_Century |= myds->time->Century << CENTURY_BIT_POS;
	}
	else
	{
		printf("Incorrect Month...Setting to Month 1\n");
		txrx_reg.Month_Century = 1;
	}

	//Year
	if(myds->time->Year <= 99)
	{
		txrx_reg.Year = bin2bcd(myds->time->Year);
	}
	else
	{
		printf("Incorrect Date...Setting to Year 0\n");
		txrx_reg.Year = 0;
	}

	return OK;
}

err_t ds3231_alarm_set(ds3231_handler_t* myds)
{
	uint8_t temp_val = 0;
	if(myds->alarm->ALM_1_2 == ALARM_1)
	{
		//seconds
		txrx_reg.ALM1_Seconds = bin2bcd(myds->alarm->ALM_Seconds);
		txrx_reg.ALM1_Seconds |= (myds->alarm->ALM_A1M1 << ALM1_SEC_A1M1_BIT_POS);

		//minutes
		txrx_reg.ALM1_Minutes = bin2bcd(myds->alarm->ALM_Minutes);
		txrx_reg.ALM1_Minutes |= (myds->alarm->ALM_A1M2 << ALM1_MIN_A1M2_BIT_POS);

		//Hours
		if(myds->alarm->ALM_Clock_Mode_12_24 == CLOCK_MODE_12)
		{
			temp_val |= ((myds->alarm->ALM_Clock_Mode_12_24) << ALM1_HOURS_12_24_BIT_POS) | ((myds->alarm->ALM_Clock_AM_PM) << ALM1_HOURS_AM_PM_BIT_POS);
			if(myds->alarm->ALM_Hours > 20)
			{
				printf("Incorrect Time\n");
				return NOT_OK;
			}
			else if(myds->alarm->ALM_Hours > 10)
			{
				temp_val |= (0x1 << ALM1_HOURS_10_BIT_POS);
			}
			temp_val |= (myds->alarm->ALM_Hours % 10) & 0xF;
		}
		else
		{
			temp_val = 0;
			temp_val |= ((myds->alarm->ALM_Clock_Mode_12_24) << HOURS_12_24_POS) | ((myds->alarm->ALM_Hours / 10) << ALM1_HOURS_10_BIT_POS) | ((myds->alarm->ALM_Hours % 10) & 0xF);
		}
		txrx_reg.ALM1_Hours = temp_val;
		txrx_reg.ALM1_Hours |= (myds->alarm->ALM_A1M3 << ALM1_HOURS_A1M3_BIT_POS);

		//Day_Date
		if(myds->alarm->ALM_DY_DT == ALM_DY)
		{
			txrx_reg.ALM1_Day_Date = myds->alarm->ALM_Day;
		}
		else
		{
			txrx_reg.ALM1_Day_Date = bin2bcd(myds->alarm->ALM_Date);
		}
		txrx_reg.ALM1_Day_Date |= (0x1 << ALM1_DY_DT_BIT_POS) | (myds->alarm->ALM_A1M4 << ALM1_A1M4_BIT_POS);
	}
	else
	{
		printf("No support for Alarm 2 yet...Please use Alarm 1\n");
		return 0;
	}

	return OK;
}

void ds3231_read_time(ds3231_time_handler_t* mytime)
{
	my_memset(&txrx_reg, sizeof(txrx_reg));
	txrx_reg.DEV_ADD = DS3231_ADDRESS << 1;
	txrx_reg.reg_address = SECONDS_REG;

	HAL_I2C_Master_Transmit(&myi2c, txrx_reg.DEV_ADD, &txrx_reg.reg_address, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&myi2c, txrx_reg.DEV_ADD, &txrx_reg.Seconds, 7, HAL_MAX_DELAY);
	mytime->seconds = bcd2bin(txrx_reg.Seconds);
	mytime->minutes = bcd2bin(txrx_reg.Minutes);
	mytime->Day = txrx_reg.Day;
	mytime->Date = bcd2bin(txrx_reg.Date);
	mytime->Year = bcd2bin(txrx_reg.Year);
	mytime->Century = (txrx_reg.Month_Century >> CENTURY_BIT_POS) & (0x1);
	txrx_reg.Month_Century &= ~(CENTURY_BIT_MSK);
	mytime->Month = bcd2bin(txrx_reg.Month_Century);

	if((txrx_reg.Hours >> HOURS_12_24_POS) & 0x1)
	{
		mytime->Clock_Mode_AM_PM = (txrx_reg.Hours >> HOURS_AM_PM_POS) & 0x1;
		txrx_reg.Hours &= ~(HOURS_AM_PM_MSK);
	}
		mytime->Clock_Mode_12_24 = (txrx_reg.Hours >> HOURS_12_24_POS) & 0x1;
		txrx_reg.Hours &= ~(HOURS_12_24_MSK);
		mytime->Hour = bcd2bin(txrx_reg.Hours);
}
