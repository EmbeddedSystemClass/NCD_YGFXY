#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		DeviceTypeString					"YGFXY_1\0"

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceLanguageString				"CH"
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceLanguageString				"EN"
#endif

#define		AdminPassWord						"201300\0"								//管理员密码，用于修改设备id
#define		TestPassWord						"201302\0"								//老化测试密码
#define		CheckQRPassWord						"201303\0"								//测试二维码密码
#define		AdjLedPassWord						"201304\0"								//校准led密码··················
#define		FactoryResetPassWord				"201305\0"							//恢复出厂设置密码
#define		ChangeValueShowTypePassWord			"201306\0"						//切换结果显示模式，是否显示真实值
#define		UnlockLCDPassWord					"201307\0"								//解锁屏幕一次
#define		EnableUSBPassWord					"201308\0"								//挂载USB

#if (NCDServerType == NCDServerFinal)
#define		NcdServerReadTimeUrlStr				"/NCD_Server/up_dtime\0"
#define		NcdServerUpDeviceUrlStr				"/NCD_Server/up_device\0"
#define		NcdServerUpTestDataUrlStr			"/NCD_Server/UpLoadYGFXY\0"
#define		NcdServerQuerySoftUrlStr			"/NCD_Server/deviceSoftInfo\0"
#define		NcdServerDownSoftUrlStr				"/NCD_Server/DownloadSoftFile\0"
#elif (NCDServerType == NCDServerDebug)
#define		NcdServerReadTimeUrlStr				"/NCD_ServerDebug/up_dtime\0"
#define		NcdServerUpDeviceUrlStr				"/NCD_ServerDebug/up_device\0"
#define		NcdServerUpTestDataUrlStr			"/NCD_ServerDebug/UpLoadYGFXY\0"
#define		NcdServerQuerySoftUrlStr			"/NCD_ServerDebug/deviceSoftInfo\0"
#define		NcdServerDownSoftUrlStr				"/NCD_ServerDebug/DownloadSoftFile\0"
#endif


#define ChangguiStr			"R\0"
#define JizhenStr			"Q\0"
#define ZhikongStr			"S\0"

#define	SuccessStr								"success\0"
#define	FailStr									"fail\0"

#define	USBStartCmdStr							"Please Send Your Data\0"
#define	USBStartCmdStrLen						strlen(USBStartCmdStr)
#define	USBCommunicateCmdStr					">\0"
#define	USBCommunicateCmdStrLen					strlen(USBCommunicateCmdStr)

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceNameStr 		"荧光免疫定量分析仪\0"
	#define	CompanyNameStr		"武汉纽康度生物科技股份有限公司\0"
	#define	TesterNameStr		"测试人\0"
	#define	SampleIdStr			"样品编号\0"
	#define	ItemNameStr			"测试项目\0"
	#define	ResultStr			"测试结果\0"
	#define	ReferenceValueStr	"参考值\0"
	#define	TestTimeStr			"测试时间\0"
	#define	PrintTimeStr		"打印时间\0"
	#define	StatementStr		"声明: 本结果仅对本标本负责!\0"
	
	#define	BloodStr			"全血\0"
	#define	SerumStr			"血清\0"
	#define	PlasmaStr			"血浆\0"
	
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceNameStr 		"Fluor Immun Quant Analyzer\0"
	#define	CompanyNameStr		"Wuhan Newcando Biotechnology Co.,Ltd.\0"
	#define	TesterNameStr		"Tester\0"
	#define	SampleIdStr			"SampleId\0"
	#define	ItemNameStr			"Item\0"
	#define	ResultStr			"Result\0"
	#define	ReferenceValueStr	"Reference\0"
	#define	TestTimeStr			"Test Time\0"
	#define	PrintTimeStr		"Print Time\0"
	#define	StatementStr		"Statement: This result is only responsible for this specimen!\0"
	
	#define	BloodStr			"Blood\0"
	#define	SerumStr			"Serum\0"
	#define	PlasmaStr			"Plasma\0"
	
	
#endif

#endif

