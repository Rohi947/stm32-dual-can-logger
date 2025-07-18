/*
 * msp.c
 *
 *  Created on: Jun 19, 2025
 *      Author: mrohi
 */

#include "main.h"

GPIO_InitTypeDef gpio_init = {0};
extern UART_HandleTypeDef myuart;

int __io_putchar(int ch)
{
	HAL_UART_Transmit(&myuart, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	return 0;
}

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

 void HAL_UART_MspInit(UART_HandleTypeDef *huart)
 {
	 __HAL_RCC_GPIOD_CLK_ENABLE();
	 __HAL_RCC_USART3_CLK_ENABLE();

	 gpio_init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	 gpio_init.Mode = GPIO_MODE_AF_PP;
	 gpio_init.Pull = GPIO_NOPULL;
	 gpio_init.Speed = GPIO_SPEED_FAST;
	 gpio_init.Alternate = GPIO_AF7_USART3;
	 HAL_GPIO_Init(GPIOD, &gpio_init);
 }

 void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
 {
	 __HAL_RCC_GPIOB_CLK_ENABLE();

	 gpio_init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	 gpio_init.Mode = GPIO_MODE_AF_OD;
	 gpio_init.Pull = GPIO_NOPULL;
	 gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 gpio_init.Alternate = GPIO_AF4_I2C1;
	 HAL_GPIO_Init(GPIOB, &gpio_init);

	 HAL_NVIC_SetPriority(I2C1_ER_IRQn, 15, 15);
	 HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

	 HAL_NVIC_SetPriority(I2C1_EV_IRQn, 15, 15);
	 HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
 }

 void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
 {
	 if(hadc->Instance == ADC1)
	 {
		 __HAL_RCC_ADC1_CLK_ENABLE();
		 __HAL_RCC_GPIOA_CLK_ENABLE();

		 gpio_init.Pin = GPIO_PIN_0;
		 gpio_init.Mode = GPIO_MODE_ANALOG;
		 gpio_init.Pull = GPIO_NOPULL;
		 gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		 HAL_GPIO_Init(GPIOC, &gpio_init);

		 gpio_init.Pin = GPIO_PIN_3;
		 gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		 gpio_init.Pull = GPIO_NOPULL;
		 gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		 HAL_GPIO_Init(GPIOC, &gpio_init);

		 HAL_NVIC_SetPriority(ADC_IRQn, 15, 15);
		 HAL_NVIC_EnableIRQ(ADC_IRQn);
	 }
	 else if(hadc->Instance == ADC2)
	 {
		 __HAL_RCC_ADC2_CLK_ENABLE();
		 __HAL_RCC_GPIOC_CLK_ENABLE();

		 gpio_init.Pin = GPIO_PIN_3;
		 gpio_init.Mode = GPIO_MODE_ANALOG;
		 gpio_init.Pull = GPIO_NOPULL;
		 gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		 HAL_GPIO_Init(GPIOA, &gpio_init);

		 HAL_NVIC_SetPriority(ADC_IRQn, 15, 15);
		 HAL_NVIC_EnableIRQ(ADC_IRQn);
	 }
 }


 void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim)
 {
   if(htim->Instance==TIM4)
   {
		__HAL_RCC_TIM4_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();

		gpio_init.Pin = GPIO_PIN_12|GPIO_PIN_13;
		gpio_init.Mode = GPIO_MODE_AF_PP;
		gpio_init.Pull = GPIO_NOPULL;
		gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
		gpio_init.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(GPIOD, &gpio_init);

		HAL_NVIC_SetPriority(TIM4_IRQn, 15, 15);
		HAL_NVIC_EnableIRQ(TIM4_IRQn);

   }
 }

 void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
 {
	 if(hspi->Instance == SPI1)
	 {
	 __HAL_RCC_SPI1_CLK_ENABLE();
	 __HAL_RCC_GPIOA_CLK_ENABLE();

	 gpio_init.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	 gpio_init.Mode = GPIO_MODE_AF_PP;
	 gpio_init.Pull = GPIO_NOPULL;
	 gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 gpio_init.Alternate = GPIO_AF5_SPI1;

	 HAL_GPIO_Init(GPIOA, &gpio_init);

	 HAL_NVIC_SetPriority(SPI1_IRQn, 15, 15);
	 HAL_NVIC_EnableIRQ(SPI1_IRQn);
 }
	 else if(hspi->Instance == SPI3)
	 {
		    __HAL_RCC_SPI3_CLK_ENABLE();
		    __HAL_RCC_GPIOC_CLK_ENABLE();

		    gpio_init.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
		    gpio_init.Mode = GPIO_MODE_AF_PP;
		    gpio_init.Pull = GPIO_NOPULL;
		    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		    gpio_init.Alternate = GPIO_AF6_SPI3;
		    HAL_GPIO_Init(GPIOC, &gpio_init);

		    gpio_init.Pin =GPIO_PIN_9;
		    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		    gpio_init.Pull = GPIO_NOPULL;
		    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		    HAL_GPIO_Init(GPIOC,  &gpio_init);
	 }
 }
