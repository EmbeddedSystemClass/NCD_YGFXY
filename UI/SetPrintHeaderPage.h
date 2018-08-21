#ifndef __SETPRINTHEADER_P_H__
#define __SETPRINTHEADER_P_H__

#include	"SystemSet_Data.h"
#include	"UI_Data.h"
#include    "Define.h"

typedef struct SetPrintHeaderPageBuffer_tag {
	SystemSetData systemSetData;
	PrintInfo printInfo;
	unsigned short lcdinput[100];
	char tempBuf[100];
    unsigned short tempShort;
    MyBool ismodify;
}SetPrintHeaderPageBuffer;


MyState_TypeDef createSetPrintHeaderActivity(Activity * thizActivity, Intent * pram);

#endif

