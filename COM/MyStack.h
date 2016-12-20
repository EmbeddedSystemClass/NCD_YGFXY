/****************************************file start****************************************************/
#ifndef	MYSTACK_H_H
#define	MYSTACK_H_H

#include	"Define.h"

//����һ��ҳ����Ϣ
#pragma pack(1)
typedef struct Activity_Tag
{
	char * pageName;												//activity���֣�ָ��һ��Ψһ��ַ
	MyState_TypeDef (* pageCreate)(void * thizactivity, void * pram);					//����ÿ��ҳ�����ĺ���������ҳ���ڴ�
	void (* pageStart)(void);										//��ʾҳ�棬��ʼ��ҳ������
	void (* pageInput)(unsigned char *pbuf , unsigned short len);		//��ȡҳ������
	void (* pageFresh)(void);											//ҳ��ˢ��
	void (* pageHide)(void);											//ҳ�����أ��л�����ҳ��
	void (* pageResume)(void);											//������״̬���أ�������ʾ
	void (* pageDestroy)(void);										//ҳ�����٣���ʾ��һ��ҳ��
	
	void * pram;													//��һ��ҳ�洫�����Ĳ�����ַ����ȡ����Ҫ�ͷ�
}Activity;
#pragma pack()

#pragma pack(1)
typedef struct _stacknode_
{
    Activity * activity;									//��ǰ�ڵ��ҳ����Ϣ
    struct _stacknode_ * lastStackNode;						//��һ�ڵ��ַ
}StackNode;
#pragma pack()

#pragma pack(1)
typedef struct
{
	StackNode * top;										//ջ���ڵ�
}LinkStack;
#pragma pack()

void InitLinkStack(LinkStack * linkStack);
MyState_TypeDef StackPush(LinkStack * linkStack, Activity * nodeData);
MyState_TypeDef StackPop(LinkStack * linkStack, Activity ** activity, bool isResume);
MyState_TypeDef StackTop(LinkStack * linkStack, Activity ** activity);

#endif

/****************************************end of file************************************************/