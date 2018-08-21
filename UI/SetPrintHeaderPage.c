/******************************************************************************************/
/*****************************************头文件*******************************************/

#include	"SetPrintHeaderPage.h"

#include	"LCD_Driver.h"
#include	"MyMem.h"
#include	"ShowDeviceInfoPage.h"
#include	"System_Data.h"
#include	"SystemSet_Dao.h"
#include	"SleepPage.h"
#include    "CRC16.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************局部变量声明*************************************/
static SetPrintHeaderPageBuffer * page = NULL;
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

static void showDeviceInfoText(void);
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
MyState_TypeDef createSetPrintHeaderActivity(Activity * thizActivity, Intent * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "SetPrintHeaderActivity\0", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
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
	readPrintInfo(&page->printInfo);
	
    showDeviceInfoText();
	
	SelectPage(102);
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
	page->lcdinput[0] = pbuf[4];
	page->lcdinput[0] = (page->lcdinput[0]<<8) + pbuf[5];
		
	/*返回*/
	if(page->lcdinput[0] == 0x1B00)
	{
		backToFatherActivity();
	}
	/*确认*/
	else if(page->lcdinput[0] == 0x1B01)
	{			
		if(TRUE == page->ismodify)
        {
            page->printInfo.crc = CalModbusCRC16Fun1(&page->printInfo, sizeof(PrintInfo)-2);
            copyGBSystemSetData(&page->systemSetData);
            memcpy(&page->systemSetData.printInfo, &page->printInfo, sizeof(PrintInfo));
            
            if(My_Pass == SaveSystemSetData(&page->systemSetData))
			{
				SendKeyCode(1);
				page->ismodify = FALSE;
			}
			else
				SendKeyCode(2);
        }
	}
	
	//打印数据头
	else if(page->lcdinput[0] == 0x1B70)
	{
        page->tempShort = GetBufLen(&pbuf[7] , 2*pbuf[6]);
        if(page->tempShort > 30)
            page->tempShort = 30;
        
        memset(page->printInfo.header, 0, 30);
        memcpy(page->printInfo.header, &pbuf[7], page->tempShort);
        page->printInfo.crc = CalModbusCRC16Fun1(&page->printInfo, sizeof(PrintInfo)-2);
        
        page->ismodify = TRUE;
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
	SelectPage(102);
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
	if(NULL == page)
	{
		page = MyMalloc(sizeof(SetPrintHeaderPageBuffer));
		
		if(page)
		{
			memset(page, 0, sizeof(SetPrintHeaderPageBuffer));
	
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
	MyFree(page);
	page = NULL;
}

static void showDeviceInfoText(void)
{
	DisText(0x1b70, page->printInfo.header, 30);
}
