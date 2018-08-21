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
    PrintBuffer * printBuffer = NULL;
    
    printBuffer = MyMalloc(sizeof(PrintBuffer));
    if(printBuffer && testd2)
    {
        //首先复制数据到自己的缓冲器，以防多任务下，其他任务释放原数据区
		memcpy(&printBuffer->tempTestData, testd2, sizeof(TestData));
        
        readPrintInfo(&printBuffer->printInfo);

        sprintf(printBuffer->tempBuf, "%.30s\n", printBuffer->printInfo.header);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), 1, 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printBuffer->tempBuf, "%s: %s\n", TesterNameStr, printBuffer->tempTestData.user.user_name);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printBuffer->tempBuf, "%s: %s\n", SampleIdStr, printBuffer->tempTestData.sampleid);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printBuffer->tempBuf, "%s: %s\n", ItemNameStr, printBuffer->tempTestData.temperweima.itemConstData.itemName);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		if(printBuffer->tempTestData.testResultDesc != ResultIsOK)
			sprintf(printBuffer->tempBuf, "%s: ERROR\n", ResultStr);
		else if(IsShowRealValue())
			sprintf(printBuffer->tempBuf, "%s: %.*f %-8.8s\n", ResultStr, printBuffer->tempTestData.temperweima.itemConstData.pointNum, printBuffer->tempTestData.testline.BasicResult, printBuffer->tempTestData.temperweima.itemConstData.itemMeasure);
		else if(printBuffer->tempTestData.testline.BasicResult <= printBuffer->tempTestData.temperweima.itemConstData.lowstResult)
			sprintf(printBuffer->tempBuf, "%s: <%.*f %-8.8s\n", ResultStr, printBuffer->tempTestData.temperweima.itemConstData.pointNum, printBuffer->tempTestData.temperweima.itemConstData.lowstResult, printBuffer->tempTestData.temperweima.itemConstData.itemMeasure);
		else
			sprintf(printBuffer->tempBuf, "%s: %.*f %-8.8s\n", ResultStr, printBuffer->tempTestData.temperweima.itemConstData.pointNum, printBuffer->tempTestData.testline.BasicResult, printBuffer->tempTestData.temperweima.itemConstData.itemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printBuffer->tempBuf, "%s: %s\n", ReferenceValueStr, printBuffer->tempTestData.temperweima.itemConstData.normalResult);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		sprintf(printBuffer->tempBuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", TestTimeStr, printBuffer->tempTestData.TestTime.year, printBuffer->tempTestData.TestTime.month, printBuffer->tempTestData.TestTime.day
			, printBuffer->tempTestData.TestTime.hour, printBuffer->tempTestData.TestTime.min, printBuffer->tempTestData.TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		
		GetGB_Time(&printBuffer->tempTestData.TestTime);
		sprintf(printBuffer->tempBuf, "%s: 20%02d-%02d-%02d %02d:%02d:%02d\n", PrintTimeStr, printBuffer->tempTestData.TestTime.year, printBuffer->tempTestData.TestTime.month, printBuffer->tempTestData.TestTime.day
			, printBuffer->tempTestData.TestTime.hour, printBuffer->tempTestData.TestTime.min, printBuffer->tempTestData.TestTime.sec);
		SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		 
		sprintf(printBuffer->tempBuf, "%s\n \n \n \n", StatementStr);
        SendDataToQueue(GetUsart3TXQueue(), NULL, printBuffer->tempBuf, strlen(printBuffer->tempBuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, 100 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
    }
    
    MyFree(printBuffer);
 
}

/****************************************end of file************************************************/
