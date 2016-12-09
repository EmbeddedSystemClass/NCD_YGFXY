/****************************************file start****************************************************/
#ifndef _TESTDATA_D_H
#define	_TESTDATA_D_H

#include	"Define.h"

#define	DataShowNumInPage		8							//һҳ8������

#pragma pack(1)
typedef struct TestDataHead_tag {
	unsigned int datanum;
	unsigned int readindex;
	unsigned short crc;
}TestDataHead;
#pragma pack()

#pragma pack(1)
typedef struct ReadTestDataPackage_Tag
{
	TestData testData[DataShowNumInPage];									//��ȡ���ݻ�����
	TestDataHead testDataHead;												//�������ݱ����ļ����ļ�ͷ
	unsigned int startReadIndex;											//��ʼ��ȡ����,0��ʼ
	unsigned char maxReadNum;												//����ȡ��Ŀ
	unsigned char readDataNum;												//�ɹ���ȡ����������Ŀ
}ReadTestDataPackage;
#pragma pack()

MyState_TypeDef WriteTestData(TestData * testdata);
MyState_TypeDef ReadTestData(ReadTestDataPackage * readpackage);

#endif

/****************************************end of file************************************************/
