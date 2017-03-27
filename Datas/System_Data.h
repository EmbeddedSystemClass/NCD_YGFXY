/****************************************file start****************************************************/
#ifndef	SYSTEM_D_H
#define	SYSTEM_D_H

#include	"Define.h"

typedef enum
{
	Connect_Ok = 1,
	Connect_Error = 0
}PaiduiStatus;

void * GetTestDataForLab(void);
void SetTestStatusFlorLab(unsigned char status);
unsigned char GetTestStatusFlorLab(void);

//ϵͳʱ��
void GetGB_Time(void * time);
void SetGB_Time(void * time);

//�����¶�
float GetGB_EnTemperature(void);
void SetGB_EnTemperature(float temp);

//�Ŷ�ģ������״̬
void setPaiduiModuleStatus(PaiduiStatus paiduiStatus);
PaiduiStatus getPaiduiModuleStatus(void);
	
#endif

/****************************************end of file************************************************/

