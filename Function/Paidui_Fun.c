/***************************************************************************************************
*FileName: Paidui_Fun
*Description: �Ŷӹ���
*Author: xsx_kair
*Data: 2016��12��13��11:47:20
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Paidui_Fun.h"
#include	"OutModel_Fun.h"

#include	"CardLimit_Driver.h"

#include	"PlaySong_Task.h"

#include	"PaiDuiPage.h"
#include	"PreReadCardPage.h"

#include	"CardStatues_Data.h"
#include	"UI_Data.h"
#include	"MyTest_Data.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: PaiDuiHandler
*Description: �Ŷ����̿���
*Input: 
*Output: 
*Return: 
*Author: xsx
*Date: 2016��12��13��11:58:58
***************************************************************************************************/
void PaiDuiHandler(void)
{
	unsigned char index = 0;
	ItemData * temp = NULL;
	unsigned short tempvalue = 0;
	PageInfo * currentpage = NULL;
	
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
				AddNumOfSongToList(index+27, 0);
			}
			//�ȴ��γ�����
			if(temp->statues == statues1)
			{
				if(!CardPinIn)
					temp->statues = statues2;

				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+27, 0);
				}
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
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+35, 0);
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
					
					//�����ǰ�����Ĳ����Լ�����˵���Լ�ֻ�Ǳ��쳣�γ���ֻ����뼴�ɣ������ĵ�ǰ��������
					if(temp == GetCurrentTestItem())					
						SetCurrentTestItem(NULL);
				}
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+27, 0);
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
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+35, 0);
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
					AddNumOfSongToList(index+27, 0);
					timer_restart(&(temp->timer3));
				}
			}

			//������ڵ���ʱ
			if((timerIsStartted(&(temp->timer))) && (false == timerIsStartted(&(temp->timer2))))
			{
				tempvalue = timer_surplus(&(temp->timer));
				
				if((0 == tempvalue)&&(temp->statues != statues7))
				{
					UpOneModelData(index, R_ON_G_OFF, 0);
					timer_restart(&(temp->timer2));				//������ʱ��ʱ��
					AddNumOfSongToList(index+11, 0);
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
						AddNumOfSongToList(index+35, 0);
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
							AddNumOfSongToList(index+35, 0);
						}
					}
				}
				else if(tempvalue <= 60)
				{
					//������ڵ���ʱ�����л����浽�Ŷӽ���
					if((temp->statues == statues5) && (GetCurrentTestItem() == NULL))
					{
						if(My_Pass == GetCurrentPage(&currentpage))
						{
							if(currentpage->CurrentPage != DspPaiDuiPage)
							{
								PageResetToOrigin(NoDisplayPage);
								PageAdvanceTo(DspPaiDuiPage, NULL);
							}
						}
					}
				}
			}
			
			//������ڳ�ʱ��ʱ
			if(timerIsStartted(&(temp->timer2)))
			{
				//����п�����ʱС��40S�����жϴ˳�ʱ���Ĳ���
				if(GetMinWaitTime() < 40)
				{
					if(temp->statues == statues3)
					{
						SetCurrentTestItem(NULL);
						UpOneModelData(index, R_ON_G_OFF, 0);
						temp->statues = statues6;
					}
				}
				
				/*�����ʱʱ����������*/
				if((temp->statues == statues6) && (GetMinWaitTime() > 60) && (GetCurrentTestItem() == NULL))
				{
					SetCurrentTestItem(temp);
					UpOneModelData(index, R_ON_G_OFF, 5);
					temp->statues = statues3;
					timer_restart(&(temp->timer3));
					AddNumOfSongToList(index+35, 0);
				}
				
			}
		}
		
	}
}

/****************************************end of file************************************************/
