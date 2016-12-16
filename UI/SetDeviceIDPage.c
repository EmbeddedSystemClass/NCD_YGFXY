/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SetDeviceIDPage.h"

#include	"LCD_Driver.h"
#include	"System_Data.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"ShowDeviceInfoPage.h"
#include	"ReadBarCode_Fun.h"
#include	"SDFunction.h"
#include	"DeviceDao.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SetDeviceIDPage * S_SetDeviceIDPage = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/


static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);

static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspSetDeviceIDPage(void *  parm)
{
	PageInfo * currentpage = NULL;
	
	if(My_Pass == GetCurrentPage(&currentpage))
	{
		currentpage->PageInit = PageInit;
		currentpage->PageUpDate = PageUpDate;
		currentpage->LCDInput = Input;
		currentpage->PageBufferMalloc = PageBufferMalloc;
		currentpage->PageBufferFree = PageBufferFree;
		
		currentpage->PageInit(currentpage->pram);
	}
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_SetDeviceIDPage)
	{
		/*����*/
		S_SetDeviceIDPage->lcdinput[0] = pbuf[4];
		S_SetDeviceIDPage->lcdinput[0] = (S_SetDeviceIDPage->lcdinput[0]<<8) + pbuf[5];
		

		/*�޸��豸id*/
		/*����*/
		if(S_SetDeviceIDPage->lcdinput[0] == 0x1c00)
		{
			PageBufferFree();
			PageBackTo(ParentPage);
		}
		/*ȷ��*/
		else if(S_SetDeviceIDPage->lcdinput[0] == 0x1C01)
		{
			if(S_SetDeviceIDPage->ismodify == 1)
			{
				S_SetDeviceIDPage->temp_deviceinfo.isnew = 1;
				if(My_Pass == SaveDeviceInfo(&(S_SetDeviceIDPage->temp_deviceinfo)))
				{
					SendKeyCode(1);
					S_SetDeviceIDPage->ismodify = 0;
				}
				else
					SendKeyCode(2);
			}
		}
		/*id����*/
		else if(S_SetDeviceIDPage->lcdinput[0] == 0x1C10)
		{
			memset(S_SetDeviceIDPage->temp_deviceinfo.deviceid, 0 , MaxDeviceIDLen);
			
			if(MaxDeviceIDLen >= GetBufLen(&pbuf[7] , 2*pbuf[6]))
				memcpy(S_SetDeviceIDPage->temp_deviceinfo.deviceid, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			else
				memcpy(S_SetDeviceIDPage->temp_deviceinfo.deviceid, &pbuf[7], MaxDeviceIDLen);
				
			S_SetDeviceIDPage->ismodify = 1;
		}
	}
}

static void PageUpDate(void)
{
	if(S_SetDeviceIDPage)
	{
		if(My_Pass == CheckBarCodeHasRead())
		{
			GetGB_BarCode(S_SetDeviceIDPage->tempbuf);
			
			memcpy(S_SetDeviceIDPage->temp_deviceinfo.deviceid, S_SetDeviceIDPage->tempbuf, MaxDeviceIDLen);
			DisText(0x1C10, S_SetDeviceIDPage->temp_deviceinfo.deviceid, MaxDeviceIDLen);
		
			S_SetDeviceIDPage->ismodify = 1;
		}	
	}
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	SelectPage(104);
	
	if(parm)
	{
		memcpy(&(S_SetDeviceIDPage->temp_deviceinfo), parm, sizeof(DeviceInfo));
		DisText(0x1C10, S_SetDeviceIDPage->temp_deviceinfo.deviceid, strlen(S_SetDeviceIDPage->temp_deviceinfo.deviceid));
		return My_Pass;
	}
	
	return My_Fail;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	if(S_SetDeviceIDPage == NULL)
	{
		S_SetDeviceIDPage = (SetDeviceIDPage *)MyMalloc(sizeof(SetDeviceIDPage));
			
		if(NULL == S_SetDeviceIDPage)
			return My_Fail;
	}
	
	memset(S_SetDeviceIDPage, 0, sizeof(SetDeviceIDPage));
	
	return My_Pass;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_SetDeviceIDPage);
	S_SetDeviceIDPage = NULL;
	
	return My_Pass;
}

