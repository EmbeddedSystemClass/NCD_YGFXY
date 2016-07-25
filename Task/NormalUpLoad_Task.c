/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/

#include	"NormalUpLoad_Task.h"
#include	"UpLoad_Fun.h"

#include 	"FreeRTOS.h"
#include 	"task.h"




/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/
#define vNormalUpLoadTask_PRIORITY			( ( unsigned portBASE_TYPE ) 2U )
const char * NormalUpLoadTaskName = "vNormalUpLoadTask";




/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/

static void vNormalUpLoadTask( void *pvParameters );





/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void StartvNormalUpLoadTask(void)
{
	xTaskCreate( vNormalUpLoadTask, NormalUpLoadTaskName, configMINIMAL_STACK_SIZE, NULL, vNormalUpLoadTask_PRIORITY, NULL );
}


static void vNormalUpLoadTask( void *pvParameters )
{

	while(1)
	{
		UpLoadFunction();
	}
}
