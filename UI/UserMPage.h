#ifndef __USERM_P_H__
#define __USERM_P_H__

#include	"Define.h"


typedef struct UserMPageBuffer_tag {
	unsigned char selectindex;										//ѡ�е�����
	unsigned char pageindex;										//ҳ������
	unsigned char filemaxitem;							//��ǰ�ļ�����������Ŀ
	unsigned char filemaxpagenum;						//��ǰ�ļ��������ݵ�ҳ��
	User_Type user[MaxUserNum];									//ѡ�е���
	User_Type * tempuser;
	User_Type tempnewuser;
}UserMPageBuffer;

unsigned char DspUserMPage(void *  parm);

#endif

