/***************************************************************************************************
*FileName��SelfTest_Fun
*Description���Լ칦��
*Author��xsx
*Data��2016��1��27��10:29:14
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SelfTest_Fun.h"

#include	"CodeScanner_Driver.h"
#include	"SDFunction.h"
#include	"WifiFunction.h"
#include	"Ads8325_Driver.h"
#include	"TLV5617_Driver.h"
#include	"MAX4051_Driver.h"
#include	"Motor_Fun.h"
#include	"Motor_Data.h"
#include	"CardLimit_Driver.h"
#include	"DRV8825_Driver.h"
#include	"System_Data.h"

#include	"DeviceDao.h"
#include	"SelfCheck_Data.h"
#include	"MyMem.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
#include 	"queue.h"
#include	"semphr.h"

#include	"stdlib.h"
#include	<string.h>
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static MyState_TypeDef ADDASelfTest(void);

static MyState_TypeDef MotorSelfTest(void);

static void GB_DataInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName��SelfTest_Function
*Description���Լ칦��ʵ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��1��27��10:38:06
***************************************************************************************************/
void SelfTest_Function(void)
{	
	GB_DataInit();
	vTaskDelay(50 *portTICK_RATE_MS);
	
//	ErWeiMaSelfTest();
	
	CheckSDFunction();
	vTaskDelay(50 * portTICK_RATE_MS);
	
//	WIFICheck();
	vTaskDelay(50 * portTICK_RATE_MS);
	
	ADDASelfTest();
	vTaskDelay(50 *portTICK_RATE_MS);
	
	MotorSelfTest();
	vTaskDelay(50 *portTICK_RATE_MS);
	
	SetGB_SelfCheckStatus(SelfCheck_OK);
}


/***************************************************************************************************
*FunctionName��ADDASelfTest
*Description���ɼ�ģ���Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��17:21:14
***************************************************************************************************/
static MyState_TypeDef ADDASelfTest(void)
{
	static double tempvalue1 = 0.0, tempvalue2 = 0.0;
	
	SetGB_LedValue(0);
	vTaskDelay(100 / portTICK_RATE_MS);	
	SetGB_CLineValue(0);
	vTaskDelay(100 / portTICK_RATE_MS);	
	SelectChannel(0);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	tempvalue1 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	
	SetGB_LedValue(300);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue2 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	if(tempvalue2 <= tempvalue1)
		return My_Fail;
	
	SetGB_CLineValue(5);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue1 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);
	if(tempvalue2 < tempvalue1)
		return My_Fail;
	
	SelectChannel(7);
	vTaskDelay(100 / portTICK_RATE_MS);
	tempvalue2 = ADS8325();
	vTaskDelay(100 / portTICK_RATE_MS);	
	if(tempvalue2 < tempvalue1)
		return My_Fail;
	else
		return My_Pass;
}



/***************************************************************************************************
*FunctionName��MotorSelfTest
*Description������Լ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��27��17:39:36
***************************************************************************************************/
static MyState_TypeDef MotorSelfTest(void)
{
	static unsigned char count = 0;
	
	SetDRVPowerStatues(LowPower);

	SetGB_MotorLocation(10000);
	MotorMoveTo(0, 1);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	while(!BackLimited)
	{
		vTaskDelay(500 / portTICK_RATE_MS);
		
		count++;
		if(count > 6)
			break;
	}
	
	if(count > 6)
	{
		StopMotor();
		return My_Fail;
	}
	
	
	SetDRVPowerStatues(NonamalPower);
	vTaskDelay(100 / portTICK_RATE_MS);
	MotorMoveTo(MaxLocation, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	
	//�ߵ�����г̣����ǰ��λ���������쳣
	if(PreLimited)
		return My_Fail;
	
	return My_Pass;
}


static void GB_DataInit(void)
{
	NetData *mynetdata = NULL;
	DeviceInfo *mydeviceinfo;
	
	mynetdata = MyMalloc(sizeof(NetData));
	
	mydeviceinfo = MyMalloc(sizeof(DeviceInfo));
	
	if(mynetdata && mydeviceinfo)
	{
		memset(mynetdata, 0, sizeof(NetData));
		memset(mydeviceinfo, 0, sizeof(DeviceInfo));
		
		//��ȡϵͳ��������
		ReadNetData(mynetdata);
		SetGB_NetConfigureData(mynetdata);

		//��ȡ�豸��Ϣ
		ReadDeviceInfo(mydeviceinfo);
	}
	
	MyFree(mynetdata);
	MyFree(mydeviceinfo);
}
