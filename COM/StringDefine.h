#ifndef _STRINGDEFINE_H_H
#define _STRINGDEFINE_H_H

#include	"Define.h"

#define		DeviceTypeString					"YGFXY_1\0"

#if(DeviceLanguage == DEVICE_CN)
	#define	DeviceLanguageString				"CH"
#elif(DeviceLanguage == DEVICE_EN)
	#define	DeviceLanguageString				"EN"
#endif

#define		AdminPassWord						"201300\0"								//����Ա���룬�����޸��豸id
#define		TestPassWord						"201302\0"								//�ϻ���������
#define		CheckQRPassWord						"201303\0"								//���Զ�ά������
#define		AdjLedPassWord						"201304\0"								//У׼led���롤����������������������������������
#define		FactoryResetPassWord				"201305\0"							//�ָ�������������
#define		ChangeValueShowTypePassWord			"201306\0"						//�л������ʾģʽ���Ƿ���ʾ��ʵֵ
#define		UnlockLCDPassWord					"201307\0"								//������Ļһ��
#define		EnableUSBPassWord					"201308\0"								//����USB

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
	#define	DeviceNameStr 		"ӫ�����߶���������\0"
	#define	CompanyNameStr		"�人Ŧ��������Ƽ��ɷ����޹�˾\0"
	#define	TesterNameStr		"������\0"
	#define	SampleIdStr			"��Ʒ���\0"
	#define	ItemNameStr			"������Ŀ\0"
	#define	ResultStr			"���Խ��\0"
	#define	ReferenceValueStr	"�ο�ֵ\0"
	#define	TestTimeStr			"����ʱ��\0"
	#define	PrintTimeStr		"��ӡʱ��\0"
	#define	StatementStr		"����: ��������Ա��걾����!\0"
	
	#define	BloodStr			"ȫѪ\0"
	#define	SerumStr			"Ѫ��\0"
	#define	PlasmaStr			"Ѫ��\0"
	
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

