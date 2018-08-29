#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"
/**************************************************************************************************/
/***************************************************************************************************/
/*****************************************»Ìº˛∞Ê±æ**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define QRVersion0Define		0								//∂˛Œ¨¬Î‘≠ º∞Ê±æ∂®“Â
#define QRVersion2Define		'2'								//∂˛Œ¨¬Î∞Ê±æ2∂®“Â
#define QRVersion3Define		'3'								//∂˛Œ¨¬Î∞Ê±æ3∂®“Â
#define QRVersionUnDefine		0xff							//≤ª÷ß≥÷∂˛Œ¨¬Î∞Ê±æ
#define	GB_QRVersion			QRVersion3Define				//µ±«∞÷ß≥÷µƒ∂˛Œ¨¬Î∞Ê±æ£¨Õ˘œ¬ºÊ»›

//build by device id
#define	DeviceAll						0x30
#define	DeviceNCD13011703019			0x31                    //∑œ∆˙
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

#define	UserNormalProgram				0x60					//Õ®”√≥Ã–Ú
#define	UserXGProgram					0x61					//–¢∏–÷––ƒ“Ω‘∫≥Ã–Ú
#define	UserProgramType					UserNormalProgram			//≥Ã–Ú”√ªß—°‘Ò∂®“Â

#if (UserProgramType == UserNormalProgram)
	#define	USB_USE							1
#elif (UserProgramType == UserXGProgram)
	#define	USB_USE							0
#endif

/*∑˛ŒÒ∆˜–≈œ¢*/
#define	GB_ServerIp_1		116
#define	GB_ServerIp_2		62
#define	GB_ServerIp_3		108
#define	GB_ServerIp_4		201
#define	GB_ServerPort		8080

/*”√ªß∑˛ŒÒ∆˜*/
#define	GB_UserServerIp_1		192
#define	GB_UserServerIp_2		168
#define	GB_UserServerIp_3		0
#define	GB_UserServerIp_4		37
#define	GB_UserServerPort		9200
#define	GBDefaultPoctServer				"/NCDPoctServer/UpHandler\0"
/**********************************************************************************************************/
/******************************************≤Ÿ◊˜Ω·π˚±‰¡ø*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//≤Ÿ◊˜≥…π¶
	My_Fail = pdFAIL			//≤Ÿ◊˜ ß∞‹
}MyState_TypeDef;

typedef enum
{ 
	TRUE = 1,
	FALSE = 0
}MyBool;

typedef enum
{ 
	LED_Error = 1,		//led¥ÌŒÛ
	LED_OK = 0			//led’˝≥£
}MyLEDCheck_TypeDef;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	NormalTestType = 0,																	//≥£πÊ≤‚ ‘
	PaiDuiTestType = 1																	//≈≈∂”≤‚ ‘
}TestType;
/**********************************************************************************************************/
/******************************************fatfsΩ·ππÃÂ∂®“Â*************************************************/
/**********************************************************************************************************/
/*¥ÀΩ·ππÃÂ”√”⁄‘⁄Ω¯––sd≤Ÿ◊˜ «…Í«Îƒ⁄¥Ê*/
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

#define	PaiDuiWeiNum	8							//≈≈∂”Œª÷√ ˝ƒø
typedef enum
{
	statusNull = 0,									//√ªø™ º,ƒ¨»œ◊¥Ã¨
	status_user = 1,								//—°‘Ò≤Ÿ◊˜»À◊¥Ã¨
	status_sample = 2,								//ªÒ»°—˘∆∑ID◊¥Ã¨
	status_wait1 = 3,								//µ»¥˝≤Âø®1
	status_preread = 4,								//‘§∂¡ø®◊¥Ã¨
	status_start = 5,								//∆Ù∂Ø≈≈∂”
	status_outcard = 6,								//µ»¥˝¥”ø®≤€∞Œ≥ˆ
	status_in_n = 7,								//µπº∆ ±◊¥Ã¨ -- µ»¥˝≤Â»Î≈≈∂”Œª
	status_out_n = 8,								//µπº∆ ±◊¥Ã¨ -- µ»¥˝∞Œ≥ˆ≈≈∂”Œª
	status_incard_n = 9,							//µπº∆ ±◊¥Ã¨ -- µ»¥˝≤Â»Îø®≤€
	status_timedown = 10,							//µπº∆ ±◊¥Ã¨
	status_prereadagain_n = 11,						//µπº∆ ±◊¥Ã¨ -- ‘Ÿ¥Œ‘§∂¡ø®£¨÷˜“™ºÏ≤‚ø®±‰∏¸
	status_in_o = 12,								//≥¨ ±◊¥Ã¨ -- µ»¥˝≤Â»Î≈≈∂”Œª
	status_out_o = 13,								//≥¨ ±◊¥Ã¨ -- µ»¥˝∞Œ≥ˆ≈≈∂”Œª
	status_incard_o = 14,							//≥¨ ±◊¥Ã¨ -- µ»¥˝≤Â»Îø®≤€
	status_timeup = 15,								//≥¨ ±◊¥Ã¨
	status_prereadagain_o = 16,						//≥¨ ±◊¥Ã¨ -- ‘Ÿ¥Œ‘§∂¡ø®£¨÷˜“™ºÏ≤‚ø®±‰∏¸
	status_timedownagain = 17,						//µπº∆ ±ΩÁ√Ê
	status_test = 18,								//≤‚ ‘
}MyPaiDuiStatues;

/**********************************************************************************************************/
/****************************************** ±º‰Ω·ππÃÂ∂®“Â**************************************************/
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
/******************************************Œ¬∂»Ω·ππÃÂ∂®“Â**************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct
{
	float  E_Temperature;			//ª∑æ≥Œ¬∂»
	float	O_Temperature;			//±ª≤‚ŒÔÃÂŒ¬∂»
	float I_Temperature;			//ƒ⁄≤øŒ¬∂»
}MyTemp_Def;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************∂˛Œ¨¬Îœ‡πÿ∂®“Â**************************************************/
/**********************************************************************************************************/


#define	ITEM_NUM	10
#define	ItemNameLen						20			//????????
#define	ItemMeasureLen					10			//????????

#pragma pack(1)
typedef struct ItemConstData_Tag
{
	char itemName[ItemNameLen];									//œÓƒø√˚≥∆
	unsigned short icoIndex;								//≈≈∂”ΩÁ√Êœ‘ æÕº±ÍµƒÀ˜“˝
	unsigned char pointNum;								//–° ˝µ„∏ˆ ˝
	float lowstResult;									//◊ÓµÕºÏ≤‚œﬂ
	float highestResult;								//◊Ó∏ﬂºÏ≤‚œﬂ
	char normalResult[50];								//≤Œøº÷µ
	char itemMeasure[10];								//µ•Œª
}ItemConstData;
#pragma pack()

typedef enum
{
	CardCodeScanning = 0,											//’˝‘⁄…®√Ë
	CardCodeScanOK = 1,												//…®√Ë∂˛Œ¨¬Î≥…π¶
	CardCodeScanFail = 2,											//…®√Ë ß∞‹
	CardCodeTimeOut = 3,											//π˝∆⁄
	CardCodeCardOut = 4,											//ø®±ª∞Œ≥ˆ
	CardCodeScanTimeOut = 5,										//…®√Ë≥¨ ±
	CardCodeCRCError = 6,											//crc¥ÌŒÛ
	CardUnsupported = 7												//µ±«∞≥Ã–Ú≤ª÷ß≥÷
}ScanCodeResult;

#pragma pack(1)
typedef struct QRCode_Tag
{
	unsigned char qu1Ise;									//«˙œﬂ1 «∑Ò «÷∏ ˝
	unsigned char qu2Ise;									//«˙œﬂ2 «∑Ò «÷∏ ˝
	unsigned char qu3Ise;									//«˙œﬂ3 «∑Ò «÷∏ ˝
	float qu1_d;	
	float qu2_d;
	float qu3_d;
	unsigned char parm[5];								//‘§¡Ù
	unsigned short ItemLocation;						//TœﬂŒª÷√
	unsigned char ChannelNum;							//Õ®µ¿∫≈(0-7)
	float	ItemFenDuan[2];								//∑÷∂Œ∑Â∏ﬂ±»
	float	ItemBiaoQu[3][3];							//±Í◊º«˙œﬂ
			//2 --- ?2???
			//4 --- a , b, c d (????)
	unsigned char CardWaitTime;							//ºÏ≤‚ø®∑¥”¶ ±º‰
	unsigned short CLineLocation;						//cœﬂŒª÷√
	char	PiHao[15];									//≈˙¥Œ∫≈
	char	piNum[10];
	MyTime_Def	CardBaoZhiQi;							//±£÷ ∆⁄
	ItemConstData itemConstData;						//¥ÀœÓƒøµƒπÃ∂® ˝æ›
	unsigned char qrVersion;							//qr version in qr version 2.0 inplace crc high byte in qr version 1.0
	unsigned char calMode;								//calculate result mode(1:T/C, 2:T/T+C) in qr version 2.0 inplace crc low byte in qr version 1.0
}QRCode;
#pragma pack()



#define	MaxLocation			6500						//◊Ó¥Û––≥Ã
#define	StartTestLocation	1250							//≤‚ ‘∆ ºŒª÷√
#define	EndTestLocation		4300						//≤‚ ‘Ω· ¯Œ™÷π
#define	WaittingCardLocation	MaxLocation					//µ»¥˝≤Âø®µƒŒª÷√
#define	AvregeNum		10								//∆Ωæ˘÷µ¬À≤®∏ˆ ˝
#define	FilterNum		5								//??????
#define	MaxPointLen	((EndTestLocation - StartTestLocation)/AvregeNum - FilterNum) //≤‚ ‘µ„∏ˆ ˝


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************—˘∆∑IDœ‡πÿ∂®“Â**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//◊ÓÛ÷ß≥÷—˘∆∑id≥§∂»Œ™20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************ΩÁ√Êœ‡πÿ∂®“Â****************************************************/
/***************************************************************************¥*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				// µ ±«˙œﬂ◊Ó¥Ûµƒ ˝÷µ
	unsigned short Y_Scale;				//«˙œﬂy÷·◊ÓµÕµƒøÃ∂»
	unsigned short MUL_Y;				//Y÷·∑≈¥Û±∂ ˝
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************∆¡ƒªœ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//–≠“ÈÕ∑1
#define	LCD_Head_2		0xBB											//–≠“ÈÕ∑2

#define W_REGSITER 		0x80          									//–¥»Î ˝æ›,Œﬁ”¶¥          
#define R_REGSITER 	    0x81          									//∂¡»° ˝æ›£¨”–”¶¥
#define W_ADDRESS  		0x82											//–¥±‰¡øºƒ¥Ê∆˜£¨Œﬁ”¶¥
#define R_ADDRESS  	    0x83											//∂¡±‰¡øºƒ¥Ê∆˜ ˝æ›£¨”–”¶¥
#define W_LINE     		0x84          									//–¥«˙œﬂª∫≥Â«¯£¨Œﬁ”¶¥

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
/******************************************≤Ÿ◊˜»Àœ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/
#define	MaxUserNum	20													//◊Ó∂‡±£¥Êµƒ”√ªß ˝ƒø
#define	MaxPageShowNum	5												//“ª“≥œ‘ æ”√ªß ˝ƒø

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
/******************************************…Ë±∏–≈œ¢œ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************◊‘ºÏπ¶ƒ‹œ‡πÿ∂®“Â****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//◊‘ºÏŒ¥ø™ º
	SelfChecking = 1,
	SelfCheck_OK = 2,													//◊‘ºÏ’˝≥£
	SelfCheck_Error = 3,												//◊‘ºÏ”–≤ªø…∫ˆ¬‘µƒ¥ÌŒÛ
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxDeviceIDLen	21
#define	MaxDeviceUnitLen	51

/*…Ë±∏–≈œ¢…Ë÷√*/
#pragma pack(1)
typedef struct DeviceInfo_Tag
{
	char deviceid[MaxDeviceIDLen];													//…Ë±∏id
	char deviceunit[MaxDeviceUnitLen];												//…Ë±∏ π”√µ•Œª
	User_Type deviceuser;															//…Ë±∏ π”√»À
	MyBool isnew;															//…Ë±∏–≈œ¢ «∑Ò”–∏¸–¬
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
/******************************************Õ¯¬Áœ‡πÿ∂®“Â****************************************************/
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

#define	MaxSaveWifiNum	50												//◊Ó∂‡±£¥Ê50∏ˆ»»µ„
#define	MaxWifiListNum	20												//◊Ó∂‡œ‘ æ20∏ˆ»»µ„
#define	PageWifiNum		8
#define	MaxSSIDLen		50
#define	MaxKEYLen		20

/*wifi»»µ„*/
#pragma pack(1)
typedef struct wifi_Tag
{
	char ssid[MaxSSIDLen];														//SSID
	char auth[20];														//»œ÷§ƒ£ Ω
	char encry[10];														//º”√‹∑Ω Ω
	unsigned char indicator;											//–≈∫≈«ø∂»
	char key[MaxKEYLen];														//√‹¬Î
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



/*ipªÒ»°∑Ω Ω*/
typedef enum
{
	DHCP_Mode = 1,														//◊‘∂ØªÒ»°∂ØÃ¨ip
	User_Mode = 2														//”√ªß…Ë÷√ip
}NetIP_Type;



/*”–œﬂÕ¯…Ë÷√*/
#pragma pack(1)
typedef struct NetSet_Tag
{
	IP_Def myip;														//Œ“µƒip
	NetIP_Type ipmode;
	unsigned short crc;
}NetSet;
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP_Def serverIP;														//∑˛ŒÒ∆˜IP
	unsigned short serverPort;											//∑˛ŒÒ∆˜∂Àø⁄∫≈
	unsigned short crc;
}ServerSet; 
#pragma pack()

#define	ServerStructSize	sizeof(ServerSet)


/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000
#define MAX_TEST_CNT        5                       //◊Ó¥Û÷ÿ≤‚¥Œ ˝
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************∂® ±∆˜********************************************************/
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
	TimeNotTo = 0,		//º∆ ± ±º‰Œ¥µΩ
	TimeOut = 1,		//º∆ ± ±º‰µΩ
	TimeError = 2,		//¥ÌŒÛ
}TimerState_Def;


typedef enum
{
	NoResult = 0,										//ªπŒ¥≤‚ ‘£¨Ω·π˚Œ¥null
	NoSample = 1,										//Œ¥º”—˘
	PeakError = 2,										//ºÏ≤‚ø®√ª”–C∑Â
	MemError = 3,										//ƒ⁄¥Ê¥ÌŒÛ
    T_CV_0_05 = 4,                                      //’“≤ªµΩT£¨µ´ «ƒ¨»œŒª÷√µƒcv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //cŒª÷√–°”⁄TŒª÷√
    C_L_T_L_2 = 9,                                        //cŒª÷√ - TŒª÷√ (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //≤–¡Ù
    B_V_ERR = 12,                                       //ª˘œﬂ÷µ¥ÌŒÛ
	TestInterrupt = 8,									//≤‚ ‘÷–∂œ
	ResultIsOK = 99,									//≤‚ ‘≥…π¶
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
	unsigned char CMdifyNum;					//cœﬂ≤π≥•±∂ ˝£¨*10
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
	MyTime_Def TestTime;						//≤‚ ‘ ±º‰
	MyTemp_Def TestTemp;						//≤‚ ‘Œ¬∂»
	unsigned short time;						//≥¨ ± ±º‰
	ResultState testResultDesc;					//≤‚ ‘Ω·π˚◊¥Ã¨
	unsigned short crc;
}TestData;
#pragma pack()

typedef struct ItemData_tag {
	TestData testdata;
	Timer timer;																	//≥£πÊµπº∆ ±º∆ ±∆˜
	Timer timer2;																	//≥¨ ±µπº∆ ±º∆ ±∆˜
	Timer timer3;																	//≥¨ ±µπº∆ ±º∆ ±∆˜
	unsigned char errorcount;														//Œ¥≤Ÿ◊˜¥Œ ˝
	MyPaiDuiStatues statues;														//≤‚ ‘Ω◊∂Œ
	unsigned char jieduan;															//≤‚ ‘Ω◊∂Œ
	unsigned char testlocation;
	unsigned short ledLight;														//≤‚ ‘ ±ledµƒ¡¡∂»£¨Œ™œµÕ≥…Ë÷√÷–±£¥Êµƒ–£◊ºled÷µ
}ItemData;

#define	TestDataStructSize	sizeof(TestData)
#define	TestDataStructCrcSize	TestDataStructSize - 2

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************¿œªØ≤‚ ‘ ˝æ›**************************************************/
/*********************************************************************************************/

typedef struct ReTestData_tag{
	unsigned int retestcount;								//◊‹¥Œ ˝
	unsigned short retestsurpluscount;						// £”‡≤‚ ‘¥Œ ˝
	unsigned short retestedcount;							//“—æ≠≤‚ ‘¥Œ ˝
	unsigned char reteststatus;								//¿œªØ≤‚ ‘◊¥Ã¨£¨0Õ£÷π£¨∑«0≤‚ ‘
	unsigned char retestStep;								//≤‚ ‘≤Ω÷Ë£¨1µ»¥˝≤Âø®£¨2∂¡∂˛Œ¨¬Î£¨3∂¡ø®
	Timer retesttimer;										//¿œªØ≤‚ ‘º∆ ±∆˜
	Timer oneretesttimer;									//“ª¥Œ¿œªØ≤‚ ‘º∆ ±∆˜
	ItemData itemData;
	char result[30];										//µ±«∞≤‚ ‘µƒΩ·¬€
	unsigned short playcount;								//¿œªØ≤•∑≈º∆ ˝∆˜£¨º«¬º≤•∑≈¥Œ ˝
	Timer playtimer;										//¿œªØ“Ù∆µ◊‹ ±º‰	
	Timer oneplaytimer;										//¿œªØ≤•∑≈º∆ ±∆˜£¨º«¬º√ø¥Œ≤•∑≈µƒ ±≥§
	MyTime_Def startplayTime;								//µ±«∞“Ù∆µ∆ º ±º‰
	MyTime_Def endplayTime;									//µ±«∞“Ù∆µΩ· ¯ ±º‰
	double advalue1;										//DA÷µ100 ±µƒAD÷µ
	double advalue2;										//DA÷µ200 ±µƒAD÷µ
	double advalue3;										//DA÷µ300 ±µƒAD÷µ
	MyLEDCheck_TypeDef ledstatus;							//∑¢π‚ƒ£øÈ◊¥Ã¨
	unsigned char playstatus;								//≤•∑≈◊¥Ã¨
}ReTestData;


#endif

