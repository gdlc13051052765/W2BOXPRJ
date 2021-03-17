#include "oled_drv.h"
#include "includes.h"


static _Screen_Info mScreen_Info[MAX_SCREEN_NUM];

static _pSpi_Class pScreen_R =  &mSpi_Class[oled_1];
static _pSpi_Class pScreen_L =  &mSpi_Class[oled_2];

/*==================================================================================
* 函 数 名： oled_write_byte
* 参    数： screen: 0左屏， 1右屏
*						dat:要写入的数据/命令 
*            cmd:数据/命令标志 0,表示命令;1,表示数据;
* 功能描述:  向SSD1315写入一个字节	
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2015.10
==================================================================================*/
void oled_write_byte(uint8_t screen,uint8_t value,uint8_t cmd_type)
{
	_pSpi_Class pthis = NULL; 
	
  if(cmd_type)
	{ 
    OLED_DC_HIGH();
	}
  else 
	{
		OLED_DC_LOW();
	}
  
	if(screen == SCREEN_LEFT)
	{ 
		pthis = pScreen_L;
		
	}
	else if(screen == SCREEN_RIGHT)
	{ 
		pthis = pScreen_R; 
	}
	else
	{
		return ;
	}
	
	pthis->cs_set(pthis->pContext, 0);	//cs 0
	pthis->sed_rev_byte(pthis->pContext, value);
	pthis->cs_set(pthis->pContext, 1); //cs 1
	 
	OLED_DC_HIGH();   	  
}
 

 /*==================================================================================
* 函 数 名： screen_aversion
* 参    数： 0
* 功能描述:  屏幕显示移位
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-19 171313
==================================================================================*/
void screen_aversion(uint8_t screen,uint8_t row,uint8_t col)
{
	oled_write_byte(screen, 0x21, OLED_CMD);    //0x21,横向显示范围,page0~page15
	oled_write_byte(screen, col/8,OLED_CMD); 
	oled_write_byte(screen, 0x0f, OLED_CMD); 
	oled_write_byte(screen, 0x22, OLED_CMD); //0x22,垂直显示范围,row0~row63
	oled_write_byte(screen, row,  OLED_CMD); 
	oled_write_byte(screen, 0x3f, OLED_CMD); 
	
}
 /*==================================================================================
* 函 数 名： screen_refresh
* 参    数： 0
* 功能描述:  屏幕刷新
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-19 171313
==================================================================================*/
void screen_refresh(uint8_t screen)
{
  uint16_t i,n;		 
 	_pScreen_Info pthis = NULL;
 
	pthis = &mScreen_Info[screen]; 
	
	//刷新屏幕显示
	for(i=0;i<DISP_WIDTH;i++){  
//		oled_write_byte(screen, 0xb0+i, OLED_CMD);    //设置页地址（0~7）
//		oled_write_byte(screen, 0x00, OLED_CMD);      //设置显示位置—列低地址
//		oled_write_byte(screen, 0x10, OLED_CMD);      //设置显示位置—列高地址  
		
		for(n=0;n<DISP_HEIGHT;n++){
			oled_write_byte(screen,pthis->frame_buffer[n][i],OLED_DATA);
		} 
	}
}

/*==================================================================================
* 函 数 名： screen_clear
* 参    数： None
* 功能描述:  清屏
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-23 115439
==================================================================================*/ 
void screen_clear(uint8_t screen, uint8_t color)
{
  uint16_t i,n;
	
 	_pScreen_Info pthis = NULL;
 
	pthis = &mScreen_Info[screen]; 

	//刷新屏幕显示
	for(i=0;i<DISP_WIDTH;i++)  
	{
		for(n=0;n<DISP_HEIGHT;n++)
		{
			pthis->frame_buffer[n][i] = ((color == BLACK)?(0x00):(0xFF));
		} 
	}
	screen_aversion(screen,0,0);
	screen_refresh(screen);		//刷新屏幕
}

/*==================================================================================
* 函 数 名： screen_refresh_all
* 参    数： 0
* 功能描述:  刷新所有屏幕显示
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 115958
==================================================================================*/
void screen_refresh_all(void)
{
	screen_refresh(SCREEN_LEFT);
	screen_refresh(SCREEN_RIGHT);
}

 /*==================================================================================
* 函 数 名： screen_display_on
* 参    数： 0
* 功能描述:  开显示
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_display_on(uint8_t screen)
{
	oled_write_byte(screen,0X8D,OLED_CMD);  //SET DCDC命令
	oled_write_byte(screen,0X14,OLED_CMD);  //DCDC ON
	oled_write_byte(screen,0XAF,OLED_CMD);  //DISPLAY ON 
}

 /*==================================================================================
* 函 数 名： screen_display_off
* 参    数： 0
* 功能描述:  关显示
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_display_off(uint8_t screen)
{
	 oled_write_byte(screen,0X8D,OLED_CMD);  //SET DCDC命令
	 oled_write_byte(screen,0X10,OLED_CMD);  //DCDC OFF
	 oled_write_byte(screen,0XAE,OLED_CMD);  //DISPLAY OFF
}

 /*==================================================================================
* 函 数 名： screen_set_pos
* 参    数： 0
* 功能描述:  设置屏幕显示位置
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_set_pos(uint8_t screen, uint8_t x, uint8_t y)
{
	 oled_write_byte(screen,0xb0+y,OLED_CMD);
	 oled_write_byte(screen,((x&0xf0)>>4)|0x10,OLED_CMD);
	 oled_write_byte(screen,(x&0x0f)|0x01,OLED_CMD); 
}

 /*==================================================================================
* 函 数 名： screen_draw_point
* 参    数： None
* 功能描述:  打点函数
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_draw_point(uint8_t screen, uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t pos, bx, temp = 0;
	
  if((x > (X_MAX_PIXEL-1)) || (y > (Y_MAX_PIXEL-1)))
	{
		return ;
	}
	
	bx = y%8;
	pos	 = y/8;
	temp = 1<<(bx);
	 
	if(color)
	{
		mScreen_Info[screen].frame_buffer[x][pos] |= temp;
	}
	else
	{
		mScreen_Info[screen].frame_buffer[x][pos] &= ~temp;
	}
	 
//	screen_refresh_all(); //刷新显示
}

/*==================================================================================
* 函 数 名： screen_show_char
* 参    数： font_size:字体大小：14、 24、 32
* 功能描述:  显示一个字符
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_show_char(uint8_t screen, uint8_t x, uint8_t y, uint8_t *c_buff, uint8_t c_size, uint8_t color)
{
	c_size = 16;
	uint8_t temp = 0;
	uint8_t y0 = y;
	for(int i=0; i<c_size; i++)
	{
		temp = c_buff[i];
		for(int j=0; j<8; j++)
		{
			if(temp & 0x80)
			{
				screen_draw_point(screen, x, y, color);
			}
			else
			{
				screen_draw_point(screen, x, y, !color);
			}
			
			temp <<= 1;
			y++;
			if((y-y0) == c_size)
			{
				y=y0;
				x++;
				break;
			}
		}
	}
}

/*==================================================================================
* 函 数 名： screen_show_char
* 参    数： font_size:字体大小：14、 24、 32
* 功能描述:  显示一个字符
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-20 121045
==================================================================================*/
void screen_show_bmp(uint8_t screen, uint8_t x_s, uint8_t y_s, uint8_t x_e, uint8_t y_e, uint8_t *c_buff, uint8_t color)
{
	uint8_t x, y;
	uint16_t j = 0;
 	_pScreen_Info pthis = NULL;
	pthis = &mScreen_Info[screen]; 
	  
	for(x=x_s; x<x_e; x++)
	{
		for(y=y_s; y<y_e; y++)
		{
		//	for(x=x_s; x<x_e; x++)
			{
				pthis->frame_buffer[x][y] = c_buff[j++];
			}
		}
	}
	screen_refresh(screen);		//刷新屏幕
}
 

/*横置横排打点函数 -----------------------------------------------------------------*/
void WriteData(uint8_t screen, uint16_t Xpos, uint16_t Ypos, uint8_t data, uint16_t charColor, uint16_t bkColor,uint8_t sizeType)
{
		uint16_t j,i;
		unsigned char count=0;
		for( j=0; j<8; j++ )
		{
				if( ((data >> (7-j))&0x01)== 0x01 ){
						for(count=0;count<sizeType;count++){
							for(i=0;i<sizeType;i++){
								screen_draw_point(screen, Xpos + sizeType*j+count, Ypos+i, charColor );	//修改此函数
							}
						}
				}
				else{
						for(count=0;count<sizeType;count++){
							for(i=0;i<sizeType;i++){
								screen_draw_point(screen, Xpos + sizeType*j+count, Ypos+i, bkColor );	//修改此函数
							}
						}    
				}
		}
}
 

/********o横置横排显示函数************/
void DisZK_DZ_W(uint8_t screen, uint16_t Xpos, uint16_t Ypos, uint16_t W,uint16_t H, uint16_t charColor, uint16_t bkColor,uint8_t*DZ_Data,uint8_t sizeType)
{
	uint16_t Vertical,Horizontal;
	uint16_t n_Vertical = Vertical;
	uint32_t bit=0;
	Vertical=Ypos;
  Horizontal=Xpos; 
	for(bit=0;bit<((W+7)/8*H);bit++) //data sizeof (byte)
	{
			if((bit%((W+7)/8)==0)&&(bit>0))//W/8 sizeof
			{
				Horizontal=Xpos;
				n_Vertical=Vertical+sizeType;				
			}
			else if(bit>0)
			{
				Vertical+=sizeType;
				Horizontal+=sizeType*8; 
				n_Vertical=Vertical;
			}
				
			WriteData(screen, Horizontal,n_Vertical,DZ_Data[bit],charColor,bkColor,sizeType);
	}
}

/*==================================================================================
* 函 数 名： screen_show_string
* 参    数： font_size:字体大小：14、 24、 32
* 功能描述:  显示一个字符串
* 返 回 值： None
* 备    注： 
* 作    者：  
* 创建时间： 2019-09-23 120610
==================================================================================*/
void screen_show_string(_pFont_Info pmsg)
{
	gt_get_func p_func;
	
	uint8_t p_bits[512] = {0};
	uint8_t x = 0, y=0;
	uint8_t *p_str = pmsg->p_text;
	uint8_t ascii_size = 0, ascii_type = 0;
	uint8_t hz_size = 0;
	
	if(pmsg->p_text == NULL)		//判读数据合法性
	{
		return ;
	}
	
	//字体选择
	if(pmsg->font_size == FONT_12)		//12号
	{ 
		hz_size = 12;
		ascii_size = 12;
		ascii_type = ASCII_12_A;
		
		p_func = gt_12_GetData;	//汉字获取接口
	}
	else if(pmsg->font_size == FONT_16)		//16号
	{ 
		hz_size = 16;
		ascii_size = 16;
		ascii_type = ASCII_16_A;
		
		p_func = gt_16_GetData;	//汉字获取接口
	}
	else if(pmsg->font_size == FONT_24)		//24号
	{
		ascii_size = 24;
		hz_size = 24;
		ascii_type = ASCII_24_A;
		
		p_func = gt_24_GetData;	//汉字获取接口
	}
	else if(pmsg->font_size == FONT_32)		//32号
	{
		ascii_size = 32;
		hz_size = 32;
		ascii_type = ASCII_32_A;
		
		p_func = gt_32_GetData;	//汉字获取接口
	}
	else
	{
		ascii_size = 16;
		hz_size = 16;
		ascii_type = ASCII_16_A;
		p_func = gt_16_GetData;	//汉字获取接口
	}
	
	x = pmsg->x;
	y = pmsg->y;
	
	//显示
	while(*p_str != '\0')	//判断结尾
	{
		if(*p_str < 0x80)	//ASCII码
		{
			ASCII_GetData(*p_str, ascii_type, p_bits);	//示例, 按实际函数名进行修改, 每次读取第一个字节为0xFF
			DisZK_DZ_W(pmsg->screen, x, y,ascii_size,ascii_size,pmsg->font_color ,!pmsg->font_color ,p_bits,pmsg->font_scale);  //显示8X16点ASCII函数
			x=x+ascii_size*pmsg->font_scale;
		}
		else
		{//汉字编码*text是汉字编码的高位，*(text+1)是汉字编码的低位
			p_func(*p_str, *(p_str+1),0, 0, p_bits);	//示例, 按实际函数名进行修改
			DisZK_DZ_W(pmsg->screen, x,y,hz_size,hz_size,pmsg->font_color, !pmsg->font_color ,p_bits,pmsg->font_scale);  //显示汉字
			x=x+hz_size*pmsg->font_scale;
			p_str++; 
		}
		
		p_str++;
	}
}
   
/*==================================================================================
* 函 数 名： oled_init
* 参    数： None
* 功能描述:  OLED屏初始化
* 返 回 值： None
* 备    注： SSD1315
* 作    者：  xiaozh
* 创建时间： 2019-09-23 132006
==================================================================================*/
void oled_init(uint8_t screen)
{
//	oled_write_byte(screen,0xAE,OLED_CMD);//--turn off oled panel
//	oled_write_byte(screen,0x00,OLED_CMD);//---set low column address
//	oled_write_byte(screen,0x10,OLED_CMD);//---set high column address
//	oled_write_byte(screen,0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	oled_write_byte(screen,0x81,OLED_CMD);//--set contrast control register
//	oled_write_byte(screen,0xCF,OLED_CMD); // Set SEG Output Current Brightness
//	oled_write_byte(screen,0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	oled_write_byte(screen,0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//	oled_write_byte(screen,0xA6,OLED_CMD);//--set normal display
//	oled_write_byte(screen,0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
//	oled_write_byte(screen,0x3f,OLED_CMD);//--1/64 duty
//	oled_write_byte(screen,0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	oled_write_byte(screen,0x00,OLED_CMD);//-not offset
//	oled_write_byte(screen,0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
//	oled_write_byte(screen,0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
//	oled_write_byte(screen,0xD9,OLED_CMD);//--set pre-charge period
//	oled_write_byte(screen,0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	oled_write_byte(screen,0xDA,OLED_CMD);//--set com pins hardware configuration
//	oled_write_byte(screen,0x12,OLED_CMD);
//	oled_write_byte(screen,0xDB,OLED_CMD);//--set vcomh
//	oled_write_byte(screen,0x40,OLED_CMD);//Set VCOM Deselect Level
//	oled_write_byte(screen,0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	oled_write_byte(screen,0x02,OLED_CMD);//
//	oled_write_byte(screen,0x8D,OLED_CMD);//--set Charge Pump enable/disable
//	oled_write_byte(screen,0x14,OLED_CMD);//--set(0x10) disable
//	oled_write_byte(screen,0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
//	oled_write_byte(screen,0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
//	oled_write_byte(screen,0xAF,OLED_CMD);//--turn on oled panel
	
	oled_write_byte(screen,0xAE,OLED_CMD);//--turn off oled panel
	oled_write_byte(screen,0x40,OLED_CMD);//---set low column address
	oled_write_byte(screen,0x81,OLED_CMD);//---set high column address
	oled_write_byte(screen,0xff,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_write_byte(screen,0xA1,OLED_CMD);//--set contrast control register
	oled_write_byte(screen,0xA4,OLED_CMD); // Set SEG Output Current Brightness
	oled_write_byte(screen,0xA6,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	oled_write_byte(screen,0xA8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	oled_write_byte(screen,0x3F,OLED_CMD);//--set normal display
	oled_write_byte(screen,0xAD,OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_write_byte(screen,0x10,OLED_CMD);//--1/64 duty
	oled_write_byte(screen,0xC8,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_write_byte(screen,0xD3,OLED_CMD);//-not offset
	oled_write_byte(screen,0x00,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	oled_write_byte(screen,0xD5,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_write_byte(screen,0x90,OLED_CMD);//--set pre-charge period
	oled_write_byte(screen,0xD9,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_write_byte(screen,0x22,OLED_CMD);//--set com pins hardware configuration
	oled_write_byte(screen,0x20,OLED_CMD);
	oled_write_byte(screen,0x04,OLED_CMD);//--set vcomh
	oled_write_byte(screen,0x21,OLED_CMD);//Set VCOM Deselect Level
	oled_write_byte(screen,0x00,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_write_byte(screen,0x0F,OLED_CMD);//
	oled_write_byte(screen,0x22,OLED_CMD);//--set Charge Pump enable/disable
	oled_write_byte(screen,0x00,OLED_CMD);//--set(0x10) disable
	oled_write_byte(screen,0x3F,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	oled_write_byte(screen,0xDA,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	oled_write_byte(screen,0x12,OLED_CMD);//--turn on oled panel
	oled_write_byte(screen,0xDB,OLED_CMD);//--set Charge Pump enable/disable
	oled_write_byte(screen,0x30,OLED_CMD);//--set(0x10) disable
	oled_write_byte(screen,0x8D,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	oled_write_byte(screen,0x14,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	oled_write_byte(screen,0xAF,OLED_CMD);//--turn on oled panel
	HAL_Delay(50);
	//清屏
	screen_clear(screen,WHITE); 	
	screen_clear(screen,BLACK); 	
}

/*==================================================================================
* 函 数 名： gt32l_init
* 参    数： None
* 功能描述:  字库芯片初始化
* 返 回 值： None
* 备    注： 默认上电读取一个，不然第一个自己会为0xFF
* 作    者：  xiaozh
* 创建时间： 2019-09-23 132006
==================================================================================*/
void gt32l_init(void)
{
	uint8_t zk_buff[64] = {0};
	ASCII_GetData('A',ASCII_16_A,zk_buff);
}

/*==================================================================================
* 函 数 名： oled_init
* 参    数： None
* 功能描述:  OLED屏初始化
* 返 回 值： None
* 备    注： SSD1315
* 作    者：  xiaozh
* 创建时间： 2019-09-23 132006
==================================================================================*/
void oled_gt_init(void)
{
	//GPIO初始化
	HAL_GPIO_WritePin(GPIOB, OLED_CS2_Pin|OLED_CS1_Pin|CS_FLASH_Pin, GPIO_PIN_SET);	//失能片选

	//OLED使能
	//	ALL_RES_HIGH();
	HAL_GPIO_WritePin(GPIOB, OLED_RES1_Pin|OELD_RES2_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOB, OLED_RES1_Pin|OELD_RES2_Pin, GPIO_PIN_RESET);
	//	ALL_RES_LOW();
	HAL_Delay(200);
	//	ALL_RES_HIGH();
	HAL_GPIO_WritePin(GPIOB, OLED_RES1_Pin|OELD_RES2_Pin, GPIO_PIN_SET);
	
	//OLED初始化
	oled_init(SCREEN_LEFT);
	oled_init(SCREEN_RIGHT);

	HAL_GPIO_WritePin(GPIOB, OLED_DC_Pin|OLED_RES1_Pin|OELD_RES2_Pin|OLED_CS2_Pin 
												|CS_FLASH_Pin|OLED_CS1_Pin, GPIO_PIN_SET);
	//字库芯片初始化
//	gt32l_init();
}

 
unsigned char BMP1[] =
{
	0X00,0X00,0X00,0X00,0X00,0X00,0X61,0X30,0X02,0X02,0X04,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X82,0X70,0X02,0X00,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X01,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X80,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X78,0X00,0X00,0X80,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0X00,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X03,0XC0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X03,0XC0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X07,0XE0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7C,0X07,0XE0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X07,0XF0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0XF0,0X03,0XF0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X01,0XF0,0X00,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X81,0X80,0X01,0XE0,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X83,0X00,0X00,0XE0,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X03,0X00,0X00,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X07,0X00,0X00,0X00,0X10,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X06,0X00,0X00,0X00,0X10,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X06,0X00,0X00,0X00,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X0E,0X0C,0X00,0X00,0X08,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X0E,0X1E,0X00,0X00,0X08,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X1E,0X1C,0X00,0X08,0X24,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3E,0X1E,0X00,0X08,0X20,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3E,0X1E,0X08,0X08,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3C,0X1F,0X18,0X08,0X2F,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3C,0X0F,0X18,0X04,0X2F,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0F,0X18,0X0F,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X00,0X0F,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X80,0X06,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X80,0X00,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X9F,0XC0,0X00,0X38,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X9F,0XF1,0XC0,0X38,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XDF,0XF9,0XE0,0X78,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XF8,0XC0,0XF8,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XF8,0X0F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0X9F,0XF8,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFE,0X7F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFE,0X7F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XF8,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0X7F,0XF0,0X02,0X33,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X7F,0XF0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X7F,0XF0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFB,0XFC,0X3F,0XF0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFB,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XFB,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XF9,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XF9,0XFC,0X3F,0XC0,0X02,0X07,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0XF9,0XFE,0X7F,0XC0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFE,0X7F,0X80,0X00,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X7F,0X80,0X00,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X3F,0X00,0X00,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0X3E,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X1C,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0E,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
};


unsigned char BMP2[] =
{
0X00,0X00,0X00,0X00,0X00,0X00,0X61,0X30,0X02,0X02,0X04,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X82,0X70,0X02,0X00,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X40,0X01,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X01,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X03,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X30,0X00,0X00,0X80,0X87,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X78,0X00,0X00,0X80,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XF8,0X00,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X03,0XC0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X03,0XC0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFC,0X07,0XE0,0X40,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7C,0X07,0XE0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X70,0X07,0XF0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0XF0,0X03,0XF0,0X20,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X01,0XF0,0X00,0X47,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X81,0X80,0X01,0XE0,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X83,0X00,0X00,0XE0,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X03,0X00,0X00,0X00,0X00,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0X07,0X00,0X00,0X00,0X10,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0X06,0X00,0X00,0X00,0X10,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X06,0X00,0X00,0X00,0X10,0X07,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X0E,0X0C,0X00,0X00,0X08,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X0E,0X1E,0X00,0X00,0X08,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X1E,0X1C,0X00,0X08,0X24,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3E,0X1E,0X00,0X08,0X20,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3E,0X1E,0X08,0X08,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3C,0X1F,0X18,0X08,0X2F,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0E,0X3C,0X0F,0X18,0X04,0X2F,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X0F,0X18,0X0F,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X00,0X0F,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X80,0X06,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X0F,0X80,0X00,0X18,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X9F,0XC0,0X00,0X38,0X04,0X27,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0X9F,0XF1,0XC0,0X38,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XDF,0XF9,0XE0,0X78,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XF8,0XC0,0XF8,0X06,0X27,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XF8,0X0F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0X9F,0XF8,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFE,0X7F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFE,0X7F,0XF8,0X06,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0XFF,0XF8,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0X7F,0XF0,0X02,0X33,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X7F,0XF0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X7F,0XF0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFB,0XFC,0X3F,0XF0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X03,0XFB,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XFB,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XF9,0XFC,0X3F,0XE0,0X02,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XF9,0XFC,0X3F,0XC0,0X02,0X07,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0XF9,0XFE,0X7F,0XC0,0X02,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X01,0XFE,0X7F,0X80,0X00,0X37,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X7F,0X80,0X00,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0XFE,0X3F,0X00,0X00,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0X3E,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X1C,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X3F,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X0E,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X17,0X80,0X00,0X00,0X00,0X00,
};

/*==================================================================================
* 函 数 名： oleddrv_disp
* 参    数： None
* 功能描述:  OLED刷新数据
* 返 回 值： None
* 备    注： SSD1315
* 作    者： lc
* 创建时间： 2021-02-22 132006
==================================================================================*/
void oleddrv_disp(_Disp_Param pmsg )
{
	_Font_Info mFont_Info = _FONT_INIT();
	
	mFont_Info.x = pmsg.startCol;
	mFont_Info.y = pmsg.startRow;
	mFont_Info.p_text = pmsg.data;
	mFont_Info.font_size = pmsg.fontSize;
	screen_show_string(&mFont_Info);
	mFont_Info.screen = pmsg.id;
	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
}
/*==================================================================================
* 函 数 名： main_oled_test
* 参    数： None
* 功能描述:  OLED和字库测试函数
* 返 回 值： None
* 备    注： SSD1315
* 作    者：  xiaozh
* 创建时间： 2019-09-23 132006
==================================================================================*/
void main_oled_test()
{
	unsigned char jtwb[128]="Abcdefghijk菜名:宫保鸡丁";	//每个中文字符实际由两个字节组成, 对应GBK等编码
  uint8_t size =FONT_16;
	
	oled_gt_init();
	
	_Font_Info mFont_Info = _FONT_INIT();

	//汉字显示测试
	mFont_Info.p_text = jtwb;
	mFont_Info.font_size = size;
	screen_show_string(&mFont_Info);
	mFont_Info.screen = SCREEN_RIGHT;
	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
//	
//	mFont_Info.x = 0;
//	mFont_Info.y = 0;
//	mFont_Info.p_text = jtwb;
//	screen_show_string(&mFont_Info);
//	mFont_Info.screen = SCREEN_LEFT;
//	screen_show_string(&mFont_Info);
//	
//	mFont_Info.y = 16;
//	mFont_Info.p_text = jtwb;
//	screen_show_string(&mFont_Info);
//	mFont_Info.screen = SCREEN_LEFT;
//	screen_show_string(&mFont_Info);
//	
//	mFont_Info.y = 32;
//	mFont_Info.p_text = jtwb;
//	screen_show_string(&mFont_Info);
//	mFont_Info.screen = SCREEN_LEFT;
//	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
	
	//HAL_Delay(10000);
	
	
//	//图片显示测试
	screen_aversion(SCREEN_RIGHT,0,0);
	screen_show_bmp(SCREEN_RIGHT,0,0,16,64,BMP1,1);
	
	screen_aversion(SCREEN_LEFT,0,0);
	screen_show_bmp(SCREEN_LEFT,0,0,64,16,BMP2,1);
	//screen_show_bmp(SCREEN_LEFT,0,0,128,8,BMP2,1);
	//	screen_show_bmp(0, )
	while(1);
}


extern _App_Param mApp_Param;
void show_read_tag(uint8_t read_num, uint8_t real_num)
{ 
//	uint8_t count_step = 0; 
//	unsigned char show_str[]="read:000";	//每个中文字符实际由两个字节组成, 对应GBK等编码
//	unsigned char show_str_real[]="real:000";	//每个中文字符实际由两个字节组成, 对应GBK等编码
//  unsigned char show_can_addr[]="can:0000";	// 
//	unsigned char show_ver[]="ver:00";	// 
//	
//	_Font_Info mFont_Info = _FONT_INIT();
//	
//	count_step = sizeof("read:")-1;
//	show_str[count_step++] = read_num/100+'0'; 
//	show_str[count_step++] = read_num/10%10+'0';
//	show_str[count_step++] = read_num%10+'0';
//	
//	count_step = sizeof("real:")-1;
//	show_str_real[count_step++] = real_num/100+'0';
//	show_str_real[count_step++] = real_num/10%10+'0';
//	show_str_real[count_step++] = real_num%10+'0';
//	 
//	count_step = sizeof("can:")-1; 
//	show_can_addr[count_step++] = mApp_Param.cc_can_addr/10%10+'0';
//	show_can_addr[count_step++] = mApp_Param.cc_can_addr%10+'0';
//	show_can_addr[count_step++] = mApp_Param.can_addr/10%10+'0';
//	show_can_addr[count_step++] = mApp_Param.can_addr%10+'0';
//	
//	//汉字显示测试
//	mFont_Info.p_text = show_str;
//	screen_show_string(&mFont_Info);

//	mFont_Info.p_text = show_str_real;
//	mFont_Info.screen = SCREEN_RIGHT;
//	screen_show_string(&mFont_Info);
//	
//	mFont_Info.y = 16;
//	mFont_Info.p_text = show_can_addr;
//	mFont_Info.screen = SCREEN_RIGHT;
//	screen_show_string(&mFont_Info);
//	
//	count_step = sizeof("ver:")-1; 
//	show_ver[count_step++] = SOFT_VERSION/10%10+'0';
//	show_ver[count_step++] = SOFT_VERSION%10+'0';
//	mFont_Info.y = 32;
//	mFont_Info.p_text = show_ver;
//	mFont_Info.screen = SCREEN_RIGHT;
//	screen_show_string(&mFont_Info);
//	
//	screen_refresh_all(); //刷新显示 
}
/*==================================================================================
* 函 数 名： show_upgrade_tag
* 参    数： None
* 功能描述:  显示升级收到的包号
* 返 回 值： None
* 备    注： SSD1315
* 作    者： lc
* 创建时间： 2021-03-01 132006
==================================================================================*/
void show_upgrade_tag(uint16_t read_num)
{
	uint8_t count_step = 0; 
	unsigned char show_str[]="upda:000";	//每个中文字符实际由两个字节组成, 对应GBK等编码
	
	_Font_Info mFont_Info = _FONT_INIT();
	
	count_step = sizeof("read:")-1;
	show_str[count_step++] = read_num/100+'0'; 
	show_str[count_step++] = read_num/10%10+'0';
	show_str[count_step++] = read_num%10+'0';
	

	//汉字显示测试
	mFont_Info.y = 16*3;
	mFont_Info.p_text = show_str;
	screen_show_string(&mFont_Info);
	mFont_Info.screen = SCREEN_LEFT;
	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
}


/*==================================================================================
* 函 数 名： show_lost_num_tag
* 参    数： None
* 功能描述:  显示升级收到的包号
* 返 回 值： None
* 备    注： SSD1315
* 作    者： lc
* 创建时间： 2021-03-04 132006
==================================================================================*/
void show_lost_num_tag(uint16_t read_num)
{
	uint8_t count_step = 0; 
	unsigned char show_str[]="lost:000";	//每个中文字符实际由两个字节组成, 对应GBK等编码
	
	_Font_Info mFont_Info = _FONT_INIT();
	
	count_step = sizeof("lost:")-1;
	show_str[count_step++] = read_num/100+'0'; 
	show_str[count_step++] = read_num/10%10+'0';
	show_str[count_step++] = read_num%10+'0';
	

	//汉字显示测试
	mFont_Info.y = 16*3;
	mFont_Info.p_text = show_str;
	screen_show_string(&mFont_Info);
	mFont_Info.screen = SCREEN_RIGHT;
	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
}
/*==================================================================================
* 函 数 名： oled_show_string
* 参    数： None
* 功能描述:  显示字符串
* 返 回 值： None
* 备    注： SSD1315
* 作    者： lc
* 创建时间： 2021-03-01 132006
==================================================================================*/
void oled_show_string(uint8_t num,uint8_t *data)
{
	uint8_t count_step = 0; 
	unsigned char show_str[]="";	//每个中文字符实际由两个字节组成, 对应GBK等编码
	
	_Font_Info mFont_Info = _FONT_INIT();
	
	memcpy(show_str,data,strlen(data));
	

	//汉字显示测试
	mFont_Info.y = 16*num;
	mFont_Info.p_text = show_str;
	screen_show_string(&mFont_Info);
	mFont_Info.screen = SCREEN_LEFT;
	screen_show_string(&mFont_Info);
	
	screen_refresh_all(); //刷新显示 
}
