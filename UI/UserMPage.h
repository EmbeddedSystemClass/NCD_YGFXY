#ifndef __USERM_P_H__
#define __USERM_P_H__

#include	"Define.h"
#include	"UI_Data.h"

typedef struct UserMPageBuffer_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	unsigned char filemaxitem;							//��ǰ�ļ�����������Ŀ
	unsigned char filemaxpagenum;						//��ǰ�ļ��������ݵ�ҳ��
	User_Type user[MaxUserNum];									//ѡ�е���
	User_Type * tempuser;
	User_Type tempnewuser;
	unsigned short lcdinput[100];
}UserMPageBuffer;

MyState_TypeDef createUserManagerActivity(Activity * thizActivity, Intent * pram);

#endif

