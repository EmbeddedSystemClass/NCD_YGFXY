/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"NetInfoPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SDFunction.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"WifiFunction.h"
#include	"SleepPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static NetInfoPageBuffer *S_NetInfoPageBuffer = NULL;
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

static void ReadNetInfo(void);
static void ShowNetInfo(void);
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
MyState_TypeDef createNetInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetInfoActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	if(S_NetInfoPageBuffer)
	{
		//��ȡϵͳ����
		getSystemSetData(&(S_NetInfoPageBuffer->systemSetData));
		
		timer_set(&(S_NetInfoPageBuffer->timer2), S_NetInfoPageBuffer->systemSetData.ledSleepTime);
		
		timer_set(&(S_NetInfoPageBuffer->timer), 2);
	
		ReadNetInfo();
		
		ShowNetInfo();	
	}
	
	SelectPage(145);
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
	if(S_NetInfoPageBuffer)
	{
		/*����*/
		S_NetInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_NetInfoPageBuffer->lcdinput[0] = (S_NetInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		timer_restart(&(S_NetInfoPageBuffer->timer2));
		
		//����
		if(S_NetInfoPageBuffer->lcdinput[0] == 0x1ca0)
		{
			backToFatherActivity();
		}
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
	if(S_NetInfoPageBuffer)
	{
		if(TimeOut == timer_expired(&(S_NetInfoPageBuffer->timer)))
		{
			ReadNetInfo();
			ShowNetInfo();
			timer_reset(&(S_NetInfoPageBuffer->timer));
		}
		
		if(TimeOut == timer_expired(&(S_NetInfoPageBuffer->timer2)))
		{
			startActivity(createSleepActivity, NULL);
		}
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
	if(S_NetInfoPageBuffer)
	{
		timer_restart(&(S_NetInfoPageBuffer->timer2));
	}
	
	SelectPage(145);
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
	if(NULL == S_NetInfoPageBuffer)
	{
		S_NetInfoPageBuffer = MyMalloc(sizeof(NetInfoPageBuffer));
		
		if(S_NetInfoPageBuffer)
		{
			memset(S_NetInfoPageBuffer, 0, sizeof(NetInfoPageBuffer));
	
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
	MyFree(S_NetInfoPageBuffer);
	S_NetInfoPageBuffer = NULL;
}

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: ReadNetInfo
*Description: ��ȡ������Ϣ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��5��15:38:30
***************************************************************************************************/
static void ReadNetInfo(void)
{
	GetWifiStaIP(&(S_NetInfoPageBuffer->netinfo.WifiIP));
}

/***************************************************************************************************
*FunctionName: ShowNetInfo
*Description: ��ʾ������Ϣ
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��5��15:38:44
***************************************************************************************************/
static void ShowNetInfo(void)
{
	GetGB_NetInfo(&(S_NetInfoPageBuffer->netinfo));
	
	//��ʾ��������Ϣ
	if(S_NetInfoPageBuffer->netinfo.LineStatus == 0)
		S_NetInfoPageBuffer->lineico.ICO_ID = 30;
	else
		S_NetInfoPageBuffer->lineico.ICO_ID = 31;
	S_NetInfoPageBuffer->lineico.X = 185;
	S_NetInfoPageBuffer->lineico.Y = 130;
	
	BasicUI(0x1CB0 ,0x1907 , 1, &(S_NetInfoPageBuffer->lineico) , sizeof(Basic_ICO));
	
	memset(S_NetInfoPageBuffer->tempbuffer1, 0, 100);
	sprintf(S_NetInfoPageBuffer->tempbuffer1, "%03d.%03d.%03d.%03d", S_NetInfoPageBuffer->netinfo.LineIP.ip_1, S_NetInfoPageBuffer->netinfo.LineIP.ip_2, 
		S_NetInfoPageBuffer->netinfo.LineIP.ip_3, S_NetInfoPageBuffer->netinfo.LineIP.ip_4);
	DisText(0x1CC0, S_NetInfoPageBuffer->tempbuffer1, strlen(S_NetInfoPageBuffer->tempbuffer1));
	
	memset(S_NetInfoPageBuffer->tempbuffer1, 0, 100);
	sprintf(S_NetInfoPageBuffer->tempbuffer1, "%02X-%02X-%02X-%02X-%02X-%02X", S_NetInfoPageBuffer->netinfo.LineMAC[0], S_NetInfoPageBuffer->netinfo.LineMAC[1], 
		S_NetInfoPageBuffer->netinfo.LineMAC[2], S_NetInfoPageBuffer->netinfo.LineMAC[3], S_NetInfoPageBuffer->netinfo.LineMAC[4], 
		S_NetInfoPageBuffer->netinfo.LineMAC[5]);
	DisText(0x1CD0, S_NetInfoPageBuffer->tempbuffer1, strlen(S_NetInfoPageBuffer->tempbuffer1));
	
	//��ʾwifi��Ϣ
	
}
