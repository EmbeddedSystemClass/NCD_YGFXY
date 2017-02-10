#ifndef __SUSER_P_H__
#define __SUSER_P_H__

#include	"Define.h"
#include	"MyTest_Data.h"
#include	"Timer_Data.h"
#include	"UI_Data.h"

typedef struct UserPage_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	User_Type user[MaxUserNum];										//���в������б�
	User_Type * tempUser;											//��ʱָ��
	User_Type * tempUser2;											//��ǰѡ�еĲ����ˣ�null��ʾδѡ��
	ItemData * currenttestdata;										//��ǰ��������
	unsigned short lcdinput[100];									//lcd�������
}UserPageBuffer;

MyState_TypeDef createSelectUserActivity(Activity * thizActivity, Intent * pram);

#endif

