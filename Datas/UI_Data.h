#ifndef UI_D_H
#define UI_D_H

#include	"Define.h"

//����һ��ҳ����Ϣ
#pragma pack(1)
typedef struct PageInfo_Tag
{
	unsigned char (*CurrentPage)(void * pram);
	void * pram;														//��������Ĳ���
	void * tempP;														//����ָ��
	void (*LCDInput)(unsigned char *pbuf , unsigned short len);
	void (*PageUpDate)(void);
	MyState_TypeDef (*PageInit)(void * pram);
	MyState_TypeDef (*PageBufferMalloc)(void);
	MyState_TypeDef (*PageBufferFree)(void);
	
	unsigned char (*ParentPage)(void *  parm);
	unsigned char (*ChildPage)(void *  parm);
}PageInfo;
#pragma pack()

//���漶������ջ
#pragma pack(1)
typedef struct PageStackNode
{
    PageInfo * pageinfo;											//��ǰ�ڵ��ҳ����Ϣ
    struct PageStackNode * lastpagenode;						//��һ�ڵ��ַ
}PageStackNode;
#pragma pack()

#pragma pack(1)
typedef struct
{
    struct PageStackNode * top;									//ջ���ڵ�
}PageLinkStack;
#pragma pack()

typedef enum
{ 
	NoDisplayPage = 0,											//����ʾ
	DisplayPage = 1												//��ʾ
}DisplayType;

#define	OriginPageIndex			3								//�����ӽ������ʼ������������ϵͳ��˵��ӭ����Ϊ1���Լ�Ϊ2��������Ϊ3
#define	OriginPage				0xff							//�ص���ʼ����Ĳ㼶
#define	ParentPage				1								//���ڵ�ǰ������˵���ظ����棬�����л���ǰһ������

MyState_TypeDef PageAdvanceTo(unsigned char (*page)(void * pram), void * pram);
MyState_TypeDef PageBackTo(unsigned char index);
MyState_TypeDef GetCurrentPage(PageInfo ** pageinfo);
MyState_TypeDef PageResetToOrigin(DisplayType distype);

#endif

