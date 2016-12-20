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
static void activityStart(void);
static void activityInput(unsigned char *pbuf , unsigned short len);
static void activityFresh(void);
static void activityHide(void);
static void activityResume(void);
static void activityDestroy(void);

static MyState_TypeDef activityBufferMalloc(void);
static void activityBufferFree(void);

static void DspPageText(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

MyState_TypeDef createLunchActivity(Activity * thizActivity, void * pram)
{
	if(NULL == thizActivity)
		return My_Fail;
	
	if(My_Pass == activityBufferMalloc())
	{
		InitActivity(thizActivity, "LunchActivity", activityStart, activityInput, activityFresh, activityHide, activityResume, activityDestroy);
		
		return My_Pass;
	}
	
	return My_Fail;
}

static void activityStart(void)
{
	if(S_LunchPageBuffer)
	{
		timer_set(&(S_LunchPageBuffer->timer), 10);
	
		DspPageText();
	}
	
	SelectPage(82);

}
static void activityInput(unsigned char *pbuf , unsigned short len)
{
	if(S_LunchPageBuffer)
	{
		/*����*/
		S_LunchPageBuffer->lcdinput[0] = pbuf[4];
		S_LunchPageBuffer->lcdinput[0] = (S_LunchPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		//����
		if(S_LunchPageBuffer->lcdinput[0] == 0x1103)
		{

		}
		//�鿴����
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1102)
		{	

		}
		//�������
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1100)
		{	
			S_LunchPageBuffer->error = CreateANewTest(NormalTestType);
			//�����ɹ�
			if(Error_OK == S_LunchPageBuffer->error)
			{
				//PageBufferFree();
				
				//PageAdvanceTo(DspSelectUserPage, NULL);
			}
			//��ֹ�������
			else if(Error_StopNormalTest == S_LunchPageBuffer->error)
			{
				SendKeyCode(1);
				AddNumOfSongToList(45, 0);
			}
			//����ʧ��
			else if(Error_Mem == S_LunchPageBuffer->error)
			{
				SendKeyCode(2);
				AddNumOfSongToList(41, 0);
			}
		}
		//��������
		else if(S_LunchPageBuffer->lcdinput[0] == 0x1101)
		{
			//�п��Ŷӣ�������Ŷӽ���
			if(true == IsPaiDuiTestting())
			{
				//PageBufferFree();
				//PageAdvanceTo(DspPaiDuiPage, NULL);
			}
			//�޿��Ŷ���ʼ����
			else
			{
				S_LunchPageBuffer->error = CreateANewTest(PaiDuiTestType);
				//�����ɹ�
				if(Error_OK == S_LunchPageBuffer->error)
				{
					//PageBufferFree();
					//PageAdvanceTo(DspSelectUserPage, NULL);
				}
				//����ʧ��
				else if(Error_Mem == S_LunchPageBuffer->error)
				{
					SendKeyCode(2);
					AddNumOfSongToList(41, 0);
				}
			}
		}
	}
}
static void activityFresh(void)
{
	if( S_LunchPageBuffer && (TimeOut == timer_expired(&(S_LunchPageBuffer->timer))))
	{
		startActivity(createSleepActivity, NULL);
	}
}
static void activityHide(void)
{

}
static void activityResume(void)
{
	if(S_LunchPageBuffer)
	{
		timer_set(&(S_LunchPageBuffer->timer), 10);
	}
	
	SelectPage(82);
}
static void activityDestroy(void)
{
	activityBufferFree();
}

static MyState_TypeDef activityBufferMalloc(void)
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

static void activityBufferFree(void)
{
	MyFree(S_LunchPageBuffer);
	S_LunchPageBuffer = NULL;
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
		DisText(0x1110, S_LunchPageBuffer->buf, 30);
	}
}

