#include "includes.h" 
#include "oled_drv.h"
#include "oled_interface.h"
#include "board_info.h"
#include "gt32l_drv.h"

static void oled_init(void);
static uint8_t oled_load_picture_exflash(const uint8_t *data,uint16_t *addr);
static void oled_disp_flash_picture(_Disp_Param);
static void oled_directry_disp(_Disp_Param pmsg);
	
_Oled_Param mOled_Param = 
{
	.w_flash = &mEx_Flash_Func,
	.r_flash = &mEx_Flash_Func,
};

_Oled_Func mOled_Func = 
{
	.init 			= oled_init,
	.updataPic_opt = oled_load_picture_exflash,
	.dispPic_opt = oled_disp_flash_picture,
	.directlyDisp_opt = oled_directry_disp,
};

//oled显示部分接口函数
_pOled_Func pOled_Func = &mOled_Func;


/*==================================================================================
* 函 数 名： oled_init
* 参    数： None
* 功能描述:  oled初始化
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-20 025540
==================================================================================*/
static void oled_init(void)
{

}
/*==================================================================================
* 函 数 名： oled_picture_compare
* 参    数： None
* 功能描述:  图像数据回读比较
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-22 025540
==================================================================================*/
uint8_t oled_picture_compare(const uint8_t *src_buff, const uint8_t *rev_buff, uint8_t len)
{
	for(int i=0; i<len; i++)
	{
		if(src_buff[i] != rev_buff[i])
		{
			return 0x81;
		}
	}
	
	return 0x01;
}
/*==================================================================================
* 函 数 名： oled_load_picture_exflash
* 参    数： None
* 功能描述:  下载图像数据到外部flash
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-20 025540
==================================================================================*/
static uint8_t oled_load_picture_exflash(const uint8_t *data,uint16_t *addr)
{
	uint32_t nAddress = *addr*MAX_DISP_LEN +OLED_PIC_ADDR;	
	uint8_t r_buff[MAX_DISP_LEN] = {0};
	uint8_t try_count =3;
	
	//memcpy(r_buff,data,MAX_DISP_LEN);
	//mOled_Param.w_flash->erase(nAddress,MAX_DISP_LEN);	
	printf("write data==");
	for(int i=0;i<MAX_DISP_LEN;i++)
		printf("%2X ",data[i]);
	printf("\r\n");

	if(nAddress%GT32L32_FLASH_SECTOR_SIZE == 0)
	{
		mOled_Param.w_flash->erase(nAddress,MAX_DISP_LEN);
		HAL_Delay(50);//不加延时flash写失败
	}

	do{
			if(mOled_Param.w_flash->write(nAddress, data, MAX_DISP_LEN) == 0x01)
			{
				if(mOled_Param.r_flash->read(nAddress,r_buff, MAX_DISP_LEN) == 0x01)
				{
					printf("read dara==");
					for(int i=0;i<MAX_DISP_LEN;i++)
						printf("%2X ",r_buff[i]);
					printf("\r\n");
					if(oled_picture_compare(r_buff, data, MAX_DISP_LEN) == 0x01)
					{
						printf("read ok \r\n");
						return 0;
					}
				}
			}	
	}while(try_count--);
	
	return 0xff;
}
/*==================================================================================
* 函 数 名： oled_directry_disp
* 参    数： None
* 功能描述:  直接显示安卓发过来的数据
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-22 025540
==================================================================================*/
static void oled_directry_disp(_Disp_Param pmsg)
{
	oleddrv_disp(pmsg);
}
/*==================================================================================
* 函 数 名： oled_load_picture_exflash
* 参    数： None
* 功能描述:  显示flash里面的图片及发过来的字符串
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-02-22 025540
==================================================================================*/
static void oled_disp_flash_picture(_Disp_Param pmsg )
{
	uint16_t bmplen=0;
  uint8_t bmpdata[1024];
	uint32_t nAddress = pmsg.dispAddr*MAX_DISP_LEN +OLED_PIC_ADDR;	
	
	switch(pmsg.cmd )
	{
		case 01://显示灰度图		
		case 02://显示黑白图
			if((pmsg.endCol>pmsg.startCol )&&(pmsg.endRow >pmsg.startRow))
			{
				bmplen = ((pmsg.endCol - pmsg.startCol)  * (pmsg.endRow - pmsg.startRow));
				if(mOled_Param.r_flash->read(nAddress,bmpdata, bmplen) == 0x01)
				{
					screen_show_bmp(pmsg.id ,pmsg.startCol, pmsg.startRow, pmsg.endCol, pmsg.endRow, bmpdata, 1);
				}
			}
		break;
		
		case 03://显示字符串
			oleddrv_disp(pmsg);
		break;
	}
}
