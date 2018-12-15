/*
 * main.c
 *
 *  Created on: Dec 13, 2018
 *      Author: yonur
 */

#include "main.h"
#include <string.h>

void Error_handler(void);
void GPIO_Init(void);
void SystemClock_Config_HSE(uint8_t clock_freq );
void LSE_Configuration(void);
void TIMER2_Init(void);

TIM_HandleTypeDef htimer2;

int main(void) {

	HAL_Init();
	SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ);
	GPIO_Init();
	TIMER2_Init();
	LSE_Configuration();

	while(1);

	return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq )
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	uint32_t FLatency =0;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.LSEState = RCC_LSE_ON;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(clock_freq)
	{
		case SYS_CLOCK_FREQ_50_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

		    clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
		    					RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
		    clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		    clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		    clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		    clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

		    FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYS_CLOCK_FREQ_84_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			 FLatency = FLASH_ACR_LATENCY_2WS;

			break;
		}
		case SYS_CLOCK_FREQ_120_MHZ:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 240;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_3WS;
			break;

		}


		case SYS_CLOCK_FREQ_180_MHZ:
		{
			//Enable the clock for the power controller
			__HAL_RCC_PWR_CLK_ENABLE();

			//set regulator voltage scale as 1
			__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

			//turn on the over drive mode of the voltage regulator
			__HAL_PWR_OVERDRIVE_ENABLE();

			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 360;
			osc_init.PLL.PLLP = 2;
			osc_init.PLL.PLLQ  = 2;
			osc_init.PLL.PLLR=2;

			clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
			clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

			FLatency = FLASH_ACR_LATENCY_5WS;
			break;

		}

		default:
			return;

	}

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}


	if(HAL_RCC_ClockConfig(&clk_init,FLatency) != HAL_OK)
	{
		Error_handler();
	}


	//Systick configuration

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void TIMER2_Init(void) {

	TIM_IC_InitTypeDef timer2_InputCaptureConfig;

	htimer2.Instance = TIM2;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.Prescaler = 1;
	htimer2.Init.Period = 0xFFFFFFFF;
	if( HAL_TIM_IC_Init(&htimer2) != HAL_OK ) {
		Error_handler();
	}

	timer2_InputCaptureConfig.ICFilter = 0;
	timer2_InputCaptureConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2_InputCaptureConfig.ICPrescaler = TIM_ICPSC_DIV1;
	timer2_InputCaptureConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if( HAL_TIM_IC_ConfigChannel(&htimer2, &timer2_InputCaptureConfig, TIM_CHANNEL_1) != HAL_OK ) {
		Error_handler();
	}
}

void GPIO_Init(void) {

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);
}

void LSE_Configuration(void) {

#if 0
	RCC_OscInitTypeDef osc_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	osc_init.LSEState = RCC_LSE_ON;

	if( HAL_RCC_OscConfig(&osc_init) != HAL_OK ) {
		Error_handler();
	}
#endif

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

void Error_handler(void) {
	while(1);
}
