/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SystemSetPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"LunchPage.h"
#include	"ShowDeviceInfoPage.h"
#include	"UserMPage.h"
#include	"NetPreSetPage.h"
#include	"RecordPage.h"
#include	"OtherSetPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpData(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspSystemSetPage(void *  parm)
{
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspSystemSetPage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = DspLunchPage;
	myPage->ChildPage = NULL;

	SelectPage(68);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	unsigned short *pdata = NULL;
	SysPage * myPage = GetSysPage();
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*������Ϣ*/
	if(pdata[0] == 0x2200)
	{
		myPage->ChildPage = DspShowDeviceInfoPage;
		myPage->ChildPage(NULL);
	}
	/*�����˹���*/
	else if(pdata[0] == 0x2201)
	{
		myPage->ChildPage = DspUserMPage;
		myPage->ChildPage(NULL);
	}
	/*��������*/
	else if(pdata[0] == 0x2202)
	{
		myPage->ChildPage = DspNetPreSetPage;
		myPage->ChildPage(NULL);
	}
	/*���ݹ���*/
	else if(pdata[0] == 0x2203)
	{
		myPage->ChildPage = DspRecordPage;
		myPage->ChildPage(NULL);
	}
	/*У׼����*/
	else if(pdata[0] == 0x2204)
	{
		myPage->ChildPage = DspSystemSetPage;
		myPage->ChildPage(NULL);
	}
	/*��������*/
	else if(pdata[0] == 0x2205)
	{
		myPage->ChildPage = DspOtherSetPage;
		myPage->ChildPage(NULL);
	}
	/*����*/
	else if(pdata[0] == 0x2206)
	{
		myPage->ParentPage(NULL);
	}
	
	MyFree(pdata);
}

static void PageUpData(void)
{

}





