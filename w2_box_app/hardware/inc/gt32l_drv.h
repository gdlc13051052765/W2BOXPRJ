#ifndef  __GT32L_DRV_H
#define  __GT32L_DRV_H
#include <stdint.h>
#include "main.h"
#include "GT32L32M0180.h"


#define GT32L32_FLASH_BASE 			0x000000 	

#define GT32L32_FLASH_PAGE_SIZE			256 
#define GT32L32_FLASH_SECTOR_SIZE		4096
#define GT32L32_FLASH_MAX_ADDR	    0x0FFFFF 


#define WRITE_DATA_CMD			0x02		//д����������
#define SECTOR_ERASE_CMD		0x20		//������������
#define BLOCK_ERASE_CMD			0xD8		//���������
#define CHIP_ERASE_CMD0			0x60		//оƬ����0
#define CHIP_ERASE_CMD1			0xC7		//оƬ����1
#define READ_DATA_CMD				0x03		//��ȡ��������
#define WRITE_DIS_CMD				0x04		//дʧ������
#define WRITE_EN_CMD				0x06		//дʹ������
#define READ_STATUS_CMD			0x05		//��ȡ״̬����
#define FAST_READ_CMD				0x0B		//���ٶ�����
 
uint8_t gt32_sector_erase(uint32_t sector_address);
uint8_t gt32_chip_read(uint32_t address, uint8_t* buff, uint32_t length);
uint8_t gt32_chip_write(uint32_t address, const uint8_t* buff, uint32_t length);
uint8_t gt32_erase_size(uint32_t address, uint32_t length);
#endif
