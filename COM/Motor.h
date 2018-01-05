/****************************************file start****************************************************/
#ifndef _MOTOR_E_H
#define	_MOTOR_E_H

#include	"Define.h"

#pragma pack(1)
typedef struct Motor_Tag
{
	unsigned char highTime;							//高电平时间，*100us
	unsigned char lowTime;							//低电平时间，*100us
	unsigned char periodCnt;						//当前周期内，定时器溢出中断次数
	MyBool isFront;									//是否前进
	unsigned int moveStepNum;						//运动步数
	unsigned int motorLocation;					//电机当前位置
	unsigned int motorTargetLocation;				//电机目标位置
	unsigned int motorMaxLocation;				//电机最大位置
	unsigned int parm1;							//电机参数1
	MyBool parm2; 									//电机参数2
	unsigned int parm3; 									//电机参数3
}Motor;
#pragma pack()

#endif

/****************************************end of file************************************************/
