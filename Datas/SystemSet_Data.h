/****************************************file start****************************************************/
#ifndef SYSTEMSET_D_H
#define	SYSTEMSET_D_H

#include	"Define.h"

#pragma pack(1)
typedef struct SystemSetData_Tag
{
	DeviceInfo deviceInfo;												//�豸��Ϣ
	NetSet netSet;														//����������
	MyBool isAutoPrint;													//�Ƿ��Զ���ӡ
	MyBool isMute;														//�Ƿ���
	unsigned short ledSleepTime;										//����ʱ�� s
	unsigned char ledLightIntensity;									//��Ļ���� 0-100
	unsigned int testDataNum;											//���������ѱ������Ŀ
	unsigned int upLoadIndex;											//�������ݵȴ��ϴ�������
	unsigned short testLedLightIntensity;								//����ʱ���õ�led����ֵ��ΪУ׼�������Ĭ��Ϊ200
	unsigned char parm1[700];
	unsigned int userUpLoadIndex;										//�û��������ϴ�����
	ServerSet serverSet;												//����������
    PrintInfo printInfo;                                                //��ӡ����
	unsigned char parm2[116];											//Ԥ������1
	
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
