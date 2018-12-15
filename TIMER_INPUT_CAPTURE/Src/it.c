	/*
 * it.c
 *
 *  Created on: Dec 13, 2018
 *      Author: yonur
 */

#include "main.h"

void SysTick_Handler(void) {

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
