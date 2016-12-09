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
#include	"Define.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
static LimitState_Def GB_PreLimitState = NotLimitted;							//ǰ��λ״̬
static LimitState_Def GB_BackLimitState = NotLimitted;							//����λ״̬
static unsigned short GB_MotorLocation = 10000;									//�����ǰλ��
static unsigned short GB_MotorTargetLocation = 0;								//Ŀ��λ��
static DRVDir_Type GB_MotorDir = Reverse;										//�������

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
	return GB_PreLimitState;
}

void SetMotorPreLimitState(LimitState_Def value)
{
	GB_PreLimitState = value;
}

LimitState_Def GetMotorBackLimitState(void)
{
	return GB_BackLimitState;
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
	return GB_MotorDir;
}

void SetGB_MotorDir(DRVDir_Type dir)
{
	GB_MotorDir = dir;
}



