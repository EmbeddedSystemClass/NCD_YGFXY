#ifndef __RCODE_F_H__
#define __RCODE_F_H__

#include	"Define.h"

#pragma pack(1)
typedef struct ReadCodeBuffer_tag {
	char originalcode[320];						//ԭʼ�Ķ�ά��
	unsigned short rxcount;						//��ǽ������ݳ���
	char decryptcode[320];						//���ܺ�Ķ�ά��
	char tempbuf[64];							//��ʱ����
	char * pbuf1;								//��ʱָ��1
	char * pbuf2;								//��ʱָ��2
	MyTime_Def temptime;						//���浱ǰʱ��
} ReadCodeBuffer;
#pragma pack()

ScanCodeResult ScanCodeFun(void * parm);

#endif

