/***************************************************************************************************
*FileName��CardLimit_Driver
*Description����⿨��λ����
*Author��xsx
*Data��2016��5��3��10:48:53
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"CardLimit_Driver.h"

#include	"Motor_Data.h"
#include	"CardStatues_Data.h"

#include	"Define.h"
#include	"stdio.h"

#include	"Delay.h"

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
*FunctionName��DRV_Init
*Description�����IO��ʼ��
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��23��11:30:52
***************************************************************************************************/
void Limit_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef   	NVIC_InitStructure;
//	EXTI_InitTypeDef   	EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(PreLimit_RCC | BackLimit_RCC | CardIn_RCC, ENABLE); /*ʹ��LED��ʹ�õ�GPIOʱ��*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = PreLimit_Pin; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PreLimit_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = BackLimit_Pin; 
	GPIO_Init(BackLimit_Group, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CardIn_Pin; 
	GPIO_Init(CardIn_Group, &GPIO_InitStructure);
	
/*	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PA0 ���ӵ��ж���0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);//PA0 ���ӵ��ж���0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);//PA0 ���ӵ��ж���0
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);*/

}


/***************************************************************************************************
*FunctionName��EXTI9_5_IRQHandler
*Description����������ж�
*Input��None
*Output��None
*Author��xsx
*Data��2016��4��23��11:31:10
***************************************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	/*����λ*/
/*	if(EXTI_GetITStatus(EXTI_Line13) != RESET)	 
	{
		if(BackLimitPinIn)
			SetMotorBackLimitState(NotLimitted);
		else
			SetMotorBackLimitState(Limitted);
		
		SetGB_MotorLocation(0);
		
		SetDRVDir(Forward);
		EXTI_ClearITPendingBit(EXTI_Line13);
	}*/
	/*ǰ��λ*/
/*	else if(EXTI_GetITStatus(EXTI_Line14) != RESET)	 
	{	
		if(PreLimitPinIn)
			SetMotorPreLimitState(NotLimitted);
		else
			SetMotorPreLimitState(Limitted);
		
		SetGB_MotorLocation(MaxLocation);
		SetDRVDir(Reverse);
		SetDRVSleepStatues(OnLine);
		EXTI_ClearITPendingBit(EXTI_Line14);
	}*/
	/*������*/
/*	else if(EXTI_GetITStatus(EXTI_Line15) != RESET)	 
	{			
		if(CardPinIn)
			SetCardState(CardIN);
		else
			SetCardState(NoCard);
	
		EXTI_ClearITPendingBit(EXTI_Line15);
	}	*/
}


