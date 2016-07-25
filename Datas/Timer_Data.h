#ifndef _TIMER_S_D_H__
#define _TIMER_S_D_H__

typedef struct
{
	unsigned int start;
	unsigned int interval;
}Timer;

typedef enum
{ 
	TimeNotTo = 0,		//��ʱʱ��δ��
	TimeOut = 1,		//��ʱʱ�䵽
	TimeError = 2,		//����
}TimerState_Def;

void ResetGBClockTime(void);
void timer_set(Timer *t, unsigned int interval);
void timer_reset(Timer *t);
void timer_restart(Timer *t);
TimerState_Def timer_expired(Timer *t);
unsigned int timer_surplus(Timer *t);
unsigned int timer_used(Timer *t);
unsigned int timer_Count(Timer * t);
#endif
