#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include	"ff.h"

#define		NormalCode	0											//��������
#define		DebugCode	1											//���Գ���
#define		TestCode	2											//���Գ���
	
#define		CodeType	DebugCode									//�������
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
	My_Pass = 1,		//�����ɹ�
	My_Fail = 0			//����ʧ��
}MyState_TypeDef;
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

#pragma pack(1)
typedef struct
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned int num;
	unsigned int index;
}TestDateInfo_Def;
#pragma pack()
#define	TestDataDateRepeatNum	10					//ͬһ�����������ڹ����ļ��г��ֵĴ���
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
	CardCodeScanning = 0,												//����ɨ��
	CardCodeScanOK = 1,												//ɨ���ά��ɹ�
	CardCodeScanFail = 2,											//ɨ��ʧ��
	CardCodeTimeOut = 3,											//����
}ScanCodeResult;

#pragma pack(1)
typedef struct CardInfo_Tag
{
	char	ItemName[ItemNameLen];						//????,??10???
	unsigned char	TestType;							//??????
	float	NormalResult;								//????
	float	LowstResult;								//????????
	float	HighestResult;								//????????
	char	ItemMeasure[ItemMeasureLen];				//????	
	unsigned short ItemLocation;						//T???
	unsigned char ItemBiaoQuNum;						//????????????,???2?
	float	ItemFenDuan;			//?????????2????????,3??2?
	float	ItemBiaoQu[2][3];			//?????????
			//2 --- ?2???
			//3 --- a , b, c  (????)
	unsigned char CardWaitTime;										//???????
	unsigned short CLineLocation;									//c???
	char	CardPiCi[CardPiCiHaoLen];								//?????
	RTC_DateTypeDef	CardBaoZhiQi;										//???
	unsigned short CRC16;												//????CRC
}CardCodeInfo;
#pragma pack()


#define	MaxLocation			7000						//����г�
#define	StartTestLocation	700							//������ʼλ��
#define	EndTestLocation		5000						//���Խ���Ϊֹ
#define	WaittingCardLocation	6500					//�ȴ��忨��λ��
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

#define	DataNumInPage		8							//һҳ8������
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
	char isfresh;																	//�Ƿ��и��£������1�����ϴ�����
	char deviceid[MaxDeviceIDLen];													//�豸id
	char devicename[MaxDeviceNameLen];												//�豸����
	char devicemaker[MaxDeviceMakerLen];												//�豸������
	char devicemakerphone[MaxDeviceMakerPhoneLen];											//�豸�����̵绰
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
	SelfCheck_NoResult = 0,														//�Լ�δ���
	SelfCheck_OK = 1,													//�Լ�����
	SelfCheck_Error = 2,												//�Լ��в��ɺ��ԵĴ���
	SelfCheck_Alam = 3,													//�Լ��и澯��
}SelfCheckResult_TypeDef;
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

/*��������*/
typedef enum
{
	NetNone = 0,														//������
	Line_Mode = 1,														//������
	Wifi_Mode = 2														//wiif
}NetCard_Type;

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
	IP_Def serverip;													//������ip
	NetIP_Type ipmode;
	unsigned short crc;
}NetData;
#pragma pack()

#define	DefaultNetCard		Line_Mode									//Ĭ������
#define	DefaultIPMode		DHCP_Mode									//ip��ȡ��ʽ�����������

/*��������������IP*/
#define	NCD_ServerIP_1		123
#define	NCD_ServerIP_2		57
#define	NCD_ServerIP_3		94
#define	NCD_ServerIP_4 		39
#define	NCD_ServerPort 		8080

/*�ͻ�������*/
#define	User_ServerIP_1		192
#define	User_ServerIP_2		168
#define	User_ServerIP_3		1
#define	User_ServerIP_4		100
#define	User_ServerPort		9600
/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000

#endif

