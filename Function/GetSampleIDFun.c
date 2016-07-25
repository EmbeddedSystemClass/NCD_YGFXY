/***************************************************************************************************
*FileName:GetSampleIDFun
*Description:�ɼ���ƷID
*Author:xsx
*Data:2016��4��26��21:24:20
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"GetSampleIDFun.h"
#include	"Usart1_Driver.h"
#include	"MyMem.h"
#include	"QueueUnits.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static xQueueHandle xSampleIDQueue = NULL;						//����ȡ�������ݵĵ�ַ���ⷢ��
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static void AnalysisData(void *pbuf , unsigned short len);
static MyState_TypeDef SendSampleIDData(void * data);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void ScannerInit(void)
{
	if(xSampleIDQueue == NULL)
		xSampleIDQueue = xQueueCreate( 10, 4);								//ָ��Ĵ�СΪ4���ֽڣ���ౣ��10��ָ��
}

/***************************************************************************************************
*FunctionName��GetOutScanerData
*Description����ȡ��ƷID����
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��26��21:24:53
***************************************************************************************************/
void GetOutScanerData(void)
{
	char *buf = NULL;
	unsigned short RXCount = 0;
	
	buf = MyMalloc(100);
	if(buf == NULL)
		return;
	
	memset(buf, 0, 100);

	while(pdPASS == ReceiveCharFromQueue(GetUsart1RXQueue(), GetUsart1RXMutex(), (buf+RXCount) , 100 * portTICK_RATE_MS))	
		RXCount++;

	
	if(RXCount > 0)
		AnalysisData(buf, RXCount);
	
	MyFree(buf);
}

/***************************************************************************************************
*FunctionName��AnalysisData
*Description��������յ�����
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��26��21:25:21
***************************************************************************************************/
static void AnalysisData(void *pbuf , unsigned short len)
{
	unsigned char *tempbuf = NULL;
	
	tempbuf = MyMalloc(len+10);
	if(tempbuf)
	{
		memset(tempbuf, 0, len+10);
		memcpy(tempbuf, pbuf, len);
		
		if(My_Fail == SendSampleIDData(&tempbuf))
			MyFree(tempbuf);
	}
}

/***************************************************************************************************
*FunctionName��GiveSampleIDStateMutex
*Description������֪ͨ
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��19:24:37
***************************************************************************************************/
static MyState_TypeDef SendSampleIDData(void * data)
{
	if(xSampleIDQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueSend( xSampleIDQueue, data, 10*portTICK_RATE_MS ))
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��TakeSampleIDStateMutex
*Description����ȡ��Ʒ��Ÿı�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��19:24:37
***************************************************************************************************/
MyState_TypeDef TakeSampleIDData(void * data)
{
	if(xSampleIDQueue == NULL)
		return My_Fail;
	
	if(pdPASS == xQueueReceive( xSampleIDQueue, data, 10*portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;	
}
