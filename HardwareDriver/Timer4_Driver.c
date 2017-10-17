/***************************************************************************************************
*FileName:
*Description:
*Author: xsx_kair
*Data:
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"Timer4_Driver.h"
#include	"stm32f4xx_tim.h"
#include	"CardLimit_Driver.h"
#include	"DRV8825_Driver.h"
#include	"Define.h"
#include	"Motor_Fun.h"
#include	"Motor.h"
#include	"Motor_Data.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
extern Motor GB_Motors;
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/

/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/****************************************File Start*************************************************/
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************
*FunctionName:Timer_Init
*Description:???3???,?????1S??
*Input:None
*Output:None
*Author:xsx
*Data:2016?5?14?15:48:03
***************************************************************************************************/
void Timer4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //???PF9
	GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period= 50;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//??????14
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE?? 

    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //???3??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x04; //?????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //????3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

void StartTimer4(void)
{
	TIM_Cmd(TIM4, ENABLE); 						//?????3
	TIM_SetCounter(TIM4, 200);
}

void StopTimer4(void)
{
	TIM_Cmd(TIM4, DISABLE); 						//?????3
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //????
	{
		if(GB_Motors.moveStepNum > 0)
		{
			SetDRVSleepStatues(OnLine);
			
			GB_Motors.periodCnt++;
			
			if(GB_Motors.periodCnt <= GB_Motors.highTime)
				GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET);
			else if(GB_Motors.periodCnt < GB_Motors.lowTime)
				GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
			else if(GB_Motors.periodCnt == GB_Motors.lowTime)
			{
				GB_Motors.periodCnt = 0;
				GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
				if(GB_Motors.moveStepNum > 0)
					GB_Motors.moveStepNum--;

				if(GB_Motors.isFront)
					GB_Motors.motorLocation++;
				else if(GB_Motors.motorLocation > 0)
					GB_Motors.motorLocation--;
				
				if(BackLimited)
					GB_Motors.motorLocation = 0;
				
				if(PreLimited)
					GB_Motors.motorLocation = 60000;
				
				if(GB_Motors.motorLocation == GB_Motors.motorTargetLocation)
					GB_Motors.moveStepNum = 0;
			}
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //???????
}
/****************************************end of file************************************************/

