/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "main.h"

int main(void)
{

	DWT->CTRL |= (1 << 0);//Enable CYCCNT in DWT_CTRL.

	//Reset the RCC clock to go to default values
	//RCC clock configuration in SetSysClock method in the System Init method,
	//default configuration was for 180MHz but we don't need for this basic example
	//Default RCC clock settings: HSI ON, PLL OFF, HSE OFF, system clock = 16MHz, cpu_clock = 16MHz
	RCC_DeInit();

	//Update the system clock because we set HSI 16MHz as RCC clock source
	SystemCoreClockUpdate();

	//Set the uart2 peripheral
	prvSetupHardware();

	//Start Recording
	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	//Create task 1
	xTaskCreate(led_handle_task_1, "TASK_1", 500, NULL, 2, &xTaskHandle1);

	//Create task 2
	xTaskCreate(led_handle_task_2, "TASK_2", 500, NULL, 3, &xTaskHandle2);

	//Start the scheduler
	vTaskStartScheduler();

	for(;;);
}

void led_handle_task_1(void *parameter) {

	while(1) {

		if( button_status_flag == TRUE ) {

			button_status_flag = FALSE;
			sprintf(usr_msg, "Turn to task 2\r\n");
			printmsg(usr_msg);
			vTaskPrioritySet(xTaskHandle1, 2);
			vTaskPrioritySet(xTaskHandle2, 3);
		}else {

			rtos_delay(100);
			//vTaskDelay(100);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
			sprintf(usr_msg, "Task 1 is running\r\n");
			printmsg(usr_msg);

		}
	}
}

void led_handle_task_2(void *parameter) {

	while(1) {

		if( button_status_flag == TRUE ) {

			button_status_flag = FALSE;
			sprintf(usr_msg, "Turn to task 1\r\n");
			printmsg(usr_msg);
			vTaskPrioritySet(xTaskHandle2, 2);
			vTaskPrioritySet(xTaskHandle1, 3);
		} else {

			rtos_delay(1000);
			//vTaskDelay(1000);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
			sprintf(usr_msg, "Task 2 is running\r\n");
			printmsg(usr_msg);
		}
	}
}

static void prvSetupUsart(void) {

	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef usart_init_conf;

	//Enable the UART2 and GPIOA peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//PA2 is UART2_TX and PA3 is UART2_Rx
	//Alternate function configuration of MCU pins to behave as UART2_TX and UART2_RX
	memset(&gpio_uart_pins, 0, sizeof(gpio_uart_pins)); //To avoid from garbage values
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &gpio_uart_pins);

	//AF mode settings for the pins
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	//UART parameter initializations
	memset(&usart_init_conf, 0, sizeof(usart_init_conf)); //To avoid from garbage values
	usart_init_conf.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_conf.USART_BaudRate = 115200;
	usart_init_conf.USART_Parity = USART_Parity_No;
	usart_init_conf.USART_StopBits = USART_StopBits_1;
	usart_init_conf.USART_WordLength = USART_WordLength_8b;
	usart_init_conf.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &usart_init_conf);

	//Enable USART
	USART_Cmd(USART2, ENABLE);
}

static void prvSetupGpio(void) {

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	GPIO_InitTypeDef gpio_pin;

	memset(&gpio_pin, 0, sizeof(gpio_pin));
	gpio_pin.GPIO_Pin = GPIO_Pin_5;
	gpio_pin.GPIO_Mode = GPIO_Mode_OUT;
	gpio_pin.GPIO_OType = GPIO_OType_PP;
	gpio_pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_pin.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOA, &gpio_pin);

	gpio_pin.GPIO_Pin = GPIO_Pin_13;
	gpio_pin.GPIO_Mode = GPIO_Mode_IN;
	gpio_pin.GPIO_OType = GPIO_OType_PP;
	gpio_pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_pin.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOC, &gpio_pin);

	//Interrupt configuration for the button

	//1. System configuration for EXTI line
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	//2.EXTI line configuration for GPIOC Pin 13 falling edge interrupt mode
	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line13;
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&exti);

	//3. NVIC settings (IRQ settings for the selected EXTI line(13)
	NVIC_SetPriority(EXTI15_10_IRQn, 5);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

static void prvSetupHardware(void) {

	//Setup USART2
	prvSetupUsart();

	//Setup GPIOs
	prvSetupGpio();
}

void printmsg(char *msg) {

	for( uint32_t i = 0; i < strlen(msg); i++) {

		while( USART_GetFlagStatus( USART2, USART_FLAG_TXE) != SET );
		USART_SendData(USART2, msg[i]);
	}
}

void rtos_delay(uint32_t delay_in_ms) {

	uint32_t delay_in_ticks;
	uint32_t current_tick_count;

	delay_in_ticks = ( delay_in_ms * configTICK_RATE_HZ ) / 1000;

	current_tick_count = xTaskGetTickCount();

	while( xTaskGetTickCount() < (current_tick_count + delay_in_ticks));
}

//void button_handler(void *params)
//{
//
//	button_status_flag ^=1;
//
//}
//
void EXTI15_10_IRQHandler(void)
{
    traceISR_ENTER();
	//1. clear the interrupt pending bit of the EXTI line (13)
	EXTI_ClearITPendingBit(EXTI_Line13);
	//button_handler(NULL);
	button_status_flag = TRUE;
	traceISR_EXIT();

}
