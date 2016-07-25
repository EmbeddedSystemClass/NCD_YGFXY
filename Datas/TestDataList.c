/******************************************************************************************/
/*****************************************ͷ�ļ�*******************************************/

#include	"TestDataList.h"
#include	"malloc.h"
#include	"Timer_Data.h"


/******************************************************************************************/
/*****************************************�ֲ���������*************************************/
static List GB_TestList;													//�ҵĲ�����������
/******************************************************************************************/
/*****************************************�ֲ���������*************************************/

/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/
/******************************************************************************************/

List *GetMyTestDataList(void)
{
	return &GB_TestList;
}

TestData *GetCurrentNodeData(void)
{
	return (TestData *)(GB_TestList.current->data);
}

/***************************************************************************************************
*FunctionName��GetUsableLocation
*Description�����ؿ��õ�λ�ú�
*Input��None
*Output��0xff -- ��λ�ÿ���
*Author��xsx
*Data��
***************************************************************************************************/
unsigned char GetUsableLocation(void)
{
	unsigned char i=0;

	for(i=1; i<= MaxCardLocation; i++)
	{
		if(GetNodeFromList(&GB_TestList, &i, Compare) == NULL)
			return i;
	}
	
	return 0xff;
}


MyState_TypeDef Compare(void *data, void *key)
{
	TestData *temp = (TestData *)data;
	unsigned char *tempkey = (unsigned char *)key;
	
	if(temp->testlocation == *tempkey)
		return My_Pass;
	else
		return My_Fail;
}

/*���µ�ǰ���ԵĲ�����*/
MyState_TypeDef SetTester(User_Type *user)
{
	TestData *temp = NULL;
	
	if(user == NULL)
		return My_Fail;
	
	if(GB_TestList.current == NULL)
		return My_Fail;
	
	temp = (TestData *)GB_TestList.current->data;
	
	mymemcpy(&(temp->user), user, sizeof(User_Type));
	
	return My_Pass;
}

/*���µ�ǰ���ԵĲ�����*/
MyState_TypeDef SetSampleID(void *sampleid)
{
	TestData *temp = NULL;
	
	if(sampleid == NULL)
		return My_Fail;
	
	if(GB_TestList.current == NULL)
		return My_Fail;
	
	temp = (TestData *)GB_TestList.current->data;
	
	mymemcpy(&(temp->sampleid), sampleid, MaxSampleIDLen);
	
	return My_Pass;
}

MyState_TypeDef SetErWeiMaData(TestCardCodeInfo * data)
{
	TestData *temp = NULL;

	if(data == NULL)
		return My_Fail;
	
	if(GB_TestList.current == NULL)
		return My_Fail;
	
	temp = (TestData *)GB_TestList.current->data;
	
	mymemcpy(&(temp->temperweima), data, sizeof(TestCardCodeInfo));
	
	timer_set(&temp->timer, temp->temperweima.S_CardCodeInfo.CardWaitTime);
	
	return My_Pass;
}

Timer *GetCurrentTimer(void)
{
	TestData *temp = NULL;

	if(GB_TestList.current == NULL)
		return NULL;
	
	temp = (TestData *)GB_TestList.current->data;
	
	return &temp->timer;
}











/***************************************************************************************************
*FunctionName��CreateTestData
*Description������һ���µĲ������ݽṹ
*Input��index -- ��ǰ�����Ŀ���λ�ú�
*Output��null -- ����ʧ��
*Author��xsx
*Data��
***************************************************************************************************/
TestData * CreateTestData(unsigned char index)
{
	TestData *temp = NULL;
	temp = mymalloc(sizeof(TestData));
	if(temp)
	{
		mymemset(temp, 0, sizeof(TestData));
		
		if(index < MaxCardLocation)
			temp->testlocation = index;
		else
		{
			myfree(temp);
			temp = NULL;
		}
	}
	
	return temp;
}



/********************************************************************************************/

