/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"NetInfoPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SDFunction.h"
#include	"NetPreSetPage.h"
#include	"MyMem.h"
#include	"WifiFunction.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static NetInfoPageBuffer *S_NetInfoPageBuffer = NULL;
/******************************************************************************************/
/*****************************************局部函数声明*************************************/
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
*Description: 创建选择操作人界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:09
***************************************************************************************************/
MyState_TypeDef createNetInfoActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "NetInfoActivity", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: activityStart
*Description: 显示主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:32
***************************************************************************************************/
static void activityStart(void)
{
	if(S_NetInfoPageBuffer)
	{
		timer_set(&(S_NetInfoPageBuffer->timer), 10);
	
		ReadNetInfo();
		
		ShowNetInfo();	
	}
	
	SelectPage(145);
}

/***************************************************************************************************
*FunctionName: activityInput
*Description: 界面输入
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_NetInfoPageBuffer)
	{
		/*命令*/
		S_NetInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_NetInfoPageBuffer->lcdinput[0] = (S_NetInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//返回
		if(S_NetInfoPageBuffer->lcdinput[0] == 0x1ca0)
		{
			backToFatherActivity();
		}
	}
}

/***************************************************************************************************
*FunctionName: activityFresh
*Description: 界面刷新
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:16
***************************************************************************************************/
static void activityFresh(void)
{
	if(TimeOut == timer_expired(&(S_NetInfoPageBuffer->timer)))
	{
		ReadNetInfo();
		ShowNetInfo();
		timer_reset(&(S_NetInfoPageBuffer->timer));
	}
}

/***************************************************************************************************
*FunctionName: activityHide
*Description: 隐藏界面时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:40
***************************************************************************************************/
static void activityHide(void)
{

}

/***************************************************************************************************
*FunctionName: activityResume
*Description: 界面恢复显示时要做的事
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:01:58
***************************************************************************************************/
static void activityResume(void)
{

}

/***************************************************************************************************
*FunctionName: activityDestroy
*Description: 界面销毁
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:02:15
***************************************************************************************************/
static void activityDestroy(void)
{
	activityBufferFree();
}

/***************************************************************************************************
*FunctionName: activityBufferMalloc
*Description: 界面数据内存申请
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
*Description: 界面内存释放
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:03:10
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
*Description: 读取网络信息
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月5日15:38:30
***************************************************************************************************/
static void ReadNetInfo(void)
{
	GetWifiStaIP(&(S_NetInfoPageBuffer->netinfo.WifiIP));
}

/***************************************************************************************************
*FunctionName: ShowNetInfo
*Description: 显示网络信息
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月5日15:38:44
***************************************************************************************************/
static void ShowNetInfo(void)
{
	GetGB_NetInfo(&(S_NetInfoPageBuffer->netinfo));
	
	//显示有线网信息
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
	
	//显示wifi信息
	
}
