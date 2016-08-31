/***************************************************************************************************
*FileName:SystemInit
*Description: ϵͳ����ģ���ʼ��
*Author : xsx
*Data: 2016��4��21��14:18:28
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"SystemInit.h"

#include	"Led_Driver.h"
#include 	"Usart3_Driver.h"
#include 	"Usart2_Driver.h"
#include 	"Usart1_Driver.h"
#include	"SPI1_Driver.h"
#include	"Ads8325_Driver.h"
#include	"DRV8825_Driver.h"
#include	"CodeScanner_Driver.h"
#include	"usbd_cdc_vcp.h"
#include	"user_fatfs.h"
#include 	"MLX90614_Driver.h"
#include	"RTC_Driver.h"
#include 	"Usart6_Driver.h"
#include 	"Iwdg_Driver.h"
#include	"CardLimit_Driver.h"
#include 	"Usart4_Driver.h"
#include	"Wifi_Driver.h"
#include	"Timer4_Driver.h"
#include	"Timer3_Driver.h"
#include	"PT8211_Driver.h"
#include	"TM1623_Driver.h"
#include	"MAX4051_Driver.h"
#include	"TLV5617_Driver.h"
#include	"LEDCheck_Driver.h"

#include	"WifiFunction.h"
#include	"OutModel_Fun.h"
#include	"Net_Data.h"
#include	"System_Data.h"
#include	"Motor_Fun.h"
#include	"SDFunction.h"

#include	"Delay.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
extern void SystemInit(void);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��MySystemInit
*Description��ϵͳ��ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��21��14:20:44
***************************************************************************************************/
void MySystemBSPInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	SystemInit();						//�ֶ�ϵͳ��ʼ��
	delay_ms(50);
	
	Led_GPIOInit();						//ϵͳָʾ�Ƴ�ʼ��
	delay_ms(50);
	
	Timer4_Init();						//��ʱ����ʼ��
	Timer3_Init();						//��ʱ����ʼ��
	delay_ms(50);
	
	Usart3_Init();						//����3��ʼ��
	delay_ms(50);
	
	Usart2_Init();						//����2��ʼ��
	delay_ms(50);
	
	Usart4_Init();						//����4��ʼ��
	delay_ms(50);
	
	Usart6_Init();						//����6��ʼ��
	delay_ms(50);
	
	Usart1_Init();						//����2��ʼ��
	delay_ms(50);

	ADS_IO_Init();						//ADC IO��ʼ��
	delay_ms(50);
	
	DA_IO_Init();						//DA io��ʼ��
	delay_ms(50);
	
	Limit_Init();						//��λ���س�ʼ��
	delay_ms(50);
	
	DRV_Init();							//��� IO��ʼ��
	delay_ms(50);
	
	CodeScanner_GPIOInit();				//��ά���ʼ��
	delay_ms(50);
	
	My_USBInit();						//USB�˿ڳ�ʼ��
	delay_ms(50);
	
	SMBus_Init();						//�¶ȼ���ʼ��
	delay_ms(50);
	
	My_RTC_Init();						//RTC��ʼ��
	delay_ms(50);
	
	Wifi_GPIOInit();					//WIFI�˿ڳ�ʼ��
	delay_ms(50);
	
	MAX4051_Init();						//�ɼ��Ŵ����˿ڳ�ʼ��
	delay_ms(50);
	
	PT8211_GPIOInit();					//��Ƶ�˿ڳ�ʼ��
	ConfigI2S(16000);
	delay_ms(50);

	TM1623_Init();						//�Ŷ�ģ���ʼ��
	delay_ms(50);
	
	LEDCheck_Init();					//��ⷢ��ģ��ķ��������״̬��ʼ��
	delay_ms(10);
	
	FatfsInit();						//�ļ�ϵͳ��ʼ��
	
	IWDG_Init(3, 2000);					//���Ź���ʼ��,��ʱʱ��2S
	delay_ms(50);
}

/***************************************************************************************************
*FunctionName��MySystemModuleInit
*Description��system module init
*Input��None
*Output��None
*Author��xsx
*Data��2016��6��1��14:03:27
***************************************************************************************************/
void MySystemDataInit(void)
{	
	OutModel_Init();					//�Ŷ�ģ�����ݳ�ʼ��
	
	CreateNetQueue();					//�������ݳ�ʼ��
	
	InitMutex();						//wifi���ݳ�ʼ��
	
	InitMotorData();
	
	ReadNetData(GetGB_NetData());		//��ȡ��������
	
	ReadDeviceInfo(GetGB_DeviceInfo());	//��ȡ�豸��Ϣ
}
