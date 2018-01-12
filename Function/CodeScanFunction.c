/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"CodeScanner_Driver.h"
#include	"CodeScanFunction.h"
#include	"Usart2_Driver.h"

#include	"MyEncryptTool.h"
#include	"QueueUnits.h"
#include	"Motor_Fun.h"
#include	"Motor_Data.h"
#include	"CardStatues_Data.h"
#include	"System_Data.h"
#include	"Timer_Data.h"
#include	"ItemConst_Data.h"

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
/*****************************************局部变量声明*************************************/

/******************************************************************************************/
/*****************************************局部函数声明*************************************/
static void ReadBasicCodeData(ReadQRCodeBuffer * readQRCodeBuffer);
static void AnalysisCode(ReadQRCodeBuffer * readQRCodeBuffer);
static MyState_TypeDef CheckCardIsTimeOut(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion0(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion2(ReadQRCodeBuffer * readQRCodeBuffer);
static ScanCodeResult AnalysisQRVersion3(ReadQRCodeBuffer * readQRCodeBuffer);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

ScanCodeResult ScanCodeFun(QRCode * parm)
{
	ReadQRCodeBuffer * readQRCodeBuffer;
	ScanCodeResult scanresult;							//扫码二维码结果
	
	if(parm == NULL)
		return CardCodeScanFail;
	
	readQRCodeBuffer = MyMalloc(sizeof(ReadQRCodeBuffer));
	if(NULL == readQRCodeBuffer)
		return CardCodeScanFail;
	
	memset(readQRCodeBuffer, 0, sizeof(ReadQRCodeBuffer));
	
	readQRCodeBuffer->cardQR = parm;
	memset(readQRCodeBuffer->cardQR, 0, sizeof(QRCode));
		
	MotorMoveTo(1, 2, 0, TRUE);
	MotorMoveTo(1, 2, 500, TRUE);
	readQRCodeBuffer->motorLocation = GetGB_MotorLocation();
		
	OpenCodeScanner();
	
	while(pdPASS == xQueueReceive(GetUsart2RXQueue(), readQRCodeBuffer->originalcode , 10/portTICK_RATE_MS))
		;

	readQRCodeBuffer->scanresult = CardCodeScanning;
	
	//设置定时器
	timer_set(&(readQRCodeBuffer->timer), MAX_SCAN_QR_TIME);
	
	while(readQRCodeBuffer->scanresult == CardCodeScanning)
	{
		if(GetCardState() == NoCard)				//卡被拔出
		{
			readQRCodeBuffer->scanresult = CardCodeCardOut;
			break;
		}
		else
		{
			//if((readQRCodeBuffer->motorLocation >= 1000)||(readQRCodeBuffer->motorLocation <= 200))
				readQRCodeBuffer->motorDir++;
			
			if(readQRCodeBuffer->motorDir%2 == 0)
				MotorMoveTo(10, 15, 1000, TRUE);
			else
				MotorMoveTo(10, 15, 200, TRUE);
			
			ReadBasicCodeData(readQRCodeBuffer);

			vTaskDelay(2 / portTICK_RATE_MS);
			
			if(TimeOut == timer_expired(&(readQRCodeBuffer->timer)))
				readQRCodeBuffer->scanresult = CardCodeScanTimeOut;				
		}
	}

	CloseCodeScanner();
	
	scanresult = readQRCodeBuffer->scanresult;
	
	MyFree(readQRCodeBuffer);
	
	return scanresult;
}

/***************************************************************************************************
*FunctionName：ReadBasicCodeData
*Description：读取原始二维码
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月25日14:07:38
***************************************************************************************************/
static void ReadBasicCodeData(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return;
	
	memset(readQRCodeBuffer->originalcode, 0, MAX_QR_CODE_LENGHT+1);
	ReceiveDataFromQueue(GetUsart2RXQueue(), NULL, readQRCodeBuffer->originalcode , MAX_QR_CODE_LENGHT, 
		&(readQRCodeBuffer->originalCodeLen), 1, 10 / portTICK_RATE_MS, 10 / portTICK_RATE_MS);
	
	if(readQRCodeBuffer->originalCodeLen > 0)
	{
		readQRCodeBuffer->originalCodeLen -= 1;
		AnalysisCode(readQRCodeBuffer);
	}
}

/***************************************************************************************************
*FunctionName：DecryptCode
*Description：解码并解析二维码数据
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月25日14:08:25
***************************************************************************************************/
static void AnalysisCode(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return;
	
	/*清空二维码空间*/
	memset(readQRCodeBuffer->decryptcode, 0, MAX_QR_CODE_LENGHT);

	/*数据解密失败*/
	if(pdFAIL == MyDencrypt(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen))
	{
		readQRCodeBuffer->scanresult = CardCodeScanFail;
		return;
	}
	
	memcpy(readQRCodeBuffer->originalcode, readQRCodeBuffer->decryptcode, readQRCodeBuffer->originalCodeLen);
	readQRCodeBuffer->pbuf2 = readQRCodeBuffer->originalcode;
	
	//check qr version
	readQRCodeBuffer->cardQR->qrVersion = readQRCodeBuffer->decryptcode[0];
	if(readQRCodeBuffer->cardQR->qrVersion >= '0' && readQRCodeBuffer->cardQR->qrVersion<= '9')
		;
	//have no qr version filed
	else
		readQRCodeBuffer->cardQR->qrVersion = QRVersion0Define;			//version 0

	switch(readQRCodeBuffer->cardQR->qrVersion)
	{
		case QRVersion0Define: readQRCodeBuffer->scanresult = AnalysisQRVersion0(readQRCodeBuffer); break;
		
		case QRVersion2Define: readQRCodeBuffer->scanresult = AnalysisQRVersion2(readQRCodeBuffer); break;
		
		case QRVersion3Define: readQRCodeBuffer->scanresult = AnalysisQRVersion3(readQRCodeBuffer); break;
		
		default : readQRCodeBuffer->scanresult = CardUnsupported; break;
	}
}
 

static MyState_TypeDef CheckCardIsTimeOut(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer)
	{
		GetGB_Time(&(readQRCodeBuffer->temptime));
		
		if(readQRCodeBuffer->cardQR->CardBaoZhiQi.year == readQRCodeBuffer->temptime.year)
		{
			if(readQRCodeBuffer->cardQR->CardBaoZhiQi.month == readQRCodeBuffer->temptime.month)
			{
				if(readQRCodeBuffer->cardQR->CardBaoZhiQi.day >= readQRCodeBuffer->temptime.day)
					return My_Pass;
			}
			else if(readQRCodeBuffer->cardQR->CardBaoZhiQi.month > readQRCodeBuffer->temptime.month)
				return My_Pass;
		}
		else if(readQRCodeBuffer->cardQR->CardBaoZhiQi.year > readQRCodeBuffer->temptime.year)
			return My_Pass;
	}
	
	return My_Fail;
}

static ScanCodeResult AnalysisQRVersion0(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;

	readQRCodeBuffer->cardQR->calMode = 1;				//T/C
	readQRCodeBuffer->cardQR->qu1Ise = 0;
	readQRCodeBuffer->cardQR->qu2Ise = 0;
	readQRCodeBuffer->cardQR->qu3Ise = 0;
	readQRCodeBuffer->cardQR->qu1_d = 0;
	readQRCodeBuffer->cardQR->qu2_d = 0;
	readQRCodeBuffer->cardQR->qu3_d = 0;
		
	//item
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "#");
	if(readQRCodeBuffer->pbuf1)
	{
		if(getItemConstData(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->pbuf1) == My_Fail)
			return CardUnsupported;
	}
	else
		return CardCodeScanFail;
	
	//channel num
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ChannelNum = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
	
	//t location
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;
	
	//fenduan 1
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	//fenduan 2
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	//quxian 1
	for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*标准曲线2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*标准曲线3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*读取检测卡反应时间*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;
		
	/*读取检测卡C线位置*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*读取检测卡保质期*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	/*读取二维码CRC*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
		
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun1(readQRCodeBuffer->originalcode , readQRCodeBuffer->originalCodeLen - strlen(readQRCodeBuffer->pbuf1)))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;
	
	return CardCodeScanOK;
}

static ScanCodeResult AnalysisQRVersion2(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;

	readQRCodeBuffer->cardQR->qu1Ise = 0;
	readQRCodeBuffer->cardQR->qu2Ise = 0;
	readQRCodeBuffer->cardQR->qu3Ise = 0;
	readQRCodeBuffer->cardQR->qu1_d = 0;
	readQRCodeBuffer->cardQR->qu2_d = 0;
	readQRCodeBuffer->cardQR->qu3_d = 0;
	
	//check crc
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "&");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->pbuf2 = strtok(NULL, "&");
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf2, NULL, 10);
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun1(readQRCodeBuffer->pbuf1 , strlen(readQRCodeBuffer->pbuf1)))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;
		
	//qr versin
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "#");
	if(!readQRCodeBuffer->pbuf1)
		return CardCodeScanFail;
					
	//item
	readQRCodeBuffer->pbuf1 = strtok(NULL, "#");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
		if(getItemConstDataByIndex(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->tempV1) == My_Fail)
			return CardUnsupported;
	}
	else
		return CardCodeScanFail;
	
	//channel num
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ChannelNum = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
	
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->calMode = strtol(readQRCodeBuffer->pbuf1, NULL, 10);
	else
		return CardCodeScanFail;
		
	//t location
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;
	
	//fenduan 1
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	//fenduan 2
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	//quxian 1
	for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[0][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*标准曲线2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[1][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*标准曲线3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		for(readQRCodeBuffer->tempV1=0; readQRCodeBuffer->tempV1<3; readQRCodeBuffer->tempV1++)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->ItemBiaoQu[2][readQRCodeBuffer->tempV1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*读取检测卡反应时间*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;
		
	/*读取检测卡C线位置*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*读取检测卡保质期*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	return CardCodeScanOK;
}

static ScanCodeResult AnalysisQRVersion3(ReadQRCodeBuffer * readQRCodeBuffer)
{
	if(readQRCodeBuffer == NULL)
		return CardCodeScanFail;
	
	//check crc
	readQRCodeBuffer->pbuf1 = strtok(readQRCodeBuffer->decryptcode, "&");
	if(readQRCodeBuffer->pbuf1)
	{
		readQRCodeBuffer->pbuf2 = strtok(NULL, "&");
		readQRCodeBuffer->tempV1 = strtol(readQRCodeBuffer->pbuf2, NULL, 10);
		if(readQRCodeBuffer->tempV1 != CalModbusCRC16Fun1(readQRCodeBuffer->pbuf1 , strlen(readQRCodeBuffer->pbuf1)))
			return CardCodeCRCError;
	}
	else
		return CardCodeScanFail;

	//item
	readQRCodeBuffer->tempV1 = readQRCodeBuffer->decryptcode[1]-0x30;
	if(getItemConstDataByIndex(&(readQRCodeBuffer->cardQR->itemConstData), readQRCodeBuffer->tempV1) == My_Fail)
		return CardUnsupported;
			
	//channel num
	readQRCodeBuffer->cardQR->ChannelNum = readQRCodeBuffer->decryptcode[2]-0x30;
	//T/C or T/T+C
	readQRCodeBuffer->cardQR->calMode = readQRCodeBuffer->decryptcode[3]-0x30;
	//指数标志
	readQRCodeBuffer->tempV1 = readQRCodeBuffer->decryptcode[4]-0x30;
	if(readQRCodeBuffer->tempV1&0x01)
		readQRCodeBuffer->cardQR->qu1Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu1Ise = 0;
			
	if(readQRCodeBuffer->tempV1&0x02)
		readQRCodeBuffer->cardQR->qu2Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu2Ise = 0;
			
	if(readQRCodeBuffer->tempV1&0x04)
		readQRCodeBuffer->cardQR->qu3Ise = 1;
	else
		readQRCodeBuffer->cardQR->qu3Ise = 0;
			
	//T location
	readQRCodeBuffer->pbuf1 = strtok(&readQRCodeBuffer->decryptcode[5] , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemLocation = strtol(readQRCodeBuffer->pbuf1 , NULL, 10);
	else
		return CardCodeScanFail;

	/*读取检测卡标准曲线临界浓度1*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
	
	/*读取检测卡标准曲线临界浓度2*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemFenDuan[1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;

	/*标准曲线1*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->ItemBiaoQu[0][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
	else
		return CardCodeScanFail;
		
	if(readQRCodeBuffer->cardQR->qu1Ise)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->qu1_d = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
	}
	
	/*标准曲线2*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[0] > 0)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[1][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		if(readQRCodeBuffer->cardQR->qu2Ise)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->qu2_d = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
	
	/*标准曲线3*/
	if(readQRCodeBuffer->cardQR->ItemFenDuan[1] > 0)
	{
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][0] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][1] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
		if(readQRCodeBuffer->pbuf1)
			readQRCodeBuffer->cardQR->ItemBiaoQu[2][2] = strtod(readQRCodeBuffer->pbuf1 , NULL);
		else
			return CardCodeScanFail;
		
		if(readQRCodeBuffer->cardQR->qu3Ise)
		{
			readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
			if(readQRCodeBuffer->pbuf1)
				readQRCodeBuffer->cardQR->qu3_d = strtod(readQRCodeBuffer->pbuf1 , NULL);
			else
				return CardCodeScanFail;
		}
	}
		
	/*读取检测卡反应时间*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CardWaitTime = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;
		
	/*读取检测卡C线位置*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		readQRCodeBuffer->cardQR->CLineLocation = strtol(readQRCodeBuffer->pbuf1 , NULL , 10);
	else
		return CardCodeScanFail;


	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->PiHao, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;
	
	
	/*读取检测卡批号*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
		memcpy(readQRCodeBuffer->cardQR->piNum, readQRCodeBuffer->pbuf1, strlen(readQRCodeBuffer->pbuf1));
	else
		return CardCodeScanFail;

	/*读取检测卡保质期*/
	readQRCodeBuffer->pbuf1 = strtok(NULL , "#");
	if(readQRCodeBuffer->pbuf1)
	{
		static char year[10] ,month[10],day[10];
		memcpy(year, readQRCodeBuffer->pbuf1, 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.year = strtol(year , NULL , 10);

		memcpy(month, &readQRCodeBuffer->pbuf1[2], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.month = (unsigned char)strtod(month , NULL );

		memcpy(day, &readQRCodeBuffer->pbuf1[4], 2);
		readQRCodeBuffer->cardQR->CardBaoZhiQi.day = (unsigned char)strtod(day , NULL );
		
		if(My_Fail == CheckCardIsTimeOut(readQRCodeBuffer))
			return CardCodeTimeOut;
	}
	else
		return CardCodeScanFail;

	return CardCodeScanOK;
}
