#include "includes.h" 
#include "oled_drv.h"
#include "oled_interface.h"
#include "board_info.h"
#include "gt32l_drv.h"


uint8_t Length_ASCII_12[94]={
   	2, 4, 8, 6, 8, 8, 2, 4, 4, 6, 6, 2, 4, 2, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 4, 6, 6, 6, 6, 10, 8, 8, 8, 8, 6,
		6, 8, 8, 2, 6, 8, 6, 10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 10, 8, 8, 8, 4, 6, 4, 6, 6, 4, 6, 6, 6, 6, 6, 4, 6, 6, 2, 4,
		6, 2, 10, 6, 6, 6, 6, 4, 6, 4, 6, 6, 10, 6, 6, 6, 4, 4, 4, 8};

uint8_t Length_ASCII_16[94]={
    4, 6, 12, 9, 13, 12, 3, 5, 6, 7, 9, 4, 9, 4, 9, 10, 8, 10, 10, 11, 10, 10, 10, 10, 10, 4, 4, 10, 8, 10, 9, 14, 11, 11, 11, 11, 10, 10, 12, 11, 4, 9, 11, 10, 13, 11, 12, 11, 12, 11, 10, 11, 11, 12, 15, 11, 13, 10, 6, 9, 7, 9, 10, 4, 9, 9, 9, 9, 9, 8, 9, 9, 4, 6, 8, 4, 11, 9, 9, 9, 9, 8, 8, 8, 9, 9, 11, 9, 9, 9, 7, 3, 8, 10};

uint8_t Length_ASCII_24[94]={
    5, 8, 15, 12, 19, 19, 4, 7, 7, 10, 14, 6, 14, 5, 13, 13, 11, 13, 13, 14, 13, 13, 13, 13, 13, 5, 6, 11, 13, 11,
		11, 21, 16, 15, 16, 15, 14, 14, 16, 15, 5, 11, 16, 14, 17, 15, 17, 15, 17, 15, 16, 16, 16, 15, 22, 15, 16, 14,
		6, 13, 6, 10, 12, 6, 13, 13, 13, 13, 13, 9, 13, 12, 5, 7, 12, 5, 18, 12, 13, 13, 13, 8, 11, 9, 12, 12, 18, 12,
		13, 12, 8, 5, 8, 16};

uint8_t Length_ASCII_32[94]={
    9, 13, 22, 20, 30, 26, 6, 12, 12, 14, 20, 8, 20, 8, 19, 20, 19, 20, 20, 21, 20, 20, 20, 20, 20, 8, 8, 20, 20, 20, 18, 28, 23, 21, 23, 22, 21, 20, 24, 22, 7, 17, 23, 20, 26, 22, 24, 21, 25, 21, 21, 22, 22, 25, 32, 24, 24, 21, 12, 19, 12, 17, 21, 10, 19, 20, 19, 19, 20, 14, 20, 18, 9, 12, 19, 9, 28, 18, 17, 19, 19, 14, 17, 14, 18, 18, 28, 17, 18, 19, 13, 7, 13, 19};
	
#ifdef ASCII_ORIGINAL_24_Arial
	uint8_t Length_ASCII_Original_24[95]={
	0x07, 0x07, 0x0B, 0x0D, 0x0D, 0x12, 0x11, 0x06, 0x08, 0x08, 
	0x09, 0x0E, 0x07, 0x08, 0x07, 0x07, 0x0D, 0x0D, 0x0D, 0x0D,
	0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x07, 0x07, 0x0E, 0x0E,
	0x0E, 0x0F, 0x17, 0x11, 0x11, 0x11, 0x11, 0x10, 0x0F, 0x13,
	0x11, 0x07, 0x09, 0x11, 0x0F, 0x15, 0x11, 0x12, 0x10, 0x12, 
	0x11, 0x10, 0x0F, 0x11, 0x09, 0x17, 0x09, 0x0F, 0x0E, 0x08,
	0x07, 0x08, 0x0E, 0x0D, 0x08, 0x0D, 0x0F, 0x0D, 0x0F, 0x0D,
	0x08, 0x0F, 0x0F, 0x07, 0x07, 0x0D, 0x07, 0x15, 0x0F, 0x0F,
	0x0F, 0x0F, 0x09, 0x0D, 0x08, 0x0F, 0x0D, 0x13, 0x0D, 0x0D,
	0x0C, 0x09, 0x07, 0x09, 0x0E};
#else
	uint8_t Length_ASCII_Original_24[95]={
	0x06, 0x08, 0x0A, 0x0C, 0x0C, 0x14, 0x13, 0x04, 0x08, 0x08, 
	0x0C, 0x0E, 0x06, 0x08, 0x06, 0x07, 0x0C, 0x0C, 0x0C, 0x0C,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x07, 0x0E, 0x0E,
	0x0E, 0x0A, 0x16, 0x11, 0x10, 0x10, 0x11, 0x0F, 0x0D, 0x11,
	0x11, 0x08, 0x09, 0x11, 0x0F, 0x15, 0x11, 0x11, 0x0E, 0x11, 
	0x10, 0x0C, 0x0E, 0x10, 0x10, 0x17, 0x11, 0x11, 0x0E, 0x08,
	0x07, 0x08, 0x0A, 0x0C, 0x08, 0x0B, 0x0C, 0x0B, 0x0C, 0x0B,
	0x08, 0x0B, 0x0C, 0x06, 0x06, 0x0C, 0x06, 0x12, 0x0C, 0x0C,
	0x0C, 0x0C, 0x08, 0x09, 0x07, 0x0C, 0x0C, 0x11, 0x0C, 0x0C,
	0x0A, 0x0C, 0x05, 0x0C, 0x0D};
#endif

static uint8_t ALFF[128] = {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};	
//static void oled_init(void);
static uint8_t oled_load_picture_exflash(const uint8_t *data,uint16_t *addr);
static uint8_t oled_disp_flash_picture(_Disp_Param);
static uint8_t oled_directry_disp(_Disp_Param pmsg);
static uint8_t oled_toal_picNumber(uint16_t num);
static uint16_t oled_check_data(uint16_t *minNum);
static uint8_t oled_repair_data(uint16_t *num,uint8_t *data,uint16_t *nextPack);
	
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
	.toalNumber_opt = oled_toal_picNumber,
	.checkData_opt = oled_check_data,
	.repairPackage_opt = oled_repair_data,
};

//oled显示部分接口函数
_pOled_Func pOled_Func = &mOled_Func;

static uint16_t lost_num[100]={0};//丢失字库包号
static uint16_t charBank_toalNum=0;//字库总包号
static uint8_t lost_toalNum = 0;//丢失的总包号 


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
static uint16_t allffNumBuf[20];
static uint8_t allffNum = 0;
static uint8_t oled_load_picture_exflash(const uint8_t *data,uint16_t *addr)
{
	uint32_t nAddress = *addr*MAX_DISP_LEN +OLED_PIC_ADDR;	
	uint8_t r_buff[MAX_DISP_LEN] = {0};
	uint8_t try_count =3;
	
	printf("addr==%d \r\n",*addr);

	if(nAddress%GT32L32_FLASH_SECTOR_SIZE == 0)
	{
		mOled_Param.w_flash->erase(nAddress,MAX_DISP_LEN);
		//HAL_Delay(10);//
	}
	if(!memcmp(ALFF, data, MAX_DISP_LEN))//检查是不是全0xff
	{
		allffNumBuf[allffNum++] = *addr;//备份全FF的包号
		return 0;
	}
	do{
			if(mOled_Param.w_flash->write(nAddress, data, MAX_DISP_LEN) == 0x01)
			{
				if(mOled_Param.r_flash->read(nAddress,r_buff, MAX_DISP_LEN) == 0x01)
				{
//					printf("read dara==");
//					for(int i=0;i<MAX_DISP_LEN;i++)
//						printf("%2X ",r_buff[i]);
//					printf("\r\n");
				
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
				
	if(offset!=0){	
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
	else{
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


/**************************************************************************************************/
/**
	*Flash寻址计算：与字符串在Flash中寻址相关
	*/
static uint32_t AddressASCII(uint8_t size,uint8_t c1,uint8_t height)
{
	uint32_t address,offset;	
	
	offset=c1-0x21;	
	
	switch(size)
	{
		case 12:	
			address=offset*14*2+ADDRESS_BASE_ASCII_12+height*2;
		break;	
		
		case 16:	
			address=offset*18*2+ADDRESS_BASE_ASCII_16+height*2;
		break;	
		
		case 24:	
			address=offset*26*3+ADDRESS_BASE_ASCII_24+height*3;
		break;		
		
		case 32:	
			address=offset*34*4+ADDRESS_BASE_ASCII_32+height*4;
		break;	
		
		default:	
			address=ADDRESS_BASE_CUSTOM_NULL;
		break;
	}
	
	return address;
}

/***************************************
ASCII 调用
ASCIICode：表示ASCII 码（8bits）
height：高度(逐行递增)
size：字体大小
address:返回原生字库地址
****************************************/
static uint32_t ASCII_OriginalAddr(uint8_t ASCIICode,uint8_t height,uint8_t size)
{	
	uint32_t address;
		
	switch(size)
	{
		case 12:
			address=(ASCIICode-0x20)*26+ADDRESS_BASE_ASCII_12+2+height*2;//12X12 T
		break;
		case 16:
			address=(ASCIICode-0x20)*34+ADDRESS_BASE_ASCII_16+2+height*2;//16X16 T
		break;
		case 24:
			address=(ASCIICode-0x20)*74+ADDRESS_BASE_ASCII_24+2+height*3; //24X24 T 
		break;
		case 32:
			address=(ASCIICode-0x20)*130+ADDRESS_BASE_ASCII_32+2+height*4;//32X32 T
		break;
		default: break;
	}
	return address;
}

static uint32_t LengthASCII(uint8_t size,uint8_t c1)
{	
	switch(size)
	{
		case 12:	
			return Length_ASCII_12[c1-0x21];
		
		case 16:	
			return Length_ASCII_16[c1-0x21];
		
		case 24:	
			return Length_ASCII_24[c1-0x21];	
		
		case 32:	
			return Length_ASCII_32[c1-0x21];
		
		default:	
			return 0;
	}
}

static uint32_t LengthASCIIOriginal(uint8_t size,uint8_t c1)
{	
	switch(size)
	{
		case 12:	
			return Length_ASCII_12[c1-0x21];
		
		case 16:	
			return Length_ASCII_16[c1-0x21];
		
		case 24:
		  return Length_ASCII_Original_24[c1-0x20];	
		
		case 32:	
			return Length_ASCII_32[c1-0x21];
		
		default:	
			return 0;
	}
}

static uint32_t AddressGB2(uint8_t size,uint8_t c1,uint8_t c2,uint8_t height)
{
	uint32_t address,offset;	
	
	if(0xA1<=c1&&c1<=0xA9&&0xA1<=c2&&c2<=0xFE)
		offset=(c1-0xA1)*94+(c2-0xA1);	
	
	else if(0xA8<=c1&&c1<=0xA9&&0x40<=c2&&c2<=0x7E)
		offset=(c1-0xA8)*96+(c2-0x40)+846;
	
	else if(0xA8<=c1&&c1<=0xA9&&0x80<=c2&&c2<=0xA0)
		offset=(c1-0xA8)*96+(c2-0x41)+846;
	
	else if(0xB0<=c1&&c1<=0xF7&&0xA1<=c2&&c2<=0xFE)
		offset=(c1-0xB0)*94+(c2-0xA1)+1038;
	
	else if(0x81<=c1&&c1<=0xA0&&0x40<=c2&&c2<=0x7E)
		offset=(c1-0x81)*190+(c2-0x40)+1038+6768;	
	
	else if(0x81<=c1&&c1<=0xA0&&0x80<=c2&&c2<=0xFE)
		offset=(c1-0x81)*190+(c2-0x41)+1038+6768;	
	
	else if(0xAA<=c1&&c1<=0xFE&&0x40<=c2&&c2<=0x7E)
		offset=(c1-0xAA)*96+(c2-0x40)+1038+12848;	
	
	else if(0xAA<=c1&&c1<=0xFE&&0x80<=c2&&c2<=0xA0)
		offset=(c1-0xAA)*96+(c2-0x41)+1038+12848;	
	
	switch(size)
	{
		case 16:	
			address=offset*32+ADDRESS_BASE_GB_16+height*2;
		break;	
		
		case 24:
			address=offset*72+ADDRESS_BASE_GB_24+height*3;
		break;	
		
		case 32:
			if(c1<0xA1||0xA9<c1)
				offset-=192;
			address=offset*128+ADDRESS_BASE_GB_32+height*4;
		break;	
		
		default:
			address=ADDRESS_BASE_CUSTOM_NULL;
		break;
	}
	
	return address;
}

static uint32_t AddressGB4(uint8_t size,uint8_t c1,uint8_t c2,uint8_t c3,uint8_t c4,uint8_t height)
{
	uint32_t address,offset;	
	
	if(c1==0x81)
		offset=1038+21008+(c3-0xEE)*10+c4-0x39;	
	
	else
		offset=1038+21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30;
	
	switch(size)
	{
		case 16:	
			address=offset*32+ADDRESS_BASE_GB_16+height*2;
		break;		
		
		case 24:	
			address=offset*72+ADDRESS_BASE_GB_24+height*3;
		break;			
		
		case 32:	
			offset-=192;
			address=offset*128+ADDRESS_BASE_GB_32+height*4;
		break;	
		
		default:	
			address=ADDRESS_BASE_CUSTOM_NULL;
		break;
	}
	
	return address;
}
/*******************************************************************************************************/


static uint8_t SCREEN[64][16]={0};
static uint8_t screenid_bak =0;
static uint8_t dispdata[128];
static uint8_t *disp_pString;
//默认汉字字符和空格最小为16*16，ascii字符最小可12*12
uint8_t FlashReadCharacter(uint8_t *string,uint8_t height,_Disp_Param pmsg,uint8_t *data,uint8_t row)
{
	int i,j,flashAddress,charDisplayLength;
	uint8_t flashLength=0;
	uint8_t totalDot=0;
	uint8_t tatalDisplayByte=0;
	uint8_t displayLength=0;      //每次从字库读出的长度
	uint8_t finaldisplayLength=0; //重新从ram读出长度(若不是从整点起始,会多读出一字节补齐头尾)
	uint8_t columnEnd=0;
	uint8_t c1,c2,c3,c4;
	uint8_t startColumn = pmsg.startCol;
	uint8_t RowInByte = row;
	uint8_t assicdata[128];
	//uint8_t SCREEN[64][16]={0};
	

	while(string<disp_pString+pmsg.dispLen)
	{
		c1=*string;
		c2=*(string+1);
		c3=*(string+2);
		c4=*(string+3);
		
		if(c1==0x20)  //space
		{
			string+=1;
			for(i=0;i<pmsg.fontSize/8;i++)
			{
				if(totalDot+pmsg.startCol>128)	//防止显示溢出
				{
					string=disp_pString+pmsg.dispLen;//跳出while
					break;
				}

				data[displayLength++]=0;	
				totalDot += 4;	
			}
			
			displayBitMapRam(startColumn,RowInByte,SCREEN,data,pmsg.fontSize/2);	

			startColumn += pmsg.fontSize/2;
			displayLength=0;
		}
		else if(0x20<c1&&c1<0x7F)
		{
	
			flashAddress=AddressASCII(pmsg.fontSize,c1,height) ;//自定义ascii存放在自由区的地址
			charDisplayLength=LengthASCII(pmsg.fontSize,c1);   //自定义ascii字符长度(实际宽度,字库某些字符有空白区)
	                   
		  string+=1;
			j+=1;

			if(charDisplayLength!=0)
				flashLength=charDisplayLength/8+1;
			else
				flashLength=charDisplayLength/8;
			
			//FlashRead(flashAddress,flashLength);   //
			if(mOled_Param.r_flash->read(flashAddress,assicdata, flashLength) != 0x01)
			{
				return 0;
			}
		  for(i=0;i<flashLength;i++)
		  {
				if(totalDot+pmsg.startCol>128)	//防止显示溢出
				{
					string=disp_pString+pmsg.dispLen;//跳出while
					charDisplayLength=0;
					break;
				}
				
		     data[displayLength++]=assicdata[i]; 
         totalDot += 8;				
		  }
		  	
			displayBitMapRam(startColumn,RowInByte,SCREEN,data,charDisplayLength);
		
		  startColumn += charDisplayLength;	
      displayLength=0;				
		}
		else
		{
			if(0x81<=c1&&c1<=0x82&&0x30<=c2&&c2<=0x39)
			{	
				flashAddress=AddressGB4(pmsg.fontSize,c1,c2,c3,c4,height) ;
				string+=4;
				j+=4;
			}			
			else
			{	
				flashAddress=AddressGB2(pmsg.fontSize,c1,c2,height);
				string+=2;
				j+=2;
			}
			
			if(mOled_Param.r_flash->read(flashAddress,assicdata, pmsg.fontSize/8) != 0x01)//字库本身并没有加入4bit灰阶信息，so除8
			{
				return 0;
			}

			for(i=0;i<(pmsg.fontSize/8);i++)		
			{
				if(totalDot+pmsg.startCol>128)	//防止显示溢出
				{
					string=disp_pString+pmsg.dispLen;//跳出while
					break;
				}
				
				if(height<pmsg.fontSize)
				  data[displayLength++]=assicdata[i];
				else
					data[displayLength++]=0; //多出的两行直接赋0
				
				totalDot += 8;
			}	
			displayByteMapRam( pmsg.startCol,startColumn>>3,RowInByte,SCREEN,data,displayLength);	
			
		  startColumn += pmsg.fontSize;	
      displayLength=0;				
		}
	}
	
	if((totalDot%8)!=0) //128-8,否则数组SCREEN_R/SCREEN_L溢出
		tatalDisplayByte=totalDot/8 + 1;  // bytes=totalDisplayLength/8(有余)
	else
		tatalDisplayByte=totalDot/8;      // bytes=totalDisplayLength/8

	//debug_print("startColumn= %d,RowInByte =%d,tatalDisplayByte = %d\r\n",pmsg.startCol,RowInByte,tatalDisplayByte);
	finaldisplayLength = readFromRam( pmsg.startCol,RowInByte,SCREEN,data,tatalDisplayByte);	
	
	
	return finaldisplayLength;
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

static uint8_t oled_disp_flash_picture(_Disp_Param pmsg )
{
	//uint16_t bmplen=0;
  uint8_t bmpdata[128],height,flashLength,i,j,n,displayLength;
	uint32_t nAddress = pmsg.dispAddr + OLED_PIC_ADDR;	
	uint32_t width;	
	uint8_t finaldisplayLength=0;
	uint8_t tatalDisplayByte=0;
	uint8_t startColumn= pmsg.startCol;
	uint8_t RowInByte=pmsg.startRow;
	
	
	//清除显示缓存数据
	if( pmsg.id !=screenid_bak)
	{
			screenid_bak = pmsg.id;
			for(i=0;i<16;i++){  	
				for(n=0;n<64;n++){
					SCREEN[n][i] = 0;
				} 
		}
	}
	switch(pmsg.cmd )
	{
		case 01://显示灰度图		
		case 02://显示黑白图
			if((pmsg.endCol>pmsg.startCol )&&(pmsg.endRow >pmsg.startRow))
			{
				printf("显示start\r\n");
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
//						printf("bmpdata== ");
//						for(int j=0;j<flashLength;j++)
//						 printf(" %2X",bmpdata[i]);
//						printf("\r\n");
						displayBitMapRam(startColumn,RowInByte,SCREEN,bmpdata,width);//bit=byte*8							
						finaldisplayLength=readFromRam(startColumn,RowInByte,SCREEN,bmpdata,flashLength);		
						screen_aversion(pmsg.id,RowInByte,startColumn);												
						for(j=0;j<finaldisplayLength;j++){
							oled_write_byte(pmsg.id,bmpdata[j],OLED_DATA);
						}
					}
				}
				printf("显示stop\r\n");
			}
			else 
					return 0x02;
		break;
		
		case 03://显示字符串
			disp_pString = dispdata;
			memcpy(dispdata,pmsg.data,pmsg.dispLen);
		
			for(i=pmsg.startRow;(i-pmsg.startRow)<(2+pmsg.fontSize );i++)
			{
				RowInByte = i;
				height= i- pmsg.startRow ;
				displayLength=FlashReadCharacter(disp_pString ,height,pmsg,bmpdata,RowInByte);
	
				screen_aversion(pmsg.id,RowInByte,startColumn);	
				for(j=0;j<displayLength;j++){
					oled_write_byte(pmsg.id,bmpdata[j],OLED_DATA);
				}
			}
		return 0;
		break;
	}
	return 0;
}


/*==================================================================================
* 函 数 名： oled_toal_picNumber
* 参    数： None
* 功能描述:  字库总包号
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-03-22 025540
==================================================================================*/
static uint8_t oled_toal_picNumber(uint16_t num)
{
	charBank_toalNum = num;
	allffNum = 0;
}
/*==================================================================================
* 函 数 名： oled_check_data
* 参    数： None
* 功能描述:  下载的字库数据效验
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-03-22 025540
==================================================================================*/
static uint16_t oled_check_data(uint16_t *minNum)
{
	uint8_t r_buff[128];
	uint16_t toalNum = 0;
	
//	charBank_toalNum = 2500;
//	allffNum =1;
//	allffNumBuf[0] = 2492;
	for(uint16_t i=0;i<charBank_toalNum;i++)
	{
		if(mOled_Param.r_flash->read(i*MAX_DISP_LEN +OLED_PIC_ADDR,r_buff, MAX_DISP_LEN) == 0x01){
			if(!memcmp(r_buff,ALFF,MAX_DISP_LEN))
			{
				lost_num[toalNum++] = i;			
				for(int j=0;j<allffNum;j++)
				{//排除全FF数据的包不需要补包
					if(lost_num[toalNum-1] == allffNumBuf[j])
						toalNum--;
				}	
			}
		}
		else{
			lost_num[toalNum++] = i;
		}
	}
	minNum[0] = lost_num[0];//丢失的最小包号
	lost_toalNum = toalNum;
	debug_print("lost toal = %d, min num = %d \r\n",toalNum,minNum[0]);
	return toalNum;
}

/*==================================================================================
* 函 数 名： oled_repair_data
* 参    数： None
* 功能描述:  字库补包
* 返 回 值： None
* 备    注： 
* 作    者： lc
* 创建时间： 2021-03-22 025540
==================================================================================*/
static uint8_t repair_num =0;
static uint8_t oled_repair_data(uint16_t *num,uint8_t *data,uint16_t *nextPack)
{
	uint32_t nAddress = *num*MAX_DISP_LEN +OLED_PIC_ADDR;	
	uint8_t r_buff[MAX_DISP_LEN] = {0};
	uint8_t try_count =3;
	
	debug_print("addr==%d \r\n",*num);

	do{
			if(mOled_Param.w_flash->write(nAddress, data, MAX_DISP_LEN) == 0x01)
			{
				if(mOled_Param.r_flash->read(nAddress,r_buff, MAX_DISP_LEN) == 0x01){
					if(oled_picture_compare(r_buff, data, MAX_DISP_LEN) == 0x01){				
						debug_print("read ok \r\n");
						nextPack[0] = lost_num[++repair_num];
						if(lost_toalNum==repair_num){return 0;}//补包完成
						else return 0x20;	
					}
					else{
						debug_print("read fail \r\n");
						nextPack[0] = lost_num[repair_num];
						return 0x20;
					}
				}
			}	
	}while(try_count--);
	
	return 0;
}
