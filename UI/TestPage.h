#ifndef __TEST_P_H__
#define __TEST_P_H__

#include	"MyTest_Data.h"
#include	"Test_Fun.h"

typedef struct TestPageBuffer_tag {
	ItemData * currenttestdata;
	ResultState cardpretestresult;				//���Խ��
	Basic_ICO myico[3];
	LineInfo line;
}TestPageBuffer;


unsigned char DspTestPage(void *  parm);

#endif

