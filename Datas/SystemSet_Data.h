/****************************************file start****************************************************/
#ifndef SYSTEMSET_D_H
#define	SYSTEMSET_D_H

#include	"Define.h"

#pragma pack(1)
typedef struct SystemSetData_Tag
{
	DeviceInfo deviceInfo;												//设备信息
	NetSet netSet;														//有线网设置
	MyBool isAutoPrint;													//是否自动打印
	MyBool isMute;														//是否静音
	unsigned short ledSleepTime;										//休眠时间 s
	unsigned char ledLightIntensity;									//屏幕亮度 0-100
	unsigned int testDataNum;											//测试数据已保存的数目
	unsigned int upLoadIndex;											//测试数据等待上传的索引
	unsigned short testLedLightIntensity;								//测试时采用的led亮度值，为校准后产生，默认为200
	unsigned char parm1[700];
	unsigned int userUpLoadIndex;										//用户服务器上传索引
	ServerSet serverSet;												//服务器设置
    PrintInfo printInfo;                                                //打印设置
	unsigned char parm2[116];											//预留参数1
	
	unsigned short crc;
}SystemSetData;
#pragma pack()

void setDefaultSystemSetData(SystemSetData * systemSetData);

void copyGBSystemSetData(SystemSetData * systemSetData);
SystemSetData * getGBSystemSetData(void);
void setSystemSetData(SystemSetData * systemSetData);

MyBool deviceInfoIsNew(void);
void set_device_state(MyBool state);
void getDeviceInfo(DeviceInfo * deviceinfo);
void readDeviceId(char * buf);
void readDeviceAddr(char * buf);
void getNetSet(NetSet * netSet);
MyBool isAutoPrint(void);
MyBool isMute(void);
unsigned short getLedSleepTime(void);
unsigned char getLedLightIntensity(void);

void plusTestDataTotalNum(unsigned char num);
unsigned int getTestDataTotalNum(void);
void setUpLoadIndex(unsigned int index);
unsigned int getUpLoadIndex(void);
unsigned int getUserUpLoadIndex(void);
	
void setTestLedLightIntensity(SystemSetData * systemSetData, unsigned short value);
unsigned short getTestLedLightIntensity(SystemSetData * systemSetData);

void setIsShowRealValue(MyBool isShow);
MyBool IsShowRealValue(void);
void setDefaultServerData(ServerSet * server);
void getGBServerData(ServerSet * server);
void readPrintInfo(PrintInfo * printInfo);
#endif

/****************************************end of file************************************************/
