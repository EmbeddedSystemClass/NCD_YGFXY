/***************************************************************************************************
*FileName:Motor_Data
*Description:�������
*Author:xsx
*Data:2016��5��11��20:22:59
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/
#include	"Motor_Data.h"

/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static LimitState_Def GB_PreLimitState = NotLimitted;							//ǰ��λ״̬
static LimitState_Def GB_BackLimitState = NotLimitted;							//����λ״̬
static unsigned short GB_MotorLocation = 10000;									//�����ǰλ��
static unsigned short GB_MotorTargetLocation = 0;								//Ŀ��λ��
static DRVDir_Type GB_MotorDir = Reverse;										//�������
static unsigned short GB_MotorMaxLocation = 0;									//������λ��
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************����********************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/





LimitState_Def GetMotorPreLimitState(void)
{
	LimitState_Def temp ;
	

		temp = GB_PreLimitState;
	
	
	return temp;
}

void SetMotorPreLimitState(LimitState_Def value)
{

		GB_PreLimitState = value;

}

LimitState_Def GetMotorBackLimitState(void)
{
	LimitState_Def temp ;
	

	
		temp = GB_BackLimitState;

	return temp;
}

void SetMotorBackLimitState(LimitState_Def value)
{

		GB_BackLimitState = value;

}

unsigned short GetGB_MotorLocation(void)
{
	return GB_MotorLocation;
}

void SetGB_MotorLocation(unsigned short value)
{

		GB_MotorLocation = value;

}

unsigned short GetGB_MotorTargetLocation(void)
{
	return GB_MotorTargetLocation;
}

void SetGB_MotorTargetLocation(unsigned short value)
{

		GB_MotorTargetLocation = value;

}


unsigned short GetGB_MotorDir(void)
{
	unsigned short temp ;

		temp = GB_MotorDir;

	
	return temp;
}

void SetGB_MotorDir(DRVDir_Type dir)
{

	
		GB_MotorDir = dir;

}


void SetGB_MotorMaxLocation(unsigned short loc)
{
	GB_MotorMaxLocation = loc;
}

unsigned short GetGB_MotorMaxLocation(void)
{
	return GB_MotorMaxLocation;
}

