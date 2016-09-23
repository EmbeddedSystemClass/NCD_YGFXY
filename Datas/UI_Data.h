#ifndef UI_D_H
#define UI_D_H

#include	"Define.h"

void SetGBSysPage(
	unsigned char (*CurrentPage)(void * pram), 								//��ǰҳ��
	unsigned char (*ParentPage)(void *  parm),								//��ҳ��
	unsigned char (*ChildPage)(void *  parm),								//��ҳ��
	void (*LCDInput)(unsigned char *pbuf , unsigned short len),				//ҳ������
	void (*PageUpDate)(void),												//ҳ�����
	MyState_TypeDef (*PageInit)(void * pram),								//ҳ���ʼ��
	MyState_TypeDef (*PageBufferMalloc)(void),								//ҳ�滺������
	MyState_TypeDef (*PageBufferFree)(void)									//ҳ�滺���ͷ�
	);
SysPage * GetGBSysPage(void);
	
void SetGBCurrentPage(unsigned char (*CurrentPage)(void * pram));
unsigned char (*GetGBCurrentPage(void))(void* pram);

void SetGBParentPage(unsigned char (*ParentPage)(void *  parm));
unsigned char GotoGBParentPage(void *  parm);

void SetGBChildPage(unsigned char (*ChildPage)(void *  parm));
unsigned char GotoGBChildPage(void *  parm);

void SetGBPageUpDate(void (*PageUpDate)(void));
void GBPageUpDate(void);

void SetGBGBPageInput(void (*LCDInput)(unsigned char *pbuf , unsigned short len));
void GBPageInput(unsigned char *pbuf , unsigned short len);

void SetGBPageInit(MyState_TypeDef (*PageInit)(void * pram));
MyState_TypeDef GBPageInit(void * parm);

void SetGBPageBufferMalloc(MyState_TypeDef (*PageBufferMalloc)(void));
MyState_TypeDef GBPageBufferMalloc(void);

void SetGBPageBufferFree(MyState_TypeDef (*PageBufferFree)(void));
MyState_TypeDef GBPageBufferFree(void);
	
#endif

