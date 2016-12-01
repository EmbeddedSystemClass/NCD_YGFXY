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

MyState_TypeDef ConnectPrintter(void)
{
/*	unsigned char txbuf[4] = {0x1b, 0x20, 0x02, 0x0d};
	
	if(pdPASS == SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), txbuf, 4, sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt))
	{
		if(pdPASS == ReceiveDataFromQueue(GetUsart3RXQueue(), GetUsart3Mutex(), txbuf, 2, sizeof(unsigned char), 2000 / portTICK_RATE_MS))
		{
			if(txbuf[0] == 0x11)
				return My_Pass;
		}
	}*/
	
	return My_Pass;
}

static void EnterUnNormalChar(void)
{

}

static void ExitUnNormalChar(void)
{

}

void PrintfData(void *data)
{
	char * printfbuf = NULL;
	TestData * testd = data;
	MyTime_Def mytime;
	float tempvalue = 0.0;
	char * tempbuf = NULL;
	char tempbuf2[20];
	
	printfbuf = MyMalloc(100);

	if(printfbuf)
	{
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "------------------------\r");
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "�人Ŧ��������Ƽ��ɷ����޹�˾\r\r");
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "������: %s\r", testd->user.user_name);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "��Ʒ���: %s\r", testd->sampleid);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		
/*		tempbuf = strstr(testd->temperweima.ItemName, "��");
		if(tempbuf)
		{	
			memset(tempbuf2, 0, 20);
			memcpy(tempbuf2, testd->temperweima.ItemName, tempbuf - testd->temperweima.ItemName);
			sprintf(printfbuf, "������Ŀ: %s", tempbuf2);
			SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
			vTaskDelay( 500 / portTICK_RATE_MS);
			
			EnterUnNormalChar();		
			sprintf(printfbuf, "��");
			SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
			vTaskDelay( 100 / portTICK_RATE_MS);	
			ExitUnNormalChar();
			
			memset(tempbuf2, 0, 20);
			memcpy(tempbuf2, tempbuf + 2, strlen(tempbuf + 2));
			sprintf(printfbuf, "%s\r", tempbuf2);
			SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
			vTaskDelay( 100 / portTICK_RATE_MS);
		}
		else
		{*/
			sprintf(printfbuf, "������Ŀ: %s\r", testd->temperweima.ItemName);
			SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
			vTaskDelay( 500 / portTICK_RATE_MS);
		//}
		
		
		memset(printfbuf, 0, 100);
		
		tempvalue = testd->testline.AdjustResult;
/*		if(tempvalue <= testd->temperweima.LowstResult)
			sprintf(printfbuf, "���Խ��: <%.2f %-8.8s\r", testd->temperweima.LowstResult, testd->temperweima.ItemMeasure);
		else if(tempvalue >= testd->temperweima.HighestResult)
			sprintf(printfbuf, "���Խ��: >%.2f %-8.8s\r", testd->temperweima.HighestResult, testd->temperweima.ItemMeasure);
		else*/
			sprintf(printfbuf, "���Խ��: %.2f %-8.8s\r", testd->testline.AdjustResult, testd->temperweima.ItemMeasure);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "����ʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\r", testd->TestTime.year, testd->TestTime.month, testd->TestTime.day
			, testd->TestTime.hour, testd->TestTime.min, testd->TestTime.sec);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		GetGB_Time(&mytime);
		sprintf(printfbuf, "��ӡʱ��: 20%02d-%02d-%02d %02d:%02d:%02d\r", mytime.year, mytime.month, mytime.day
			, mytime.hour, mytime.min, mytime.sec);
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
		
		memset(printfbuf, 0, 100);
		sprintf(printfbuf, "\r------------------------\r\r\r\r\r\r");
		
		SendDataToQueue(GetUsart3TXQueue(), GetUsart3Mutex(), printfbuf, strlen(printfbuf), sizeof(unsigned char), 50 / portTICK_RATE_MS, EnableUsart3TXInterrupt);
		vTaskDelay( 500 / portTICK_RATE_MS);
	}
	
	MyFree(printfbuf);

}

/****************************************end of file************************************************/
