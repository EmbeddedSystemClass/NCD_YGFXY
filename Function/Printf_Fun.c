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
	
	printfbuf = MyMalloc(100);
	tempTestData = MyMalloc(sizeof(TestData));

	if(printfbuf && testd2 && tempTestData)
	{
		//首先复制数据到自己的缓冲器，以防多任务下，其他任务释放原数据区
		memcpy(tempTestData, testd2, sizeof(TestData));
		
		sprintf(printfbuf, "%s\n", PrintCompanyNameStr);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", TesterNameStr, tempTestData->user.user_name);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", SampleIdStr, tempTestData->sampleid);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n", ItemNameStr, tempTestData->temperweima.itemConstData.itemName);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		if(tempTestData->testResultDesc != ResultIsOK)
			sprintf(printfbuf, "%s: ERROR\n", ResultStr);
		else if(IsShowRealValue())
			sprintf(printfbuf, "%s: %.*f %-8.8s\n", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.BasicResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else if(tempTestData->testline.BasicResult <= tempTestData->temperweima.itemConstData.lowstResult)
			sprintf(printfbuf, "%s: <%.*f %-8.8s\n", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->temperweima.itemConstData.lowstResult, tempTestData->temperweima.itemConstData.itemMeasure);
		else
			sprintf(printfbuf, "%s: %.*f %-8.8s\n", ResultStr, tempTestData->temperweima.itemConstData.pointNum, tempTestData->testline.BasicResult, tempTestData->temperweima.itemConstData.itemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: %s\n\0", ReferenceValueStr, tempTestData->temperweima.itemConstData.normalResult);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", TestTimeStr, tempTestData->TestTime.year, tempTestData->TestTime.month, tempTestData->TestTime.day
			, tempTestData->TestTime.hour, tempTestData->TestTime.min, tempTestData->TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		GetGB_Time(&tempTestData->TestTime);
		sprintf(printfbuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", PrintTimeStr, tempTestData->TestTime.year, tempTestData->TestTime.month, tempTestData->TestTime.day
			, tempTestData->TestTime.hour, tempTestData->TestTime.min, tempTestData->TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
        
		sprintf(printfbuf, "%s\n \n \n \n", StatementStr);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
	}
	
	MyFree(tempTestData);
	MyFree(printfbuf);
}

/****************************************end of file************************************************/
