/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"ShowDeviceInfoPage.h"

#include	"SetDeviceInfoPage.h"
#include	"SetDeviceIDPage.h"
#include	"SystemSetPage.h"

#include	"LCD_Driver.h"

#include	"UI_Data.h"
#include	"SystemSet_Dao.h"
#include	"MyMem.h"
#include	"MyTools.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static ShowDeviceInfoPageBuffer * S_ShowDeviceInfoPageBuffer = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void showDeviceInfo(void);

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
	if(S_ShowDeviceInfoPageBuffer)
	{
		/*����*/
		S_ShowDeviceInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_ShowDeviceInfoPageBuffer->lcdinput[0] = (S_ShowDeviceInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*������Ϣ*/
		if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a03)
			S_ShowDeviceInfoPageBuffer->presscount = 0;
		
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a04)
			S_ShowDeviceInfoPageBuffer->presscount++;
		
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a05)
		{
			if(S_ShowDeviceInfoPageBuffer->presscount > 15)
				SendKeyCode(2);
		}
		/*��ȡ����*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a10)
		{
			if(pdPASS == CheckStrIsSame(&pbuf[7] , AdminPassWord ,GetBufLen(&pbuf[7] , 2*pbuf[6])))
			{
				PageAdvanceTo(DspSetDeviceIDPage, NULL);
			}
			else
				SendKeyCode(1);
		}
		/*����*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a00)
		{
			PageBackTo(ParentPage);
		}
		/*�޸�*/
		else if(S_ShowDeviceInfoPageBuffer->lcdinput[0] == 0x1a01)
		{
			PageAdvanceTo(DspSetDeviceInfoPage, NULL);
		}
	}
}

static void PageUpDate(void)
{

}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Pass == PageBufferMalloc())
	{
		getSystemSetData(&(S_ShowDeviceInfoPageBuffer->systemSetData));
		
		showDeviceInfo();
	}
	
	SelectPage(100);

	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	if(S_ShowDeviceInfoPageBuffer == NULL)
	{
		S_ShowDeviceInfoPageBuffer = MyMalloc(sizeof(ShowDeviceInfoPageBuffer));
		if(S_ShowDeviceInfoPageBuffer)
		{
			memset(S_ShowDeviceInfoPageBuffer, 0, sizeof(ShowDeviceInfoPageBuffer));
		
			return My_Pass;
		}
		return My_Fail; 
	}

	return My_Pass;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_ShowDeviceInfoPageBuffer);
	S_ShowDeviceInfoPageBuffer = NULL;
	
	return My_Pass;
}

static void showDeviceInfo(void)
{
	if(S_ShowDeviceInfoPageBuffer)
	{
		/*��ʾ�豸id*/
		DisText(0x1a40, S_ShowDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceid, MaxDeviceIDLen);
			
		/*��ʾʹ�õ�λ*/
		DisText(0x1a60, S_ShowDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceunit, MaxDeviceUnitLen);

		/*��ʾ������*/
		DisText(0x1a90, S_ShowDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_name, MaxNameLen);
	}
}

