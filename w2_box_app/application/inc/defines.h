#ifndef  __DEFINES_H
#define  __DEFINES_H
#include "stm32f1xx_hal.h"

typedef unsigned char						u8;
typedef unsigned char						byte;
typedef unsigned char						uchar;
typedef unsigned char						uint8;
typedef unsigned char						uint8_t;
typedef unsigned char						BYTE;

typedef unsigned int 						u16;
typedef unsigned int						uint;
typedef unsigned int						UInt16;
typedef unsigned int						uint16;
//typedef unsigned int						uint16_t;
typedef unsigned int						HALF;


typedef unsigned long						u32;
typedef unsigned long						ulong;
typedef unsigned long						UInt32;
typedef unsigned long						uint32;
//typedef unsigned long						uint32_t;
//typedef unsigned long						WORD;
 
#ifndef  bool
	typedef enum {false = 0, true = !false} bool;
#endif
	
//typedef unsigned           int uint32_t;
//#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
	


//typedef __IO uint32_t  vu32;

////uint32
//#define BigtoLittle32(A)   ((( (uint32_t)(A) & 0xff000000 ) >> 24) | \
//                                               (( (uint32_t)(A) & 0x00ff0000 ) >> 8)   | \
//                                               (( (uint32_t)(A) & 0x0000ff00 ) << 8)   | \
//                                               (( (uint32_t)(A) & 0x000000ff ) << 24))

////uint16
//#define BigtoLittle16(A)   (( ((uint16_t)(A) & 0xff00) >> 8 )    | \
//                            (( (uint16_t)(A) & 0x00ff ) << 8))
//	
//typedef enum
//{
//	RES_OK=0x01,		//返回成功
//	RES_ERROR				//返回失败
//}RES_STATE;


#define mMIN(a,b) (((a)<(b))?(a):(b))
#define mMAX(a,b) (((a)>(b))?(a):(b))

#endif

