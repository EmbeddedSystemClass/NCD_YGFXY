#ifndef __SERVERSET_P_H__
#define __SERVERSET_P_H__

#include	"Define.h"
#include	"SystemSet_Data.h"
#include	"UI_Data.h"

typedef struct ServerSetPageBuffer_tag {
	ServerSet serverSet;
	SystemSetData systemSetData;
	char buf[100];
	unsigned short lcdinput[50];
}ServerSetPageBuffer;


MyState_TypeDef createServerSetActivity(Activity * thizActivity, Intent * pram);

#endif

