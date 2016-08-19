/***************************************************************************************************
*FileName:Temperature_Data
*Description:温度数据
*Author:xsx
*Data:2016年4月27日18:14:54
***************************************************************************************************/

/***************************************************************************************************/
/******************************************头文件***************************************************/
/***************************************************************************************************/
#include	"Temperature_Data.h"

#include 	"FreeRTOS.h"
#include 	"task.h"
/***************************************************************************************************/
/**************************************局部变量声明*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/**************************************局部函数声明*************************************************/
/***************************************************************************************************/
static MyTemp_Def GB_TempData;					//温度
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************正文********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

MyTemp_Def *GetGBTempData(void)
{
	MyTemp_Def * temp = NULL;
	
	
		temp = &GB_TempData;
	

	return temp;
}

/***************************************************************************************************
*FunctionName：GetCardTemperature  SetCardTemperature
*Description：读写检测卡温度
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月27日18:19:16
***************************************************************************************************/
float GetCardTemperature(void)
{
	float temp = 0;
	
		temp = GB_TempData.O_Temperature;
	

	
	return temp;
}
void SetCardTemperature(float temp)
{
	
		GB_TempData.O_Temperature = temp;
	

}

/***************************************************************************************************
*FunctionName：GetEnviTemperature  SetEnviTemperature
*Description：读写机壳内温度
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月27日18:19:30
***************************************************************************************************/
float GetEnviTemperature(void)
{
	float temp = 0;
	

		temp = GB_TempData.E_Temperature;
	

	return temp;
}
void SetEnviTemperature(float temp)
{

		GB_TempData.E_Temperature = temp;

}

/***************************************************************************************************
*FunctionName：GetEnviTemperature  SetEnviTemperature
*Description：读写机壳内温度
*Input：None
*Output：None
*Author：xsx
*Data：2016年4月27日18:19:30
***************************************************************************************************/
float GetInnerTemperature(void)
{
	float temp = 0;
	

		temp = GB_TempData.I_Temperature;
	

	return temp;
}
void SetInnerTemperature(float temp)
{

		GB_TempData.I_Temperature = temp;

}
