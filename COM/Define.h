#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"

#define		NormalCode	0											//��������
#define		LabCode	1											//���Գ���
	
#define		CodeType	LabCode									//�������

#define		AdminPassWord	"123456"								//����Ա���룬�����޸��豸id
#define		AdjustPassWord	"111111"								//У׼����
#define		TestPassWord	"222222"								//�ϻ���������

#define		MY_SUCCESS	1
#define		MY_FALSE	0

/***************************************************************************************************/
/***************************************************************************************************/
/*****************************************����汾**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/*V1.0.03*/
#define	GB_SoftVersion_1	1
#define	GB_SoftVersion_2	0
#define	GB_SoftVersion_3	3
/**********************************************************************************************************/
/******************************************�����������*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,		//�����ɹ�
	My_Fail = pdFAIL			//����ʧ��
}MyState_TypeDef;

typedef enum
{ 
	true = 1,
	false = 0
}bool;

typedef enum
{ 
	LED_Error = 1,		//led����
	LED_OK = 0			//led����
}MyLEDCheck_TypeDef;
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************fatfs�ṹ�嶨��*************************************************/
/**********************************************************************************************************/
/*�˽ṹ�������ڽ���sd�����������ڴ�*/
typedef struct
{
	FIL file;
	UINT br;
	UINT bw;
	FRESULT res;
	FSIZE_t size;
}FatfsFileInfo_Def; 

typedef struct
{
	DIR dir;
	FRESULT res;
	FSIZE_t size;
}FatfsDirInfo_Def; 


/**********************************************************************************************************/
/**********************************************************************************************************/

#define	PaiDuiWeiNum	9							//�Ŷ�λ����Ŀ
typedef enum
{
	NotStart = 0,									//û��ʼ,���ڿ���
	statues1 = 1,									//�ȴ��ӿ��۰γ����׶�1��2����
	statues2 = 2,									//�Ѵӿ��۰γ����ȴ������Ŷ�λ���׶�1��2����
	statues3 = 3,									//�Ѳ����Ŷ�λ���ȴ�������������ʾ�򣬼�����ȷ�������׶�1
	statues4 = 4,									//�Ѽ���������ʱ��ʼ���׶�1
	statues5 = 5,									//ʱ��δ���������쳣�γ�����ʾ��أ����ȴ���أ��׶�1��2����
	statues6 = 6,									//�쳣�γ�ʱ�Ѳ�أ��׶�1��2����
	statues7 = 7,									//ʱ��С��60S���л��Ŷӽ��棬�׶�1��2����
	statues8 = 8,									//ʱ��С��20S����ʾ�������뿨�ۣ��ȴ����ŶӰγ�״̬���׶�1��2����
	statues9 = 9,									//�ȴ����뿨��״̬���׶�1��2����
	statues10 = 10,									//�ȴ����뿨��״̬���׶�1��2����
	statues11 = 11,									//�ȴ����뿨��״̬���׶�1��2����
	statues12 = 12,									//�ȴ����뿨��״̬���׶�1��2����
	statues13 = 13,									//�ȴ����뿨��״̬���׶�1��2����
	statuesNull = 0xff,
}MyPaiDuiStatues;


/**********************************************************************************************************/
/******************************************ʱ��ṹ�嶨��**************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
}MyTime_Def;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/



/**********************************************************************************************************/
/******************************************�¶Ƚṹ�嶨��**************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct
{
	float  E_Temperature;			//�����¶�
	float	O_Temperature;			//���������¶�
	float I_Temperature;			//�ڲ��¶�
}MyTemp_Def;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��ά����ض���**************************************************/
/**********************************************************************************************************/

#define	ScanOutTime						10			//????s
#define	CodeMaxLen						300			//???????
#define	ItemNameLen						20			//????????
#define	ItemMeasureLen					10			//????????
#define	CardPiCiHaoLen					30			//????????


/*???????*/
typedef enum
{
	CardCodeScanning = 0,											//����ɨ��
	CardCodeScanOK = 1,												//ɨ���ά��ɹ�
	CardCodeScanFail = 2,											//ɨ��ʧ��
	CardCodeTimeOut = 3,											//����
	CardCodeCardOut = 4,											//�����γ�
	CardCodeScanTimeOut = 5,										//ɨ�賬ʱ
	CardCodeCRCError = 6											//crc����
}ScanCodeResult;

#pragma pack(1)
typedef struct CardInfo_Tag
{
	char	ItemName[ItemNameLen];						//������Ŀ
	unsigned char	TestType;							//��߼��㷽ʽ
	float	NormalResult;								//����ֵ
	float	LowstResult;								//��ͼ����
	float	HighestResult;								//��߼����
	char	ItemMeasure[ItemMeasureLen];				//���Ե�λ
	unsigned short ItemLocation;						//T��λ��
	unsigned char ItemBiaoQuNum;						//��׼������Ŀ
	float	ItemFenDuan;								//�ֶη�߱�
	float	ItemBiaoQu[2][3];							//��׼����
			//2 --- ?2???
			//3 --- a , b, c  (????)
	unsigned char CardWaitTime;							//��⿨��Ӧʱ��
	unsigned short CLineLocation;						//c��λ��
	char	CardPiCi[CardPiCiHaoLen];					//���κ�
	RTC_DateTypeDef	CardBaoZhiQi;						//������
	unsigned short CRC16;								//crc
}CardCodeInfo;
#pragma pack()


#pragma pack(1)
typedef struct ScanQRTaskData_Tag
{
	CardCodeInfo * cardQR;
	ScanCodeResult scanresult;
}ScanQRTaskData;
#pragma pack()

#define	MaxLocation			7000						//����г�
#define	StartTestLocation	700							//������ʼλ��
#define	EndTestLocation		5000						//���Խ���Ϊֹ
#define	WaittingCardLocation	7000					//�ȴ��忨��λ��
#define	AvregeNum		10															//??????????,??????????
#define	MaxPointLen		(EndTestLocation - StartTestLocation)/AvregeNum				//?????


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��ƷID��ض���**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//���֧����Ʒid����Ϊ20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************������ض���****************************************************/
/**********************************************************************************************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//ʵʱ����������ֵ
	unsigned short Y_Scale;				//����y����͵Ŀ̶�
	unsigned short MUL_Y;				//Y��Ŵ���
}LineInfo;

typedef struct SysPage_Tag
{
	unsigned char (*CurrentPage)(void * pram);
	void (*LCDInput)(unsigned char *pbuf , unsigned short len);
	void (*PageUpDate)(void);
	MyState_TypeDef (*PageInit)(void * pram);
	MyState_TypeDef (*PageBufferMalloc)(void);
	MyState_TypeDef (*PageBufferFree)(void);
	
	unsigned char (*ParentPage)(void *  parm);
	unsigned char (*ChildPage)(void *  parm);
}SysPage;

#define	DataNumInPage		9							//һҳ8������
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��Ļ��ض���****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//Э��ͷ1
#define	LCD_Head_2		0xBB											//Э��ͷ2

#define W_REGSITER 		0x80          									//д������,��Ӧ��          
#define R_REGSITER 	0x81          									//��ȡ���ݣ���Ӧ��
#define W_ADDRESS  		0x82											//д�����Ĵ�������Ӧ��
#define R_ADDRESS  	0x83											//�������Ĵ������ݣ���Ӧ��
#define W_LINE     		0x84          									//д���߻���������Ӧ��

#pragma pack(1)
typedef struct BasicICO_Tag
{
	unsigned short X;
	unsigned short Y;
	unsigned short ICO_ID;
}Basic_ICO;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��������ض���****************************************************/
/**********************************************************************************************************/
#define	MaxUserNum	20													//��ౣ����û���Ŀ
#define	MaxPageShowNum	10												//һҳ��ʾ�û���Ŀ

#define	MaxNameLen	11
#define	MaxSexLen	5
#define	MaxAgeLen	7
#define	MaxPhoneLen	17
#define	MaxJobLen	33
#define	MaxDescLen	65

#pragma pack(1)
typedef struct UserTag
{ 
	char user_name[MaxNameLen];
	char user_sex[MaxSexLen];
	char user_age[MaxAgeLen];
	char user_phone[MaxPhoneLen];
	char user_job[MaxJobLen];
	char user_desc[MaxDescLen];
	unsigned short crc;
}User_Type;
#pragma pack() 
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************�豸��Ϣ��ض���****************************************************/
/**********************************************************************************************************/
#define	MaxDeviceIDLen	21
#define	MaxDeviceNameLen	51
#define	MaxDeviceMakerLen	51
#define	MaxDeviceMakerPhoneLen	21
#define	MaxDeviceUnitLen	51

#pragma pack(1)
typedef struct DeviceInfo_Tag
{
	char deviceid[MaxDeviceIDLen];													//�豸id
	char deviceunit[MaxDeviceUnitLen];												//�豸ʹ�õ�λ
	User_Type deviceuser;												//�豸ʹ����
	unsigned short crc;
}DeviceInfo;
#pragma pack() 
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************�Լ칦����ض���****************************************************/
/**********************************************************************************************************/

typedef enum
{ 
	ReadServerData = 0,												//���ط���������
	ReadServerError = 1,												//����������ʧ��
	ReadServerSuccess = 2,												//���������ݳɹ�
	
	ErWeiMaChecking = 3,												//���ڼ���ά��
	ErWeiMaError = 4,													//��ά�����
	ErWeiMaSuccess = 5,													//��ά������
	
	DataBasesChecking = 6,												//���ڼ��洢ģ��
	DataBasesError = 7,													//�洢ģ�����
	DataBasesSuccess = 8,												//�洢ģ������
	
	WIFIChecking = 9,													//���ڼ��WIFI
	WIFIError = 10,														//WIFI����
	WIFISuccess = 11,													//WIFI����
	
	ADDAChecking = 12,													//���ڼ��ɼ�ģ��
	ADDAError = 13,														//�ɼ�ģ�����
	ADDASuccess = 14,													//�ɼ�ģ������
	
	MotorChecking = 15,													//���ڼ��ɼ�ģ��
	MotorError = 16,														//�ɼ�ģ�����
	MotorSuccess = 17,													//�ɼ�ģ������
	
	
	
	SelfCheckOver = 18													//�Լ����
}SelfCheck_TypeDef;

typedef enum
{
	SelfCheck_None = 0,													//�Լ�δ��ʼ
	SelfChecking = 1,
	SelfCheck_OK = 2,													//�Լ�����
	SelfCheck_Error = 3,												//�Լ��в��ɺ��ԵĴ���
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/


/**********************************************************************************************************/
/******************************************������ض���****************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct ip_Tag
{
	unsigned char ip_1;
	unsigned char ip_2;
	unsigned char ip_3;
	unsigned char ip_4;
}IP_Def;
#pragma pack()

#define	MaxWifiListNum	20												//�����ʾ20���ȵ�
#define	PageWifiNum		10
#define	MaxSSIDLen		50
#define	MaxKEYLen		20

/*wifi�ȵ�*/
#pragma pack(1)
typedef struct wifi_Tag
{
	char ssid[MaxSSIDLen];														//SSID
	char auth[20];														//��֤ģʽ
	char encry[10];														//���ܷ�ʽ
	unsigned char indicator;											//�ź�ǿ��
	char key[MaxKEYLen];														//����
	unsigned short crc;
}WIFI_Def;
#pragma pack()


typedef struct mynetbuf_tag
{
	void * data;
	unsigned short datalen;
}mynetbuf;

typedef enum
{ 
	Link_Down = 0,
	Link_Up = 1
}LinkState_TypeDef;

/*ip��ȡ��ʽ*/
typedef enum
{
	DHCP_Mode = 1,														//�Զ���ȡ��̬ip
	User_Mode = 2														//�û�����ip
}NetIP_Type;


#pragma pack(1)
typedef struct NetData_Tag
{
	IP_Def myip;														//�ҵ�ip

	NetIP_Type ipmode;
	unsigned short crc;
}NetData;
#pragma pack()


/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000
	

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/**************************У׼����***********************************************************/
/*********************************************************************************************/

#pragma pack(1)
typedef struct AdjustData_tag
{
	char ItemName[ItemNameLen];
	float parm;
	unsigned short crc;
}AdjustData;
#pragma pack()

typedef enum
{
	NoResult = 0,
	ValueIsTooLarge = 1,
	ValueIsTooSmall = 2,
	BValueIsTooLarge = 3,
	BValueIsTooSmall = 6,
	PeakNumError = 4,
	ErrorCountOut = 5,
	MemError = 6,
	PeakNumZero = 7,									//�޷�
	TestInterrupt = 8,
	ResultIsOK = 99,
}ResultState;

#pragma pack(1)
typedef struct TestLine_tag {
	unsigned short TestPoint[MaxPointLen];
	unsigned short C_Point[2];
	unsigned short T_Point[2];
	unsigned short B_Point[2];
	float BasicBili;
	float BasicResult;
	float AdjustResult;
} TestLine;
#pragma pack()

#pragma pack(1)
typedef struct TestData_tag {
	User_Type user;
	char sampleid[MaxSampleIDLen];
	CardCodeInfo temperweima;
	AdjustData tempadjust;
	TestLine testline;
	MyTime_Def TestTime;					//����ʱ��
	MyTemp_Def TestTemp;					//�����¶�
	unsigned short time;					//��ʱʱ��
	unsigned short crc;
}TestData;
#pragma pack()


#pragma pack(1)
typedef struct TestTaskData_tag {
	TestData * testdata;
	ResultState testresult;
}TestTaskData;
#pragma pack()

#pragma pack(1)
typedef struct UpLoadIndex_tag {
	unsigned int index;
	unsigned short crc;
}UpLoadIndex;
#pragma pack()

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************��ʱ��********************************************************/
/*********************************************************************************************/
#pragma pack(1)
typedef struct
{
	unsigned int start;
	unsigned int interval;
}Timer;
#pragma pack()

typedef enum
{ 
	TimeNotTo = 0,		//��ʱʱ��δ��
	TimeOut = 1,		//��ʱʱ�䵽
	TimeError = 2,		//����
}TimerState_Def;


/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************�ϻ���������**************************************************/
/*********************************************************************************************/
#pragma pack(1)
typedef struct ReTestData_tag{
	unsigned int retestcount;								//�ܴ���
	unsigned short retestsurpluscount;						//ʣ����Դ���
	unsigned short retestedcount;							//�Ѿ����Դ���
	unsigned char reteststatus;								//�ϻ�����״̬��0ֹͣ��1�ȴ��忨��2����ά�룬3����
	Timer retesttimer;										//�ϻ����Լ�ʱ��
	Timer oneretesttimer;									//һ���ϻ����Լ�ʱ��
	TestData testdata;										//�ϻ��������ݿռ�
	char result[30];										//��ǰ���ԵĽ���
	unsigned short playcount;								//�ϻ����ż���������¼���Ŵ���
	Timer playtimer;										//�ϻ���Ƶ��ʱ��	
	Timer oneplaytimer;										//�ϻ����ż�ʱ������¼ÿ�β��ŵ�ʱ��
	MyTime_Def startplayTime;								//��ǰ��Ƶ��ʼʱ��
	MyTime_Def endplayTime;									//��ǰ��Ƶ����ʱ��
	double advalue1;										//DAֵ100ʱ��ADֵ
	double advalue2;										//DAֵ200ʱ��ADֵ
	double advalue3;										//DAֵ300ʱ��ADֵ
	MyLEDCheck_TypeDef ledstatus;							//����ģ��״̬
}ReTestData;
#pragma pack()

#endif

