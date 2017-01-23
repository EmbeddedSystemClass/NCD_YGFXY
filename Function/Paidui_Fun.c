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
#include	"WaittingCardPage.h"
#include	"Motor_Data.h"
#include	"CardStatues_Data.h"
#include	"UI_Data.h"
#include	"MyTest_Data.h"
#include	"MyTools.h"

#include	<string.h>
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
	
	for(index=0; index<PaiDuiWeiNum; index++)
	{
		temp = GetTestItemByIndex(index);
		
		if((temp) && (temp->statues != statuesNull))
		{
			//�����Ŷ�ģʽ
			if(temp->statues == startpaidui)
			{
				if(MaxLocation == GetGB_MotorLocation())
				{
					temp->statues = statues1;
				
					UpOneModelData(index, R_ON_G_OFF, 5);
					//20S��ʾһ�ν��������Ŷ�λ
					timer_set(&(temp->timer3), 10);
					AddNumOfSongToList(index+22, 0);
				}
			}
			//�ȴ��γ�����
			if(temp->statues == statues1)
			{
				//����ӿ��۰γ����򽫵�ǰ��������Ϊ�գ����Խ�����������
				if(GetCardState() == NoCard)
				{
					temp->statues = statues2;
					SetCurrentTestItem(NULL);
				}
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+22, 0);
				}
			}
			//�ȴ������Ŷ�λ
			else if(temp->statues == statues2)
			{
				//��������Ŷ�λ���л�����ʱ���߳�ʱ״̬
				if(KEY_Pressed == GetKeyStatues(index))
				{
					//ֹͣ��������
					stopPlay();
					
					//��ʱ��ʱ����룬������ʱ��ʱ
					if(timerIsStartted(&(temp->timer2)))
						temp->statues = statues6;
					else
						temp->statues = statues5;
						
					UpOneModelData(index, R_ON_G_OFF, 0);
				}
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+22, 0);
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
							
					AddNumOfSongToList(index+30, 0);
				}
			}
			//�ȴ����뿨��
			else if(temp->statues == statues4)
			{
				if(GetCardState() == CardIN)
				{
					//ֹͣ��������
					stopPlay();
					
					UpOneModelData(index, R_OFF_G_ON, 0);
					temp->statues = statues7;
					
					startActivity(createWaittingCardActivity, NULL);
				}
				else if(TimeOut == timer_expired(&(temp->timer3)))
				{
					timer_restart(&(temp->timer3));
							
					AddNumOfSongToList(index+30, 5);
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
					AddNumOfSongToList(index+22, 0);
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
					AddNumOfSongToList(index+38, 0);
					
					if(temp->statues == statues5)
						temp->statues = statues6;
				}
				else if(tempvalue <= 30)
				{
					//�Ŷ����
					if(temp->statues == statues1)
					{
						UpOneModelData(index, R_OFF_G_ON, 0);
						temp->statues = statues7;

						startActivity(createWaittingCardActivity, NULL);
					}
					//�ȴ������Ŷ�λ���������ʱ��С��30�룬��תΪ״̬4����ʾ�������λ���ԣ����ڲ����Ŷ�λ
					else if(temp->statues == statues2)
					{
						if(NULL == GetCurrentTestItem())
						{
							UpOneModelData(index, R_OFF_G_ON, 0);
							temp->statues = statues4;
							timer_restart(&(temp->timer3));
							AddNumOfSongToList(index+30, 0);
						}
					}
					//������ڵ���ʱ������ʾ�γ��Ŷ�λ��
					else if(temp->statues == statues5)
					{
						//�������
						if(GetCurrentTestItem() == NULL)
						{
							//��������п�����ʾ���
							if(GetCardState() == CardIN)
							{
								if(TimeOut == timer_expired(&(temp->timer3)))
								{
									timer_restart(&(temp->timer3));
											
									AddNumOfSongToList(46, 0);					//��ʾ��տ���
								}
							}
							else
							{
								SetCurrentTestItem(temp);
								UpOneModelData(index, R_ON_G_OFF, 5);
								temp->statues = statues3;
								timer_restart(&(temp->timer3));
								AddNumOfSongToList(index+30, 0);
							}
							
						}
					}
				}
				else if(tempvalue <= 60)
				{
					//������ڵ���ʱ�����л����浽�Ŷӽ���
					if((temp->statues == statues5) && (GetCurrentTestItem() == NULL))
					{
						if(false == CheckStrIsSame(paiduiActivityName, getCurrentActivityName(), strlen(paiduiActivityName)))
						{
							backToActivity(lunchActivityName);
							startActivity(createPaiDuiActivity, NULL);
						}
					}
				}
			}
			
			//������ڳ�ʱ��ʱ
			if(timerIsStartted(&(temp->timer2)))
			{
				//����п�����ʱС��60S�����жϴ˳�ʱ���Ĳ���
				if(GetMinWaitTime() < 60)
				{
					if(temp->statues == statues3)
					{
						SetCurrentTestItem(NULL);
						UpOneModelData(index, R_ON_G_OFF, 0);
						temp->statues = statues6;
					}
				}
				else
				{
					if(temp->statues == statues2)
					{
						if(NULL == GetCurrentTestItem())
						{
							UpOneModelData(index, R_OFF_G_ON, 0);
							temp->statues = statues4;
							timer_restart(&(temp->timer3));
							AddNumOfSongToList(index+30, 0);
						}
					}
					if(temp->statues == statues6)
					{
						if(NULL == GetCurrentTestItem())
						{
							//��������п�����ʾ���
							if(GetCardState() == CardIN)
							{
								if(TimeOut == timer_expired(&(temp->timer3)))
								{
									timer_restart(&(temp->timer3));
											
									AddNumOfSongToList(46, 0);					//��ʾ��տ���
								}
							}
							else
							{
								SetCurrentTestItem(temp);
								UpOneModelData(index, R_ON_G_OFF, 5);
								temp->statues = statues3;
								timer_restart(&(temp->timer3));
								AddNumOfSongToList(index+30, 0);
							}
						}
					}
				}
			}
		}
		
	}
}

/****************************************end of file************************************************/
