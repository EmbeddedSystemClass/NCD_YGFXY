/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"LunchPage.h"

#include	"LCD_Driver.h"
#include	"Define.h"
#include	"MyMem.h"

#include	"RecordPage.h"
#include	"SystemSetPage.h"
#include	"SelectUserPage.h"
#include	"PaiDuiPage.h"
#include	"SleepPage.h"
#include	"PlaySong_Task.h"

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
static void PageUpDate(void);
static MyState_TypeDef PageInit(void *  parm);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);

static void DspPageText(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspLunchPage(void *  parm)
{
	PageInfo * currentpage = NULL;
	
	if(My_Pass == GetCurrentPage(&currentpage))
	{
		currentpage->PageInit = PageInit;
		currentpage->PageUpDate = PageUpDate;
		currentpage->LCDInput = Input;
		currentpage->PageBufferMalloc = PageBufferMalloc;
		currentpage->PageBufferFree = PageBufferFree;
		currentpage->tempP = &S_LunchPageBuffer;
		
		currentpage->PageInit(currentpage->pram);
	}
	
	return 0;
}


static void Input(unsigned char *pbuf , unsigned short len)
{
	if(S_LunchPageBuffer)
	{
		/*����*/
		S_LunchPageBuffer->lcdinput[0] = pbuf[4];
		S_LunchPageBuffer->lcdinput[0] = (S_LunchPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//
		if(S_LunchPageBuffer->lcdinput[0] == 0x1205)
		{
			PageBufferFree();
			PageAdvanceTo(DspSystemSetPage, NULL);
		}
		//�鿴����
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1206)
		{	
			PageBufferFree();
			PageAdvanceTo(DspRecordPage, NULL);
		}
		//�������
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1200)
		{	
			S_LunchPageBuffer->error = CreateANewTest(NormalTestType);
			//�����ɹ�
			if(Error_OK == S_LunchPageBuffer->error)
			{
				PageBufferFree();
				
				PageAdvanceTo(DspSelectUserPage, NULL);
			}
			//��ֹ�������
			else if(Error_StopNormalTest == S_LunchPageBuffer->error)
			{
				SendKeyCode(2);
				AddNumOfSongToList(45, 0);
			}
			//����ʧ��
			else if(Error_Mem == S_LunchPageBuffer->error)
			{
				SendKeyCode(3);
				AddNumOfSongToList(41, 0);
			}
		}
		//��������
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1201)
		{
			//�п��Ŷӣ�������Ŷӽ���
			if(true == IsPaiDuiTestting())
			{
				PageBufferFree();
				PageAdvanceTo(DspPaiDuiPage, NULL);
			}
			//�޿��Ŷ���ʼ����
			else
			{
				S_LunchPageBuffer->error = CreateANewTest(PaiDuiTestType);
				//�����ɹ�
				if(Error_OK == S_LunchPageBuffer->error)
				{
					PageBufferFree();
					PageAdvanceTo(DspSelectUserPage, NULL);
				}
				//�Ŷ�λ������������
				else if(Error_PaiDuiBusy == S_LunchPageBuffer->error)
				{
					SendKeyCode(1);
					AddNumOfSongToList(40, 0);
				}
				//����ʧ��
				else if(Error_Mem == S_LunchPageBuffer->error)
				{
					SendKeyCode(3);
					AddNumOfSongToList(41, 0);
				}
				//�п���������
				else if(Error_PaiDuiBusy == S_LunchPageBuffer->error)
				{
					SendKeyCode(1);
					AddNumOfSongToList(61, 0);
				}
			}
		}
	}
}

static void PageUpDate(void)
{
	if(TimeOut == timer_expired(&(S_LunchPageBuffer->timer)))
	{
		PageBufferFree();
		PageAdvanceTo(DspSleepPage, NULL);
	}
}

static MyState_TypeDef PageInit(void *  parm)
{	
	if(My_Pass == PageBufferMalloc())
	{
		timer_set(&(S_LunchPageBuffer->timer), 30);
	
		DspPageText();
	}
	
	SelectPage(82);
	
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

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void DspPageText(void)
{
	if(S_LunchPageBuffer)
	{
		memset(S_LunchPageBuffer->buf, 0, 100);
		sprintf(S_LunchPageBuffer->buf, "V %d.%d.%02d", GB_SoftVersion_1, GB_SoftVersion_2, GB_SoftVersion_3);
		DisText(0x1b10, S_LunchPageBuffer->buf, strlen(S_LunchPageBuffer->buf));
	}
}

