/***************************************************************************************************
*FileName:main
*Description:程序入口
*Author:xsx
*Data:2016年4月21日14:54:04
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include 	"netconf.h"
#include	"ReadInputData_Task.h"
#include	"CodeScan_Task.h"
#include	"USBCMD_Task.h"
#include	"SelfCheck_Task.h"
#include	"SystemUI_Task.h"
#include	"Iwdg_Task.h"
#include	"Universal_Task.h"
#include	"Test_Task.h"
#include	"OutModel_Task.h"
#include	"PlaySong_Task.h"
#include	"NormalUpLoad_Task.h"
#include	"user_fatfs.h"
#include	"WelcomePage.h"
#include	"UI_Data.h"
#include	"ReadBarCode_Task.h"
#include	"SelfTest_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"

#include	"stdio.h"

/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
#define 	vStartTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )		//任务优先级
const char * StartTaskName = "vStartTask";										//任务名
/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static void vStartTask(void * param);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName：main
*Description：程序入口
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月21日14:54:56
***************************************************************************************************/
int main(void)
{
	MySystemBSPInit();

	xTaskCreate( vStartTask, StartTaskName, configMINIMAL_STACK_SIZE*2, NULL, vStartTask_PRIORITY, NULL );
	
	vTaskStartScheduler();	//开始任务调度
	for(;;);
}

/***************************************************************************************************
*FunctionName：vStartTask
*Description：起始任务，创建其他任何，和初始化
*Input：param -- 传进来的参数
*Output：None
*Author：xsx
*Data：2016年4月21日15:01:17
***************************************************************************************************/
static void vStartTask(void * param)
{
	/*加载系统数据*/
	MySystemDataInit();

	/*开启看门狗任务*/
	StartvIwdgTask();
	
	SelfTest_Function();

	/*读取输入数据任务*/
	StartvReadInDataTask();
	
	/*创建读取条码枪任务*/
	StartBarCodeTask();
	
	/*开启测试任务*/
	StartvTestTask();
	
	/*开启网络任务*/
	StartEthernet();

	/*上传任务*/
	StartvNormalUpLoadTask();
	
	/*开启读二维码任务*/
	StartCodeScanTask();
	
	/*USB测试任务*/
//	StartUSBCMDTask();
	
	/*通用任务*/
	StartvUniversalTask();
	
	/*播放音频*/
	StartvPlaySongTask();
	
	vTaskDelay(5000 / portTICK_RATE_MS);
	/*显示欢迎界面*/
	DspWelcomePage(NULL);
	
	StartvSystemUITask();

	vTaskDelete(NULL);
}
