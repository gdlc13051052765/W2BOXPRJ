#ifndef  __GT32L_DRV_H
#define  __GT32L_DRV_H
#include <stdint.h>
#include "main.h"
#include "GT32L32M0180.h"


#define GT32L32_FLASH_BASE 			0x000000 	

#define GT32L32_FLASH_PAGE_SIZE			256 
#define GT32L32_FLASH_SECTOR_SIZE		4096
#define GT32L32_FLASH_MAX_ADDR	    0x0FFFFF 


#define WRITE_DATA_CMD			0x02		//Ğ´ÈëÊı¾İÃüÁî
#define SECTOR_ERASE_CMD		0x20		//ÉÈÇø²Á³ıÃüÁî
#define BLOCK_ERASE_CMD			0xD8		//¿ì²Á³ıÃüÁî
#define CHIP_ERASE_CMD0			0x60		//Ğ¾Æ¬²Á³ı0
#define CHIP_ERASE_CMD1			0xC7		//Ğ¾Æ¬²Á³ı1
#define READ_DATA_CMD				0x03		//¶ÁÈ¡Êı¾İÃüÁî
#define WRITE_DIS_CMD				0x04		//Ğ´Ê§ÄÜÃüÁî
#define WRITE_EN_CMD				0x06		//Ğ´Ê¹ÄÜÃüÁî
#define READ_STATUS_CMD			0x05		//¶ÁÈ¡×´Ì¬ÃüÁî
#define FAST_READ_CMD				0x0B		//¿ìËÙ¶ÁÃüÁî
 
uint8_t gt32_sector_erase(uint32_t sector_address);
uint8_t gt32_chip_read(uint32_t address, uint8_t* buff, uint32_t length);
uint8_t gt32_chip_write(uint32_t address, const uint8_t* buff, uint32_t length);
uint8_t gt32_erase_size(uint32_t address, uint32_t length);
#endif
