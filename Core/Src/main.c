/*
 * main.c
 *
 *  Created on: Jun 19, 2025
 *      Author: mrohi
 */
#include "main.h"
#include "peripheral_init.h"
#include "adxl345.h"
#include "fatfs.h"

#define LIGHT_THRESH 999

//UART
extern UART_HandleTypeDef myuart;
extern I2C_HandleTypeDef myi2c;
extern ADC_HandleTypeDef myadc1;
extern ADC_HandleTypeDef myadc2;
extern  TIM_HandleTypeDef mytim;
extern SPI_HandleTypeDef myspi1;
extern SPI_HandleTypeDef myspi2;

volatile uint16_t adc1_result = 0;
volatile uint8_t adc1_flag = 0;
volatile uint16_t adc2_result = 0;
volatile uint8_t adc2_flag = 0;
volatile uint8_t pwm_flag = 0;

int get_water_level(void)
{
    if (adc1_result <= 800) return 100;
    if (adc1_result >= 3600) return 0;

    return ((3600 - adc1_result) * 0.03125);
}

int get_photoresistor_level(void)
{
	if(adc2_result < 100)
		return LIGHT_THRESH;
	if(adc2_result > 3000)
		return 0;

	int temp  = (3000 - adc2_result) * 0.084;
	return (temp);
}

int main()
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_UART_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_TIM4_Init();
	MX_SPI1_Init();
	MX_SPI3_Init();
	MX_FATFS_Init();

	HAL_TIM_PWM_Start(&mytim, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&mytim, TIM_CHANNEL_2);

	HAL_TIM_Base_Start_IT(&mytim);

	while(1)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, ENABLE);
		HAL_Delay(250);

		HAL_ADC_Start_IT(&myadc1);
		HAL_ADC_Start_IT(&myadc2);

    	int adc1_val = 0;
    	int adc2_val = 0;

        if (adc1_flag && adc2_flag && pwm_flag)
        {
        	__disable_irq();
				adc1_flag = 0;
				adc2_flag = 0;
				pwm_flag = 0;

				adc1_val = get_water_level();
				adc2_val = get_photoresistor_level();
				__enable_irq();

				__HAL_TIM_SET_COMPARE(&mytim, TIM_CHANNEL_1, adc1_val);
				__HAL_TIM_SET_COMPARE(&mytim, TIM_CHANNEL_2,  adc2_val);

				char msg[150] = {0};
				snprintf(msg, 150, "Water Level: %d\n Light Level: %d\n", adc1_val, adc2_val);
        }

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, DISABLE);
		HAL_Delay(250);
	}
	return 0;
}

//Callbacks
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1)
	{
		adc1_flag = 1;
		adc1_result = HAL_ADC_GetValue(&myadc1);
	}
	else if(hadc->Instance == ADC2)
	{
		adc2_flag = 1;
		adc2_result = HAL_ADC_GetValue(&myadc2);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
    	pwm_flag = 1;
    }
}

