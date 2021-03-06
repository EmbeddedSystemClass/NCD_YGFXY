/****************************************file start****************************************************/
#ifndef	MD5_C_H
#define	MD5_C_H

#define FOPRTXT "r"  
  
  
struct MD5Context {  
    unsigned int buf[4];  
    unsigned int bits[2];  
    unsigned char in[64];  
};

void MD5Init(struct MD5Context *context);  
void MD5Update(struct MD5Context *context, unsigned char const *buf, unsigned len);  
void MD5Final(unsigned char digest[16], struct MD5Context *context);  
void MD5Transform(unsigned int buf[4], unsigned int const in[16]);  
  
/* 
 * This is needed to make RSAREF happy on some MS-DOS compilers. 
 */  
typedef struct MD5Context MD5_CTX;  


void md5sum(char * md5Buf) ;  
void md5sumOneTime(char * md5Buf, void * dataBuf, unsigned short dataLen);

#endif

/****************************************end of file************************************************/
