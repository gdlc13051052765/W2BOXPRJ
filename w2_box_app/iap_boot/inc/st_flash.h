#ifndef  __ST_FLASH_H
#define  __ST_FLASH_H
#include <stdint.h>
#include <stdbool.h>
#include "iap_struct.h"

#define  STM32_FLASH_BASE					0x08000000

#define  STM32F105RB_PAGE_SIZE					(2*1024)		//page��С  	STM32F105RB
#define  STM32F105RB_TOTAL_SIZE					(128*1024)		//�ܴ�С  	STM32F105RB
#define  STM32F105RB_FLASH_END   				(STM32_FLASH_BASE | STM32F105RB_TOTAL_SIZE)

#define  STM32F103C8_PAGE_SIZE					(1*1024)		//page��С		STM32F103C8
#define  STM32F103C8_TOTAL_SIZE					(64*1024)		//�ܴ�С		STM32F103C8
#define  STM32F103C8_FLASH_END   				(STM32_FLASH_BASE | STM32F103C8_TOTAL_SIZE)

 

#define STM32FLASH_BASE						STM32_FLASH_BASE
#define STM32FLASH_SIZE						STM32F105RB_TOTAL_SIZE
#define STM32FLASH_PAGE_SIZE     	STM32F105RB_PAGE_SIZE
#define STM32FLASH_END						STM32F105RB_FLASH_END
/* FLASH��ҳ�� */
#define STM32FLASH_PAGE_NUM  (STM32FLASH_SIZE / STM32FLASH_PAGE_SIZE)


#define  STM32_FLAG_PAGE(page)		(STM32_FLASH_BASE+1024*page)


#define  STM32_FLASH_PAGE0		STM32_FLAG_PAGE(0)
#define  STM32_FLASH_PAGE1		STM32_FLAG_PAGE(1)
#define  STM32_FLASH_PAGE2		STM32_FLAG_PAGE(2)
#define  STM32_FLASH_PAGE3		STM32_FLAG_PAGE(3)
#define  STM32_FLASH_PAGE4		STM32_FLAG_PAGE(4)

extern const _Flash_Func stFlash_Func ;
 
#endif