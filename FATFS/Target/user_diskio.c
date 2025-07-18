/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "user_diskio.h"

/* Private typedef -----------------------------------------------------------*/
#include "main.h"
/* Private define ------------------------------------------------------------*/
extern SPI_HandleTypeDef myspi3;
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/
uint8_t send_command(uint8_t cmd)
{
	//Error returns

	uint8_t dummy_tx[CMD_FRAME_SIZE] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint8_t cmd_frame[CMD_FRAME_SIZE] = {0};
	uint8_t response_frame[CMD_FRAME_SIZE] = {0};

	cmd_frame[0] = BASE | cmd;

	if(cmd == POWER_ON)
	{
		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

		for(int i = 0; i < 10; i++)
			HAL_SPI_Transmit(&myspi3, dummy_tx, 1, HAL_MAX_DELAY);

		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);
	}
	else if(cmd == CMD0)
	{
		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);

		cmd_frame[1] = 0x00;
		cmd_frame[2] = 0x00;
		cmd_frame[3] = 0x00;
		cmd_frame[4] = 0x00;
		cmd_frame[5] = 0x95;

		HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

		uint8_t retry = 50;
		do{
			HAL_SPI_TransmitReceive(&myspi3, dummy_tx, response_frame, 1, HAL_MAX_DELAY);
		}while((response_frame[0] & 0x80) && --retry);

		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);
	}
	else if(cmd == CMD8)
	{
		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);

		cmd_frame[1] = 0x00;
		cmd_frame[2] = 0x00;
		cmd_frame[3] = 0x01;
		cmd_frame[4] = 0xAA;
		cmd_frame[5] = 0x87;

		HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

		HAL_SPI_TransmitReceive(&myspi3, dummy_tx, response_frame, 5, HAL_MAX_DELAY);

		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

		if(((response_frame[3] << 8) | (response_frame[4])) != 0x1AA)
			return 1;

		return 0;
	}
	else if(cmd == ACMD41)
	{
		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);
		uint8_t retry = 50;

		do{
			uint8_t retry_inner = 50;
			//CMD55
			cmd_frame[0] = BASE | CMD55;
			cmd_frame[1] = 0x00;
			cmd_frame[2] = 0x00;
			cmd_frame[3] = 0x00;
			cmd_frame[4] = 0x00;
			cmd_frame[5] = DUMMY_CRC;

			HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

			do{
				HAL_SPI_TransmitReceive(&myspi3, dummy_tx, response_frame, 1, HAL_MAX_DELAY);
			}while((response_frame[0] & 0x80) && --retry_inner);


			if(response_frame[0] != 0x01)
				return 1;

			retry_inner = 50;


			cmd_frame[0] = BASE | ACMD41;
			cmd_frame[1] = 0x1 << 6;
			cmd_frame[2] = 0x00;
			cmd_frame[3] = 0x00;
			cmd_frame[4] = 0x00;
			cmd_frame[5] = DUMMY_CRC;

			HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

			do{
				HAL_SPI_TransmitReceive(&myspi3, dummy_tx, response_frame, 1, HAL_MAX_DELAY);
			}while((response_frame[0] & 0x80) && --retry_inner);

		}while((response_frame[0] & 0x80) && --retry);

		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

		if(response_frame[0] != 0x00)
			return 1;

		return 0;
	}
	else if(cmd == CMD58)
	{
		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);

		cmd_frame[1] = 0x00;
		cmd_frame[2] = 0x00;
		cmd_frame[3] = 0x00;
		cmd_frame[4] = 0x00;
		cmd_frame[5] = DUMMY_CRC;

		HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

		HAL_SPI_TransmitReceive(&myspi3, dummy_tx, response_frame, 5, HAL_MAX_DELAY);

		HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

		if(!((response_frame[0] == 0x00) && (response_frame[1] & (0x1 << 6))))
			return 1;
		return 0;
	}
	return 0;
}
/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
	if(pdrv != 0)
	{
		Stat = STA_NOINIT;
		return Stat;
	}

	send_command(CMD0);

	send_command(CMD8);

	send_command(ACMD41);

	send_command(CMD58);

	Stat = 0;

	return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    Stat = STA_NOINIT;
    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
	if (pdrv != 0) return RES_PARERR;

	HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);

	uint8_t response = 0;
	uint8_t dummy = 0xFF;
	uint8_t dummy_tx[SINGLE_DATA_READ_LEN] = {0};
	uint8_t crc[CRC_SIZE] = {DUMMY_CRC, DUMMY_CRC};
	memset(dummy_tx, 0xFF, sizeof(dummy_tx));

	//Frame construction
	uint8_t cmd_frame[CMD_FRAME_SIZE] = {0};
	cmd_frame[1] = (sector >> 24) & 0xFF;
	cmd_frame[2] = (sector >> 16) & 0xFF;
	cmd_frame[3] = (sector >> 8) & 0xFF;
	cmd_frame[4] = sector & 0xFF;
	cmd_frame[5] = DUMMY_CRC;

	if(count == 1)
		cmd_frame[0] = BASE | CMD17;
	else
		cmd_frame[0] = BASE | CMD18;

	//Send Command
	HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

	//Wait for R1 Response
	uint16_t retry = RETRY_LIMIT;
	do{
		HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
		HAL_Delay(1);
	}while((response & 0x80) && retry);

	if((response != 0x00) || (retry == RETRY_LIMIT))
	{
			HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);
			return RES_ERROR;
	}

	for(int i = 0; i < count; i++)
	{
		//Wait for DATA Token
		retry = RETRY_LIMIT;
		do{
			HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
			HAL_Delay(1);
		}while((response != CMD17_18_24_TOKEN) && --retry);

		//Verify Data Token
		if((response != CMD17_18_24_TOKEN)  || (retry == RETRY_LIMIT))
		{
			HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);
			return RES_ERROR;
		}

		//Data Read
		HAL_SPI_TransmitReceive(&myspi3, dummy_tx, (buff + i*BLOCK_SIZE), BLOCK_SIZE, HAL_MAX_DELAY);

		//Discard CRC
		HAL_SPI_TransmitReceive(&myspi3, dummy_tx, crc, CRC_SIZE, HAL_MAX_DELAY);
	}

	//Stop Read
	if(count > 1) {
	    uint8_t stop_cmd[CMD_FRAME_SIZE] = {BASE | CMD12, 0x00, 0x00, 0x00, 0x00, DUMMY_CRC};
	    HAL_SPI_Transmit(&myspi3, stop_cmd, 6, HAL_MAX_DELAY);

	    // Wait for 0xFF
	    retry = RETRY_LIMIT;
	    do {
	        HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
	    } while (response != 0xFF && --retry);
	}

	HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

	//To settle
	for (int i = 0; i < 8; ++i)
		HAL_SPI_Transmit(&myspi3, &dummy, 1, HAL_MAX_DELAY);

    return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */

	if (pdrv != 0) return RES_PARERR;

	HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, DISABLE);

	uint8_t response = 0;
	uint8_t dummy = 0xFF;
	uint8_t dummy_tx[SINGLE_DATA_READ_LEN] = {0};
	uint8_t crc[CRC_SIZE] = {DUMMY_CRC, DUMMY_CRC};
	uint8_t data_response = 0x0;
	memset(dummy_tx, 0xFF, sizeof(dummy_tx));

	//Frame construction
	uint8_t cmd_frame[CMD_FRAME_SIZE] = {0};
	cmd_frame[1] = (sector >> 24) & 0xFF;
	cmd_frame[2] = (sector >> 16) & 0xFF;
	cmd_frame[3] = (sector >> 8) & 0xFF;
	cmd_frame[4] = sector & 0xFF;
	cmd_frame[5] = DUMMY_CRC;

	if(count == 1)
		cmd_frame[0] = BASE | CMD24;
	else
		cmd_frame[0] = BASE | CMD25;

	//Send Command
	HAL_SPI_Transmit(&myspi3, cmd_frame, CMD_FRAME_SIZE, HAL_MAX_DELAY);

	//Wait for R1 Response
	uint16_t retry = RETRY_LIMIT;
	do{
		HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
		HAL_Delay(1);
	}while((response & 0x80) && retry);

	if((response != 0x00) || (retry == RETRY_LIMIT))
	{
			HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);
			return RES_ERROR;
	}

	//Stalling
	HAL_SPI_Transmit(&myspi3, &dummy, 1, HAL_MAX_DELAY);

	uint8_t token = (count == 1) ? CMD17_18_24_TOKEN : CMD25_TOKEN;

	//Write
	for(int i = 0; i < count; i++)
	{
		HAL_SPI_Transmit(&myspi3, &token, 1, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&myspi3, &buff[i*BLOCK_SIZE], BLOCK_SIZE, HAL_MAX_DELAY);
		HAL_SPI_Transmit(&myspi3, crc, CRC_SIZE, HAL_MAX_DELAY);

		HAL_SPI_TransmitReceive(&myspi3, &dummy, &data_response, 1, HAL_MAX_DELAY);

		if((data_response & 0x0F) != (DATA_ACCEPTED))
		{
			HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);
			return RES_ERROR;
		}

	    // Wait for 0xFF
	    retry = RETRY_LIMIT;
	    do {
	        HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
	    } while (response != 0xFF && --retry);
	}

	//Stop Write
	if(count > 1) {
		token = CMD25_STOP_TRAN_TOKEN;
	    HAL_SPI_Transmit(&myspi3, &token, 1, HAL_MAX_DELAY);

	    // Wait for 0xFF
	    retry = RETRY_LIMIT;
	    do {
	        HAL_SPI_TransmitReceive(&myspi3, &dummy, &response, 1, HAL_MAX_DELAY);
	    } while (response != 0xFF && --retry);
	}

	HAL_GPIO_WritePin(GPIOC, SD_CS_PIN, ENABLE);

	//To settle
	for (int i = 0; i < 8; ++i)
		HAL_SPI_Transmit(&myspi3, &dummy, 1, HAL_MAX_DELAY);

    return RES_OK;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */

    DRESULT res = RES_OK;

    switch (cmd)
    {
        case CTRL_SYNC:
        	break;

        case GET_SECTOR_COUNT:
            *(DWORD*)buff = SECTOR_COUNT;
        	break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = SECTOR_SIZE;
        	break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = ERASE_BLOCK_SIZE;
        	break;

        default:
        	res = RES_PARERR;
        	break;
    }

    return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

