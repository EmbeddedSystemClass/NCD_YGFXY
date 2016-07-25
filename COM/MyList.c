/***************************************************************************************************
*FileName: MyList
*Description: �������
*Author:xsx
*Data:2016��4��21��14:22:46
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"MyList.h"
#include	"malloc.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

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
*FunctionName��CreateList
*Description���½�����
*Input��None
*Output�������½����������Ϊnull��ʾ�½�ʧ��
*Author��xsx
*Data��2016��5��16��20:29:30
***************************************************************************************************/
List *CreateList(void)
{
	List * myList = NULL;
	
	myList = (List *)mymalloc(sizeof(List));
	if(myList)
	{
		mymemset(myList, 0, sizeof(List));
	}
	
	return myList;
}

/***************************************************************************************************
*FunctionName��DestroyList
*Description��ɾ�������ͷſռ�
*Input��list -- Ҫɾ��������
		MyState_TypeDef (*fun)(void *data) -- �����Ϊnull����ִ������ɾ���ڵ��ڵ�����
*Output�����ز������
*Author��xsx
*Data��2016��5��16��20:31:44
***************************************************************************************************/
MyState_TypeDef DestroyList(List *list, MyState_TypeDef (*fun)(void *data))
{
	struct Node *node = NULL;
	struct Node *next = NULL;
	
	if(list)
	{
		/*��ȡ��һ���ڵ�*/
		node = list->head;
		
		/*����ڵ����*/
		while(node)
		{
			/*�Ƿ���Ҫɾ���ڵ��ڵ�����*/
			if(fun != NULL)
			{
				if(fun(node->data) != My_Fail)
					return My_Fail;
			}
			
			/*��ȡ��һ���ڵ�*/
			next = node->next;
			
			/*ɾ����ǰ���*/
			myfree(node);
			
			node = next;
		}
	
		myfree(list);
	
		return My_Pass;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��InsertNodeToList
*Description��������Ľ�β����ڵ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��16��20:41:07
***************************************************************************************************/
MyState_TypeDef InsertNodeToList(List *list, void * data, unsigned char mode)
{
	struct Node *node = NULL;
	
	if(list&&data)
	{
		node = mymalloc(sizeof(Node));
		if(node)
		{
			if(list->head == NULL)
			{
				list->head = node;
				list->tail = node;
				node->next = NULL;
				node->pre = NULL;
				node->data = data;
			}
			else
			{
				node->pre = list->tail;
				node->next = NULL;
				list->tail->next = node;
				list->tail = node;
				node->data = data;
			}
			
			if(mode)
				list->current = node;
			
			return My_Pass;
		}
		else
			return My_Fail;
		
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��ListIsContainNode
*Description���ж��������Ƿ����ָ���ڵ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��16��20:47:22
***************************************************************************************************/
MyState_TypeDef ListIsContainNode(List *list, Node * n)
{
	struct Node *node = NULL;
	if(list&&n)
	{
		node = list->head;
		
		/*����ڵ����*/
		while(node)
		{
			if(node == n)
				return My_Pass;
			
			node = node->next;
		}
		return My_Fail;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��DeleteNodeFromList
*Description����������ɾ���ڵ�
*Input��list -- ����
		n -- Ҫɾ���Ľڵ�
		fun -- �Ƿ�Ҫ��ɾ���Ľڵ�������null��ʾ������
*Output��None
*Author��xsx
*Data��2016��5��16��20:42:57
***************************************************************************************************/
MyState_TypeDef DeleteNodeFromList(List *list, Node * n, MyState_TypeDef (*fun)(void *data))
{
	if(list&&n)
	{
		if(My_Pass == ListIsContainNode(list, n))
		{
			if(n->pre)
				n->pre->next = n->next;
			else
				list->head = n->next;
			
			if(n->next)
				n->next->pre = n->pre;
			else
				list->tail = n->pre;
			
			if(list->current == n)
				list->current = NULL;
			
			if(fun != NULL)
			{
				if(My_Fail == fun(n->data))
					return My_Fail;
			}

			myfree(n);
			
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��GetNodeFromList
*Description������������ȡ�ڵ�
*Input��fun -- ��ȡ�ڵ����������
*Output��None
*Author��xsx
*Data��2016��5��16��20:56:09
***************************************************************************************************/
Node *GetNodeFromList(List *list, void *key, MyState_TypeDef (*fun)(void *data, void *key))
{
	struct Node *node = NULL;

	if(list)
	{
		node = list->head;
		while(node)
		{
			if(fun == NULL)
				return NULL;
			else
			{
				/*�Ƚϳɹ��󷵻ؽڵ�*/
				if(My_Pass == fun(node->data, key))
				{
					return node;
				}
				else
					node = node->next;
			}
		}
		
		return NULL;
	}
	else
		return NULL;
}

/***************************************************************************************************
*FunctionName��ListSize
*Description����ȡ����Ĵ�С
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��16��20:59:01
***************************************************************************************************/
unsigned char ListSize(List *list)
{
	struct Node *node = NULL;
	unsigned char count = 0;
	
	if(list)
	{
		node = list->head;
		while(node)
		{
			count++;
			node = node->next;
		}
		return count;
	}
	else
		return 0;
}

/***************************************************************************************************
*FunctionName��SetCurrentNode
*Description�����õ�ǰ�����Ľڵ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��16��21:05:10
***************************************************************************************************/
MyState_TypeDef SetCurrentNode(List *list, Node *n)
{
	if(list&&n)
	{
		if(ListIsContainNode(list, n) == My_Pass)
		{
			list->current = n;
			return My_Pass;
		}
		else
			return My_Fail;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��GetCurrentNode
*Description����ȡ��ǰ�����Ľڵ�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��16��21:05:26
***************************************************************************************************/
Node *GetCurrentNode(List *list)
{
	if(list)
	{
		return list->current;
	}
	else
		return NULL;
}
