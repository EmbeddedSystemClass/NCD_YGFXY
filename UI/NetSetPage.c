/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"NetSetPage.h"
#include	"LCD_Driver.h"
#include	"SystemSet_Dao.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"SleepPage.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static NetSetPageBuffer *S_NetSetPageBuffer = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);
static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);

static void UpPageValue(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/***************************************************************************************************
*FunctionName: createSelectUserActivity
*Description: ����ѡ������˽���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:09
***************************************************************************************************/
MyState_TypeDef createNetSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: ��ʾ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	copyGBSystemSetData(&(S_NetSetPageBuffer->systemSetData));
		
	memcpy(&(S_NetSetPageBuffer->myNetSet), &(S_NetSetPageBuffer->systemSetData.netSet), sizeof(NetSet));
		
	UpPageValue();
	
	SelectPage(110);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	/*����*/
	S_NetSetPageBuffer->lcdinput[0] = pbuf[4];
	S_NetSetPageBuffer->lcdinput[0] = (S_NetSetPageBuffer->lcdinput[0]<<8) + pbuf[5];

	/*��������ip��ȡģʽ*/
	if(S_NetSetPageBuffer->lcdinput[0] == 0x1E09)
	{
		/*����*/
		S_NetSetPageBuffer->lcdinput[1] = pbuf[7];
		S_NetSetPageBuffer->lcdinput[1] = (S_NetSetPageBuffer->lcdinput[1]<<8) + pbuf[8];
			
		/*�Զ���ȡip*/
		if(S_NetSetPageBuffer->lcdinput[1] == 0x8000)
			S_NetSetPageBuffer->myNetSet.ipmode = DHCP_Mode;
		/*ʹ�����õ�ip*/
		else if(S_NetSetPageBuffer->lcdinput[1] == 0x0000)
			S_NetSetPageBuffer->myNetSet.ipmode = User_Mode;
	}
	/*����IP*/
	else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E10)
	{
		memset(S_NetSetPageBuffer->buf, 0, 50);
		memcpy(S_NetSetPageBuffer->buf, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
		if(My_Pass != parseIpString(&S_NetSetPageBuffer->myNetSet.myip, S_NetSetPageBuffer->buf))
		{
			memset(&S_NetSetPageBuffer->myNetSet.myip, 0, 4);
			ClearText(0x1E10);
			SendKeyCode(3);
		}
	}
	/*ȷ���޸�*/
	else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E05)
	{
		copyGBSystemSetData(&(S_NetSetPageBuffer->systemSetData));
		
		S_NetSetPageBuffer->myNetSet.crc = CalModbusCRC16Fun1(&(S_NetSetPageBuffer->myNetSet), sizeof(NetSet)-2);
		memcpy(&(S_NetSetPageBuffer->systemSetData.netSet), &(S_NetSetPageBuffer->myNetSet), sizeof(NetSet));
				
		if(My_Pass == SaveSystemSetData(&(S_NetSetPageBuffer->systemSetData)))
			SendKeyCode(1);
		else
			SendKeyCode(2);
	}
	/*����*/
	else if(S_NetSetPageBuffer->lcdinput[0] == 0x1E04)
	{
		backToFatherActivity();
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: ����ˢ��
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(S_NetSetPageBuffer)
	{

	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: ���ؽ���ʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: ����ָ���ʾʱҪ������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:01:58
***************************************************************************************************/
static void activityResume(void)
{
	if(S_NetSetPageBuffer)
	{

	}
	
	SelectPage(110);
}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: ��������
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: ���������ڴ�����
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 
***************************************************************************************************/
static MyState_TypeDef activityBufferMalloc(void)
{
	if(NULL == S_NetSetPageBuffer)
	{
		S_NetSetPageBuffer = MyMalloc(sizeof(NetSetPageBuffer));
		
		if(S_NetSetPageBuffer)
		{
			memset(S_NetSetPageBuffer, 0, sizeof(NetSetPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Pass;
}

/***************************************************************************************************
*FunctionName: activityBufferFree
*Description: �����ڴ��ͷ�
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��21��09:03:10
***************************************************************************************************/
static void activityBufferFree(void)
{
	MyFree(S_NetSetPageBuffer);
	S_NetSetPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void UpPageValue(void)
{
	if(S_NetSetPageBuffer)
	{
		memset(S_NetSetPageBuffer->buf, 0, 100);
		
		/*����ip��ȡ��ʽ*/
		if(S_NetSetPageBuffer->myNetSet.ipmode != User_Mode)
			S_NetSetPageBuffer->buf[0] = 0x80;	
		else
			S_NetSetPageBuffer->buf[0] = 0x00;

		WriteRadioData(0x1E09, S_NetSetPageBuffer->buf, 2);
			
		/*����ip*/
		if(S_NetSetPageBuffer->myNetSet.ipmode == User_Mode)
		{
			sprintf((S_NetSetPageBuffer->buf), "%03d.%03d.%03d.%03d", S_NetSetPageBuffer->myNetSet.myip.ip_1, S_NetSetPageBuffer->myNetSet.myip.ip_2, 
				S_NetSetPageBuffer->myNetSet.myip.ip_3, S_NetSetPageBuffer->myNetSet.myip.ip_4);
			DisText(0x1E10, S_NetSetPageBuffer->buf, strlen((S_NetSetPageBuffer->buf)));
		}
		else
			ClearText(0x1E10);
	}
}

