#ifndef __RCODE_F_H__
#define __RCODE_F_H__

#include	"Define.h"

#define	MAX_QR_CODE_LENGHT	300					//��ά����󳤶�
#define	MAX_SCAN_QR_TIME	7					//ɨ���ά��ʱ��

#pragma pack(1)
typedef struct ReadQRCodeBuffer_tag {
	char originalcode[MAX_QR_CODE_LENGHT+1];						//ԭʼ�Ķ�ά��
	unsigned short originalCodeLen;									//ԭʼ���ݳ���
	char decryptcode[MAX_QR_CODE_LENGHT+1];						//���ܺ�Ķ�ά��
	char tempbuf[64];											//��ʱ����
	char * pbuf1;												//��ʱָ��1
	char * pbuf2;												//��ʱָ��2
	MyTime_Def temptime;										//���浱ǰʱ��
	QRCode * cardQR;											//�����ά��
	Timer timer;												//��ʱ��
	unsigned short motorLocation;								//���λ��
	unsigned char motorDir;										//�������
	ScanCodeResult scanresult;
}ReadQRCodeBuffer;
#pragma pack()

ScanCodeResult ScanCodeFun(QRCode * parm);

#endif

