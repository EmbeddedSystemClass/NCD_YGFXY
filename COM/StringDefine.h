#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		AdminPassWord		"201300\0"								//����Ա���룬�����޸��豸id
#define		AdjustPassWord		"201301\0"								//У׼����
#define		TestPassWord		"201302\0"								//�ϻ���������
#define		CheckQRPassWord		"201303\0"								//���Զ�ά������
#define		AdjLedPassWord		"201304\0"								//У׼led���롤����������������������������������
#define		FactoryResetPassWord	"201305\0"							//�ָ�������������
#define		ChangeValueShowTypePassWord	"201306\0"						//�л������ʾģʽ���Ƿ���ʾ��ʵֵ
#define		UnlockLCDPassWord	"201307\0"								//������Ļһ��

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceName "ӫ�����߶���������\0"
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceName "Fluor Immun Quant Analyzer\0"
#endif

#endif

