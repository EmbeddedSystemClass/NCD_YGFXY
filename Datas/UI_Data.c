/***************************************************************************************************
*FileName:
*Description:
*Author:xsx
*Data:
***************************************************************************************************/


/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"UI_Data.h"




/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static SysPage GB_SysPage;												//ϵͳ��������



/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/







/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName��SetGBSysPage
*Description��һ������ҳ����������
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:13:18
***************************************************************************************************/
void SetGBSysPage(
	unsigned char (*CurrentPage)(void * pram), 								//��ǰҳ��
	unsigned char (*ParentPage)(void *  parm),								//��ҳ��
	unsigned char (*ChildPage)(void *  parm),								//��ҳ��
	void (*LCDInput)(unsigned char *pbuf , unsigned short len),				//ҳ������
	void (*PageUpDate)(void),												//ҳ�����
	MyState_TypeDef (*PageInit)(void * pram),								//ҳ���ʼ��
	MyState_TypeDef (*PageBufferMalloc)(void),								//ҳ�滺������
	MyState_TypeDef (*PageBufferFree)(void)									//ҳ�滺���ͷ�
	)
{
	GB_SysPage.CurrentPage = CurrentPage;
	GB_SysPage.ParentPage = ParentPage;
	GB_SysPage.ChildPage = ChildPage;
	GB_SysPage.LCDInput = LCDInput;
	GB_SysPage.PageUpDate = PageUpDate;
	GB_SysPage.PageInit = PageInit;
	GB_SysPage.PageBufferMalloc = PageBufferMalloc;
	GB_SysPage.PageBufferFree = PageBufferFree;
}

/***************************************************************************************************
*FunctionName��SetGBCurrentPage, GetGBCurrentPage
*Description: ���ã���ȡ��ǰҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��19:36:13
***************************************************************************************************/
void SetGBCurrentPage(unsigned char (*CurrentPage)(void * pram))
{
	GB_SysPage.CurrentPage = CurrentPage;
}
unsigned char (*GetGBCurrentPage(void))(void* pram)
{
	return GB_SysPage.CurrentPage;
}

/***************************************************************************************************
*FunctionName��SetParentPage
*Description�����ø�ҳ���ַ
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:18:36
***************************************************************************************************/
void SetGBParentPage(unsigned char (*ParentPage)(void *  parm))
{
	GB_SysPage.ParentPage = ParentPage;
}

/***************************************************************************************************
*FunctionName��GotoParentPage
*Description���л�ҳ�浽��ҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:19:02
***************************************************************************************************/
unsigned char GotoGBParentPage(void *  parm)
{
	return GB_SysPage.ParentPage(parm);
}

/***************************************************************************************************
*FunctionName��SetChildPage
*Description��������ҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:19:58
***************************************************************************************************/
void SetGBChildPage(unsigned char (*ChildPage)(void *  parm))
{
	GB_SysPage.ChildPage = ChildPage;
}

/***************************************************************************************************
*FunctionName��GotoChildPage
*Description���л�����ҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:20:24
***************************************************************************************************/
unsigned char GotoGBChildPage(void *  parm)
{
	return GB_SysPage.ChildPage(parm);
}

/***************************************************************************************************
*FunctionName��GBPageUpDate
*Description��ˢ�µ�ǰҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:25:20
***************************************************************************************************/
void SetGBPageUpDate(void (*PageUpDate)(void))
{
	GB_SysPage.PageUpDate = PageUpDate;
}
void GBPageUpDate(void)
{
	GB_SysPage.PageUpDate();
}

/***************************************************************************************************
*FunctionName��GBPageInput
*Description��lcd����
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��19:44:56
***************************************************************************************************/
void SetGBGBPageInput(void (*LCDInput)(unsigned char *pbuf , unsigned short len))
{
	GB_SysPage.LCDInput = LCDInput;
}
void GBPageInput(unsigned char *pbuf , unsigned short len)
{
	GB_SysPage.LCDInput(pbuf, len);
}

/***************************************************************************************************
*FunctionName��GBPageInit
*Description����ʼ����ǰҳ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:26:42
***************************************************************************************************/
void SetGBPageInit(MyState_TypeDef (*PageInit)(void * pram))
{
	GB_SysPage.PageInit = PageInit;
}
MyState_TypeDef GBPageInit(void * parm)
{
	return GB_SysPage.PageInit(parm);
}

/***************************************************************************************************
*FunctionName��GBPageBufferMalloc
*Description����ǰҳ�滺������
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:28:16
***************************************************************************************************/
void SetGBPageBufferMalloc(MyState_TypeDef (*PageBufferMalloc)(void))
{
	GB_SysPage.PageBufferMalloc = PageBufferMalloc;
}
MyState_TypeDef GBPageBufferMalloc(void)
{
	return GB_SysPage.PageBufferMalloc();
}

/***************************************************************************************************
*FunctionName��GBPageBufferFree
*Description����ǰҳ�滺���ͷ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��8��3��17:28:35
***************************************************************************************************/
void SetGBPageBufferFree(MyState_TypeDef (*PageBufferFree)(void))
{
	GB_SysPage.PageBufferFree = PageBufferFree;
}
MyState_TypeDef GBPageBufferFree(void)
{
	return GB_SysPage.PageBufferFree();
}
