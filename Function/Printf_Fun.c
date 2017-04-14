/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Printf_Fun.h"
#include 	"Usart3_Driver.h"
#include	"SystemSet_Data.h"
#include	"System_Data.h"
#include	"QueueUnits.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void PrintfData(TestData * testd2)
{
	char * printfbuf = NULL;
	TestData * tempTestData = NULL;
	MyTime_Def mytime;
	float tempvalue = 0.0;
	
	printfbuf = MyMalloc(100);
	tempTestData = MyMalloc(sizeof(TestData));

	if(printfbuf && testd2 && tempTestData)
	{
		//���ȸ������ݵ��Լ��Ļ��������Է��������£����������ͷ�ԭ������
		memcpy(tempTestData, testd2, sizeof(TestData));
		
		sprintf(printfbuf, "------------------------------\r\0");
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1000 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "�人Ŧ��������Ƽ��ɷ����޹�˾\r\r\0");
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "������: %s\r\0", tempTestData->user.user_name);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "��Ʒ���: %s\r\0", tempTestData->sampleid);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "������Ŀ: %s\r\0", tempTestData->temperweima.ItemName);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		tempvalue = tempTestData->testline.AdjustResult;
		if(tempTestData->testResultDesc != ResultIsOK)
			sprintf(printfbuf, "���Խ��: ERROR\r\0");
		else if(IsShowRealValue() == true)
			sprintf(printfbuf, "���Խ��: %.*f %-8.8s\r\0", tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.AdjustResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else if(tempvalue <= tempTestData->temperweima.itemConstData.lowstResult)
			sprintf(printfbuf, "���Խ��: <%.*f %-8.8s\r\0", tempTestData->temperweima.itemConstData.pointNum, tempTestData->temperweima.itemConstData.lowstResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else if(tempvalue >= tempTestData->temperweima.itemConstData.highestResult)
			sprintf(printfbuf, "���Խ��: >%.*f %-8.8s\r\0", tempTestData->temperweima.itemConstData.pointNum, tempTestData->temperweima.itemConstData.highestResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else
			sprintf(printfbuf, "���Խ��: %.*f %-8.8s\r\0", tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.AdjustResult, tempTestData->temperweima.itemConstData.itemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "����ʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\r\0", tempTestData->TestTime.year, tempTestData->TestTime.month, tempTestData->TestTime.day
			, tempTestData->TestTime.hour, tempTestData->TestTime.min, tempTestData->TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		GetGB_Time(&mytime);
		sprintf(printfbuf, "��ӡʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\r\0", mytime.year, mytime.month, mytime.day
			, mytime.hour, mytime.min, mytime.sec);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1200 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "��������������Ա��걾����\r\0");
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(1500 / portTICK_RATE_MS);
		
		sprintf(printfbuf, "\r------------------------------\r\r\r\r\r\n\n\r\n\0");
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay(2000 / portTICK_RATE_MS);
	}
	
	MyFree(tempTestData);
	MyFree(printfbuf);
}

/****************************************end of file************************************************/
