/******************************************************************************************/
/*****************************************头文件*******************************************/

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
/*****************************************局部变量声明*************************************/
static ServerSetPageBuffer * pageBuffer = NULL;
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

static void UpPageValue(void);
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
*Description: 显示主界面
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:32
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
*Description: 界面输入
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:00:59
***************************************************************************************************/
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	/*命令*/
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
	/*确认修改*/
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
	/*返回*/
	else if(pageBuffer->lcdinput[0] == 0x1E3a)
	{
		backToFatherActivity();
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
	SelectPage(109);
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
*Description: 界面内存释放
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016年12月21日09:03:10
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

