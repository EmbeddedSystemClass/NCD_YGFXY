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
const char wavfilename[63][40]=
{
	"0:/Sound/1TimeSone.wav",
	"0:/Sound/2TimeSone.wav",
	"0:/Sound/3TimeSone.wav",
	"0:/Sound/4TimeSone.wav",
	"0:/Sound/5TimeSone.wav",
	"0:/Sound/6TimeSone.wav",										//5
	"0:/Sound/7TimeSone.wav",
	"0:/Sound/8TimeSone.wav",
	"0:/Sound/ActionTimeOutCancel.wav",
	"0:/Sound/AddSample.wav",
	"0:/Sound/AddSample2.wav",										//10
	"0:/Sound/Busy1IntoTimeOut.wav",
	"0:/Sound/Busy2IntoTimeOut.wav",
	"0:/Sound/Busy3IntoTimeOut.wav",
	"0:/Sound/Busy4IntoTimeOut.wav",
	"0:/Sound/Busy5IntoTimeOut.wav",										//15
	"0:/Sound/Busy6IntoTimeOut.wav",
	"0:/Sound/Busy7IntoTimeOut.wav",
	"0:/Sound/Busy8IntoTimeOut.wav",
	"0:/Sound/CannotTestNormal.wav",
	"0:/Sound/Error_CardTemp.wav",									//20
	"0:/Sound/Error_CardTimeOut.wav",
	"0:/Sound/Error_CardUsed.wav",
	"0:/Sound/Error_ErWeiMa.wav",
	"0:/Sound/Error_ETemp.wav",
	"0:/Sound/Error_Memery.wav",								//25
	"0:/Sound/InputSampleID.wav",
	"0:/Sound/Put1ToQueue.wav",
	"0:/Sound/Put2ToQueue.wav",
	"0:/Sound/Put3ToQueue.wav",
	"0:/Sound/Put4ToQueue.wav",										//30
	"0:/Sound/Put5ToQueue.wav",
	"0:/Sound/Put6ToQueue.wav",
	"0:/Sound/Put7ToQueue.wav",
	"0:/Sound/Put8ToQueue.wav",
	"0:/Sound/Put1ToTestPlace.wav",								//35
	"0:/Sound/Put2ToTestPlace.wav",
	"0:/Sound/Put3ToTestPlace.wav",
	"0:/Sound/Put4ToTestPlace.wav",
	"0:/Sound/Put5ToTestPlace.wav",
	"0:/Sound/Put6ToTestPlace.wav",									//40
	"0:/Sound/Put7ToTestPlace.wav",
	"0:/Sound/Put8ToTestPlace.wav",
	"0:/Sound/PutNewCardToTestPlace.wav",
	"0:/Sound/Scanning.wav",
	"0:/Sound/SelectTester.wav",								//45
	"0:/Sound/StartSystemInit.wav",
	"0:/Sound/StartTimeDown.wav",
	"0:/Sound/SystemAlarm.wav",
	"0:/Sound/SystemError.wav",								//49
	"0:/Sound/SystemInitDone.wav",
	"0:/Sound/TestQueueFullWait.wav",
	"0:/Sound/Welcome.wav",											//52
	"0:/Sound/1Ready.wav",
	"0:/Sound/2Ready.wav",
	"0:/Sound/3Ready.wav",						//55
	"0:/Sound/4Ready.wav",
	"0:/Sound/5Ready.wav",
	"0:/Sound/6Ready.wav",
	"0:/Sound/7Ready.wav",
	"0:/Sound/8Ready.wav",						//60
	"0:/Sound/TestBusyWait.wav",
	"0:/�峿.wav",
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
