/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"Define.h"

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

