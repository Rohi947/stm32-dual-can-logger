/*
 * peripheral_init.c
 *
 *  Created on: Jun 19, 2025
 *      Author: mrohi
 */

#include "peripheral_init.h"

 UART_HandleTypeDef myuart;
 I2C_HandleTypeDef myi2c;
 I2C_HandleTypeDef myi2c2;
 ADC_HandleTypeDef myadc1;
 ADC_HandleTypeDef myadc2;
 TIM_HandleTypeDef mytim;
 SPI_HandleTypeDef myspi1;
 SPI_HandleTypeDef myspi3;
 CAN_HandleTypeDef mycan1;
 CAN_HandleTypeDef mycan2;

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef myosc = {0};
	RCC_ClkInitTypeDef myclk = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	myosc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	myosc.HSIState = RCC_HSI_ON;
	myosc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	myosc.PLL.PLLState = RCC_PLL_NONE;

	if(HAL_RCC_OscConfig(&myosc) != HAL_OK)
	{
		Error_Handler();
	}

	myclk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	myclk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	myclk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	myclk.APB1CLKDivider = RCC_HCLK_DIV2;
	myclk.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&myclk, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef mygpio = {0};

	//clock enable
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	//Led setup
	mygpio.Pin = LD1_Pin | LD2_Pin | LD3_Pin;
	mygpio.Mode = GPIO_MODE_OUTPUT_PP;
	mygpio.Pull = GPIO_NOPULL;
	mygpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LD1_GPIO_Port, &mygpio);

	//Button setup
	mygpio.Pin = USER_Btn_Pin;
	mygpio.Mode = GPIO_MODE_IT_RISING;
	mygpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(USER_Btn_GPIO_Port, &mygpio);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 15);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void MX_UART_Init(void)
{
	myuart.Instance = USART3;
	myuart.Init.BaudRate = 115200;
	myuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	myuart.Init.Mode = UART_MODE_TX_RX;
	myuart.Init.OverSampling = UART_OVERSAMPLING_16;
	myuart.Init.Parity = UART_PARITY_NONE;
	myuart.Init.StopBits = UART_STOPBITS_1;
	myuart.Init.WordLength = UART_WORDLENGTH_8B;
	if(HAL_UART_Init(&myuart) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_I2C1_Init(void)
{
	myi2c.Instance = I2C1;
	myi2c.Mode = HAL_I2C_MODE_MASTER;
	myi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	myi2c.Init.ClockSpeed = 100000;
	myi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	myi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	myi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	myi2c.Init.OwnAddress1 = 0;
	myi2c.Init.OwnAddress2 = 0;
	if(HAL_I2C_Init(&myi2c) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_I2C2_Init(void)
{
	myi2c2.Instance = I2C2;
	myi2c2.Mode = HAL_I2C_MODE_MASTER;
	myi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	myi2c2.Init.ClockSpeed = 100000;
	myi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	myi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	myi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	myi2c2.Init.OwnAddress1 = 0;
	myi2c2.Init.OwnAddress2 = 0;
	if(HAL_I2C_Init(&myi2c2) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_ADC1_Init(void)
{
	myadc1.Instance = ADC1;

	ADC_ChannelConfTypeDef mysconfig = {0};

	myadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	myadc1.Init.Resolution = ADC_RESOLUTION_12B;
	myadc1.Init.ScanConvMode = DISABLE;
	myadc1.Init.ContinuousConvMode = DISABLE;
	myadc1.Init.DiscontinuousConvMode = DISABLE;
	myadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	myadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	myadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	myadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	myadc1.Init.NbrOfConversion = 1;
	myadc1.Init.DMAContinuousRequests = DISABLE;
	if(HAL_ADC_Init(&myadc1) != HAL_OK)
	{
		Error_Handler();
	}

	mysconfig.Channel = ADC_CHANNEL_10;
	mysconfig.Rank = 1;
	mysconfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if(HAL_ADC_ConfigChannel(&myadc1, &mysconfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void MX_ADC2_Init(void)
{
	myadc2.Instance = ADC2;

	ADC_ChannelConfTypeDef mysconfig = {0};

	myadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	myadc2.Init.Resolution = ADC_RESOLUTION_12B;
	myadc2.Init.ScanConvMode = DISABLE;
	myadc2.Init.ContinuousConvMode = DISABLE;
	myadc2.Init.DiscontinuousConvMode = DISABLE;
	myadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	myadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	myadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	myadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	myadc2.Init.NbrOfConversion = 1;
	myadc2.Init.DMAContinuousRequests = DISABLE;
	if(HAL_ADC_Init(&myadc2) != HAL_OK)
	{
		Error_Handler();
	}

	mysconfig.Channel = ADC_CHANNEL_3;
	mysconfig.Rank = 1;
	mysconfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if(HAL_ADC_ConfigChannel(&myadc2, &mysconfig) != HAL_OK)
	{
		Error_Handler();
	}

}

void MX_TIM4_Init(void)
{

	TIM_OC_InitTypeDef sConfigOC = {0};

	mytim.Instance = TIM4;
	mytim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	mytim.Init.CounterMode = TIM_COUNTERMODE_UP;
	mytim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mytim.Init.Period = 999;
	mytim.Init.Prescaler = 15;

	if (HAL_TIM_PWM_Init(&mytim) != HAL_OK)
	  {
	    Error_Handler();
	  }

	memset(&sConfigOC, 0, sizeof(sConfigOC));


	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  if (HAL_TIM_PWM_ConfigChannel(&mytim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&mytim, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

void MX_SPI1_Init(void)
{
	myspi1.Instance = SPI1;
	myspi1.Init.Mode = SPI_MODE_MASTER;
	myspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	myspi1.Init.Direction = SPI_DIRECTION_2LINES;
	myspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	myspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
	myspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	myspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	myspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;

	if(HAL_SPI_Init(&myspi1) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_SPI3_Init(void)
{
	myspi3.Instance = SPI3;
	myspi3.Init.Mode = SPI_MODE_MASTER;
	myspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	myspi3.Init.Direction = SPI_DIRECTION_2LINES;
	myspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	myspi3.Init.NSS = SPI_NSS_SOFT;
	myspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	myspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
	myspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;

	if(HAL_SPI_Init(&myspi3) != HAL_OK)
	{
		Error_Handler();
	}
}



void MX_CAN1_Init(void)
{
  mycan1.Instance = CAN1;
  mycan1.Init.Prescaler = 6;
  mycan1.Init.Mode = CAN_MODE_NORMAL;
  mycan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  mycan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  mycan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  mycan1.Init.TimeTriggeredMode = DISABLE;
  mycan1.Init.AutoBusOff = DISABLE;
  mycan1.Init.AutoWakeUp = DISABLE;
  mycan1.Init.AutoRetransmission = ENABLE;
  mycan1.Init.ReceiveFifoLocked = DISABLE;
  mycan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&mycan1) != HAL_OK)
  {
    Error_Handler();
  }
}


void MX_CAN2_Init(void)
{
  mycan2.Instance = CAN2;
  mycan2.Init.Prescaler = 6;
  mycan2.Init.Mode = CAN_MODE_NORMAL;
  mycan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  mycan2.Init.TimeSeg1 = CAN_BS1_13TQ;
  mycan2.Init.TimeSeg2 = CAN_BS2_2TQ;
  mycan2.Init.TimeTriggeredMode = DISABLE;
  mycan2.Init.AutoBusOff = DISABLE;
  mycan2.Init.AutoWakeUp = DISABLE;
  mycan2.Init.AutoRetransmission = ENABLE;
  mycan2.Init.ReceiveFifoLocked = DISABLE;
  mycan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&mycan2) != HAL_OK)
  {
    Error_Handler();
  }
}

//Error Handler
void Error_Handler(void)
{
	for(;;);
}
