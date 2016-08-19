/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/
#if (DebugCode == CodeType)
/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Debug.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
static unsigned short GB_DebugCount = 0;
static unsigned short GB_DebugQRErrorCount = 0;
static float GB_DebugResult[100];
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

void SetGB_DebugCount(unsigned short count)
{
	GB_DebugCount = count;
}
unsigned short GetGB_DebugCount(void)
{
	return GB_DebugCount;
}

void SetGB_DebugQRErrorCount(unsigned short count)
{
	GB_DebugQRErrorCount = count;
}
unsigned short GetGB_DebugQRErrorCount(void)
{
	return GB_DebugQRErrorCount;
}

void SetGB_DebugResult(float result)
{
	GB_DebugResult[GB_DebugCount-1] = result;
}
float GetGB_DebugResult(unsigned char index)
{
	return GB_DebugResult[index];
}

void ClearDebugData(void)
{
	unsigned char i=0;
	
	SetGB_DebugCount(0);
	SetGB_DebugQRErrorCount(0);
	
	for(i=0; i<100; i++)
		GB_DebugResult[i] = 0;
}

#endif
/****************************************end of file************************************************/
