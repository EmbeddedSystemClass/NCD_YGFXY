/****************************************file start****************************************************/
#ifndef SYSTEMSET_D_H
#define	SYSTEMSET_D_H

#include	"Define.h"


#pragma pack(1)
typedef struct SystemSetData_Tag
{
	DeviceInfo deviceInfo;												//�豸��Ϣ
	NetSet netSet;														//����������
	bool isAutoPrint;													//�Ƿ��Զ���ӡ
	bool isMute;														//�Ƿ���
	unsigned short ledSleepTime;										//����ʱ�� s
	unsigned char ledLightIntensity;									//��Ļ���� 0-100
	unsigned int testDataNum;												//���������ѱ������Ŀ
	unsigned int upLoadIndex;												//�������ݵȴ��ϴ�������
	unsigned short crc;
}SystemSetData;
#pragma pack()

void setDefaultSetData(SystemSetData * systemSetData);

void getSystemSetData(SystemSetData * systemSetData);
void setSystemSetData(SystemSetData * systemSetData);

bool deviceInfoIsNew(void);
void getDeviceInfo(DeviceInfo * deviceinfo);
void getNetSet(NetSet * netSet);
bool isAutoPrint(void);
bool isMute(void);
unsigned short getLedSleepTime(void);
unsigned char getLedLightIntensity(void);

void plusTestDataTotalNum(unsigned char num);
unsigned int getTestDataTotalNum(void);

void setUpLoadIndex(unsigned int index);

#endif

/****************************************end of file************************************************/
