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
#include	"OutModel_Fun.h"
#include	"Motor_Fun.h"
#include	"CardStatues_Data.h"
#include	"PlaySong_Task.h"
#include	"CardLimit_Driver.h"

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
static void UpData(void);

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
		currentpage->tempP = &S_PaiDuiPageBuffer;
		
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
		if(S_PaiDuiPageBuffer->lcdinput[0] == 0x2600)
		{
			PageBufferFree();
			PageBackTo(ParentPage);
		}
		/*�ȴ��忨*/
		else if(S_PaiDuiPageBuffer->lcdinput[0] == 0x00a1)
		{
			
		}
	}
}

static void PageUpDate(void)
{
	static unsigned short count = 0;
	
	if(count % 50 == 0)
		UpData();
	
	count++;
}

static MyState_TypeDef PageInit(void *pram)
{
	if(My_Fail == PageBufferMalloc())
		return My_Fail;

	SelectPage(93);
	
	return My_Pass;
}

static MyState_TypeDef PageBufferMalloc(void)
{	
	S_PaiDuiPageBuffer = (PaiDuiPageBuffer *)MyMalloc(sizeof(PaiDuiPageBuffer));
			
	if(S_PaiDuiPageBuffer)
	{
		memset(S_PaiDuiPageBuffer, 0, sizeof(PaiDuiPageBuffer));
		
		return My_Pass;
	}
	else
		return My_Fail;
}

static MyState_TypeDef PageBufferFree(void)
{
	MyFree(S_PaiDuiPageBuffer);
	S_PaiDuiPageBuffer = NULL;
	
	return My_Pass;
}


static void UpData(void)
{
	unsigned char index = 0;
	static unsigned char count = 0;
	
	/*���µ���ʱ����*/
	for(index=0; index<PaiDuiWeiNum; index++)
	{
		S_PaiDuiPageBuffer->tempd2 = GetTestItemByIndex(index);
		
		if(S_PaiDuiPageBuffer->tempd2)
		{
			//��ʱ
			if(timerIsStartted(&(S_PaiDuiPageBuffer->tempd2->timer2)))
			{
				S_PaiDuiPageBuffer->tempvalue1 = timer_Count(&(S_PaiDuiPageBuffer->tempd2->timer2));
				sprintf(S_PaiDuiPageBuffer->buf, "%dm%ds", S_PaiDuiPageBuffer->tempvalue1/60, S_PaiDuiPageBuffer->tempvalue1%60);
				DisText(0x17a0+index*5, S_PaiDuiPageBuffer->buf, 8);
				
				memset(S_PaiDuiPageBuffer->buf, 0, 8);
				DisText(0x1710+index*0x10, S_PaiDuiPageBuffer->buf, 8);
					
				BasicPic(0x1610+index*16, 0, 100, 15, 0, 22, 570, 55 , 0);
			}
			else
			{
				S_PaiDuiPageBuffer->tempvalue1 = timer_surplus(&(S_PaiDuiPageBuffer->tempd2->timer));
				sprintf(S_PaiDuiPageBuffer->buf, "%dm%ds", S_PaiDuiPageBuffer->tempvalue1/60, S_PaiDuiPageBuffer->tempvalue1%60);
				DisText(0x1710+index*0x10, S_PaiDuiPageBuffer->buf, 8);
				
				memset(S_PaiDuiPageBuffer->buf, 0, 8);
				DisText(0x17a0+index*0x05, S_PaiDuiPageBuffer->buf, 8);
				
				S_PaiDuiPageBuffer->tempvalue = S_PaiDuiPageBuffer->tempvalue1;
				S_PaiDuiPageBuffer->tempvalue /= S_PaiDuiPageBuffer->tempd2->testdata.temperweima.CardWaitTime*60;
				S_PaiDuiPageBuffer->tempvalue *= 255;
				S_PaiDuiPageBuffer->tempvalue = 577-S_PaiDuiPageBuffer->tempvalue;
				BasicPic(0x1610+index*16, 1, 128, 17, (unsigned short)(S_PaiDuiPageBuffer->tempvalue), 22, 577, 59 + index*119, (unsigned short)(S_PaiDuiPageBuffer->tempvalue)-187);
			}
				
			if((S_PaiDuiPageBuffer->tempd2->statues != statues5) && (S_PaiDuiPageBuffer->tempd2->statues != statues6))
			{
				//��⿨ͼ����˸
				if((count % 2) == 0)
					S_PaiDuiPageBuffer->myico.ICO_ID = 23;
				else
					S_PaiDuiPageBuffer->myico.ICO_ID = 24;
					
				S_PaiDuiPageBuffer->myico.X = 69+(index*119);
				S_PaiDuiPageBuffer->myico.Y = 135;
					
				BasicUI(0x1690+index*0x10 ,0x1907 , 1, &(S_PaiDuiPageBuffer->myico) , sizeof(Basic_ICO));
			}

		}
		else
		{
			//�������ʱʱ��
			ClearText(0x1710+index*0x10, 8);
			ClearText(0x17a0+index*0x05, 8);
			//��ʾ��ɫ��
			S_PaiDuiPageBuffer->myico.ICO_ID = 23;
			S_PaiDuiPageBuffer->myico.X = 69+index*119;
			S_PaiDuiPageBuffer->myico.Y = 135;
			BasicUI(0x1690+index*0x10 ,0x1907 , 1, &(S_PaiDuiPageBuffer->myico) , sizeof(Basic_ICO));
			BasicPic(0x1610+index*0x10, 0, 128, 15, 0, 22, 570, 55 , 0);
		}
	}

	count++;
}


void CheckTime(void)
{
	unsigned char index = 0;
	ItemData * temp = NULL;
	unsigned short tempvalue = 0;
	
	for(index=0; index<PaiDuiWeiNum; index++)
	{
		temp = GetTestItemByIndex(index);
		
		if(temp)
		{
			//�����Ŷ�ģʽ
			if(temp->statues == startpaidui)
			{
				temp->statues = statues1;
				
				UpOneModelData(index, R_ON_G_OFF, 5);
				//20S��ʾһ�ν��������Ŷ�λ
				timer_set(&(temp->timer3), 10);
				AddNumOfSongToList(index+19, 0);
			}
			//�ȴ��γ�����
			if(temp->statues == statues1)
			{
				if(!CardPinIn)
					temp->statues = statues2;
			}
			//�ȴ����뿨��
			else if(temp->statues == statues4)
			{
				if(CardPinIn)
				{
					UpOneModelData(index, R_OFF_G_ON, 0);
					temp->statues = statues7;
					PageAdvanceTo(DspPreReadCardPage, NULL);
				}
			}
			//�ȴ������Ŷ�λ
			else if(temp->statues == statues2)
			{
				//��������Ŷ�λ���л�����ʱ���߳�ʱ״̬
				if(KEY_Pressed == GetKeyStatues(index))
				{
					//��ʱ��ʱ����룬������ʱ��ʱ
					if(timerIsStartted(&(temp->timer2)))
						temp->statues = statues6;
					else
						temp->statues = statues5;
						
					UpOneModelData(index, R_ON_G_OFF, 0);
						
					SetCurrentTestItem(NULL);
				}
			}
			//�ȴ��γ��Ŷ�λ
			else if(temp->statues == statues3)
			{
				//����γ��Ŷ�λ
				if(KEY_NoPressed == GetKeyStatues(index))
				{
					UpOneModelData(index, R_OFF_G_ON, 0);
					temp->statues = statues4;
				}
			}
			//����ʱ�����п��γ�
			else if((temp->statues == statues5) || (temp->statues == statues6))
			{
				//����γ��Ŷ�λ
				if(KEY_NoPressed == GetKeyStatues(index))
				{
					temp->statues = statues2;
					UpOneModelData(index, R_ON_G_OFF, 5);
					AddNumOfSongToList(index+19, 0);
				}
			}

			//������ڵ���ʱ
			if((timerIsStartted(&(temp->timer))) && (false == timerIsStartted(&(temp->timer2))))
			{
				tempvalue = timer_surplus(&(temp->timer));
				
				if(0 == tempvalue)
				{
					timer_restart(&(temp->timer2));
					AddNumOfSongToList(index+12, 0);
				}
				else if(tempvalue <= 30)
				{
					//�Ŷ����
					if(temp->statues == statues1)
					{
						UpOneModelData(index, R_OFF_G_ON, 0);
						temp->statues = statues7;
						PageAdvanceTo(DspPreReadCardPage, NULL);
					}
					//���뿨�ۣ������Ŷ�
					else if(temp->statues == statues2)
					{
						UpOneModelData(index, R_OFF_G_ON, 0);
						temp->statues = statues4;
						timer_restart(&(temp->timer3));
						AddNumOfSongToList(index+44, 0);
					}
					else if(temp->statues == statues3)
					{
						if(TimeOut == timer_expired(&(temp->timer3)))
						{
							timer_restart(&(temp->timer3));
							
							AddNumOfSongToList(index+36, 0);
						}
					}
					else if(temp->statues == statues4)
					{
						if(TimeOut == timer_expired(&(temp->timer3)))
						{
							timer_restart(&(temp->timer3));
							
							AddNumOfSongToList(index+44, 0);
						}
					}
					//������ڵ���ʱ������ʾ�γ��Ŷ�λ��
					else if(temp->statues == statues5)
					{
						//�������
						if(GetCurrentTestItem() == NULL)
						{
							SetCurrentTestItem(temp);
							UpOneModelData(index, R_ON_G_OFF, 5);
							temp->statues = statues3;
							timer_restart(&(temp->timer3));
							AddNumOfSongToList(index+54, 0);
						}
					}
				}
				else
				{
					//�Ŷ����
					if((temp->statues == statues1) || (temp->statues == statues2))
					{						
						if(TimeOut == timer_expired(&(temp->timer3)))
						{
							timer_restart(&(temp->timer3));
							
							AddNumOfSongToList(index+28, 0);
						}
					}
				}
			}
			
			//������ڳ�ʱ��ʱ
			if(timerIsStartted(&(temp->timer2)))
			{
				/*�����ʱʱ����������*/
				if((GetMinWaitTime() > 60)&&(GetCurrentTestItem() == NULL))
				{
					SetCurrentTestItem(temp);
					UpOneModelData(index, R_ON_G_OFF, 5);
					temp->statues = statues3;
					timer_restart(&(temp->timer3));
					AddNumOfSongToList(index+54, 0);
				}
				
			}
		}
		
	}
}


