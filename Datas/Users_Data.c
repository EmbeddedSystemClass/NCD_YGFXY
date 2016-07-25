/***************************************************************************************************
*FileName:UserS_Data
*Description:����������
*Author:xsx
*Data:2016��5��5��19:51:19
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"Users_Data.h"
#include	"malloc.h"
#include	"user_fatfs.h"
#include	"SDFunction.h"

#include	"TcpClientFunction.h"
#include	"QueueUnits.h"

#include	"cJSON.h"

#include 	"FreeRTOS.h"
#include 	"task.h"

#include	"string.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

static cJSON * GB_UserCJSON = NULL;
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
*FunctionName��GetUserNum
*Description����ȡ���в�������Ŀ
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��10��20:13:08
***************************************************************************************************/
unsigned char GetUsersNum(void)
{
	if(GB_UserCJSON == NULL)
		return 0;

	else
		return cJSON_GetArraySize(GB_UserCJSON);
}

/***************************************************************************************************
*FunctionName��GetUserListJSON
*Description����ȡ���в�������Ϣ
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��14:10:45
***************************************************************************************************/
MyState_TypeDef GetUserListJSON(void)
{
	/*�ж��Ƿ��Ѵ���*/
	if(GB_UserCJSON == NULL)
	{
		/*������ļ��л�ȡ�ɹ�*/
		GB_UserCJSON = ReadUserFileToJSON();
		
		/*�����ȡʧ�����½���JSON*/
		if(NULL == GB_UserCJSON)
		{
			GB_UserCJSON = cJSON_CreateObject();
			if(GB_UserCJSON == NULL)
				return My_Fail;
		}
	}
	
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��SaveUserListJSON
*Description�����������û���Ϣ
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��14:14:04
***************************************************************************************************/
MyState_TypeDef SaveUserListJSON(void)
{
	/*�ж��Ƿ��Ѵ���*/
	if(GB_UserCJSON == NULL)
		return My_Fail;
	
	SaveUserJSONToFile(GB_UserCJSON);
	return My_Pass;
}

/***************************************************************************************************
*FunctionName��DeleteUserListJSON
*Description�����ڴ���ɾ�������û���Ϣ
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��14:14:20
***************************************************************************************************/
void DeleteUserListJSON(void)
{
	cJSON_Delete(GB_UserCJSON);
	GB_UserCJSON = NULL;
}

/***************************************************************************************************
*FunctionName��AddAUserJSON
*Description�����һ���û�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��14:14:45
***************************************************************************************************/
MyState_TypeDef AddAUserJSON(User_Type *user)
{
	cJSON * tempjson = NULL;
	cJSON * userjson = NULL;
	if(user)
	{
		if(GB_UserCJSON == NULL)
			return My_Fail;
		
		userjson = CreateAUserJSON(user);
		if(userjson == NULL)
			return My_Fail;
		
		/*�Ȳ��ҿ��������Ƿ��Ѵ���*/
		tempjson = cJSON_GetObjectItem(GB_UserCJSON, user->user_name);
		
		/*������ڣ����滻��*/
		if(tempjson)
			cJSON_ReplaceItemInObject(GB_UserCJSON, user->user_name, userjson);
		
		/*��������ڣ����½�*/
		else
		{
			/*�Ƿ��ѵ�����*/
			if(cJSON_GetArraySize(GB_UserCJSON) > MaxUserNum)
			{
				cJSON_Delete(userjson);
				return My_Fail;
			}
			
			cJSON_AddItemToObject(GB_UserCJSON, user->user_name, userjson);
		}
		
		return My_Pass;
	}
	else
		return My_Fail;
}

/***************************************************************************************************
*FunctionName��DeleteAUserJSON
*Description��ɾ��һ��������
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��11��14:22:00
***************************************************************************************************/
MyState_TypeDef DeleteAUserJSON(User_Type *user)
{
	cJSON * tempjson = NULL;
	
	if(user)
	{	
		if(GB_UserCJSON == NULL)
			return My_Fail;
		
		/*�Ȳ��ҿ��������Ƿ��Ѵ���*/
		tempjson = cJSON_GetObjectItem(GB_UserCJSON, user->user_name);
		
		/*������ڣ����滻��*/
		if(tempjson)
			cJSON_DeleteItemFromObject(GB_UserCJSON, user->user_name);
		
		return My_Pass;
	}
	else
		return My_Fail;
}

MyState_TypeDef GetAUserByIndex(unsigned char index, User_Type * user)
{
	cJSON *json = NULL;
	
	if(GB_UserCJSON == NULL)
		return My_Fail;
	
	
	if(user == NULL)
		return My_Fail;
	
	json = cJSON_GetArrayItem(GB_UserCJSON, index);
	if(json == NULL)
		return My_Fail;
	else
	{
		return ParseJSONAUserJSON(user, json);
	}
}

MyState_TypeDef ParseJSONAUserJSON(User_Type * user, cJSON * cjson)
{
	char *out;
	
	cJSON *pArrayItem = NULL;
	
	if(cjson)
	{
		/*��ȡ����*/
		pArrayItem = cJSON_GetArrayItem(cjson, 0);
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_name, out+1, strlen(out)-2);
		
			myfree( out );
		}
		else
			return My_Fail;
		
		/*��ȡ����*/
		pArrayItem = cJSON_GetArrayItem(cjson, 1);
		
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_age, out+1, strlen(out)-2);
		
			myfree( out );
		}
		else
			return My_Fail;
		
		/*��ȡ�Ա�*/
		pArrayItem = cJSON_GetArrayItem(cjson, 2);
		
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_sex, out+1, strlen(out)-2);
		
			myfree( out );
		}
		else
			return My_Fail;
		
		/*��ȡ��ϵ��ʽ*/
		pArrayItem = cJSON_GetArrayItem(cjson, 3);
		
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_phone, out+1, strlen(out)-2);
		
			myfree( out );
		}
		else
			return My_Fail;
		
		/*��ȡְ��*/
		pArrayItem = cJSON_GetArrayItem(cjson, 4);
		
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_job, out+1, strlen(out)-2);
		
			myfree( out );
		}
		else
			return My_Fail;
		
		/*��ȡ��ע*/
		pArrayItem = cJSON_GetArrayItem(cjson, 5);
		
		if(pArrayItem)
		{
			out = cJSON_Print( pArrayItem );

			if(strlen(out) > 2)
				mymemcpy(user->user_desc, out+1, strlen(out)-2);
		
			myfree( out );
			
			return My_Pass;
		}
		else
			return My_Fail;
	}
	
	else
		return My_Fail;

}

cJSON * CreateAUserJSON(User_Type * user)
{
	char *out;
	cJSON * cjson = NULL;
	
	cJSON *pArrayItem = NULL;
	MyState_TypeDef statues = My_Fail;

	cjson = cJSON_CreateObject();
	if(cjson)
	{
		/*��ȡ����*/
		pArrayItem = cJSON_CreateString(user->user_name);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "����", pArrayItem);
		}
		else
			goto END;
		
		
		/*��ȡ����*/
		pArrayItem = cJSON_CreateString(user->user_age);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "����", pArrayItem);
		}
		else
			goto END;
		
		/*��ȡ�Ա�*/
		pArrayItem = cJSON_CreateString(user->user_sex);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "�Ա�", pArrayItem);
		}
		else
			goto END;
		
		/*��ȡ��ϵ��ʽ*/
		pArrayItem = cJSON_CreateString(user->user_phone);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "��ϵ��ʽ", pArrayItem);
		}
		else
			goto END;
		
		/*��ȡְ��*/
		pArrayItem = cJSON_CreateString(user->user_job);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "ְ��", pArrayItem);
		}
		else
			goto END;
		
		/*��ȡ��ע*/
		pArrayItem = cJSON_CreateString(user->user_desc);
		if(pArrayItem)
		{
			cJSON_AddItemToObject(cjson, "��ע", pArrayItem);
		}
		else
			goto END;

		statues = My_Pass;
	}
	
	END:
		if(statues == My_Fail)
		{
			cJSON_Delete(cjson);
			cjson = NULL;
		}
		
		return cjson;
}
