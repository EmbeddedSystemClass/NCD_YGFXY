/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"LunchPage.h"

#include	"LCD_Driver.h"

#include	"Define.h"
#include	"MyMem.h"

#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"PaiDuiPage.h"
#include	"SleepPage.h"
#include	"WifiFunction.h"
#include	"Net_Data.h"
#include	"PlaySong_Task.h"
#include	"UI_Data.h"
#include	"MyTest_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static LunchPageBuffer * S_LunchPageBuffer = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpData(void);
static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspLunchPage(void *  parm)
{
	SysPage * myPage = GetSysPage();

	myPage->CurrentPage = DspLunchPage;
	myPage->LCDInput = Input;
	myPage->PageUpData = PageUpData;
	myPage->ParentPage = NULL;
	myPage->ChildPage = NULL;
	myPage->PageInit = PageInit;
	myPage->PageBufferMalloc = PageBufferMalloc;
	myPage->PageBufferFree = PageBufferFree;
	
	SelectPage(52);
	
	myPage->PageInit(parm);
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	unsigned short *pdata = NULL;
	unsigned char error = 0;
	SysPage * myPage = GetSysPage();
	
	pdata = MyMalloc((len/2)*sizeof(unsigned short));
	if(pdata == NULL)
		return;
	
	/*����*/
	pdata[0] = pbuf[4];
	pdata[0] = (pdata[0]<<8) + pbuf[5];
	
	/*����*/
	if(pdata[0] == 0x1b04)
	{
		myPage->PageBufferFree();
		myPage->ChildPage = DspSystemSetPage;
		myPage->ChildPage(NULL);
	}
	/*�������*/
	else if(pdata[0] == 0x1b00)
	{	
		error = CreateANewTest(0);
		/*�����ɹ�*/
		if(0 == error)
		{
			myPage->PageBufferFree();
			DspSelectUserPage(NULL);
		}
		/*��ֹ�������*/
		else if(1 == error)
		{
			SendKeyCode(2);
			AddNumOfSongToList(45, 0);
		}
		/*����ʧ��*/
		else if(3 == error)
		{
			SendKeyCode(3);
			AddNumOfSongToList(41, 0);
		}
	}
	else if(pdata[0] == 0x1b01)
	{
		if(S_LunchPageBuffer)
			S_LunchPageBuffer->presscount = 0;
	}
	else if(pdata[0] == 0x1b02)
	{
		if(S_LunchPageBuffer)
			S_LunchPageBuffer->presscount++;
	}
	else if(pdata[0] == 0x1b03)
	{
		if(S_LunchPageBuffer)
		{
			/*�鿴�Ŷ�״̬*/
			if(S_LunchPageBuffer->presscount > 20)
			{
				myPage->PageBufferFree();
				DspPaiDuiPage(NULL);
			}
			/*��������*/
			else
			{
				error = CreateANewTest(111);
				/*�����ɹ�*/
				if(0 == error)
				{
					myPage->PageBufferFree();
					DspSelectUserPage(NULL);
				}
				/*�Ŷ�λ������������*/
				else if(1 == error)
				{
					SendKeyCode(1);
					AddNumOfSongToList(40, 0);
				}
				/*����ʧ��*/
				else if(2 == error)
				{
					SendKeyCode(3);
					AddNumOfSongToList(41, 0);
				}
				/*�п���������*/
				else if(3 == error)
				{
					SendKeyCode(4);
					AddNumOfSongToList(61, 0);
				}
			}
		}
	}
	
	MyFree(pdata);
}

static void PageUpData(void)
{
	if(TimeOut == timer_expired(&(S_LunchPageBuffer->timer)))
	{
		PageBufferFree();
		DspSleepPage(NULL);
	}
}

static MyState_TypeDef PageInit(void *  parm)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;
	
	timer_set(&(S_LunchPageBuffer->timer), 30);
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{
	if(NULL == S_LunchPageBuffer)
	{
		S_LunchPageBuffer = MyMalloc(sizeof(LunchPageBuffer));
		if(S_LunchPageBuffer)
		{
			memset(S_LunchPageBuffer, 0, sizeof(LunchPageBuffer));
			return My_Pass;
		}
	}
	
	return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_LunchPageBuffer);
	S_LunchPageBuffer = NULL;
	return My_Pass;
}



