#ifndef _MYTEST_S_D_H__
#define _MYTEST_S_D_H__

#include	"Define.h"
#include	"Motor_Data.h"
#include	"Timer_Data.h"

typedef enum
{ 
	Error_OK = 0,																	//�����ɹ�
	Error_Mem = 1,																	//�ڴ����
	Error_PaiDuiBusy = 2,															//�ŶӲ���æ����ֹ�ٴδ�����������
	Error_StopNormalTest = 3,														//���������У���ֹ�������
	Error_PaiduiFull = 4,															//�Ŷ�λ��
	Error_PaiduiTesting = 5															//������
}CreateTestErrorType;

typedef struct ItemData_tag {
	TestData testdata;
	Timer timer;																	//���浹��ʱ��ʱ��
	Timer timer2;																	//��ʱ����ʱ��ʱ��
	Timer timer3;																	//��ʱ����ʱ��ʱ��
	unsigned char errorcount;														//δ��������
	MyPaiDuiStatues statues;														//���Խ׶�
	unsigned char jieduan;															//���Խ׶�
	unsigned char testlocation;
	unsigned char varIcoIndex;														//�Ŷӽ�����ʾ����ͼ����������
	unsigned short ledLight;														//����ʱled�����ȣ�Ϊϵͳ�����б����У׼ledֵ
}ItemData;

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

void SetCurrentTestItem(ItemData * s_itemdata);
ItemData * GetCurrentTestItem(void);

void SetPaiduiUser(User_Type * user);
void GetPaiduiUser(User_Type * user);

#endif

