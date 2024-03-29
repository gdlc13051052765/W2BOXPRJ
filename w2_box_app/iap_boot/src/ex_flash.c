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
* 函 数 名： ex_flash_init
* 参    数： 
* 功能描述:  外部flash接口初始化
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2020-03-20 114907
==================================================================================*/
static uint8_t ex_flash_init(void)
{
	//spi接口初始化 
	
	//flash初始化
	
	return 0;
}

/*==================================================================================
* 函 数 名： EarseFlash
* 参    数： 
* 功能描述:  擦除Flash
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-12-02 123204
==================================================================================*/
static uint8_t EraseFlash(uint32_t FlashAddr,uint32_t Len)
{
	//校验数据合法性
  if(Len <= 0)
    return 0x00;
  
		//计算扇区地址和要擦除删除的长度
   gt32_erase_size(FlashAddr, Len);
	//擦除数据
  
  return 0x01;
}

/*==================================================================================
* 函 数 名： ReadFlash
* 参    数： 
* 功能描述:  读Flash
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-12-02 123204
==================================================================================*/
static uint8_t ReadFlash(uint32_t FlashAddr,uint8_t *rBuff,uint32_t Len)
{
	//校验数据合法性
  if(Len <= 0)
    return 0x00;
  gt32_chip_read(FlashAddr, rBuff, Len);
	
	//读取数据
   
  return 0x01;
}

/*==================================================================================
* 函 数 名： EarseFlash
* 参    数： 
* 功能描述:  写Flash
* 返 回 值： None
* 备    注： 不支持补包操作（没有足够缓存申请4K得缓存，存放页缓存信息）
* 作    者： xiaozh
* 创建时间： 2019-12-02 123204
==================================================================================*/
static uint8_t WriteFlash(uint32_t FlashAddr, const uint8_t *wBbuff,uint32_t Len)
{
	uint32_t nAddress = FlashAddr;
	 
	//校验数据合法性
  if(Len <= 0)
    return 0x00; 
	  
	//写入数据
	gt32_chip_write(FlashAddr, wBbuff, Len);
	
  return 0x01; 
}


/*==================================================================================
* 函 数 名： WriteFlashNoCheck
* 参    数： 
* 功能描述:  写Flash
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2021-01-18 034829
==================================================================================*/
static uint8_t WriteFlashNoCheck(uint32_t FlashAddr, const uint8_t *wBbuff,uint32_t Len)
{
	uint32_t nAddress = FlashAddr;
	 
	//校验数据合法性
  if(Len <= 0)
    return 0x00; 
	 
	//解锁FLASH
	
	//写入数据
	//写入数据
	gt32_chip_write(FlashAddr, wBbuff, Len);
	
	//锁住FLASH 
	
  return 0x01; 
}

