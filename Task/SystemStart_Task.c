/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"SystemStart_Task.h"

#include 	"netconf.h"
#include	"ReadInputData_Task.h"
#include	"CodeScan_Task.h"
#include	"USBCMD_Task.h"
#include	"SystemUI_Task.h"
#include	"Iwdg_Task.h"
#include	"Universal_Task.h"
#include	"Test_Task.h"
#include	"OutModel_Task.h"
#include	"PlaySong_Task.h"
#include	"NormalUpLoad_Task.h"
#include	"user_fatfs.h"
#include	"WelcomePage.h"
#include	"ReadBarCode_Task.h"

#include	"SelfTest_Fun.h"

#include	"UI_Data.h"
#include	"SelfCheck_Data.h"

#include	"Define.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define	SystemStartTask_PRIORITY			2
const char * SystemStartTaskName = "vSystemStartTask";
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static void vSystemStartTask( void *pvParameters );
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName: StartSystemStartTask
*Description: ����ϵͳ��ʼ������
*Input: 
*Output: 
*Author: xsx
*Date: 2016��9��21��11:58:17
***************************************************************************************************/
void StartSystemStartTask(void)
{
	xTaskCreate( vSystemStartTask, SystemStartTaskName, configMINIMAL_STACK_SIZE, NULL, SystemStartTask_PRIORITY, NULL );
}

/***************************************************************************************************
*FunctionName: vSystemStartTask
*Description: ϵͳ��ʼ���Լ��Լ죬����
*Input: 
*Output: 
*Author: xsx
*Date: 2016��9��21��11:58:46
***************************************************************************************************/
static void vSystemStartTask( void *pvParameters )
{
	/*�������Ź�����*/
	StartvIwdgTask();
	
	/*ͨ������*/
	StartvUniversalTask();
	
	/*������Ƶ*/
	StartvPlaySongTask();
	
	/*��ʾ��ӭ����*/
	PageAdvanceTo(DspWelcomePage, NULL);
	
	/*ϵͳ�����������*/
	StartvSystemUITask();
	
	/*��ȡ������������*/
	StartvReadInDataTask();
	
	/*�ȴ������Լ����*/
	while(SelfCheck_None == GetGB_SelfCheckStatus())
		vTaskDelay(500 / portTICK_RATE_MS);
	
	/*��ʼ�Լ�*/
	if(SelfChecking == GetGB_SelfCheckStatus())
	{
		SelfTest_Function();
		vTaskDelay(1000 * portTICK_RATE_MS);
	}
	
	/*�Լ����*/
	while(SelfCheck_Error == GetGB_SelfCheckStatus())
		vTaskDelay(portMAX_DELAY);
	
	/*������ȡ����ǹ����*/
	StartBarCodeTask();
	
	/*������������*/
	StartvTestTask();
	
	/*������������*/
	StartEthernet();

	/*�ϴ�����*/
	StartvNormalUpLoadTask();
	
	/*��������ά������*/
	StartCodeScanTask();
	
	/*USB��������*/
	StartUSBCMDTask();
	
	vTaskDelete(NULL);
}

/****************************************end of file************************************************/
