/***************************************************************************************************
*FileName��Timer_Driver
*Description����ʱ��������ʹ��ͨ�ö�ʱ��3��1Sʱ�������ڼ�⿨��Ӧʱ�䵹��ʱ
*Author��xsx
*Data��2015��8��26��14:30:46
***************************************************************************************************/

/***************************************************************************************************/
/******************************************ͷ�ļ�***************************************************/
/***************************************************************************************************/

#include	"Timer4_Driver.h"
#include	"stm32f4xx_tim.h"
#include	"CardLimit_Driver.h"
#include	"DRV8825_Driver.h"
#include	"Motor_Fun.h"
/***************************************************************************************************/
/**************************************�ֲ���������*************************************************/
/***************************************************************************************************/
extern unsigned char motorstautes;
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
*FunctionName��Timer_Init
*Description����ʱ��3��ʼ�����������ó�1S�ж�
*Input��None
*Output��None
*Author��xsx
*Data��2016��5��14��15:48:03
***************************************************************************************************/
void Timer4_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PF9
	  GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period= 200;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //����ʱ��3�����ж�
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 100-1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	
	
    NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x04; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	StopTimer4();
}

void StartTimer4(void)
{
	TIM_Cmd(TIM4, ENABLE); 						//ʹ�ܶ�ʱ��3
	TIM_SetCounter(TIM4, 200);
}

void StopTimer4(void)
{
	TIM_Cmd(TIM4, DISABLE); 						//ʹ�ܶ�ʱ��3
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		SetDRVSleepStatues(OnLine);
		
		if(GetGB_MotorDir() == Forward)
			SetGB_MotorLocation(GetGB_MotorLocation() + 1);
		else
			SetGB_MotorLocation(GetGB_MotorLocation() - 1);
		
		if(BackLimited)
			SetGB_MotorLocation(0);
		
		if(PreLimited)
			SetGB_MotorLocation(MaxLocation);
		
		if(GetGB_MotorTargetLocation() > GetGB_MotorLocation())
			SetDRVDir(Forward);
		else
			SetDRVDir(Reverse);
	
		if(GetGB_MotorTargetLocation() == GetGB_MotorLocation())
		{
			StopTimer4();
			motorstautes = 0;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}

