#ifndef _DEFINE_H_H
#define _DEFINE_H_H

#include	"stm32f4xx.h"
#include 	"FreeRTOS.h"
#include	"ff.h"
/**************************************************************************************************/
/***************************************************************************************************/
/*****************************************软件版本**************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
#define QRVersion0Define		0								//二维码原始版本定义
#define QRVersion2Define		'2'								//二维码版本2定义
#define QRVersion3Define		'3'								//二维码版本3定义
#define QRVersionUnDefine		0xff							//不支持二维码版本
#define	GB_QRVersion			QRVersion3Define				//当前支持的二维码版本，往下兼容

//device con type
#define	DEVICE_WIFI					    0x88
#define	DEVICE_GPRS					    0x89
#define	DEVICE_CON_TYPE					DEVICE_GPRS

//build by device id
#define	DeviceAll						0x30
#define	DeviceNCD13011703019			0x31                    //废弃
#define	DeviceBuildId					DeviceAll	

#if (DEVICE_CON_TYPE == DEVICE_WIFI)
    #if (DeviceBuildId == DeviceAll)

        #define	GB_SoftVersion			(unsigned short)1080
        #define	GB_SoftVersionStr		"V1.0.80\0"
        #define	GB_SoftVersion_Build	"Build18082701\0"
        
    #elif (DeviceBuildId == DeviceNCD13011703019)

        #define	GB_SoftVersion			(unsigned short)1079
        #define	GB_SoftVersionStr		"V1.0.79\0"
        #define	GB_SoftVersion_Build	"Build18071001\0"

    #endif  //DeviceBuildId
#elif (DEVICE_CON_TYPE == DEVICE_GPRS)
    #if (DeviceBuildId == DeviceAll)

        #define	GB_SoftVersion			(unsigned short)1001
        #define	GB_SoftVersionStr		"V1.0.01\0"
        #define	GB_SoftVersion_Build	"Build18082901\0"

    #endif  //DeviceBuildId
#endif  //DEVICE_CON_TYPE
	

#define	DEVICE_EN						100
#define	DEVICE_CN						101
#define	DeviceLanguage					DEVICE_CN

#define	Device_Final					0x27
#define	Device_Demo						0x28
#define	Device_FastDemo					0x29
#define	DeviceUseType					Device_FastDemo

#define	UserNormalProgram				0x60					//通用程序
#define	UserXGProgram					0x61					//孝感中心医院程序
#define	UserProgramType					UserNormalProgram			//程序用户选择定义

#if (UserProgramType == UserNormalProgram)
	#define	USB_USE							1
#elif (UserProgramType == UserXGProgram)
	#define	USB_USE							0
#endif

/*服务器信息*/
#define	GB_ServerIp_1		116
#define	GB_ServerIp_2		62
#define	GB_ServerIp_3		108
#define	GB_ServerIp_4		201
#define	GB_ServerPort		8080

/*用户服务器*/
#define	GB_UserServerIp_1		192
#define	GB_UserServerIp_2		168
#define	GB_UserServerIp_3		0
#define	GB_UserServerIp_4		37
#define	GB_UserServerPort		9200
#define	GBDefaultPoctServer				"/NCDPoctServer/UpHandler\0"

#define FREERTOS_DELAY_0            0
#define FREERTOS_DELAY_500_MS      500
#define FREERTOS_DELAY_1_S          1000
#define FREERTOS_DELAY_2_S          2000
#define FREERTOS_DELAY_20_S          20000l
#define FREERTOS_DELAY_30_S          30000l
#define FREERTOS_DELAY_40_S          40000l
/**********************************************************************************************************/
/******************************************操作结果变量*************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	My_Pass = pdPASS,			//操作成功
	My_Fail = pdFAIL			//操作失败
}MyState_TypeDef;

typedef enum
{ 
	TRUE = 1,
	FALSE = 0
}MyBool;

typedef enum
{ 
	LED_Error = 1,		//led错误
	LED_OK = 0			//led正常
}MyLEDCheck_TypeDef;
/**********************************************************************************************************/
/**********************************************************************************************************/
typedef enum
{ 
	NormalTestType = 0,																	//常规测试
	PaiDuiTestType = 1																	//排队测试
}TestType;
/**********************************************************************************************************/
/******************************************fatfs结构体定义*************************************************/
/**********************************************************************************************************/
/*此结构体用于在进行sd操作是申请内存*/
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

#define	PaiDuiWeiNum	8							//排队位置数目
typedef enum
{
	statusNull = 0,									//没开始,默认状态
	status_user = 1,								//选择操作人状态
	status_sample = 2,								//获取样品ID状态
	status_wait1 = 3,								//等待插卡1
	status_preread = 4,								//预读卡状态
	status_start = 5,								//启动排队
	status_outcard = 6,								//等待从卡槽拔出
	status_in_n = 7,								//倒计时状态 -- 等待插入排队位
	status_out_n = 8,								//倒计时状态 -- 等待拔出排队位
	status_incard_n = 9,							//倒计时状态 -- 等待插入卡槽
	status_timedown = 10,							//倒计时状态
	status_prereadagain_n = 11,						//倒计时状态 -- 再次预读卡，主要检测卡变更
	status_in_o = 12,								//超时状态 -- 等待插入排队位
	status_out_o = 13,								//超时状态 -- 等待拔出排队位
	status_incard_o = 14,							//超时状态 -- 等待插入卡槽
	status_timeup = 15,								//超时状态
	status_prereadagain_o = 16,						//超时状态 -- 再次预读卡，主要检测卡变更
	status_timedownagain = 17,						//倒计时界面
	status_test = 18,								//测试
}MyPaiDuiStatues;

/**********************************************************************************************************/
/******************************************时间结构体定义**************************************************/
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
/******************************************温度结构体定义**************************************************/
/**********************************************************************************************************/
#pragma pack(1)
typedef struct
{
	float  E_Temperature;			//环境温度
	float	O_Temperature;			//被测物体温度
	float I_Temperature;			//内部温度
}MyTemp_Def;
#pragma pack()
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************二维码相关定义**************************************************/
/**********************************************************************************************************/


#define	ITEM_NUM	10
#define	ItemNameLen						20			//????????
#define	ItemMeasureLen					10			//????????

#pragma pack(1)
typedef struct ItemConstData_Tag
{
	char itemName[ItemNameLen];									//项目名称
	unsigned short icoIndex;								//排队界面显示图标的索引
	unsigned char pointNum;								//小数点个数
	float lowstResult;									//最低检测线
	float highestResult;								//最高检测线
	char normalResult[50];								//参考值
	char itemMeasure[10];								//单位
}ItemConstData;
#pragma pack()

typedef enum
{
	CardCodeScanning = 0,											//正在扫描
	CardCodeScanOK = 1,												//扫描二维码成功
	CardCodeScanFail = 2,											//扫描失败
	CardCodeTimeOut = 3,											//过期
	CardCodeCardOut = 4,											//卡被拔出
	CardCodeScanTimeOut = 5,										//扫描超时
	CardCodeCRCError = 6,											//crc错误
	CardUnsupported = 7												//当前程序不支持
}ScanCodeResult;

#pragma pack(1)
typedef struct QRCode_Tag
{
	unsigned char qu1Ise;									//曲线1是否是指数
	unsigned char qu2Ise;									//曲线2是否是指数
	unsigned char qu3Ise;									//曲线3是否是指数
	float qu1_d;	
	float qu2_d;
	float qu3_d;
	unsigned char parm[5];								//预留
	unsigned short ItemLocation;						//T线位置
	unsigned char ChannelNum;							//通道号(0-7)
	float	ItemFenDuan[2];								//分段峰高比
	float	ItemBiaoQu[3][3];							//标准曲线
			//2 --- ?2???
			//4 --- a , b, c d (????)
	unsigned char CardWaitTime;							//检测卡反应时间
	unsigned short CLineLocation;						//c线位置
	char	PiHao[15];									//批次号
	char	piNum[10];
	MyTime_Def	CardBaoZhiQi;							//保质期
	ItemConstData itemConstData;						//此项目的固定数据
	unsigned char qrVersion;							//qr version in qr version 2.0 inplace crc high byte in qr version 1.0
	unsigned char calMode;								//calculate result mode(1:T/C, 2:T/T+C) in qr version 2.0 inplace crc low byte in qr version 1.0
}QRCode;
#pragma pack()



#define	MaxLocation			6500						//最大行程
#define	StartTestLocation	1250							//测试起始位置
#define	EndTestLocation		4300						//测试结束为止
#define	WaittingCardLocation	MaxLocation					//等待插卡的位置
#define	AvregeNum		10								//平均值滤波个数
#define	FilterNum		5								//??????
#define	MaxPointLen	((EndTestLocation - StartTestLocation)/AvregeNum - FilterNum) //测试点个数


/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************样品ID相关定义**************************************************/
/**********************************************************************************************************/
#define	MaxSampleIDLen	20											//最笾С盅穒d长度为20
/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************界面相关定义****************************************************/
/***************************************************************************�*******************************/

typedef struct LineInfo_Tag
{
	unsigned short MaxData;				//实时曲线最大的数值
	unsigned short Y_Scale;				//曲线y轴最低的刻度
	unsigned short MUL_Y;				//Y轴放大倍数
}LineInfo;

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************屏幕相关定义****************************************************/
/**********************************************************************************************************/
#define	LCD_Head_1		0xAA											//协议头1
#define	LCD_Head_2		0xBB											//协议头2

#define W_REGSITER 		0x80          									//写入数据,无应答          
#define R_REGSITER 	    0x81          									//读取数据，有应答
#define W_ADDRESS  		0x82											//写变量寄存器，无应答
#define R_ADDRESS  	    0x83											//读变量寄存器数据，有应答
#define W_LINE     		0x84          									//写曲线缓冲区，无应答

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
/******************************************操作人相关定义****************************************************/
/**********************************************************************************************************/
#define	MaxUserNum	20													//最多保存的用户数目
#define	MaxPageShowNum	5												//一页显示用户数目

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
/******************************************设备信息相关定义****************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/**********************************************************************************************************/

/**********************************************************************************************************/
/******************************************自检功能相关定义****************************************************/
/**********************************************************************************************************/



typedef enum
{
	SelfCheck_None = 0,													//自检未开始
	SelfChecking = 1,
	SelfCheck_OK = 2,													//自检正常
	SelfCheck_Error = 3,												//自检有不可忽略的错误
}SelfCheckStatus;
/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxDeviceIDLen	21
#define	MaxDeviceUnitLen	51

/*设备信息设置*/
#pragma pack(1)
typedef struct DeviceInfo_Tag
{
	char deviceid[MaxDeviceIDLen];													//设备id
	char deviceunit[MaxDeviceUnitLen];												//设备使用单位
	User_Type deviceuser;															//设备使用人
	MyBool isnew;															//设备信息是否有更新
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
/******************************************网络相关定义****************************************************/
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

#define	MaxSaveWifiNum	50												//最多保存50个热点
#define	MaxWifiListNum	20												//最多显示20个热点
#define	PageWifiNum		8
#define	MaxSSIDLen		50
#define	MaxKEYLen		20

/*wifi热点*/
#pragma pack(1)
typedef struct wifi_Tag
{
	char ssid[MaxSSIDLen];														//SSID
	char auth[20];														//认证模式
	char encry[10];														//加密方式
	unsigned char indicator;											//信号强度
	char key[MaxKEYLen];														//密码
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



/*ip获取方式*/
typedef enum
{
	DHCP_Mode = 1,														//自动获取动态ip
	User_Mode = 2														//用户设置ip
}NetIP_Type;



/*有线网设置*/
#pragma pack(1)
typedef struct NetSet_Tag
{
	IP_Def myip;														//我的ip
	NetIP_Type ipmode;
	unsigned short crc;
}NetSet;
#pragma pack()

#pragma pack(1)
typedef struct
{
	IP_Def serverIP;														//服务器IP
	unsigned short serverPort;											//服务器端口号
	unsigned short crc;
}ServerSet; 
#pragma pack()

#define	ServerStructSize	sizeof(ServerSet)


/**********************************************************************************************************/
/**********************************************************************************************************/

#define	MaxTestDataSaveNum	(unsigned int)1000000
#define MAX_TEST_CNT        5                       //最大重测次数
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************定时器********************************************************/
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
	TimeNotTo = 0,		//计时时间未到
	TimeOut = 1,		//计时时间到
	TimeError = 2,		//错误
}TimerState_Def;


typedef enum
{
	NoResult = 0,										//还未测试，结果未null
	NoSample = 1,										//未加样
	PeakError = 2,										//检测卡没有C峰
	MemError = 3,										//内存错误
    T_CV_0_05 = 4,                                      //找不到T，但是默认位置的cv>0.05
    C_CV_ERR_1 = 5,
    C_CV_ERR_2 = 6,
    C_L_T_L_1 = 7,                                        //c位置小于T位置
    C_L_T_L_2 = 9,                                        //c位置 - T位置 (50-100)
    C_CV_T_CV = 10,                                     //c_cv + t_cv < 0.13
    CANLIU = 11,                                        //残留
    B_V_ERR = 12,                                       //基线值错误
	TestInterrupt = 8,									//测试中断
	ResultIsOK = 99,									//测试成功
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
	unsigned char CMdifyNum;					//c线补偿倍数，*10
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
	MyTime_Def TestTime;						//测试时间
	MyTemp_Def TestTemp;						//测试温度
	unsigned short time;						//超时时间
	ResultState testResultDesc;					//测试结果状态
	unsigned short crc;
}TestData;
#pragma pack()

typedef struct ItemData_tag {
	TestData testdata;
	Timer timer;																	//常规倒计时计时器
	Timer timer2;																	//超时倒计时计时器
	Timer timer3;																	//超时倒计时计时器
	unsigned char errorcount;														//未操作次数
	MyPaiDuiStatues statues;														//测试阶段
	unsigned char jieduan;															//测试阶段
	unsigned char testlocation;
	unsigned short ledLight;														//测试时led的亮度，为系统设置中保存的校准led值
}ItemData;

#define	TestDataStructSize	sizeof(TestData)
#define	TestDataStructCrcSize	TestDataStructSize - 2

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
/*******************************老化测试数据**************************************************/
/*********************************************************************************************/

typedef struct ReTestData_tag{
	unsigned int retestcount;								//总次数
	unsigned short retestsurpluscount;						//剩余测试次数
	unsigned short retestedcount;							//已经测试次数
	unsigned char reteststatus;								//老化测试状态，0停止，非0测试
	unsigned char retestStep;								//测试步骤，1等待插卡，2读二维码，3读卡
	Timer retesttimer;										//老化测试计时器
	Timer oneretesttimer;									//一次老化测试计时器
	ItemData itemData;
	char result[30];										//当前测试的结论
	unsigned short playcount;								//老化播放计数器，记录播放次数
	Timer playtimer;										//老化音频总时间	
	Timer oneplaytimer;										//老化播放计时器，记录每次播放的时长
	MyTime_Def startplayTime;								//当前音频起始时间
	MyTime_Def endplayTime;									//当前音频结束时间
	double advalue1;										//DA值100时的AD值
	double advalue2;										//DA值200时的AD值
	double advalue3;										//DA值300时的AD值
	MyLEDCheck_TypeDef ledstatus;							//发光模块状态
	unsigned char playstatus;								//播放状态
}ReTestData;


#endif

