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
#include	"StringDefine.h"

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
		//首先复制数据到自己的缓冲器，以防多任务下，其他任务释放原数据区
		memcpy(tempTestData, testd2, sizeof(TestData));
		
		sprintf(printfbuf, "%s\n\0", CompanyNameStr);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n\0", TesterNameStr, tempTestData->user.user_name);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n\0", SampleIdStr, tempTestData->sampleid);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n\0", ItemNameStr, tempTestData->temperweima.ItemName);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		tempvalue = tempTestData->testline.BasicResult;
		if(tempTestData->testResultDesc != ResultIsOK)
			sprintf(printfbuf, "%s: ERROR\n\0", ResultStr);
		else if(IsShowRealValue() == true)
			sprintf(printfbuf, "%s: %.*f %-8.8s\n\0", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.BasicResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else if(tempvalue <= tempTestData->temperweima.itemConstData.lowstResult)
			sprintf(printfbuf, "%s: <%.*f %-8.8s\n\0", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->temperweima.itemConstData.lowstResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else
			sprintf(printfbuf, "%s: %.*f %-8.8s\n\0", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.BasicResult, tempTestData->temperweima.itemConstData.itemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n\0", ReferenceValueStr, tempTestData->temperweima.itemConstData.normalResult);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n\0", TestTimeStr, tempTestData->TestTime.year, tempTestData->TestTime.month, tempTestData->TestTime.day
			, tempTestData->TestTime.hour, tempTestData->TestTime.min, tempTestData->TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		GetGB_Time(&mytime);
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n\0", PrintTimeStr, mytime.year, mytime.month, mytime.day
			, mytime.hour, mytime.min, mytime.sec);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s\n\n\n\n\n\0", StatementStr);
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
	}
	
	MyFree(tempTestData);
	MyFree(printfbuf);
}

/****************************************end of file************************************************/
