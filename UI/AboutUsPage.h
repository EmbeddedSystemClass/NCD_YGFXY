#ifndef __ABOUTUS_P_H__
#define __ABOUTUS_P_H__

#include	"UI_Data.h"
#include	"Timer_Data.h"

typedef struct AboutUsPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	char deviceId[MaxDeviceIDLen];
	unsigned short tempVersion;
	Timer timer;
}AboutUsPageBuffer;

MyState_TypeDef createAboutUsActivity(Activity * thizActivity, Intent * pram);

#endif

