/*
 * main.h
 *
 *  Created on: Dec 24, 2018
 *      Author: yonur
 */

#ifndef MAIN_H_
#define MAIN_H_

//C libraries
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//Local libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

//Logical defines
#define TRUE  1
#define FALSE 0

#define AVAILABLE 	  TRUE
#define NON_AVAILABLE FALSE

//Flag and buffer variables
uint8_t button_status_flag = FALSE;
char usr_msg[250]={0};

//Structure types
typedef struct appCmdType {
	uint8_t appComandNumber;
	uint8_t appCommandArguments[10];
}appCmd_t;

typedef struct uartWriteType {
	uint8_t uartBuffer[150];
}uartWrite_t;

//Handlers defines
TaskHandle_t xTaskHandle1 = NULL;
TaskHandle_t xTaskHandle2 = NULL;
TaskHandle_t xTaskHandle3 = NULL;
TaskHandle_t xTaskHandle4 = NULL;

//Handler functions prototypes
void vTaskMenuDisplay(void);
void vTaskCmdHandling(void);
void vTaskCmdProcessing(void);
void vTaskUartWrite(void);

//void led_handle_task_1(void *parameter);
//void led_handle_task_2(void *parameter);

//Local function prototypes
void printmsg(char *msg);
static void prvSetupUsart(void);
static void prvSetupGpio(void);
static void prvSetupHardware(void);
void rtos_delay(uint32_t delay_in_ms);

#endif /* MAIN_H_ */
