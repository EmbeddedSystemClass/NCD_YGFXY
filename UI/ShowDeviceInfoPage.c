/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ShowDeviceInfoPage.h"

#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SetDeviceInfoPage.h"
#include	"SetDeviceIDPage.h"
#include	"SystemSetPage.h"
#include	"MyMem.h"
#include	"SDFunction.h"
#include	"MyTools.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static unsigned char presscount = 0;
const char *GB_Password	= "123456";		//����,����У׼��������
static DeviceInfo *gb_deviceinfo = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void showDeviceInfo(void);
static void ClearPageText(void);

static MyState_TypeDef PageInit(void *  parm);
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspShowDeviceInfoPage(void *  parm)
{
	SetGBSysPage(DspShowDeviceInfoPage, DspSystemSetPage, DspSetDeviceInfoPage, Input, PageUpDate, PageInit, PageBufferMalloc, PageBufferFree);
	
	GBPageInit(parm);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	unsigned short *pdata = NULL;
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*������Ϣ*/
	if(pdata[0] == 0x220a)
	{
		presscount = 0;
	}
	else if(pdata[0] == 0x220b)
	{
		presscount++;
	}
	else if(pdata[0] == 0x220c)
	{
		if(presscount > 20)
		{
			SendKeyCode(1);
		}
	}
	/*��ȡ����*/
	else if(pdata[0] == 0x2230)
	{
		if(gb_deviceinfo)
		{
			if(pdPASS == CheckStrIsSame(&pbuf[7] , (void *)GB_Password ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
			{
				SetGBChildPage(DspSetDeviceIDPage);
				GotoGBChildPage(gb_deviceinfo);
			}
			else
				SendKeyCode(2);
		}
	}
	/*����*/
	else if(pdata[0] == 0x2207)
	{
		GBPageBufferFree();
		GotoGBParentPage(NULL);
	}
	/*�޸�*/
	else if(pdata[0] == 0x2208)
	{
		if(gb_deviceinfo)
		{
			SetGBChildPage(DspSetDeviceInfoPage);
			GotoGBChildPage(gb_deviceinfo);
		}
	}
	
	MyFree(pdata);
}

static void PageUpDate(void)
{

}

static MyState_TypeDef PageInit(void *  parm)
{
	PageBufferMalloc();
	
	SelectPage(70);
	
	ClearPageText();
	
	if(gb_deviceinfo)
	{
		ReadDeviceInfo(gb_deviceinfo);
		
		showDeviceInfo();
	}
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	if(gb_deviceinfo == NULL)
	{
		gb_deviceinfo = MyMalloc(sizeof(DeviceInfo));
		if(gb_deviceinfo)
			memset(gb_deviceinfo, 0, sizeof(DeviceInfo));
		else
			return My_Fail;
	}
	
	return My_Pass;
}

static MyState_TypeDef PageBufferFree(void)
{
	if(gb_deviceinfo)
	{
		MyFree(gb_deviceinfo);
		gb_deviceinfo = NULL;
	}
	
	return My_Pass;
}

static void showDeviceInfo(void)
{
	
	/*��ʾ�豸id*/
	DisText(0x2240, gb_deviceinfo->deviceid, strlen(gb_deviceinfo->deviceid));
	
	/*��ʾ�豸����*/
	DisText(0x2250, gb_deviceinfo->devicename, strlen(gb_deviceinfo->devicename));
	
	/*��ʾʹ�õ�λ*/
	DisText(0x2270, gb_deviceinfo->deviceunit, strlen(gb_deviceinfo->deviceunit));
	/*��ʾ�豸������*/
	DisText(0x2290, gb_deviceinfo->devicemaker, strlen(gb_deviceinfo->devicemaker));
	
	/*��ʾ�豸��������ϵ��ʽ*/
	DisText(0x22b0, gb_deviceinfo->devicemakerphone, strlen(gb_deviceinfo->devicemakerphone));
	
	/*��ʾ������*/
	DisText(0x22c0, gb_deviceinfo->deviceuser.user_name, strlen(gb_deviceinfo->deviceuser.user_name));
	
}

static void ClearPageText(void)
{
	/*��ʾ�豸id*/
	ClearText(0x2240, 20);
	
	/*��ʾ�豸����*/
	ClearText(0x2250, 50);
	
	/*��ʾ�豸������*/
	ClearText(0x2270, 50);
	
	/*��ʾ�豸��������ϵ��ʽ*/
	ClearText(0x2290, 20);
	
	/*��ʾʹ�õ�λ*/
	ClearText(0x22b0, 50);
	
	/*��ʾ������*/
	ClearText(0x22c0, 10);
}



