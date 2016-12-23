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
#include	"System_Data.h"

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
static CardCodeInfo * cardQR;								//ɨ���ά�����ݿռ��ַ
static ReadCodeBuffer * S_ReadCodeBuffer;					//���ڴ����ά��Ļ���
static ScanCodeResult scanresult;							//ɨ���ά����
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
	
	S_ReadCodeBuffer = MyMalloc(sizeof(ReadCodeBuffer));
	if(NULL == S_ReadCodeBuffer)
		return CardCodeScanFail;
	
	memset(S_ReadCodeBuffer, 0, sizeof(ReadCodeBuffer));
	
	cardQR = parm;
	memset(cardQR, 0, sizeof(CardCodeInfo));
		
	MotorMoveTo(0, 0);
	MotorMoveTo(500, 0);
		
	OpenCodeScanner();
	
		
	while(pdPASS == ReceiveDataFromQueue(GetUsart2RXQueue(), GetUsart2RXMutex(), &dir , 1, 1, 50 * portTICK_RATE_MS))
		;
		
	dir = 0;
	scanresult = CardCodeScanning;
	
	while(scanresult == CardCodeScanning)
	{
		if(GetCardState() == NoCard)				//�����γ�
		{
			scanresult = CardCodeCardOut;
			break;
		}
		else
		{
			if(dir%2 == 0)
				MotorMoveTo(GetGB_MotorLocation()+2, 0);
			else
				MotorMoveTo(GetGB_MotorLocation()-2, 0);

			count++;
			
			ReadBasicCodeData();
			
			if((GetGB_MotorLocation() >= 1000)||(GetGB_MotorLocation() <= 200))
			{
				dir++;
				
			}
			
			vTaskDelay(2 / portTICK_RATE_MS);
			if(count >= (ScanOutTime*1000/3))
				scanresult = CardCodeScanTimeOut;				
			
		}
	}

	CloseCodeScanner();
	
	MyFree(S_ReadCodeBuffer);
	
	return scanresult;
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
	S_ReadCodeBuffer->rxcount = 0;
	
	while(pdPASS == ReceiveDataFromQueue(GetUsart2RXQueue(), GetUsart2RXMutex(), (S_ReadCodeBuffer->originalcode+S_ReadCodeBuffer->rxcount) , 1, 1, 10 / portTICK_RATE_MS))	
		S_ReadCodeBuffer->rxcount++;

	if(S_ReadCodeBuffer->rxcount > 0)
		AnalysisCode(S_ReadCodeBuffer->originalcode, S_ReadCodeBuffer->rxcount);
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
	unsigned short datalen = 0;
	unsigned char j=0, i=0;
	
	/*��ն�ά��ռ�*/
	memset(S_ReadCodeBuffer->decryptcode, 0, 320);

	/*���ݽ���ʧ��*/
	if(pdFAIL == MyDencrypt(pbuf, S_ReadCodeBuffer->decryptcode, len))
		goto END;
	
	memcpy(S_ReadCodeBuffer->originalcode, S_ReadCodeBuffer->decryptcode, len);
	S_ReadCodeBuffer->pbuf2 = S_ReadCodeBuffer->originalcode;

	/*��ȡ��⿨��ά����Ϣ��ŵ�ַ*/
	if(NULL == cardQR)
		goto END;
	
	/*��ȡ����ͷ*/
	S_ReadCodeBuffer->pbuf1 = strtok(S_ReadCodeBuffer->decryptcode, "#");
	if(S_ReadCodeBuffer->pbuf1)
	{
		if(0 != strcmp(S_ReadCodeBuffer->pbuf1, "AB"))
			goto END;
		else
			S_ReadCodeBuffer->pbuf2 += 3;
	}
	else
		goto END;
	
	/*��ȡ���ݳ���*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL, "#");
	if(S_ReadCodeBuffer->pbuf1)
	{
		datalen = strtol(S_ReadCodeBuffer->pbuf1 , NULL , 10);
		S_ReadCodeBuffer->pbuf2 += (strlen(S_ReadCodeBuffer->pbuf1)+1);
	}
	else
		goto END;
	
	/*��ȡ������Ŀ����*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL, "#");
	if(S_ReadCodeBuffer->pbuf1)
		memcpy(cardQR->ItemName, S_ReadCodeBuffer->pbuf1 ,strlen(S_ReadCodeBuffer->pbuf1));
	else
		goto END;
		
	/*��ȡ��⿨�ϵļ��ָ��������Χ*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		memcpy(cardQR->NormalResult, S_ReadCodeBuffer->pbuf1 ,strlen(S_ReadCodeBuffer->pbuf1));
	else
		goto END;
		
	/*��ȡ��⿨�ϵ���ͼ��ֵ*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->LowstResult = strtod(S_ReadCodeBuffer->pbuf1 , NULL );
	else
		goto END;
		
	/*��ȡ��⿨�ϵ���߼��ֵ*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->HighestResult = strtod(S_ReadCodeBuffer->pbuf1 , NULL );
	else
		goto END;	
		
	/*��ȡ������Ŀ�ĵ�λ*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		memcpy(cardQR->ItemMeasure, S_ReadCodeBuffer->pbuf1 ,strlen(S_ReadCodeBuffer->pbuf1));
	else
		goto END;
	
	//��ȡС�������
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->ItemPoint = strtol(S_ReadCodeBuffer->pbuf1 , NULL, 10);
	else
		goto END;
	
	/*��ȡ��⿨T��λ��*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->ItemLocation = strtol(S_ReadCodeBuffer->pbuf1 , NULL, 10);
	else
		goto END;
		
	/*��ȡ��⿨��׼�����ٽ�Ũ��*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->ItemFenDuan = strtod(S_ReadCodeBuffer->pbuf1 , NULL);
	else
		goto END;

	/*��׼����1*/
	for(i=0; i<4; i++)
	{
		S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(S_ReadCodeBuffer->pbuf1)
			cardQR->ItemBiaoQu[0][i] = strtod(S_ReadCodeBuffer->pbuf1 , NULL);
		else
			goto END;
	}
	
	/*��׼����1*/
	if(cardQR->ItemFenDuan > 0)
	{
		for(i=0; i<4; i++)
		{
			S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(S_ReadCodeBuffer->pbuf1)
				cardQR->ItemBiaoQu[1][i] = strtod(S_ReadCodeBuffer->pbuf1 , NULL);
			else
				goto END;
		}
	}
		
	/*��ȡ��⿨��Ӧʱ��*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->CardWaitTime = strtol(S_ReadCodeBuffer->pbuf1 , NULL , 10);
	else
		goto END;
		
	/*��ȡ��⿨C��λ��*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		cardQR->CLineLocation = strtol(S_ReadCodeBuffer->pbuf1 , NULL , 10);
	else
		goto END;


	/*��ȡ��⿨����*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
		memcpy(cardQR->CardPiCi, S_ReadCodeBuffer->pbuf1, strlen(S_ReadCodeBuffer->pbuf1));
	else
		goto END;

	/*��ȡ��⿨������*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, S_ReadCodeBuffer->pbuf1, 2);
		cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &S_ReadCodeBuffer->pbuf1[4], 2);
		cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &S_ReadCodeBuffer->pbuf1[6], 2);
		cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
	}
	else
		goto END;

	/*��ȡ��ά��CRC*/
	S_ReadCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(S_ReadCodeBuffer->pbuf1)
	{
		cardQR->CRC16 = strtol(S_ReadCodeBuffer->pbuf1 , NULL , 10);
			
		datalen -= strlen(S_ReadCodeBuffer->pbuf1);
		goto END;
	}
	else
		goto END;
	
	END:
		if(cardQR->CRC16 != CalModbusCRC16Fun1(S_ReadCodeBuffer->pbuf2 , datalen))
			scanresult = CardCodeCRCError;		
		else if(My_Fail == CheckCardIsTimeOut(cardQR))
			scanresult = CardCodeTimeOut;
		else
			scanresult = CardCodeScanOK;
		
}


static MyState_TypeDef CheckCardIsTimeOut(CardCodeInfo * s_CardCodeInfo)
{
	if(S_ReadCodeBuffer)
	{
		GetGB_Time(&(S_ReadCodeBuffer->temptime));
		
		if(s_CardCodeInfo->CardBaoZhiQi.year == S_ReadCodeBuffer->temptime.year)
		{
			if(s_CardCodeInfo->CardBaoZhiQi.month == S_ReadCodeBuffer->temptime.month)
			{
				if(s_CardCodeInfo->CardBaoZhiQi.day >= S_ReadCodeBuffer->temptime.day)
					return My_Pass;
			}
			else if(s_CardCodeInfo->CardBaoZhiQi.month > S_ReadCodeBuffer->temptime.month)
				return My_Pass;
		}
		else if(s_CardCodeInfo->CardBaoZhiQi.year > S_ReadCodeBuffer->temptime.year)
			return My_Pass;
	}
	
	return My_Fail;
}

