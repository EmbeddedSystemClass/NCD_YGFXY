#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		AdminPassWord		"201300\0"								//管理员密码，用于修改设备id
#define		AdjustPassWord		"201301\0"								//校准密码
#define		TestPassWord		"201302\0"								//老化测试密码
#define		CheckQRPassWord		"201303\0"								//测试二维码密码
#define		AdjLedPassWord		"201304\0"								//校准led密码··················
#define		FactoryResetPassWord	"201305\0"							//恢复出厂设置密码
#define		ChangeValueShowTypePassWord	"201306\0"						//切换结果显示模式，是否显示真实值
#define		UnlockLCDPassWord	"201307\0"								//解锁屏幕一次

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceName "荧光免疫定量分析仪\0"
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceName "Fluor Immun Quant Analyzer\0"
#endif

#endif

