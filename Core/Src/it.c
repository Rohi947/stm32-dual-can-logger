
#include "main.h"
#include "stm32f4xx_it.h"

#define GET_FLAG_STATUS(reg, bit) ((reg >> bit) & (0x1))

extern ADC_HandleTypeDef myadc1;
extern ADC_HandleTypeDef myadc2;
extern  TIM_HandleTypeDef mytim;

void SysTick_Handler(void)
{
	HAL_IncTick();
}


void ADC_IRQHandler(void)
{
	if(GET_FLAG_STATUS(myadc1.Instance->SR, ADC_SR_EOC_Pos))
	{
		HAL_ADC_IRQHandler(&myadc1);
	}
	else
	{
		HAL_ADC_IRQHandler(&myadc2);
	}
}

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&mytim);
}
