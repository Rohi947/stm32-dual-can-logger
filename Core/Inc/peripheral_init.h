/*
 * peripheral_init.h
 *
 *  Created on: Jun 19, 2025
 *      Author: mrohi
 */

#ifndef INC_PERIPHERAL_INIT_H_
#define INC_PERIPHERAL_INIT_H_

#include "main.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_UART_Init(void);
void MX_I2C1_Init(void);
void MX_I2C2_Init(void);
void MX_ADC1_Init(void);
void MX_ADC2_Init(void);
void MX_TIM4_Init(void);
void MX_SPI1_Init(void);
void MX_SPI3_Init(void);
void MX_CAN1_Init(void);
void MX_CAN2_Init(void);
void can_tx(void);
void can_init(void);
void config_filters(void);
#endif /* INC_PERIPHERAL_INIT_H_ */
