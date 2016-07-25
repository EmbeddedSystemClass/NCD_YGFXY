/***************************************************************************************************
*FileName:QueueUnits
*Description:���в���������
*Author:xsx
*Data:2016��4��22��14:28:03
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"QueueUnits.h"


/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static unsigned char WaittingForMutex(xSemaphoreHandle mutex, portTickType xBlockTime);
static void GivexMutex(xSemaphoreHandle mutex);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static unsigned char WaittingForMutex(xSemaphoreHandle mutex, portTickType xBlockTime)
{
	return xSemaphoreTake(mutex, xBlockTime);
}
static void GivexMutex(xSemaphoreHandle mutex)
{
	xSemaphoreGive(mutex);
}

/***************************************************************************************************
*FunctionName��ReceiveCharFromQueue
*Description������һ���ֽڵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- �������ݵĴ�ŵ�ַ
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:36:44
***************************************************************************************************/
unsigned char ReceiveCharFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void * receivedchar , portTickType xBlockTime)
{
	unsigned char * p = (unsigned char *)receivedchar;
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}

	statues = xQueueReceive( queue, p, xBlockTime );
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	return statues;
}

/***************************************************************************************************
*FunctionName��SendCharToQueue
*Description������һ���ֽڵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- ���͵�����
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:36:15
***************************************************************************************************/
unsigned char SendCharToQueue(xQueueHandle queue, xSemaphoreHandle mutex, unsigned char receivedchar , portTickType xBlockTime,
	void (*fun)(void))
{
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}

	statues = xQueueSend( queue, &receivedchar, xBlockTime );
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	if(fun != NULL)
		fun();
	
	return statues;
} 

/***************************************************************************************************
*FunctionName��ReceiveStrFromQueue
*Description���Ӷ��н�������
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- ��Ž������ݵĵ�ַ
*		len -- ���յ����ݳ��ȣ��ֽڳ���
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:35:40
***************************************************************************************************/
unsigned char ReceiveStrFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr , unsigned short len , 
	portTickType xBlockTime)
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)receivedstr;
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}
	
	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueReceive(queue, pdata , xBlockTime))
		{
			pdata++;
			statues = pdPASS;
		}
		else
		{
			statues = pdFAIL;
			break;
		}
	}
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	return statues;
}
/***************************************************************************************************
*FunctionName��SendStrToQueue
*Description���������ݵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- ���͵����ݵ�ַ
*		len -- ���͵����ݳ��ȣ��ֽڳ���
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:33:38
***************************************************************************************************/
unsigned char SendStrToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr , unsigned short len ,  
	portTickType xBlockTime, void (*fun)(void))
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)sendstr;
	unsigned char statues = pdFAIL;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}

	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueSend(queue, pdata , xBlockTime))
		{
			pdata++;
			statues = pdPASS;
		}
		else
		{
			statues = pdFAIL;
			break;
		}
	}
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	if(fun != NULL)
		fun();
	
	return statues;
}


/***************************************************************************************************
*FunctionName��ReceiveDataFromQueue
*Description���Ӷ����ж�ȡ���޳��ȵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		receivedstr -- ��Ž������ݵĵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ����޹�)
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:35:40
***************************************************************************************************/
unsigned char ReceiveDataFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr , unsigned short len , 
	portTickType xBlockTime)
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)receivedstr;
	unsigned char statues = pdFAIL;
	
	Queue_t * const pxQueue = ( Queue_t * ) queue;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}
	
	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueReceive(queue, pdata , xBlockTime))
		{
			pdata += pxQueue->uxItemSize;
			statues = pdPASS;
		}
		else
		{
			statues = pdFAIL;
			break;
		}
	}
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	return statues;
}
/***************************************************************************************************
*FunctionName��SendDataToQueue
*Description���������ݵ�����
*Input��queue -- Ŀ�����
*		mutex -- �˶��еĻ���������Ϊnull
*		sendstr -- ���͵����ݵ�ַ
*		len -- ���յ����ݳ���(����Ϊ�˶��е�Ԫ���ݴ�С�ĸ����������ݽ������ݵ��ֽڳ����
*		xBlockTime -- ����ʱ��
*Output��None
*Author��xsx
*Data��2016��4��22��15:33:38
***************************************************************************************************/
unsigned char SendDataToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr , unsigned short len ,  
	portTickType xBlockTime, void (*fun)(void))
{
	unsigned short i=0;
	unsigned char *pdata = (unsigned char *)sendstr;
	unsigned char statues = pdFAIL;
	Queue_t * const pxQueue = ( Queue_t * ) queue;
	
	if(queue == NULL)
		return pdFAIL;
	
	if(mutex != NULL)
	{
		if(pdFAIL == WaittingForMutex(mutex, xBlockTime))
			return pdFAIL;
	}

	for(i=0; i<len; i++)
	{
		if(pdPASS == xQueueSend(queue, pdata , xBlockTime))
		{
			pdata += pxQueue->uxItemSize;
			statues = pdPASS;
		}
		else
		{
			statues = pdFAIL;
			break;
		}
	}
	
	if(mutex != NULL)
		GivexMutex(mutex);
	
	if(fun != NULL)
		fun();
	
	return statues;
}

