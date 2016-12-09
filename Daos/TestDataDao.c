/***************************************************************************************************
*FileName: TestDataDao
*Description: ��������dao
*Author: xsx_kair
*Data: 2016��12��8��10:43:26
***************************************************************************************************/

/***************************************************************************************************/
/******************************************Header List********************************************/
/***************************************************************************************************/
#include	"TestDataDao.h"

#include	"CRC16.h"
#include	"MyMem.h"

#include	"ff.h"

#include	<string.h>
#include	"stdio.h"
#include 	"stdlib.h"
/***************************************************************************************************/
/***************************************************************************************************/
/***************************************************************************************************/


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
*FunctionName: WriteTestData
*Description: д�������ݵ��ļ�
*Input: testdata -- �������ݵ�ַ
*Output: None
*Return: 	My_Pass -- ����ɹ�
*			My_Fail -- ����ʧ��
*Author: xsx
*Date: 2016��12��8��10:55:53
***************************************************************************************************/
MyState_TypeDef WriteTestData(TestData * testdata)
{
	FatfsFileInfo_Def * myfile = NULL;
	TestDataHead * myTestDataHead;
	MyState_TypeDef statues = My_Fail;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	myTestDataHead = MyMalloc(sizeof(TestDataHead));
	
	if(myfile && myTestDataHead && testdata)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		memset(myTestDataHead, 0, sizeof(TestDataHead));
		
		myfile->res = f_open(&(myfile->file), "0:/TD.NCD", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
			
		if(FR_OK == myfile->res)
		{
			//��ȡ����ͷ
			f_lseek(&(myfile->file), 0);
			f_read(&(myfile->file), myTestDataHead, sizeof(TestDataHead), &(myfile->br));
			
			if(myTestDataHead->crc != CalModbusCRC16Fun1(myTestDataHead, sizeof(TestDataHead)-2))
			{
				myTestDataHead->datanum = 0;
				myTestDataHead->readindex = 0;
			}
			
			myTestDataHead->datanum++;
			
			myTestDataHead->crc = CalModbusCRC16Fun1(myTestDataHead, sizeof(TestDataHead)-2);
			
			//д������
			f_lseek(&(myfile->file), (myTestDataHead->datanum-1) * sizeof(TestData) + sizeof(TestDataHead));

			testdata->crc = CalModbusCRC16Fun1(testdata, sizeof(TestData)-2);
			
			myfile->res = f_write(&(myfile->file), testdata, sizeof(TestData), &(myfile->bw));
			if(myfile->res != FR_OK)
				goto END;
			
			myfile->res = f_sync(&(myfile->file));
			if(myfile->res != FR_OK)
				goto END;
			
			//��������ͷ
			f_lseek(&(myfile->file), 0);
			myfile->res = f_write(&(myfile->file), myTestDataHead, sizeof(TestDataHead), &(myfile->bw));
			if(myfile->res != FR_OK)
				goto END;
			
			myfile->res = f_sync(&(myfile->file));
			if(myfile->res != FR_OK)
				goto END;
			
			if(FR_OK == myfile->res)
				statues = My_Pass;
			
			END:
				f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	MyFree(myTestDataHead);
	
	return statues;
}

/***************************************************************************************************
*FunctionName: ReadTestData
*Description: ��ȡ��������
*Input: readpackage -- ������ȡ��Ϣ�Լ���ȡ������
*Output: None
*Return: 	My_Pass -- ��ȡ�ɹ�
*			My_Fail -- ��ȡʧ��
*Author: xsx
*Date: 2016��12��8��11:25:18
***************************************************************************************************/
MyState_TypeDef ReadTestData(ReadTestDataPackage * readpackage)
{
	FatfsFileInfo_Def * myfile = NULL;
	MyState_TypeDef statues = My_Fail;
	unsigned char i=0;
	
	myfile = MyMalloc(sizeof(FatfsFileInfo_Def));
	
	if(myfile && readpackage)
	{
		memset(myfile, 0, sizeof(FatfsFileInfo_Def));
		
		readpackage->readDataNum = 0;
		
		myfile->res = f_open(&(myfile->file), "0:/TD.NCD", FA_READ);
		
		if(FR_OK == myfile->res)
		{
			myfile->size = f_size(&(myfile->file));
			
			//��ȡ����ͷ
			f_lseek(&(myfile->file), 0);
			f_read(&(myfile->file), &(readpackage->testDataHead), sizeof(TestDataHead), &(myfile->br));
			
			//����ͷУ��ʧ�ܣ����ȡʧ�ܣ����ض�ȡ��ĿΪ0
			if(readpackage->testDataHead.crc != CalModbusCRC16Fun1(&(readpackage->testDataHead), sizeof(TestDataHead)-2))
				goto END;
			
			//��ʼ��ȡ��������������������ʧ�ܣ����ض�ȡ��ĿΪ0
			if((readpackage->startReadIndex + 1) > readpackage->testDataHead.datanum)
				goto END;
			
			if(readpackage->maxReadNum > (readpackage->testDataHead.datanum - readpackage->startReadIndex))
				readpackage->maxReadNum = (readpackage->testDataHead.datanum - readpackage->startReadIndex);
			
			myfile->res = f_lseek(&(myfile->file), (readpackage->testDataHead.datanum - (readpackage->maxReadNum + readpackage->startReadIndex))*sizeof(TestData)+sizeof(TestDataHead));
			if(FR_OK == myfile->res)
			{
				for(i=0; i<readpackage->maxReadNum; i++)
				{
					myfile->res = f_read(&(myfile->file), &(readpackage->testData[readpackage->maxReadNum - 1 - i]), sizeof(TestData), &(myfile->br));
					
					if((FR_OK == myfile->res) && 
						(readpackage->testData[readpackage->maxReadNum - 1 - i].crc == CalModbusCRC16Fun1(&(readpackage->testData[readpackage->maxReadNum - 1 - i]), sizeof(TestData)-2)))
						readpackage->readDataNum++;
					else
						break;
				}
				statues = My_Pass;
			}
			
			END:
				f_close(&(myfile->file));
		}
	}
	
	MyFree(myfile);
	
	return statues;
}

/****************************************end of file************************************************/
