/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ServerSetPage.h"
#include	"LCD_Driver.h"
#include	"SystemSet_Dao.h"
#include	"MyMem.h"
#include	"CRC16.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ServerSetPageBuffer * pageBuffer = NULL;
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
MyState_TypeDef createServerSetActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "ServerSetActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	copyGBSystemSetData(&pageBuffer->systemSetData);
		
	memcpy(&pageBuffer->serverSet, &pageBuffer->systemSetData.serverSet, ServerStructSize);
		
	UpPageValue();
	
	SelectPage(109);
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
	pageBuffer->lcdinput[0] = pbuf[4];
	pageBuffer->lcdinput[0] = (pageBuffer->lcdinput[0]<<8) + pbuf[5];

	//server ip
	if(pageBuffer->lcdinput[0] == 0x1E20)
	{
		GetBufLen(&pbuf[7] , 2*pbuf[6]);

		if(My_Pass != parseIpString(&pageBuffer->serverSet.serverIP, &pbuf[7]))
		{
			memset(&pageBuffer->serverSet.serverIP, 0, 4);
			ClearText(0x1E20);
			SendKeyCode(3);
		}
	}
	//server port
	else if(pageBuffer->lcdinput[0] == 0x1E30)
	{
		GetBufLen(&pbuf[7] , 2*pbuf[6]);
		if(My_Pass != parsePortString(&pageBuffer->serverSet.serverPort, &pbuf[7]))
		{
			pageBuffer->serverSet.serverPort = 0;
			ClearText(0x1E30);
			SendKeyCode(3);
		}
	}
	/*ȷ���޸�*/
	else if(pageBuffer->lcdinput[0] == 0x1E3b)
	{
		copyGBSystemSetData(&(pageBuffer->systemSetData));
		
		pageBuffer->serverSet.crc = CalModbusCRC16Fun1(&pageBuffer->serverSet, ServerStructSize-2);
		memcpy(&pageBuffer->systemSetData.serverSet, &pageBuffer->serverSet, ServerStructSize);
				
		if(My_Pass == SaveSystemSetData(&pageBuffer->systemSetData))
			SendKeyCode(1);
		else
			SendKeyCode(2);
	}
	/*����*/
	else if(pageBuffer->lcdinput[0] == 0x1E3a)
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
	SelectPage(109);
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
	if(NULL == pageBuffer)
	{
		pageBuffer = MyMalloc(sizeof(ServerSetPageBuffer));
		
		if(pageBuffer)
		{
			memset(pageBuffer, 0, sizeof(ServerSetPageBuffer));
	
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
	MyFree(pageBuffer);
	pageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void UpPageValue(void)
{
	sprintf(pageBuffer->buf, "%03d.%03d.%03d.%03d", pageBuffer->serverSet.serverIP.ip_1, pageBuffer->serverSet.serverIP.ip_2, 
				pageBuffer->serverSet.serverIP.ip_3, pageBuffer->serverSet.serverIP.ip_4);
	DisText(0x1E20, pageBuffer->buf, strlen(pageBuffer->buf)+1);
	
	sprintf(pageBuffer->buf, "%05d", pageBuffer->serverSet.serverPort);
	DisText(0x1E30, pageBuffer->buf, strlen(pageBuffer->buf)+1);

}

