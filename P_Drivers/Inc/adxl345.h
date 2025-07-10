/*
 * adxl345.h
 *
 *  Created on: Jun 24, 2025
 *      Author: mrohi
 */

#ifndef INC_ADXL345_H_
#define INC_ADXL345_H_

#include "main.h"

//ADXL345 Address and ID
#define ADDRESS 											(0X53 << 1)
#define DEVID												11100101

//Devid Register
#define DEVID_REG										0x00

//Offset Registers
#define OFSX													0X1E
#define OFSY													0X1F
#define OFSZ													0x20

//Data rate & Power Control Registers
#define BW_RATE											0X2C
#define POWER_CTL										0X2D

//Interrupt Registers
#define INT_ENABLE										0X2E
#define INT_MAP											0X2F
#define INT_SOURCE										0X30

//Data Format Register
#define DATA_FORMAT									0X31

//Data Registers
#define DATAX0												0X32
#define DATAX1												0X33
#define DATAY0												0X34
#define DATAY1												0X35
#define DATAZ0												0X36
#define DATAZ1												0X37

//FIFO Registers
#define FIFO_CTL											0X38
#define FIFO_STATUS										0X39

#define BW_RATE_LOW_POWER_POS			0X04
#define BW_RATE_LOW_POWER_MSK			(0X01 << BW_RATE_LOW_POWER_POS)
#define BW_RATE_LOW_POWER_BIT			(0X01 << BW_RATE_LOW_POWER_POS)

#define BW_RATE_RATE_POS						0X00
#define BW_RATE_RATE_MSK						0XFF

#define POWER_CTL_MEASURE_POS			0X03
#define POWER_CTL_MEASURE_SET			(0X1 << POWER_CTL_MEASURE_POS)

#define DATA_FORMAT_FULL_RES				0X03
#define DATA_FORMAT_JUSTIFY					0X02
#define DATA_FORMAT_RANGE						0X00
#define DATA_FORMAT_RANGE_MSK			(0X3 << DATA_FORMAT_RANGE)
#define DATA_FORMAT_SPI_POS					0X06

#define DATA_FORMAT_SPI_3						(0X1 << DATA_FORMAT_SPI_POS)
#define DATA_FORMAT_SPI_4						~DATA_FORMAT_SPI_3

#define RANGE_2G											0X00
#define RANGE_4G											0X01
#define RANGE_8G											0X10
#define RANGE_16G										0X11

#define OP_MODE_LOW_POWER					0X01
#define OP_MODE_DEFAULT							0X00

#define BW_RATE_RATE_DEFAULT				0X0A

#define BW_RATE_RATE_32							0XF
#define BW_RATE_RATE_16							0XE

struct ADXL_Handle_t{
	uint8_t devid;
	uint8_t op_mode;
	uint8_t rate;
	uint8_t full_res;
	uint8_t justify;
	uint8_t range;
};


void adxl_init(struct ADXL_Handle_t *myadxl);
void adxl_start(struct ADXL_Handle_t* myadxl);
void adxl_getdata(struct ADXL_Handle_t* myadxl, int16_t* data);

#endif /* INC_ADXL345_H_ */
