/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"Define.h"

//�豸��Ϣ
void SetGB_DeviceInfo(DeviceInfo *info);
void GetGB_DeviceInfo(DeviceInfo *info);
DeviceInfo * GetGB_DeviceInfo2(void);

//�豸��Ϣ�Ƿ��и���
void SetDeviceInIsFresh(bool status);
bool GetDeviceInIsFresh(void);

void * GetTestDataForLab(void);
void SetTestStatusFlorLab(unsigned char status);
unsigned char GetTestStatusFlorLab(void);

//����ǹ����
void GetGB_BarCode(void *buf);
void SetGB_BarCode(void *buf);

//ϵͳʱ��
void GetGB_Time(void * time);
void SetGB_Time(void * time);

//ϵͳ��������
void GetGB_NetConfigureData(NetData * netdata);
void SetGB_NetConfigureData(NetData * netdata);
NetIP_Type GetGB_IpModeData(void);
	
#endif

/****************************************end of file************************************************/

