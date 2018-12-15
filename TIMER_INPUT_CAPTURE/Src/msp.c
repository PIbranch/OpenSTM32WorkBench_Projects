/*
 * msp.c
 *
 *  Created on: Dec 13, 2018
 *      Author: yonur
 */

#include "main.h"

void HAL_MspInit(void)
{
	//Bu fonksiyon iÃ§erisinde islemciye ozgu dÃ¼ÅŸÃ¼k seviyeli ilklendirmeler
	//yapÄ±lÄ±r.
		//1. Arm cortex Mx iÅŸlemci priority grouping iÅŸlemleri yapÄ±lÄ±r.
		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

		//2. Arm cortex Mx iÅŸlemcinin gerekli sistem exceptionslarÄ±
		//etkinliÅŸtirilir.
		SCB->SHCSR |= 0x7 << 16; //Usage fault, memory fault ve bus fault exception'larÄ± etkinleÅŸtirilir.

		//3. System exceptionslarÄ± iÃ§in priority konfigÃ¼re edilir.
		HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
		HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
		HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {

	GPIO_InitTypeDef timer2_Channel1_GPIO;

	//Enable peripheral clock of timer2
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//Configure GPIO to work as timer2 channel 1
	timer2_Channel1_GPIO.Pin = GPIO_PIN_0;
	timer2_Channel1_GPIO.Mode = GPIO_MODE_AF_PP;
	timer2_Channel1_GPIO.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &timer2_Channel1_GPIO);

	//Set and activate NVIC
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}
