/****************************************file start****************************************************/
#ifndef	PRINTF_F_H
#define	PRINTF_F_H

#include	"Define.h"

typedef struct PrintBuffer_tag {
	PrintInfo printInfo;
    TestData tempTestData;
	char tempBuf[100];
}PrintBuffer;

void PrintfData(TestData * testd);

#endif

/****************************************end of file************************************************/
