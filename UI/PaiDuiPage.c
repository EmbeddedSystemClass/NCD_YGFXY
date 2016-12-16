/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"PaiDuiPage.h"
#include	"Define.h"
#include	"LCD_Driver.h"
#include	"UI_Data.h"
#include	"SystemSetPage.h"
#include	"MyMem.h"
#include	"TimeDownNorPage.h"
#include	"PreReadCardPage.h"
#include	"TM1623_Driver.h"

#include	"Motor_Fun.h"
#include	"CardStatues_Data.h"
#include	"PlaySong_Task.h"
#include	"CardLimit_Driver.h"
#include	"SampleIDPage.h"

#include	"MyTest_Data.h"
#include	"LunchPage.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	<string.h>
#include	"stdio.h"

/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static PaiDuiPageBuffer * S_PaiDuiPageBuffer = NULL;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static void Input(unsigned char *pbuf , unsigned short len);
static void PageUpDate(void);

static MyState_TypeDef PageInit(void *pram);
static MyState_TypeDef PageBufferMalloc(void);
static MyState_TypeDef PageBufferFree(void);
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

unsigned char DspPaiDuiPage(void *  parm)
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
	if(S_PaiDuiPageBuffer)
	{
		/*����*/
		S_PaiDuiPageBuffer->lcdinput[0] = pbuf[4];
		S_PaiDuiPageBuffer->lcdinput[0] = (S_PaiDuiPageBuffer->lcdinput[0]<<8) + pbuf[5];
		
		/*����*/
		if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1600)
		{
			//�����У���������
			if(NULL != GetCurrentTestItem())
			{
				SendKeyCode(4);
			}
			//�������ԣ���������
			else if(GetMinWaitTime() < 60)
			{
				SendKeyCode(3);
				AddNumOfSongToList(61, 0);
			}
			else
			{
				//ҳ������ˢ�����ݣ�æ
				while(S_PaiDuiPageBuffer->pageisbusy)
				{
					vTaskDelay(100 / portTICK_RATE_MS);
				}
				
				PageBufferFree();
				PageResetToOrigin(DisplayPage);
			}
		}
		//��������
		else if(S_PaiDuiPageBuffer->lcdinput[0] == 0x1601)
		{
			S_PaiDuiPageBuffer->error = CreateANewTest(PaiDuiTestType);
			//�����ɹ�
			if(Error_OK == S_PaiDuiPageBuffer->error)
			{
				PageBufferFree();
				PageAdvanceTo(DspSampleIDPage, NULL);
			}
			//�Ŷ�λ������������
			else if(Error_PaiduiFull == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(2);
				AddNumOfSongToList(61, 0);
			}
			//����ʧ��
			else if(Error_Mem == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(1);
				AddNumOfSongToList(41, 0);
			}
			//�п���������
			else if(Error_PaiDuiBusy == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(3);
				AddNumOfSongToList(61, 0);
			}
			//�����н�ֹ���
			else if(Error_PaiduiTesting == S_PaiDuiPageBuffer->error)
			{
				SendKeyCode(4);
				AddNumOfSongToList(61, 0);
			}
		}
	}
}

static void PageUpDate(void)
{
	unsigned char index = 0;
	
	if(S_PaiDuiPageBuffer)
	{
		S_PaiDuiPageBuffer->count2++;
		
		//����æ
		S_PaiDuiPageBuffer->pageisbusy = true;

		
		//500msˢ��һ�ν���
		if(S_PaiDuiPageBuffer->count2 % 50 == 0)
		{
			//�忨
			if(CardPinIn)
			{
				//������У�˵������Ŀ������Ŷ��еĿ����򴴽����Ŷ�
				if(NULL == GetCurrentTestItem())
				{
					if(TimeOut == timer_expired(&(S_PaiDuiPageBuffer->timer0)))
					{
						S_PaiDuiPageBuffer->error = CreateANewTest(PaiDuiTestType);
						//�����ɹ�
						if(Error_OK == S_PaiDuiPageBuffer->error)
						{
							PageAdvanceTo(DspSampleIDPage, NULL);
								
							return;
						}
						//�Ŷ�λ������������
						else if(Error_PaiduiFull == S_PaiDuiPageBuffer->error)
						{
							SendKeyCode(2);
							AddNumOfSongToList(61, 0);
						}
						//����ʧ��
						else if(Error_Mem == S_PaiDuiPageBuffer->error)
						{
							SendKeyCode(1);
							AddNumOfSongToList(41, 0);
						}
						//�п���������
						else if(Error_PaiDuiBusy == S_PaiDuiPageBuffer->error)
						{
							SendKeyCode(3);
							AddNumOfSongToList(61, 0);
						}
						//�����н�ֹ���
						else if(Error_PaiduiTesting == S_PaiDuiPageBuffer->error)
						{
							SendKeyCode(4);
							AddNumOfSongToList(61, 0);
						}
						
						//��ʱ�����ó�ʱ�䣬�������ڶԴ˿���������Ҫ���°β����
						timer_set(&(S_PaiDuiPageBuffer->timer0), 10000);
					}
				}
				//����п��������Ŷ�æ����˵������Ŀ����ŶӵĿ���
				else
					timer_set(&(S_PaiDuiPageBuffer->timer0), 1);
			}
			//����޿��������ö�ʱ��
			else
				timer_set(&(S_PaiDuiPageBuffer->timer0), 1);
		
			//���µ���ʱ����
			for(index=0; index<PaiDuiWeiNum; index++)
			{
				S_PaiDuiPageBuffer->tempd2 = GetTestItemByIndex(index);
				
				if(S_PaiDuiPageBuffer->tempd2)
				{
					//��ʱ
					if(timerIsStartted(&(S_PaiDuiPageBuffer->tempd2->timer2)))
					{
						S_PaiDuiPageBuffer->tempvalue1 = timer_Count(&(S_PaiDuiPageBuffer->tempd2->timer2));
						sprintf(S_PaiDuiPageBuffer->buf, "%d:%d", S_PaiDuiPageBuffer->tempvalue1/60, S_PaiDuiPageBuffer->tempvalue1%60);
						DisText(0x1650+index*0x08, S_PaiDuiPageBuffer->buf, 10);
							
						memset(S_PaiDuiPageBuffer->buf, 0, 10);
						DisText(0x1610+index*0x08, S_PaiDuiPageBuffer->buf, 10);
								
						BasicPic(0x1510+index*16, 0, 100, 15, 0, 22, 570, 55 , 0);
					}
					else
					{
						S_PaiDuiPageBuffer->tempvalue1 = timer_surplus(&(S_PaiDuiPageBuffer->tempd2->timer));
						sprintf(S_PaiDuiPageBuffer->buf, "%d:%d", S_PaiDuiPageBuffer->tempvalue1/60, S_PaiDuiPageBuffer->tempvalue1%60);
						DisText(0x1610+index*0x08, S_PaiDuiPageBuffer->buf, 10);
							
						memset(S_PaiDuiPageBuffer->buf, 0, 10);
						DisText(0x1650+index*0x08, S_PaiDuiPageBuffer->buf, 10);
							
						S_PaiDuiPageBuffer->tempvalue = S_PaiDuiPageBuffer->tempvalue1;
						S_PaiDuiPageBuffer->tempvalue /= S_PaiDuiPageBuffer->tempd2->testdata.temperweima.CardWaitTime*60;
						S_PaiDuiPageBuffer->tempvalue *= 254;
						S_PaiDuiPageBuffer->tempvalue = 360-S_PaiDuiPageBuffer->tempvalue;
						BasicPic(0x1510+index*16, 1, 137, 142, (unsigned short)(S_PaiDuiPageBuffer->tempvalue), 147, 360, 59 + index*119, (unsigned short)(S_PaiDuiPageBuffer->tempvalue)+29);
					}
						
					if((S_PaiDuiPageBuffer->tempd2->statues == statues5) || (S_PaiDuiPageBuffer->tempd2->statues == statues6))
						S_PaiDuiPageBuffer->myico.ICO_ID = 24;

					else
					{
						//��⿨ͼ����˸
						if((S_PaiDuiPageBuffer->count % 2) == 0)
							S_PaiDuiPageBuffer->myico.ICO_ID = 23;
						else
							S_PaiDuiPageBuffer->myico.ICO_ID = 24;
					}
						
					S_PaiDuiPageBuffer->myico.X = 69+(index*119);
					S_PaiDuiPageBuffer->myico.Y = 135;
						
					BasicUI(0x1590+index*0x10 ,0x1907 , 1, &(S_PaiDuiPageBuffer->myico) , sizeof(Basic_ICO));
				}
				else
				{
					//�������ʱʱ��
					ClearText(0x1610+index*0x08, 10);
					ClearText(0x1650+index*0x08, 10);
					//��ʾ��ɫ��
					S_PaiDuiPageBuffer->myico.ICO_ID = 23;
					S_PaiDuiPageBuffer->myico.X = 69+index*119;
					S_PaiDuiPageBuffer->myico.Y = 135;
					BasicUI(0x1590+index*0x10 ,0x1907 , 1, &(S_PaiDuiPageBuffer->myico) , sizeof(Basic_ICO));
					BasicPic(0x1510+index*0x10, 0, 137, 15, 0, 22, 570, 55 , 0);
				}
			}
			
			S_PaiDuiPageBuffer->count++;
			if(S_PaiDuiPageBuffer->count > 65535)
				S_PaiDuiPageBuffer->count = 0;
		}
		
		//�������
		S_PaiDuiPageBuffer->pageisbusy = false;
	}
}

static MyState_TypeDef PageInit(void *pram)
{
	if(My_Pass == PageBufferMalloc())
		;
		
		SelectPage(93);
		
		//һ���ȡһ���Ƿ�忨
		timer_set(&(S_PaiDuiPageBuffer->timer0), 1);
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{		
	if(NULL == S_PaiDuiPageBuffer)
	{
		S_PaiDuiPageBuffer = MyMalloc(sizeof(PaiDuiPageBuffer));
		if(S_PaiDuiPageBuffer)	
		{
			memset(S_PaiDuiPageBuffer, 0, sizeof(PaiDuiPageBuffer));
	
			return My_Pass;
		}
	}

	return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_PaiDuiPageBuffer);
	S_PaiDuiPageBuffer = NULL;
	
	return My_Pass;
}



