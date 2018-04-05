#ifndef __USART1_H__
#define __USART1_H__

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stm32f4xx_gpio.h"
#include	"stm32f4xx_rcc.h"

#define xRxQueue1_Len				50

void Usart1_Init(void);
xQueueHandle GetUsart1RXQueue(void);

#endif

