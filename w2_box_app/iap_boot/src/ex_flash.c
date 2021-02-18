#include "ex_flash.h"
#include "iap_struct.h" 
#include "gt32l_drv.h"

static uint8_t ex_flash_init(void);
static uint8_t EraseFlash(uint32_t FlashAddr,uint32_t Len);
static uint8_t ReadFlash(uint32_t FlashAddr,uint8_t *rBuff,uint32_t Len);
static uint8_t WriteFlash(uint32_t FlashAddr,const uint8_t *wBbuff,uint32_t Len); 
static uint8_t WriteFlashNoCheck(uint32_t FlashAddr, const uint8_t *wBbuff,uint32_t Len);

const _Flash_Func mEx_Flash_Func = 
{
	.init						= ex_flash_init,
  .erase 					= EraseFlash,
  .write					= WriteFlash,
	.write_nocheck 	= WriteFlashNoCheck,
  .read						= ReadFlash,
};

/*==================================================================================
* �� �� ���� ex_flash_init
* ��    ���� 
* ��������:  �ⲿflash�ӿڳ�ʼ��
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2020-03-20 114907
==================================================================================*/
static uint8_t ex_flash_init(void)
{
	//spi�ӿڳ�ʼ�� 
	
	//flash��ʼ��
	
	return 0;
}

/*==================================================================================
* �� �� ���� EarseFlash
* ��    ���� 
* ��������:  ����Flash
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-12-02 123204
==================================================================================*/
static uint8_t EraseFlash(uint32_t FlashAddr,uint32_t Len)
{
	//У�����ݺϷ���
  if(Len <= 0)
    return 0x00;
  
		//����������ַ��Ҫ����ɾ���ĳ���
   gt32_erase_size(FlashAddr, Len);
	//��������
  
  return 0x01;
}

/*==================================================================================
* �� �� ���� ReadFlash
* ��    ���� 
* ��������:  ��Flash
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-12-02 123204
==================================================================================*/
static uint8_t ReadFlash(uint32_t FlashAddr,uint8_t *rBuff,uint32_t Len)
{
	//У�����ݺϷ���
  if(Len <= 0)
    return 0x00;
  gt32_chip_read(FlashAddr, rBuff, Len);
	
	//��ȡ����
   
  return 0x01;
}

/*==================================================================================
* �� �� ���� EarseFlash
* ��    ���� 
* ��������:  дFlash
* �� �� ֵ�� None
* ��    ע�� ��֧�ֲ���������û���㹻��������4K�û��棬���ҳ������Ϣ��
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-12-02 123204
==================================================================================*/
static uint8_t WriteFlash(uint32_t FlashAddr, const uint8_t *wBbuff,uint32_t Len)
{
	uint32_t nAddress = FlashAddr;
	 
	//У�����ݺϷ���
  if(Len <= 0)
    return 0x00; 
	  
	//д������
	gt32_chip_write(FlashAddr, wBbuff, Len);
	
  return 0x01; 
}


/*==================================================================================
* �� �� ���� WriteFlashNoCheck
* ��    ���� 
* ��������:  дFlash
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2021-01-18 034829
==================================================================================*/
static uint8_t WriteFlashNoCheck(uint32_t FlashAddr, const uint8_t *wBbuff,uint32_t Len)
{
	uint32_t nAddress = FlashAddr;
	 
	//У�����ݺϷ���
  if(Len <= 0)
    return 0x00; 
	 
	//����FLASH
	
	//д������
	//д������
	gt32_chip_write(FlashAddr, wBbuff, Len);
	
	//��סFLASH 
	
  return 0x01; 
}

