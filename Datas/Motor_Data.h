#ifndef _MOTOR_S_D_H__
#define _MOTOR_S_D_H__


/*�������ģʽ*/
typedef enum
{ 
	Absolute_L = 1,							//���Զ�λ
	Relative_L = 2,							//��Զ�λ
}MotorWorkMode_Def;

/*���͸����������*/
typedef struct
{ 
	MotorWorkMode_Def mode;
	short value;
}MotorWorkValue_Def;

typedef enum
{ 
	NotLimitted = 0,						//û��λ
	Limitted = 1							//��λ
}LimitState_Def;





typedef enum
{ 
	OnLine = 0,										//�������
	OffLine	= 1										//�������
}DRVSleep_Type;

typedef enum
{ 
	Forward = 1,									//�������
	Reverse	= 0										//�������
}DRVDir_Type;

typedef enum
{ 
	LowPower = 1,									//����͹���
	NonamalPower = 0								//�����������
}DRVPower_Type;

typedef enum
{ 
	FastMode = 1,									//����͹���
	LowMode = 0										//�����������
}DRVDecay_Type;


LimitState_Def GetMotorPreLimitState(void);
void SetMotorPreLimitState(LimitState_Def value);

LimitState_Def GetMotorBackLimitState(void);
void SetMotorBackLimitState(LimitState_Def value);

unsigned short GetGB_MotorLocation(void);
void SetGB_MotorLocation(unsigned short value);

unsigned short GetGB_MotorTargetLocation(void);
void SetGB_MotorTargetLocation(unsigned short value);

unsigned short GetGB_MotorDir(void);
void SetGB_MotorDir(DRVDir_Type dir);

#endif

