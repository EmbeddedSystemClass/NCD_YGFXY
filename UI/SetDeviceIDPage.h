#ifndef __SETDEVICEID_P_H__
#define __SETDEVICEID_P_H__

#include	"Define.h"

typedef struct SetDeviceIDPage_tag {
	DeviceInfo temp_deviceinfo;							//��ʱ����
	char *tempbuf;								//��ʱָ��
	unsigned char ismodify;
}SetDeviceIDPage;

unsigned char DspSetDeviceIDPage(void *  parm);

#endif

