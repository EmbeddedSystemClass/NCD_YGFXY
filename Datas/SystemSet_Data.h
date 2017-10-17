/****************************************file start****************************************************/
#ifndef SYSTEMSET_D_H
#define	SYSTEMSET_D_H

#include	"Define.h"

#define		MaxAdjDataNum	50											//��ౣ��50��Ʒ�ֵ�У׼����

#pragma pack(1)
typedef struct SystemSetData_Tag
{
	DeviceInfo deviceInfo;												//�豸��Ϣ
	NetSet netSet;														//����������
	bool isAutoPrint;													//�Ƿ��Զ���ӡ
	bool isMute;														//�Ƿ���
	unsigned short ledSleepTime;										//����ʱ�� s
	unsigned char ledLightIntensity;									//��Ļ���� 0-100
	unsigned int testDataNum;											//���������ѱ������Ŀ
	unsigned int upLoadIndex;											//�������ݵȴ��ϴ�������
	unsigned short testLedLightIntensity;								//����ʱ���õ�led����ֵ��ΪУ׼�������Ĭ��Ϊ200
	AdjustData adjustData[MaxAdjDataNum];
	unsigned int userUpLoadIndex;										//�û��������ϴ�����
	unsigned char parm1[96];											//Ԥ������1
	unsigned short parm2[10];											//Ԥ������2
	unsigned int parm3[10];												//Ԥ������3
	unsigned short crc;
}SystemSetData;
#pragma pack()

void setDefaultSystemSetData(SystemSetData * systemSetData);

void copyGBSystemSetData(SystemSetData * systemSetData);
SystemSetData * getGBSystemSetData(void);
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

void setTestLedLightIntensity(SystemSetData * systemSetData, unsigned short value);
unsigned short getTestLedLightIntensity(SystemSetData * systemSetData);

MyState_TypeDef addAdjPram(SystemSetData * systemSetData, AdjustData * adjData);
void getAdjPram(SystemSetData * systemSetData, AdjustData * adjData);
void getAllAdjPram(SystemSetData * systemSetData, AdjustData * adjData);

void setIsShowRealValue(bool isShow);
bool IsShowRealValue(void);

#endif

/****************************************end of file************************************************/
