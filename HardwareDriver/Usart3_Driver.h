#ifndef __USART3_H__
#define __USART3_H__

#include	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#define xTxQueue3_Len				100

void Usart3_Init(void);
void EnableUsart3TXInterrupt(void);
xQueueHandle GetUsart3TXQueue(void);
	
#endif

