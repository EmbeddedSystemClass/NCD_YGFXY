#ifndef __WELCOME_P_H__
#define __WELCOME_P_H__

#include	"MyStack.h"
#include	"Timer_Data.h"
#include	"SystemSet_Data.h"

typedef struct WelcomePageBuffer_tag {
	SystemSetData systemSetData;
	Timer timer;													//ҳ�泬ʱʱ��
	unsigned char tempv1;											//��ʱ����
	unsigned short lcdinput[100];									//lcd�������
}WelcomePageBuffer;

MyState_TypeDef createWelcomeActivity(Activity * thizActivity, void * pram);

#endif

