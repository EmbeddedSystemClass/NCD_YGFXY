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

#include	"MyMem.h"

#include	<string.h>
#include	"stdio.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static PageLinkStack GB_PageLinkStack = 
{
	.top = NULL
};								//ҳ����Ϣ��ջ


/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static MyState_TypeDef PageLinkStackPush(PageLinkStack * s_linkstack, PageInfo * pageinfo);
static MyState_TypeDef PageLinkStackPop(PageLinkStack * s_linkstack, PageInfo ** pageinfo);
static MyState_TypeDef PageLinkStackTop(PageLinkStack * s_linkstack, PageInfo ** pageinfo);

static MyState_TypeDef PageDetroyBackNum(unsigned char index);
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************
*FunctionName: PageAdvanceTo
*Description:  ҳ����ǰ�л���ĳ��ҳ�棬������ջ��ջ������ע��˴���ֱ����ʾ���棬�������л�ָ��
*Input: page -- ǰ��ҳ���ַ
		pram -- ���ݵ�ǰ��ҳ�����
*Output: None
*Return: My_Fail -- ǰ��ʧ��
*		My_Pass -- ǰ���ɹ�
*Author: xsx
*Date: 2016��12��7��15:29:21
***************************************************************************************************/
MyState_TypeDef PageAdvanceTo(unsigned char (*page)(void * pram), void * pram)
{
	PageInfo * temppageinfo = NULL;
	
	//��������ת����Ч����
	if(NULL == page)
		return My_Fail;
	
	temppageinfo = MyMalloc(sizeof(PageInfo));
	
	if(temppageinfo)
	{
		memset(temppageinfo, 0, sizeof(PageInfo));
		temppageinfo->CurrentPage = page;
		
		if(My_Pass == PageLinkStackPush(&GB_PageLinkStack, temppageinfo))
		{
			temppageinfo->pram = pram;
			temppageinfo->CurrentPage(temppageinfo->pram);
			
			return My_Pass;
		}
		else
		{
			MyFree(temppageinfo);
			return My_Fail;
		}
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: PageDetroyBackNum
*Description: ��ǰ����index������
*Input: index -- ��ǰ���ٵĽ�����Ŀ
*Output: None
*Return: My_Fail -- ʧ��
*		My_Pass -- �ɹ�
*Author: xsx
*Date: 2016��12��8��13:57:33
***************************************************************************************************/
static MyState_TypeDef PageDetroyBackNum(unsigned char index)
{
	PageInfo * temppageinfo = NULL;
	unsigned char i = index;
	
	//�ȳ�ջ����һ����ȵĽ���
	while(i)
	{
		if(My_Pass == PageLinkStackPop(&GB_PageLinkStack, &temppageinfo))
		{
			MyFree(temppageinfo);
			i--;
		}
		else
			break;
	}
	
	return My_Pass;
}
/***************************************************************************************************
*FunctionName: PageBackTo
*Description: �������룬ҳ�淵��һ���Ĳ���
*Input: index -- ���ز�����1��ʾ������һ��ҳ�棬2��ʾ��������һ��ҳ��
*		pram -- ������������ص�ҳ��
*Output: 
*Return: My_Fail -- ʧ��
*		My_Pass -- �ɹ�
*Author: xsx
*Date: 2016��12��7��15:58:31
***************************************************************************************************/
MyState_TypeDef PageBackTo(unsigned char index)
{
	PageInfo * temppageinfo = NULL;
	
	PageDetroyBackNum(index);
	
	//��ȡ��ǰջ�����棬����ʾ
	if(My_Pass == PageLinkStackTop(&GB_PageLinkStack, &temppageinfo))
	{	
		temppageinfo->CurrentPage(temppageinfo->pram);
		
		return My_Pass;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: PageResetToOrigin
*Description: ��������
*Input: None
*Output: None
*Return: My_Fail -- ʧ��
*		My_Pass -- �ɹ�
*Author: xsx
*Date: 2016��12��8��09:01:58
***************************************************************************************************/
MyState_TypeDef PageResetToOrigin(DisplayType distype)
{
	PageInfo * temppageinfo = NULL;
	
	PageDetroyBackNum(OriginPage);
	
	if(DisplayPage == distype)
	{
		//��ȡ��ǰջ�����棬����ʾ
		if(My_Pass == PageLinkStackTop(&GB_PageLinkStack, &temppageinfo))
		{
			temppageinfo->CurrentPage(temppageinfo->pram);
			
			return My_Pass;
		}
	}
	
	return My_Fail;
}

/***************************************************************************************************
*FunctionName: GetCurrentPage
*Description: ��ȡ��ǰҳ��
*Input: pageinfo -- ��ǰҳ�����ݴ�ŵ�ַ
*Output: None
*Return: My_Fail -- ʧ��
*		My_Pass -- �ɹ�
*Author: xsx
*Date: 2016��12��7��16:03:09
***************************************************************************************************/
MyState_TypeDef GetCurrentPage(PageInfo ** pageinfo)
{
	//��ȡ��ǰջ������
	if(My_Pass == PageLinkStackTop(&GB_PageLinkStack, pageinfo))
		return My_Pass;
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: GetParentPage
*Description: ��ȡ��ǰҳ��ĸ�ҳ��
*Input: pageinfo -- ��ǰҳ�����ݴ�ŵ�ַ
*Output: None
*Return: My_Fail -- ʧ��
*		My_Pass -- �ɹ�
*Author: xsx
*Date: 2016��12��7��16:03:09
***************************************************************************************************/
unsigned char (*GetParentPage(void))(void* pram)
{
	PageStackNode * tempnode = GB_PageLinkStack.top;
	
	if(tempnode)
	{
		if(tempnode->lastpagenode)
		{
			return tempnode->lastpagenode->pageinfo->CurrentPage;
		}
	}
	
	return NULL;
}

/*###################################################################################################
#########################################��ջ�Ļ�������##############################################
####################################################################################################*/

/***************************************************************************************************
*FunctionName: InitPageLinkStack
*Description: ��ʼ����ջ
*Input: s_linkstack - ��ջ��ַ
*Output: None
*Return: None
*Author: xsx
*Date: 2016��12��7��14:47:29
***************************************************************************************************/
void InitPageLinkStack(PageLinkStack * s_linkstack)
{
	s_linkstack->top = NULL;
}

/***************************************************************************************************
*FunctionName: PageLinkStackIsEmpty
*Description: �ж���ջ�Ƿ�Ϊ��
*Input: s_linkstack - ��ջ��ַ
*Output: None
*Return: 	true -- ��ջ��
*			false -- ��ջ��Ϊ��
*Author: xsx
*Date: 2016��12��7��14:49:37
***************************************************************************************************/
bool PageLinkStackIsEmpty(PageLinkStack * s_linkstack)
{
	if(NULL == s_linkstack->top)
		return true;
	else
		return false;
}

/***************************************************************************************************
*FunctionName: PageLinkStackLength
*Description: �����ջ�ĳ���
*Input: s_linkstack - ��ջ��ַ
*Output: None
*Return: len -- ��ջ����
*Author: xsx
*Date: 2016��12��7��14:55:19
***************************************************************************************************/
unsigned char PageLinkStackLength(PageLinkStack * s_linkstack)
{
	unsigned char len = 0;
	PageStackNode * tempnode = s_linkstack->top;
	
	while(NULL != tempnode)
	{
		len++;
		tempnode = tempnode->lastpagenode;
	}
	
	return len;
}

/***************************************************************************************************
*FunctionName: PageLinkStackPush
*Description: ��ջ����
*Input: s_linkstack -- ��ջ��ַ
*		pageinfo -- ��ջ��ֵ
*Output: None
*Return: 	My_Pass -- ��ջ�ɹ�
*			My_Fail -- ��ջʧ��
*Author: xsx
*Date: 2016��12��7��15:03:34
***************************************************************************************************/
static MyState_TypeDef PageLinkStackPush(PageLinkStack * s_linkstack, PageInfo * pageinfo)
{
	PageStackNode * pagenode = NULL;
	
	//��������ջ��ֵ
	if(NULL == pageinfo)
		return My_Fail;
	
	pagenode = MyMalloc(sizeof(PageStackNode));
	
	if(pagenode)
	{		
		pagenode->pageinfo = pageinfo;
		pagenode->lastpagenode = s_linkstack->top;
		
		s_linkstack->top = pagenode;
		
		return My_Pass;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName: PageLinkStackPop
*Description: ��ջ��ջ����
*Input: s_linkstack -- ��ջ��ַ
*		pageinfo -- ��ջ���ݴ�ŵ�ַ
*Output: None
*Return: My_Fail -- ��ջʧ��
*		My_Pass -- ��ջ�ɹ�
*Author: xsx
*Date: 2016��12��7��15:10:31
***************************************************************************************************/
static MyState_TypeDef PageLinkStackPop(PageLinkStack * s_linkstack, PageInfo ** pageinfo)
{
	PageStackNode * pagenode = NULL;
	
	//ջ�ջ���ֻ��1���ڵ㣬�������ջ��ϵͳ�����ײ�ڵ�Ϊ������
	if(PageLinkStackLength(s_linkstack) <= OriginPageIndex)
		return My_Fail;

	//����
	else
	{
		//ɾ����ǰҳ������ݿռ�
		if(s_linkstack->top)
		{
			if(s_linkstack->top->pageinfo->PageBufferFree)
				s_linkstack->top->pageinfo->PageBufferFree();
		}
		
		pagenode = s_linkstack->top;
		*pageinfo = pagenode->pageinfo;
		
		s_linkstack->top = pagenode->lastpagenode;
		
		MyFree(pagenode);
		
		return My_Pass;
	}
}

/***************************************************************************************************
*FunctionName: PageLinkStackTop
*Description: ��ȡջ���ڵ��е�ֵ�����ǲ�ɾ��ջ���ڵ�
*Input: s_linkstack -- ��ջ
*		pageinfo -- ��ȡ���ݵĴ�ŵ�ַ
*Output: None
*Return: My_Fail -- ��ȡʧ��
*		My_Pass -- ��ȡ�ɹ�
*Author: xsx
*Date: 2016��12��7��15:13:48
***************************************************************************************************/
static MyState_TypeDef PageLinkStackTop(PageLinkStack * s_linkstack, PageInfo ** pageinfo)
{	
	//ջ��
	if(PageLinkStackIsEmpty(s_linkstack))
		return My_Fail;

	//����
	else
	{
		*pageinfo = s_linkstack->top->pageinfo;

		return My_Pass;
	}
}


