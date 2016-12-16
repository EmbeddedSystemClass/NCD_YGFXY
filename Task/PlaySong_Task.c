/******************************************************************************************
*�ļ�����COM_Task.c
*�������������Ź�����
*˵�������ȼ�1����ͣ���1Sι��һ�Σ�
*���ߣ�xsx
******************************************************************************************/

#include	"PlaySong_Task.h"
#include	"PlaySong_Function.h"
#include	"QueueUnits.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

#include	"stdio.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
const char wavfilename[63][20]=
{
	"0:/Audio/ncd_1.wav\0",
	"0:/Audio/ncd_2.wav\0",
	"0:/Audio/ncd_3.wav\0",
	"0:/Audio/ncd_4.wav\0",
	"0:/Audio/ncd_5.wav\0",
	"0:/Audio/ncd_6.wav\0",
	"0:/Audio/ncd_7.wav\0",
	"0:/Audio/ncd_8.wav\0",
	"0:/Audio/ncd_9.wav\0",
	"0:/Audio/ncd_10.wav\0",
	"0:/Audio/ncd_11.wav\0",
	"0:/Audio/ncd_12.wav\0",
	"0:/Audio/ncd_13.wav\0",
	"0:/Audio/ncd_14.wav\0",
	"0:/Audio/ncd_15.wav\0",
	"0:/Audio/ncd_16.wav\0",
	"0:/Audio/ncd_17.wav\0",
	"0:/Audio/ncd_18.wav\0",
	"0:/Audio/ncd_19.wav\0",
	"0:/Audio/ncd_20.wav\0",
	"0:/Audio/ncd_21.wav\0",
	"0:/Audio/ncd_22.wav\0",
	"0:/Audio/ncd_23.wav\0",
	"0:/Audio/ncd_24.wav\0",
	"0:/Audio/ncd_25.wav\0",
	"0:/Audio/ncd_26.wav\0",
	"0:/Audio/ncd_27.wav\0",
	"0:/Audio/ncd_28.wav\0",
	"0:/Audio/ncd_29.wav\0",
	"0:/Audio/ncd_30.wav\0",
	"0:/Audio/ncd_31.wav\0",
	"0:/Audio/ncd_32.wav\0",
	"0:/Audio/ncd_33.wav\0",
	"0:/Audio/ncd_34.wav\0",
	"0:/Audio/ncd_35.wav\0",
	"0:/Audio/ncd_36.wav\0",
	"0:/Audio/ncd_37.wav\0",
	"0:/Audio/ncd_38.wav\0",
	"0:/Audio/ncd_39.wav\0",
	"0:/Audio/ncd_40.wav\0",
	"0:/Audio/ncd_41.wav\0",
	"0:/Audio/ncd_42.wav\0",
	"0:/Audio/ncd_43.wav\0",
	"0:/Audio/ncd_44.wav\0",
	"0:/Audio/ncd_45.wav\0",
	"0:/Audio/ncd_46.wav\0",
	"0:/Audio/ncd_47.wav\0",
	"0:/Audio/ncd_48.wav\0",
	"0:/Audio/ncd_49.wav\0",
	"0:/Audio/ncd_50.wav\0",
	"0:/Audio/ncd_51.wav\0",
	"0:/Audio/ncd_52.wav\0",
	"0:/Audio/ncd_53.wav\0",
	"0:/Audio/ncd_54.wav\0",
	"0:/Audio/ncd_55.wav\0",
	"0:/Audio/ncd_56.wav\0",
	"0:/Audio/ncd_57.wav\0",
	"0:/Audio/ncd_58.wav\0",
	"0:/Audio/ncd_59.wav\0",
	"0:/Audio/ncd_60.wav\0",
	"0:/Audio/ncd_61.wav\0",
	"0:/Audio/ncd_62.wav\0",
	"0:/Audio/ncd_63.wav\0",
};
#define vPlaySongTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )

#define	SongListSize	20							//�����б�ɱ���100����Ƶ
static xQueueHandle SongListQueue = NULL;			//������Ƶ�Ķ���
static xQueueHandle AudioStatusQueue = NULL;		//������Ƶ��״̬
static unsigned char S_PlayStatus = 0;
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void vPlaySongTask( void *pvParameters );

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** ������:StartvIwdgTask
** ��  ��:����������ι������
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��: 2015��5��15�� 17:04:43 
** ��  �ߣ�xsx                                                 
************************************************************************/
void StartvPlaySongTask(void)
{
	if(SongListQueue == NULL)
		SongListQueue = xQueueCreate( SongListSize, ( unsigned portBASE_TYPE ) sizeof( unsigned char ) );
	
	if(AudioStatusQueue == NULL)
		AudioStatusQueue = xQueueCreate( 10, ( unsigned portBASE_TYPE ) sizeof( unsigned char ) );
	
	xTaskCreate( vPlaySongTask, ( const char * ) "vPlaySongTask ", configMINIMAL_STACK_SIZE*2, NULL, vPlaySongTask_PRIORITY, NULL );
}

/************************************************************************
** ������:vIwdgTask
** ��  ��:ι������
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��: 2015��5��15�� 17:04:12  
** ��  �ߣ�xsx                                                 
************************************************************************/
static void vPlaySongTask( void *pvParameters )
{
	unsigned char tempdata;
	while(1)
	{
		if(pdPASS == xQueueReceive( SongListQueue, &tempdata, portMAX_DELAY ))
		{
			//������ʼ�����ź�
			S_PlayStatus = 1;
			SendDataToQueue(AudioStatusQueue, NULL, &S_PlayStatus , 1 , 1, 10 / portTICK_RATE_MS, NULL);
			
			AudioPlay(wavfilename[tempdata]);
			
			//����ֹͣ�����ź�
			S_PlayStatus = 0;
			SendDataToQueue(AudioStatusQueue, NULL, &S_PlayStatus , 1 , 1, 10 / portTICK_RATE_MS, NULL);
		}
	}
}

/***************************************************************************************************
*FunctionName��AddNumOfSongToList
*Description����Ӹ����������б������Ƶģ����У�����������
*Input��num -- ������ţ���������ļ��������Ӧ
*Output��������ӳɹ���ʧ��
*Author��xsx
*Data��2015��10��13��09:06:23
***************************************************************************************************/
unsigned char AddNumOfSongToList(unsigned char num, unsigned char mode)
{
	unsigned char songnum = num;
	
	if(SongListQueue == NULL)
		return pdFAIL;

	/*ģʽ1�� ��ֹ���ڲ��ţ���������Ƶ*/
	if(mode == 0)
	{
		StopMyPlay();
	}
	/*ģʽ2��������ڲ��ţ���ȡ������Ƶ*/
	else if(mode == 1)
	{
		return pdPASS;
	}
	/*ģʽ3������ʲô״̬�����������Ƶ���ȴ�����*/
	else if(mode == 2)
	{

	}
	
	if( xQueueSend( SongListQueue, &songnum, 0 ) != pdPASS )
	{
		return pdFAIL;
	}

	return pdPASS;
}

MyState_TypeDef TakeAudioPlayStatus(unsigned char *status)
{
	if(pdPASS == xQueueReceive( AudioStatusQueue, status,  10/portTICK_RATE_MS))
		return My_Pass;
	else
		return My_Fail;
}
