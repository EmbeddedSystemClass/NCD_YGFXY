/****************************************file start****************************************************/
#ifndef _APPFILE_D_H
#define	_APPFILE_D_H

#include	"Define.h"

MyState_TypeDef WriteAppFile(char * file, unsigned short len, bool isNew);
MyState_TypeDef ReadAppFile(unsigned int startAddr, char * dataBuf, unsigned short size, unsigned short *br);
MyState_TypeDef checkNewAppFileIsExist(void);
MyState_TypeDef deleteAppFileIfExist(void);
#endif

/****************************************end of file************************************************/
