/****************************************file start****************************************************/
#ifndef	BACKDOOR_D_H
#define	BACKDOOR_D_H

#pragma pack(1)
typedef struct BackDoorData_tag {
	float data1;
	float data2[6];
	float data3[4];
	float data4[2];
	float data5[2];
}BackDoorData;
#pragma pack()

//ÿ��ֵ���Դ���
#define	data1_t	20											//��ͼ����
#define	data2_t	3											//����
#define	data3_t	3											//׼ȷ��
#define	data4_t	10											//�ظ���
#define	data5_t	9											//�����

void SetS_TestItemName(void * name);
void SetS_ItemIndex(unsigned char index);
unsigned char GetS_ItemIndex(void);
void SetS_CategoryIndex(unsigned char index);
unsigned char GetS_CategoryIndex(void);
void SetS_TestIndex(unsigned char index);
unsigned char GetS_TestIndex(void);
double GetCurrentData(void);

void setwifiResult(double data);
double getwifiResult(void);
#endif

/****************************************end of file************************************************/
