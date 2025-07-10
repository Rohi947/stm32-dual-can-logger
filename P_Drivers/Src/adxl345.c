/*
 * adxl345.c
 *
 *  Created on: Jun 24, 2025
 *      Author: mrohi
 */


#include "adxl345.h"

extern SPI_HandleTypeDef myspi;

const uint16_t lsb_table[4] = {256, 128, 64, 32};

void adxl_init(struct ADXL_Handle_t *myadxl)
{
	//check if the device is matching
	uint8_t reg_address = DEVID_REG;
	uint8_t tx_reg[2] = {0};
	uint8_t rx_reg[2] = {0};
	uint8_t val = 0;

	//frame construction
	tx_reg[0] = (0x2 << 6) | DEVID_REG;
	tx_reg[1] = 0xFF;

	HAL_SPI_TransmitReceive(&myspi, tx_reg, rx_reg, 2, HAL_MAX_DELAY);
	//HAL_SPI_Receive(&myspi, rx_reg, 2, HAL_MAX_DELAY); Not required as we automatically receive dummy. @ADXL345 RM - P15

	val = rx_reg[1];

//	HAL_I2C_Master_Transmit(&myi2c, ADDRESS, &reg_address, sizeof(reg_address), HAL_MAX_DELAY);
//	HAL_I2C_Master_Receive(&myi2c, ADDRESS, &temp, 1, HAL_MAX_DELAY);

	if(val != DEVID)
	{
		printf("Incorrect DEVID Detected\n");
		return;
	}
	else
	{
		printf("DEVID matched\n");
	}

	//Range
	tx_reg[0] = DATA_FORMAT | 0x80;
	tx_reg[1] = 0xFF;
	HAL_SPI_TransmitReceive(&myspi, tx_reg, rx_reg, 2, HAL_MAX_DELAY);

//	HAL_I2C_Master_Transmit(&myi2c, ADDRESS, &reg_address, 1, HAL_MAX_DELAY);
//	HAL_I2C_Master_Receive(&myi2c, ADDRESS, &temp, 1, HAL_MAX_DELAY);

	rx_reg[1] &= ~(0x3);
	rx_reg[1] |=  0x3 & (myadxl->range);

	tx_reg[0] = DATA_FORMAT;
	tx_reg[1] = rx_reg[1];
	HAL_SPI_Transmit(&myspi, tx_reg, 2, HAL_MAX_DELAY);
//	HAL_I2C_Master_Transmit(&myi2c, ADDRESS, tx_reg, 2, HAL_MAX_DELAY);


	//BW_RATE

	//frame construction
	tx_reg[0] = BW_RATE | 0x80;
	tx_reg[1] = 0xFF;

	HAL_SPI_TransmitReceive(&myspi, tx_reg, rx_reg, 2, HAL_MAX_DELAY);

	rx_reg[1] &= ~(0xF << BW_RATE_RATE_POS);
	rx_reg[1] |= myadxl->rate << BW_RATE_RATE_POS;

	//frame construction
	tx_reg[0] = BW_RATE;
	tx_reg[1] = rx_reg[1];

	HAL_SPI_Transmit(&myspi, tx_reg, 2, HAL_MAX_DELAY);

}

void adxl_start(struct ADXL_Handle_t* myadxl)
{
	uint8_t tx_reg[2] = {0};
	tx_reg[0] = POWER_CTL;
	tx_reg[1] = 0x1 << POWER_CTL_MEASURE_POS;

	HAL_SPI_Transmit(&myspi, tx_reg, 2, HAL_MAX_DELAY);
}

void adxl_getdata(struct ADXL_Handle_t* myadxl, int16_t* data)
{
	uint8_t tx_reg[7] = {0};
	tx_reg[0] = DATAX0 | 0xC;
	uint8_t rx_reg[7] = {0};
	int16_t result[3] = {0};
	uint16_t lsb = 0;

	HAL_SPI_TransmitReceive(&myspi, tx_reg, rx_reg, 7, HAL_MAX_DELAY);

//	HAL_I2C_Master_Transmit(&myi2c, ADDRESS, &reg_address, 1, HAL_MAX_DELAY);
//	HAL_I2C_Master_Receive(&myi2c, ADDRESS, temp, 6, HAL_MAX_DELAY);

	result[0] = (rx_reg[2] << 8) | rx_reg[1];
	result[1] = (rx_reg[4] << 8) | rx_reg[3];
	result[2] = (rx_reg[6] << 8) | rx_reg[5];

	lsb = lsb_table[myadxl->range];

	data[0] = result[0] / lsb;
	data[1] = result[1] / lsb;
	data[2]  = result[2] / lsb;
}
