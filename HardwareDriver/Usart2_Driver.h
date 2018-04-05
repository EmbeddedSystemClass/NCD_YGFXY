#ifndef __USART2_H__
#define __USART2_H__

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xRxQueue2_Len				300

void Usart2_Init(void);
xQueueHandle GetUsart2RXQueue(void);

#endif

