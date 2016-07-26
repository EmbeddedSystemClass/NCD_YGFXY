/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"CodeScanner_Driver.h"
#include	"CodeScanFunction.h"
#include	"Usart2_Driver.h"

#include	"MyEncryptTool.h"
#include	"QueueUnits.h"
#include	"Motor_Fun.h"
#include	"Motor_Data.h"
#include	"CardStatues_Data.h"
#include	"Time_Data.h"

#include	"Define.h"
#include	"CRC16.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static CardCodeInfo * S_CardQRCodeInfo = NULL;
static ScanCodeResult S_ScanCodeResult = CardCodeScanning;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void ReadBasicCodeData(void);
static void AnalysisCode(void *pbuf , unsigned short len);
static MyState_TypeDef CheckCardIsTimeOut(CardCodeInfo * s_CardCodeInfo);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

ScanCodeResult ScanCodeFun(void * parm)
{
	unsigned short count = 0;
	unsigned char dir = 0;							//ɨ�����
	
	if(parm == NULL)
		return CardCodeScanFail;
	
	S_CardQRCodeInfo = parm;
	
	memset(S_CardQRCodeInfo, 0, sizeof(CardCodeInfo));
		
	MotorMoveTo(0, 0);
	MotorMoveTo(500, 0);
		
	OpenCodeScanner();
		
	while(pdPASS == ReceiveCharFromQueue(GetUsart2RXQueue(), GetUsart2RXMutex(), &dir , 50 * portTICK_RATE_MS))
		;
		
	dir = 0;
	S_ScanCodeResult = CardCodeScanning;
	
	while(S_ScanCodeResult == CardCodeScanning)
	{
		if(GetCardState() == NoCard)				//�����γ�
		{
			S_ScanCodeResult = CardCodeScanFail;
			break;
		}
		else
		{
			if(dir%2 == 0)
				MotorMoveTo(GetGB_MotorLocation()+2, 0);
			else
				MotorMoveTo(GetGB_MotorLocation()-2, 0);

			count++;
					
			if((GetGB_MotorLocation() >= 1500)||(GetGB_MotorLocation() <= 500))
			{
				dir++;
				ReadBasicCodeData();
			}
			
			vTaskDelay(2 / portTICK_RATE_MS);
			if(count >= (ScanOutTime*1000/2))
				S_ScanCodeResult = CardCodeScanFail;				
			
		}
	}

	CloseCodeScanner();
	
	return S_ScanCodeResult;
}

/***************************************************************************************************
*FunctionName��ReadBasicCodeData
*Description����ȡԭʼ��ά��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��25��14:07:38
***************************************************************************************************/
static void ReadBasicCodeData(void)
{
	char *buf = NULL;
	unsigned short RXCount = 0;
	
	buf = MyMalloc(CodeMaxLen);
	if(buf == NULL)
		return;
	
	memset(buf, 0, CodeMaxLen);
	
	while(pdPASS == ReceiveCharFromQueue(GetUsart2RXQueue(), GetUsart2RXMutex(), (buf+RXCount) , 10 / portTICK_RATE_MS))
		
		RXCount++;

	
	if(RXCount > 0)
		AnalysisCode(buf, RXCount);
	
	MyFree(buf);
}

/***************************************************************************************************
*FunctionName��DecryptCode
*Description�����벢������ά������
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��25��14:08:25
***************************************************************************************************/
static void AnalysisCode(void *pbuf , unsigned short len)
{
	char *basicrealbuf = NULL;									//���ܺ��ԭʼ����
	char *p = NULL;												//pָ��q������������ڴ��ַ������ͷ�
	char *q = NULL;												//q��Ϊstrtok������
	char *tempstr = NULL;
	unsigned short datalen = 0;
	unsigned char j=0;
	
	basicrealbuf = MyMalloc(len);
	q = MyMalloc(len+20);
	if((basicrealbuf == NULL)||(q == NULL))
		goto END;
	
	/*��ն�ά��ռ�*/
	memset(basicrealbuf, 0, len);
	memset(q, 0, len+20);

	/*���ݽ���ʧ��*/
	if(pdFAIL == MyDencrypt(pbuf, basicrealbuf, len))
		goto END;

	/*���ƽ��ܺ�Ķ�ά������*/
	memcpy(q, basicrealbuf, len);
	p = q;
	
	/*��ȡ��⿨��ά����Ϣ��ŵ�ַ*/
	if(NULL == S_CardQRCodeInfo)
		goto END;
	
	/*��ȡ����ͷ*/
	tempstr = strtok(q, "#");
	if(tempstr)
	{
		if(0 != strcmp(tempstr, "AB"))
			goto END;
		else
			basicrealbuf += 3;
	}
	else
		goto END;
	
	/*��ȡ���ݳ���*/
	tempstr = strtok(NULL, "#");
	if(tempstr)
	{
		datalen = strtol(tempstr , NULL , 10);
		basicrealbuf += (strlen(tempstr)+1);
	}
	else
		goto END;
	
	/*��ȡ������Ŀ����*/
	tempstr = strtok(NULL, "#");
	if(tempstr)
		memcpy(S_CardQRCodeInfo->ItemName, tempstr ,strlen(tempstr));
	else
		goto END;
		
	/*��ȡ��⿨�ϵļ��ָ����㷽ʽ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->TestType = strtol(tempstr , NULL , 10);
	else
		goto END;
		
	/*��ȡ��⿨�ϵļ��ָ��������Χ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->NormalResult = strtod(tempstr , NULL );
	else
		goto END;
		
	/*��ȡ��⿨�ϵ���ͼ��ֵ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->LowstResult = strtod(tempstr , NULL );
	else
		goto END;
		
	/*��ȡ��⿨�ϵ���߼��ֵ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->HighestResult = strtod(tempstr , NULL );
	else
		goto END;	
		
	/*��ȡ������Ŀ�ĵ�λ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		memcpy(S_CardQRCodeInfo->ItemMeasure, tempstr ,strlen(tempstr));
	else
		goto END;
		
	/*��ȡ��⿨T��λ��*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		//S_CardQRCodeInfo->ItemLocation = strtol(tempstr , NULL, 10);
	S_CardQRCodeInfo->ItemLocation = 236;
	else
		goto END;
		
	/*��ȡ��⿨��׼������Ŀ*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->ItemBiaoQuNum = strtol(tempstr , NULL, 10);
	else
		goto END;
	
	if(S_CardQRCodeInfo->ItemBiaoQuNum > 1)
	{
		/*��ȡ��⿨��׼�����ٽ�Ũ��*/
		tempstr = strtok(NULL , "#");
		if(tempstr)
			S_CardQRCodeInfo->ItemFenDuan = strtod(tempstr , NULL);
		else
			goto END;
	}
	
		
	/*��׼����*/
	for(j=0; j<S_CardQRCodeInfo->ItemBiaoQuNum; j++)
	{
		char *index = MyMalloc(50);
		
		tempstr = strtok(NULL , "#");
		
		if(tempstr && index)
		{
			memset(index, 0, 50);
			S_CardQRCodeInfo->ItemBiaoQu[j][0] = strtod(tempstr , &index );
				
			S_CardQRCodeInfo->ItemBiaoQu[j][1] = strtod(index , &index );
				
			S_CardQRCodeInfo->ItemBiaoQu[j][2] = strtod(index , &index );

			MyFree(index);
		}
		else
		{
			MyFree(index);
				
			goto END;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->CardWaitTime = strtol(tempstr , NULL , 10);
	else
		goto END;
		
	/*��ȡ��⿨C��λ��*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		S_CardQRCodeInfo->CLineLocation = 326;//strtol(tempstr , NULL , 10);
	else
		goto END;


	/*��ȡ��⿨����*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
		memcpy(S_CardQRCodeInfo->CardPiCi, tempstr, strlen(tempstr));
	else
		goto END;

	/*��ȡ��⿨������*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, tempstr, 4);
		S_CardQRCodeInfo->CardBaoZhiQi.RTC_Year = strtol(year , NULL , 10) - 2000;

		memcpy(month, &tempstr[4], 2);
		S_CardQRCodeInfo->CardBaoZhiQi.RTC_Month = (unsigned char)strtod(month , NULL );

		memcpy(day, &tempstr[6], 2);
		S_CardQRCodeInfo->CardBaoZhiQi.RTC_Date = (unsigned char)strtod(day , NULL );
	}
	else
		goto END;

	/*��ȡ��ά��CRC*/
	tempstr = strtok(NULL , "#");
	if(tempstr)
	{
		S_CardQRCodeInfo->CRC16 = strtol(tempstr , NULL , 10);
			
		datalen -= strlen(tempstr);
		goto END;
	}
	else
		goto END;
	
	END:
		if(S_CardQRCodeInfo->CRC16 != CalModbusCRC16Fun1(basicrealbuf , datalen))
			S_ScanCodeResult = CardCodeScanFail;		
		else if(My_Fail == CheckCardIsTimeOut(S_CardQRCodeInfo))
			S_ScanCodeResult = CardCodeTimeOut;
		else
			S_ScanCodeResult = CardCodeScanOK;
		
		MyFree(basicrealbuf);
		basicrealbuf = NULL;
		
		MyFree(p);
		p = NULL;
}


static MyState_TypeDef CheckCardIsTimeOut(CardCodeInfo * s_CardCodeInfo)
{
	MyTime_Def *temp = NULL;
	temp = MyMalloc(sizeof(MyTime_Def));
	
	if(temp)
	{
		GetGBTimeData(temp);
		
		if(s_CardCodeInfo->CardBaoZhiQi.RTC_Year == temp->year)
		{
			if(s_CardCodeInfo->CardBaoZhiQi.RTC_Month == temp->month)
			{
				if(s_CardCodeInfo->CardBaoZhiQi.RTC_Date >= temp->day)
					return My_Pass;
				else
					return My_Fail;
			}
			else if(s_CardCodeInfo->CardBaoZhiQi.RTC_Month > temp->month)
				return My_Pass;
			else
				return My_Fail;
		}
		else if(s_CardCodeInfo->CardBaoZhiQi.RTC_Year > temp->year)
			return My_Pass;
		else
			return My_Fail;
	}
	else
		return My_Fail;
	
	
}

