#ifndef __SDI_P_H__
#define __SDI_P_H__

#include	"Define.h"

typedef struct SetDeviceInfoPageBuffer_tag {
	DeviceInfo temp_deviceinfo;							//��ʱ����
	unsigned char ismodify;
}SetDeviceInfoPageBuffer;


unsigned char DspSetDeviceInfoPage(void *  parm);

#endif

