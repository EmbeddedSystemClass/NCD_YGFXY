#ifndef __WELCOME_P_H__
#define __WELCOME_P_H__


#include	"Timer_Data.h"

typedef struct WelcomePageBuffer_tag {
	Timer timer;													//ҳ�泬ʱʱ��
	unsigned char tempv1;											//��ʱ����
	unsigned short lcdinput[100];									//lcd�������
}WelcomePageBuffer;

unsigned char DspWelcomePage(void *  parm);

#endif

