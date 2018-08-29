#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"
/**************************************************************************************************/
/***************************************************************************************************/
/*****************************************����汾**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define QRVersion0Define		0								//��ά��ԭʼ�汾����
#define QRVersion2Define		'2'								//��ά��汾2����
#define QRVersion3Define		'3'								//��ά��汾3����
#define QRVersionUnDefine		0xff							//��֧�ֶ�ά��汾
#define	GB_QRVersion			QRVersion3Define				//��ǰ֧�ֵĶ�ά��汾�����¼���

//build by device id
#define	DeviceAll						0x30
#define	DeviceNCD13011703019			0x31                    //����
#define	DeviceBuildId					DeviceAll	

#if (DeviceBuildId == DeviceAll)

	#define	GB_SoftVersion			(unsigned short)1080
	#define	GB_SoftVersionStr		"V1.0.80\0"
	#define	GB_SoftVersion_Build	"Build18082701\0"
	
#elif (DeviceBuildId == DeviceNCD13011703019)

	#define	GB_SoftVersion			(unsigned short)1079
	#define	GB_SoftVersionStr		"V1.0.79\0"
	#define	GB_SoftVersion_Build	"Build18071001\0"

#endif
	

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	Device_Final					0x27
#define	Device_Demo						0x28
#define	Device_FastDemo					0x29
#define	DeviceUseType					Device_Final

#define	NCDServerFinal					0x88
#define	NCDServerDebug					0x89
#define	NCDServerType					NCDServerFinal

#define	UserNormalProgram				0x60					//ͨ�ó���
#define	UserXGProgram					0x61					//Т������ҽԺ����
#define	UserProgramType					UserNormalProgram			//�����û�ѡ����

#if (UserProgramType == UserNormalProgram)
	#define	USB_USE							1
#elif (UserProgramType == UserXGProgram)
	#define	USB_USE							0
#endif

/*��������Ϣ*/
#define	GB_ServerIp_1		116
#define	GB_ServerIp_2		62
#define	GB_ServerIp_3		108
#define	GB_ServerIp_4		201
#define	GB_ServerPort		8080

/*�û�������*/
#define	GB_UserServerIp_1		192
#define	GB_UserServerIp_2		168
#define	GB_UserServerIp_3		0
#define	GB_UserServerIp_4		37
#define	GB_UserServerPort		9200
#define	GBDefaultPoctServer				"/NCDPoctServer/UpHandler\0"
/**********************************************************************************************************/
/******************************************�����������*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//�����ɹ�
	My_Fail = pdFAIL			//����ʧ��
}MyState_TypeDef;

typedef enum
{ 
	TRUE = 1,
	FALSE = 0
}MyBool;

typedef enum
{ 
	LED_Error = 1,		//led����
	LED_OK = 0			//led����
}MyLEDCheck_TypeDef;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	NormalTestType = 0,																	//�������
	PaiDuiTestType = 1																	//�ŶӲ���
}TestType;
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

#define	PaiDuiWeiNum	8							//�Ŷ�λ����Ŀ
typedef enum
{
	statusNull = 0,									//û��ʼ,Ĭ��״̬
	status_user = 1,								//ѡ�������״̬
	status_sample = 2,								//��ȡ��ƷID״̬
	status_wait1 = 3,								//�ȴ��忨1
	status_preread = 4,								//Ԥ����״̬
	status_start = 5,								//�����Ŷ�
	status_outcard = 6,								//�ȴ��ӿ��۰γ�
	status_in_n = 7,								//����ʱ״̬ -- �ȴ������Ŷ�λ
	status_out_n = 8,								//����ʱ״̬ -- �ȴ��γ��Ŷ�λ
	status_incard_n = 9,							//����ʱ״̬ -- �ȴ����뿨��
	status_timedown = 10,							//����ʱ״̬
	status_prereadagain_n = 11,						//����ʱ״̬ -- �ٴ�Ԥ��������Ҫ��⿨���
	status_in_o = 12,								//��ʱ״̬ -- �ȴ������Ŷ�λ
	status_out_o = 13,								//��ʱ״̬ -- �ȴ��γ��Ŷ�λ
	status_incard_o = 14,							//��ʱ״̬ -- �ȴ����뿨��
	status_timeup = 15,								//��ʱ״̬
	status_prereadagain_o = 16,						//��ʱ״̬ -- �ٴ�Ԥ��������Ҫ��⿨���
	status_timedownagain = 17,						//����ʱ����
	status_test = 18,								//����
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


#define	ITEM_NUM	10
#define	ItemNameLen						20			//????????
#define	ItemMeasureLen					10			//????????

#pragma pack(1)
typedef struct ItemConstData_Tag
{
	char itemName[ItemNameLen];									//��Ŀ����
	unsigned short icoIndex;								//�Ŷӽ�����ʾͼ�������
	unsigned char pointNum;								//С�������
	float lowstResult;									//��ͼ����
	float highestResult;								//��߼����
	char normalResult[50];								//�ο�ֵ
	char itemMeasure[10];								//��λ
}ItemConstData;
#pragma pack()

typedef enum
{
	CardCodeScanning = 0,											//����ɨ��
	CardCodeScanOK = 1,												//ɨ���ά��ɹ�
	CardCodeScanFail = 2,											//ɨ��ʧ��
	CardCodeTimeOut = 3,											//����
	CardCodeCardOut = 4,											//�����γ�
	CardCodeScanTimeOut = 5,										//ɨ�賬ʱ
	CardCodeCRCError = 6,											//crc����
	CardUnsupported = 7												//��ǰ����֧��
}ScanCodeResult;

#pragma pack(1)
typedef struct QRCode_Tag
{
	unsigned char qu1Ise;									//����1�Ƿ���ָ��
	unsigned char qu2Ise;									//����2�Ƿ���ָ��
	unsigned char qu3Ise;									//����3�Ƿ���ָ��
	float qu1_d;	
	float qu2_d;
	float qu3_d;
	unsigned char parm[5];								//Ԥ��
	unsigned short ItemLocation;						//T��λ��
	unsigned char ChannelNum;							//ͨ����(0-7)
	float	ItemFenDuan[2];								//�ֶη�߱�
	float	ItemBiaoQu[3][3];							//��׼����
			//2 --- ?2???
			//4 --- a , b, c d (????)
	unsigned char CardWaitTime;							//��⿨��Ӧʱ��
	unsigned short CLineLocation;						//c��λ��
	char	PiHao[15];									//���κ�
	char	piNum[10];
	MyTime_Def	CardBaoZhiQi;							//������
	ItemConstData itemConstData;						//����Ŀ�Ĺ̶�����
	unsigned char qrVersion;							//qr version in qr version 2.0 inplace crc high byte in qr version 1.0
	unsigned char calMode;								//calculate result mode(1:T/C, 2:T/T+C) in qr version 2.0 inplace crc low byte in qr version 1.0
}QRCode;
#pragma pack()



#define	MaxLocation			6500						//����г�
#define	StartTestLocation	1250							//������ʼλ��
#define	EndTestLocation		4300						//���Խ���Ϊֹ
#define	WaittingCardLocation	MaxLocation					//�ȴ��忨��λ��
#define	AvregeNum		10								//ƽ��ֵ�˲�����
#define	FilterNum		5								//??????
#define	MaxPointLen	((EndTestLocation - StartTestLocation)/AvregeNum - FilterNum) //���Ե����


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
/***************************************************************************�*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//ʵʱ����������ֵ
	unsigned short Y_Scale;				//����y����͵Ŀ̶�
	unsigned short MUL_Y;				//Y��Ŵ���
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************��Ļ��ض���****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//Э��ͷ1
#define	LCD_Head_2		0xBB											//Э��ͷ2

#define W_REGSITER 		0x80          									//д������,��Ӧ��          
#define R_REGSITER 	    0x81          									//��ȡ���ݣ���Ӧ��
#define W_ADDRESS  		0x82											//д�����Ĵ�������Ӧ��
#define R_ADDRESS  	    0x83											//�������Ĵ������ݣ���Ӧ��
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
#define	MaxPageShowNum	5												//һҳ��ʾ�û���Ŀ

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

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************�Լ칦����ض���****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//�Լ�δ��ʼ
	SelfChecking = 1,
	SelfCheck_OK = 2,													//�Լ�����
	SelfCheck_Error = 3,												//�Լ��в��ɺ��ԵĴ���
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxDeviceIDLen	21
#define	MaxDeviceUnitLen	51

/*�豸��Ϣ����*/
#pragma pack(1)
typedef struct DeviceInfo_Tag
{
	char deviceid[MaxDeviceIDLen];													//�豸id
	char deviceunit[MaxDeviceUnitLen];												//�豸ʹ�õ�λ
	User_Type deviceuser;															//�豸ʹ����
	MyBool isnew;															//�豸��Ϣ�Ƿ��и���
	unsigned short crc;
}DeviceInfo;
#pragma pack()

#pragma pack(1)
typedef struct PrintInfo_Tag
{
	char header[30];
	unsigned short crc;
}PrintInfo;
#pragma pack()

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

#define	MaxSaveWifiNum	50												//��ౣ��50���ȵ�
#define	MaxWifiListNum	20												//�����ʾ20���ȵ�
#define	PageWifiNum		8
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



/*����������*/
#pragma pack(1)
typedef struct NetSet_Tag
{
	IP_Def myip;														//�ҵ�ip
	NetIP_Type ipmode;
	unsigned short crc;
}NetSet;
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP_Def serverIP;														//������IP
	unsigned short serverPort;											//�������˿ں�
	unsigned short crc;
}ServerSet; 
#pragma pack()

#define	ServerStructSize	sizeof(ServerSet)


/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000
#define MAX_TEST_CNT        5                       //����ز����
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


typedef enum
{
	NoResult = 0,										//��δ���ԣ����δnull
	NoSample = 1,										//δ����
	PeakError = 2,										//��⿨û��C��
	MemError = 3,										//�ڴ����
    T_CV_0_05 = 4,                                      //�Ҳ���T������Ĭ��λ�õ�cv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //cλ��С��Tλ��
    C_L_T_L_2 = 9,                                        //cλ�� - Tλ�� (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //����
    B_V_ERR = 12,                                       //����ֵ����
	TestInterrupt = 8,									//�����ж�
	ResultIsOK = 99,									//���Գɹ�
}ResultState;

#pragma pack(1)
typedef struct Point_tag {
	unsigned short y;
	unsigned short x;
}Point;
#pragma pack()

#pragma pack(1)
typedef struct TestLine_tag {
	unsigned short TestPoint[MaxPointLen];
	Point C_Point;
	Point T_Point;
	Point B_Point;
	float t_cValue;
	float BasicResult;
	unsigned char CMdifyNum;					//c�߲���������*10
	unsigned char parm[3];
} TestLine;
#pragma pack()

#pragma pack(1)
typedef struct TestData_tag {
	User_Type user;
	char sampleid[MaxSampleIDLen];
	QRCode temperweima;
	float t_tcValue;
	float t_cv;
	float c_cv;
    unsigned char testcnt;
	unsigned char parm;
	TestLine testline;
	MyTime_Def TestTime;						//����ʱ��
	MyTemp_Def TestTemp;						//�����¶�
	unsigned short time;						//��ʱʱ��
	ResultState testResultDesc;					//���Խ��״̬
	unsigned short crc;
}TestData;
#pragma pack()

typedef struct ItemData_tag {
	TestData testdata;
	Timer timer;																	//���浹��ʱ��ʱ��
	Timer timer2;																	//��ʱ����ʱ��ʱ��
	Timer timer3;																	//��ʱ����ʱ��ʱ��
	unsigned char errorcount;														//δ��������
	MyPaiDuiStatues statues;														//���Խ׶�
	unsigned char jieduan;															//���Խ׶�
	unsigned char testlocation;
	unsigned short ledLight;														//����ʱled�����ȣ�Ϊϵͳ�����б����У׼ledֵ
}ItemData;

#define	TestDataStructSize	sizeof(TestData)
#define	TestDataStructCrcSize	TestDataStructSize - 2

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************�ϻ���������**************************************************/
/*********************************************************************************************/

typedef struct ReTestData_tag{
	unsigned int retestcount;								//�ܴ���
	unsigned short retestsurpluscount;						//ʣ����Դ���
	unsigned short retestedcount;							//�Ѿ����Դ���
	unsigned char reteststatus;								//�ϻ�����״̬��0ֹͣ����0����
	unsigned char retestStep;								//���Բ��裬1�ȴ��忨��2����ά�룬3����
	Timer retesttimer;										//�ϻ����Լ�ʱ��
	Timer oneretesttimer;									//һ���ϻ����Լ�ʱ��
	ItemData itemData;
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
	unsigned char playstatus;								//����״̬
}ReTestData;


#endif

