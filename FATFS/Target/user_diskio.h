/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.h
  * @brief   This file contains the common defines and functions prototypes for
  *          the user_diskio driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_DISKIO_H
#define __USER_DISKIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern Diskio_drvTypeDef  USER_Driver;

//Commands
#define BASE																0x40

#define SD_CS_PIN														GPIO_PIN_9
#define SD_MISO_PIN												GPIO_PIN_11

#define BLOCK_SIZE													512
#define CMD_FRAME_SIZE										0x06
#define CMD_RESPONSE_FRAME_SIZE						0x02
#define CHECK_PATTERN											0xFF

#define POWER_ON													0xAA
#define CMD0                                                             0x00
#define CMD0_CRC                                                    0x95
#define CMD1                                                             0x01
#define CMD8                                                             0x08
#define CMD8_2_7V_3_6V				       					 0x01
#define CMD8_CRC                                                    0x87
#define CMD9                                                             0x09
#define CMD10                                                           0x0A
#define CMD12															 0x0C
#define CMD16                                                           0x10
#define CMD17                                                           0x11
#define CMD18                                                           0x12
#define CMD23                                                           0x17
#define CMD24                                                           0x18
#define CMD25                                                           0x19

#define ACMD41                                                         0x29
#define ACMD23                                                         0x17
#define CMD55                                                           0x37
#define CMD58                                                           0x3A

#define DUMMY_CRC												 0xFF
#define CRC_SIZE														 0x02

#define GO_IDLE_STATE	                                             CMD0
#define SEND_OP_COND	                                         CMD1
#define SEND_IF_COND	                                             CMD8
#define SEND_CSD	                                             		 CMD9
#define SEND_CID	                                                     CMD10
#define STOP_TRANSMISSION	                                 CMD12
#define SET_BLOCKLEN	                                             CMD16
#define READ_SINGLE_BLOCK	                                 CMD17
#define READ_MULTIPLE_BLOCK	                             CMD18
#define SET_BLOCK_COUNT	                                     CMD23
#define WRITE_BLOCK	                                             CMD24
#define WRITE_MULTIPLE_BLOCK	                             CMD25

#define APP_SEND_OP_COND	                                 ACMD41
#define SET_WR_BLOCK_ERASE_COUNT	                 ACMD23
#define APP_CMD	                                                     CMD55
#define READ_OCR	                                                 CMD58

#define CMD17_18_24_TOKEN									 0xFE
#define CMD17_TOKEN												 0xFE
#define CMD18_TOKEN												 0xFE
#define CMD24_TOKEN												 0xFE
#define CMD25_TOKEN												 0xFC
#define CMD25_STOP_TRAN_TOKEN						 0xFD

#define SINGLE_DATA_READ_LEN								 514

#define RETRY_LIMIT													500

#define R1																	 0x80

#define DATA_ACCEPTED											 0b0101
#define DATA_REJECTED_CRC									 0b1011
#define DATA_REJECTED_WRITE								 0b1101

#define SECTOR_COUNT 											67108864   // For 32GB card: (32*1024*1024*1024)/512
#define SECTOR_SIZE  												512
#define ERASE_BLOCK_SIZE   									128        // 64KB erase block / 512B = 128 sectors

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif

#endif /* __USER_DISKIO_H */
