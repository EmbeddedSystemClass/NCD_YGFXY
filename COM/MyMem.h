#ifndef MYMEM_H_H
#define MYMEM_H_H

#define	MY_HEAP_SIZE	40*1024
#define MY_ALIGNMENT			8
#define MY_ALIGNMENT_MASK ( 0x0007U )

void *MyMalloc( unsigned int xWantedSize );
void MyFree( void *pv );
unsigned int MyGetFreeHeapSize( void );
unsigned int MyGetMinimumEverFreeHeapSize( void );
//  __attribute__((at(0X10000000)))
#endif

