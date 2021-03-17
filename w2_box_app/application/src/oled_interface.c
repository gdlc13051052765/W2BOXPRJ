#include "includes.h" 
#include "oled_drv.h"
#include "oled_interface.h"
#include "board_info.h"
#include "gt32l_drv.h"

//static void oled_init(void);
static uint8_t oled_load_picture_exflash(const uint8_t *data,uint16_t *addr);
static uint8_t oled_disp_flash_picture(_Disp_Param);
static uint8_t oled_directry_disp(_Disp_Param pmsg);
	
_Oled_Param mOled_Param = 
{
	.w_flash = &mEx_Flash_Func,
	.r_flash = &mEx_Flash_Func,
};

_Oled_Func mOled_Func = 
{
	//.init 			= oled_init,
	.updataPic_opt = oled_load_picture_exflash,
	.dispPic_opt = oled_disp_flash_picture,
	.directlyDisp_opt = oled_directry_disp,
};

//oled显示部分接口函数
_pOled_Func pOled_Func = &mOled_Func;



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
	
	printf("addr==%d \r\n",*addr);
	printf("write data==");
	for(int i=0;i<MAX_DISP_LEN;i++)
		printf("%2X ",data[i]);
	printf("\r\n");

	if(nAddress%GT32L32_FLASH_SECTOR_SIZE == 0)
	{
		mOled_Param.w_flash->erase(nAddress,MAX_DISP_LEN);
		HAL_Delay(100);//不加延时flash写失败
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
					else
					{
						printf("read fail \r\n");
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
static uint8_t oled_directry_disp(_Disp_Param pmsg)
{
	oleddrv_disp(pmsg);
	return 0;
}

static void displayByteMapRam(uint8_t startColumn,uint8_t ColumnInByte,uint8_t RowInByte,uint8_t (*SCREEN)[16],uint8_t *data,uint8_t displayLength)
{
	uint8_t i;
	uint8_t headBits=0x00;
	uint8_t tailBits=0x00;
	uint8_t offset=startColumn%8;//起始点偏移整数点(即8的整数倍的点)距离
	
	if(displayLength==0)
		return ;
				
	if(offset!=0)
	{	
		for(i=0;i<offset;i++)   //把起始列所在seg的前offset置1
		{
			 headBits+=(1<<(7-i));
			 tailBits+=1<<i;					
		}
	
		SCREEN[RowInByte][ColumnInByte] = ((data[0]&(~tailBits))>>offset)+(SCREEN[RowInByte][ColumnInByte]&headBits);//组合成新byte
		for(i=1;i<displayLength;i++)
		{
			SCREEN[RowInByte][ColumnInByte+i] = ((data[i-1]&tailBits)<<(8-offset))+((data[i])>>offset);		
		}
		SCREEN[RowInByte][ColumnInByte+displayLength] = ((data[displayLength-1]&tailBits)<<(8-offset))+(SCREEN[RowInByte][ColumnInByte+displayLength]&(~headBits));
	}
	else
	{
		for(i=ColumnInByte;i<(ColumnInByte+displayLength);i++)
		{
			SCREEN[RowInByte][i]=data[i-ColumnInByte];
		}				
	}			
}

static void displayBitMapRam(uint8_t startColumn,uint8_t RowInByte,uint8_t (*SCREEN)[16],uint8_t *data,uint8_t totalCharDisplayLength)
{
	uint8_t i;
	uint8_t headBits = 0x00;
	uint8_t tailBits = 0x00;
	uint8_t headShiftBits = 0x00;	
	uint8_t ColumnInByte = startColumn>>3; //起始列所在的byte
	uint8_t offset = startColumn%8;        //起始点偏移整数点(即8的整数倍的点)距离
	uint8_t byteNum = totalCharDisplayLength/8; //整byte数
	uint8_t bitNum = totalCharDisplayLength%8;  //余下bit数
	

	displayByteMapRam(startColumn,ColumnInByte,RowInByte,SCREEN,data,byteNum); //把整byte部分映射到ram
	
	if(bitNum)  //有余
	{
		if(offset!=0)
		{
			if(bitNum<=(8-offset))
			{
				for(i=0;i<bitNum;i++)   //把起始列所在seg的前bitNum位置1
				{
					 headBits+=(1<<(7-i));
					 tailBits+=1<<i;					
				}					
				SCREEN[RowInByte][ColumnInByte+byteNum]=((data[byteNum]&headBits)>>offset)|SCREEN[RowInByte][ColumnInByte+byteNum];					
			}
			else
			{
				for(i=0;i<(8-offset);i++)   //把起始列所在seg的前bitNum位置1
				{
					 headBits+=(1<<(7-i));			
				}	
				SCREEN[RowInByte][ColumnInByte+byteNum]=((data[byteNum]&headBits)>>offset)|SCREEN[RowInByte][ColumnInByte+byteNum];
			  
				for(i=0;i<(bitNum-(8-offset));i++)  
				{
					 headShiftBits+=(1<<(7-i));			
				}
				
        SCREEN[RowInByte][ColumnInByte+byteNum+1]=((data[byteNum]<<(8-offset))&headShiftBits)|SCREEN[RowInByte][ColumnInByte+byteNum+1];			
			}	
		}
		else
		{
			for(i=0;i<bitNum;i++)   //把起始列所在seg的前bitNum位置1
			{
				 headBits+=(1<<(7-i));
				 tailBits+=1<<i;					
			}
			SCREEN[RowInByte][ColumnInByte+byteNum]=(data[byteNum]&headBits)+(SCREEN[RowInByte][ColumnInByte+byteNum]&(~headBits));
		}
	}
}
static void byteReverse(uint8_t displayLength,uint8_t mode,uint8_t *data)
{
	uint8_t i,j;
	uint8_t temp=0;
  uint8_t reverse[128];
	for(i=0;i<displayLength;i++)    //把byte的所有bit倒序
	{
		for(j=0;j<8;j++)
		{
			if(data[i]&0x01)
				 temp |= 1<<(7-j);  
			else  
				 temp &= ~(1<<(7-j)); 
			
			data[i]>>=1;	
		}
		data[i]=temp;
		reverse[i]=temp;
	}
  if(mode)
	{
		for(i=0;i<displayLength;i++)
	  {
	    data[i]=reverse[displayLength-1-i];
	  }	
	}
}

static uint8_t readFromRam(uint8_t col,uint8_t row,uint8_t (*SCREEN)[16],uint8_t *data,uint8_t displayLength)
{	
	uint8_t i;
//	if((*display.pStartColumn%8)!=0)
//		displayLength=displayLength+1;  //若不是从整点起始,会多读出一字节补齐头尾
	
  if((col/8+displayLength)<15)
    displayLength=displayLength+1;

	for(i=0;i<displayLength+1;i++)
	{
		data[i]=SCREEN[row][col/8+i];
	}

	byteReverse(displayLength+1,0,data);
	
	return displayLength+1;
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
static uint8_t SCREEN[64][16]={0};
static uint8_t oled_disp_flash_picture(_Disp_Param pmsg )
{
	//uint16_t bmplen=0;
  uint8_t bmpdata[128],height,flashLength,i,j,displayLength;
	uint32_t nAddress = pmsg.dispAddr + OLED_PIC_ADDR;	
	uint32_t width;	
	uint8_t finaldisplayLength=0;
	uint8_t tatalDisplayByte=0;
	uint8_t startColumn= pmsg.startCol;
	uint8_t RowInByte=pmsg.startRow;
	
	switch(pmsg.cmd )
	{
		case 01://显示灰度图		
		case 02://显示黑白图
			if((pmsg.endCol>pmsg.startCol )&&(pmsg.endRow >pmsg.startRow))
			{
				width = pmsg.endCol - pmsg.startCol +1 ;
				if(width>128)
					return 0x02;
				if(width%8==0){
					flashLength=width/8;
				}
				else{
					flashLength=width/8+1;
				}	
				
				for(i=pmsg.startRow;i<=pmsg.endRow;i++)
				{
					RowInByte = i;
					height= i- pmsg.startRow ;
				
					nAddress =flashLength*height +pmsg.dispAddr + OLED_PIC_ADDR;

					//printf("dispAddr==%d",nAddress);
					if(mOled_Param.r_flash->read(nAddress,bmpdata, flashLength) == 0x01)
					{
						displayBitMapRam(startColumn,RowInByte,SCREEN,bmpdata,width);//bit=byte*8							
						finaldisplayLength=readFromRam(startColumn,RowInByte,SCREEN,bmpdata,flashLength);		
						screen_aversion(pmsg.id,RowInByte,startColumn);												
						for(j=0;j<finaldisplayLength;j++){
							oled_write_byte(pmsg.id,bmpdata[j],OLED_DATA);
						}
					}
				}
			}
			else 
					return 0x02;
		break;
		
		case 03://显示字符串
			oleddrv_disp(pmsg);
		return 0;
		break;
	}
	return 0;
}
