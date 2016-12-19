/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"SetDeviceInfoPage.h"

#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"MyMem.h"
#include	"ShowDeviceInfoPage.h"
#include	"System_Data.h"
#include	"SystemSet_Dao.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static SetDeviceInfoPageBuffer * S_SetDeviceInfoPageBuffer = NULL;
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

unsigned char DspSetDeviceInfoPage(void *  parm)
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
	if(S_SetDeviceInfoPageBuffer)
	{
		/*����*/
		S_SetDeviceInfoPageBuffer->lcdinput[0] = pbuf[4];
		S_SetDeviceInfoPageBuffer->lcdinput[0] = (S_SetDeviceInfoPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B00)
		{
			PageBufferFree();
			PageBackTo(ParentPage);
		}
		/*ȷ��*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B01)
		{			
			if(S_SetDeviceInfoPageBuffer->ismodify == 1)
			{
				S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.isnew = true;
				if(My_Pass == SaveSystemSetData(&(S_SetDeviceInfoPageBuffer->systemSetData)))
				{
					SendKeyCode(1);
					//����ɹ��������ڴ��е�����
					setSystemSetData(&(S_SetDeviceInfoPageBuffer->systemSetData));
					S_SetDeviceInfoPageBuffer->ismodify = 0;
				}
				else
					SendKeyCode(2);
			}
		}
		/*����*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B10)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_name, 0, MaxNameLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_name, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*����*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B20)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_age, 0, MaxAgeLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_age, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*�Ա�*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B30)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_sex, 0, MaxSexLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_sex, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*��ϵ��ʽ*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B40)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_phone, 0, MaxPhoneLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_phone, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*ְλ*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B50)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_job, 0, MaxJobLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_job, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*��ע*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B60)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_desc, 0, MaxDescLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceuser.user_desc, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
		}
		/*�豸ʹ�õ�ַ*/
		else if(S_SetDeviceInfoPageBuffer->lcdinput[0] == 0x1B70)
		{
			memset(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceunit, 0, MaxDeviceUnitLen);
			memcpy(S_SetDeviceInfoPageBuffer->systemSetData.deviceInfo.deviceunit, &pbuf[7], GetBufLen(&pbuf[7] , 2*pbuf[6]));
			S_SetDeviceInfoPageBuffer->ismodify = 1;
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
		getSystemSetData(&(S_SetDeviceInfoPageBuffer->systemSetData));
	}
	
	SelectPage(102);
	
	return My_Fail;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	if(S_SetDeviceInfoPageBuffer == NULL)
	{
		S_SetDeviceInfoPageBuffer = MyMalloc(sizeof(SetDeviceInfoPageBuffer));
		
		if(S_SetDeviceInfoPageBuffer)
		{
			memset(S_SetDeviceInfoPageBuffer, 0, sizeof(SetDeviceInfoPageBuffer));
	
			return My_Pass;
		}
		else
			return My_Fail;
	}

	return My_Pass;
}

static MyState_TypeDef PageBufferFree(void)
{
	if(S_SetDeviceInfoPageBuffer)
	{
		MyFree(S_SetDeviceInfoPageBuffer);
		S_SetDeviceInfoPageBuffer = NULL;
	}
	
	return My_Pass;
}


