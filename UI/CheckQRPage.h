#ifndef __CHECKQR_P_H__
#define __CHECKQR_P_H__

#include	"UI_Data.h"

typedef struct CheckQRPageBuffer_tag {
	unsigned short lcdinput[20];
	char buf[100];
	QRCode qrCode;						//二维码数据缓冲区
	ScanCodeResult scancode;
	Timer timer;						//间隔提示插卡
	MyBool isScanning;				//是否正在扫描中
	double inputTC;
	double basicResult;				//计算结果
	MyBool isE;						//是否指数函数
	double tempDouble;
	unsigned char quNum;
}CheckQRPageBuffer;


MyState_TypeDef createCheckQRActivity(Activity * thizActivity, Intent * pram);

#endif

