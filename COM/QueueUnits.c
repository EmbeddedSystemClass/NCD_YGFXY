/***************************************************************************************************
*FileName:QueueUnits
*Description:队列操作工具类
*Author:xsx
*Data:2016年4月22日14:28:03
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"QueueUnits.h"


/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static unsigned char WaittingForMutex(xSemaphoreHandle mutex, portTickType xBlockTime);
static void GivexMutex(xSemaphoreHandle mutex);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
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
*FunctionName：ReceiveDataFromQueue
*Description：从队列中读取有限长度的数据
*Input：queue -- 目标队列
*		mutex -- 此队列的互斥量，可为null
*		receivedstr -- 存放接收数据的地址
*		len -- 接收的数据长度(长度为此队列单元数据大小的个数，与数据接收数据的字节长度无关)
*		itemsize -- 队列单元数据的大小
*		xBlockTime -- 阻塞时间
*Output：None
*Author：xsx
*Data：2016年4月22日15:35:40
***************************************************************************************************/
unsigned char ReceiveDataFromQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *receivedstr , unsigned short len ,
	unsigned short itemsize, portTickType xBlockTime)
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
			pdata += itemsize;
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
*FunctionName：SendDataToQueue
*Description：发送数据到队列
*Input：queue -- 目标队列
*		mutex -- 此队列的互斥量，可为null
*		sendstr -- 发送的数据地址
*		len -- 接收的数据长度(长度为此队列单元数据大小的个数，与数据接收数据的字节长度
*		itemsize -- 队列单元数据的大小
*		xBlockTime -- 阻塞时间
*Output：None
*Author：xsx
*Data：2016年4月22日15:33:38
***************************************************************************************************/
unsigned char SendDataToQueue(xQueueHandle queue, xSemaphoreHandle mutex, void *sendstr , unsigned short len ,  
	unsigned short itemsize, portTickType xBlockTime, void (*fun)(void))
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
			pdata += itemsize;
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

