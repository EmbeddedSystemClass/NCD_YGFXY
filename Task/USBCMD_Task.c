/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"USBCMD_Task.h"
#include 	"usbd_cdc_vcp.h"
#include	"CodeScanner_Driver.h"
#include	"MyEncryptTool.h"
#include	"MyMem.h"
#include	"user_fatfs.h"
#include	"Ads8325_Driver.h"
#include	"DRV8825_Driver.h"
#include 	"MLX90614_Driver.h"
#include	"RTC_Driver.h"
#include	"TLV5617_Driver.h"
#include	"DeviceInfo_Data.h"
#include 	"Usart4_Driver.h"
#include	"WifiFunction.h"
#include	"CardLimit_Driver.h"
#include	"PT8211_Driver.h"
#include	"Define.h"
#include	"OutModel_Fun.h"
#include	"MyTest_Data.h"
#include	"SDFunction.h"
#include	"CodeScan_Task.h"
#include	"MAX4051_Driver.h"
#include	"WifiFunction.h"
#include	"ServerFun.h"
#include	"PlaySong_Task.h"
#include	"Timer4_Driver.h"
#include	"Motor_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"ff.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
#include 	"time.h"
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
#define vUSBCMDTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * USBCMDTaskName = "vUSBCOMTask";

unsigned char m[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

static void vUSBCMDTask( void *pvParameters );
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
void StartUSBCMDTask(void)
{
	xTaskCreate( vUSBCMDTask, USBCMDTaskName, configMINIMAL_STACK_SIZE, NULL, vUSBCMDTask_PRIORITY, NULL );
}

/************************************************************************
** ������:
** ��  ��:
** ��  ��:��
** ��  ��:��
** ��  �أ���
** ��  ע����
** ʱ  ��:  
** ��  �ߣ�xsx                                                 
************************************************************************/
static void vUSBCMDTask( void *pvParameters )
{


	while(1)
	{

		vTaskDelay(1000/portTICK_RATE_MS);
	}

}

