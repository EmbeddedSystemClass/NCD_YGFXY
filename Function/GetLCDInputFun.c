/***************************************************************************************************
*FileName:GetLCDInputFun
*Description:获取lcd输入
*Author:xsx
*Data:2016年4月28日17:42:51
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"GetLCDInputFun.h"
#include	"Usart6_Driver.h"
#include	"UI_Data.h"

#include	"QueueUnits.h"
#include	"MyMem.h"
#include	"Define.h"
#include	"CRC16.h"

#include 	"usbd_cdc_vcp.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void AnalysisCode(void * pbuf, unsigned short len);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：GetLCDInputData
*Description：获取LCD按键输入数据
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月28日17:43:24
***************************************************************************************************/
void GetLCDInputData(void)
{
	char *buf = NULL;
	unsigned short RXCount = 0;
	
	buf = MyMalloc(100);
	if(buf)
	{
		memset(buf, 0, 100);

		while(pdPASS == ReceiveCharFromQueue(GetUsart6RXQueue(), GetUsart6RXMutex(), (buf+RXCount) , 10 * portTICK_RATE_MS))
			RXCount++;
	
		if(RXCount > 0)
			AnalysisCode(buf, RXCount);
	}
	
	MyFree(buf);
}

/***************************************************************************************************
*FunctionName：AnalysisCode
*Description：处理接收的数据
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月29日10:08:39
***************************************************************************************************/
static void AnalysisCode(void * pbuf, unsigned short len)
{
	unsigned char * p = (unsigned char *)pbuf;
	unsigned short * crc = (unsigned short *)(p+len-2);
	unsigned short *tempcrc = (unsigned short *)(p+len);
	SysPage * myPage = GetSysPage();
	
	*tempcrc = CalModbusCRC16Fun1(p+3, len-2-3);
	
	if((p[0] == LCD_Head_1)&&(p[1] == LCD_Head_2)&&(len == (p[2]+3))&&(*crc == *tempcrc))
	{
		if(p[3] == R_REGSITER)
		{

		}
		
		else if(p[3] == R_ADDRESS)
		{
			myPage->LCDInput(pbuf , len);
		}
	}

}
