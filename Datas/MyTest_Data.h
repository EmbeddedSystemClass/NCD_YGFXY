#ifndef _MYTEST_S_D_H__
#define _MYTEST_S_D_H__

#include	"Define.h"

typedef enum
{ 
	Error_OK = 0,																	//�����ɹ�
	Error_Mem = 1,																	//�ڴ����
	Error_PaiDuiBusy = 2,															//�ŶӲ���æ����ֹ�ٴδ�����������
	Error_StopNormalTest = 3,														//���������У���ֹ�������
	Error_PaiduiFull = 4,															//�Ŷ�λ��
	Error_PaiduiTesting = 5,														//������
	Error_PaiduiDisconnect = 6														//�Ŷ�ģ��ʧ��
}CreateTestErrorType;



typedef struct TestBuffer_tag{
	ItemData * NormalTestDataBuffer;												//������Ի�����
	ItemData * (PaiduiTestDataBuffer[PaiDuiWeiNum]);								//8���Ŷ�λ���Ի�����
	User_Type PaiduiUser;															//�Ŷ�ģʽ���õĲ�����
	ItemData * CurrentTestDataBuffer;												//��ǰ����
}TestBuffer;

bool IsPaiDuiTestting(void);
CreateTestErrorType CreateANewTest(TestType testtype);
MyState_TypeDef DeleteCurrentTest(void);
ItemData * GetTestItemByIndex(unsigned char index);
unsigned short GetMinWaitTime(void);
bool isInTimeOutStatus(ItemData * itemData);
void SetCurrentTestItem(ItemData * s_itemdata);
ItemData * GetCurrentTestItem(void);

void SetPaiduiUser(User_Type * user);
void GetPaiduiUser(User_Type * user);

#endif

