#ifndef __SETDEVICEID_P_H__
#define __SETDEVICEID_P_H__

#include	"Define.h"

typedef struct SetDeviceIDPage_tag {
	DeviceInfo temp_deviceinfo;							//��ʱ����
	unsigned char tempbuf[150];								//��ʱָ��
	unsigned char ismodify;
	unsigned short lcdinput[100];
}SetDeviceIDPage;

unsigned char DspSetDeviceIDPage(void *  parm);

#endif

