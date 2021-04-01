#include "gt32l_drv.h"


#define SPI_CS_HIGH()			do{HAL_GPIO_WritePin(GPIOB, CS_FLASH_Pin, GPIO_PIN_SET);}while(0)
#define SPI_CS_LOW()			do{ HAL_GPIO_WritePin(GPIOB, CS_FLASH_Pin, GPIO_PIN_RESET);}while(0)


/*==================================================================================
* �� �� ���� spi1_sed_rev
* ��    ���� None
* ��������:  SPI1�ӿڷ�������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 143321
==================================================================================*/
static uint8_t gt_sed_rev_byte( uint8_t value)
{
//	SPI_TypeDef	* pthis = SPI2;
	uint8_t ret = 0; 
	while((SPI2->SR & SPI_FLAG_TXE) == RESET);	//�ȴ����ͻ�������

	SPI2->DR = value; 	//����һ���ֽ�  

	while((SPI2->SR & SPI_FLAG_RXNE) == RESET);	 //�ȴ��Ƿ���յ�һ���ֽ�  
	
	ret = SPI2->DR; 													//��ø��ֽ�
	
	return ret; //�����յ����ֽ�
}

//---------˽�к���
unsigned char r_dat_bat(unsigned long address,unsigned long byte_long,unsigned char *p_arr)
{
	unsigned long  i = 0;
	SPI_CS_LOW();
	
	gt_sed_rev_byte(READ_DATA_CMD);	
	//���͵�ַ
//	gt_sed_rev_byte(address>>24);				//���͵�ַ��8λ
	gt_sed_rev_byte(address>>16);				//���͵�ַ
	gt_sed_rev_byte(address>>8);				//���͵�ַ
	gt_sed_rev_byte(address);						//���͵�ַ
	
	//��������
	for(i=0; i<byte_long; i++)
	{
		p_arr[i] = gt_sed_rev_byte(0xFF);
	}
	
	SPI_CS_HIGH();
	return p_arr[0]; 
}

/*==================================================================================
* �� �� ���� gt32l_execute_cmd
* ��    ���� None
* ��������:  ִ�е�������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 121940
==================================================================================*/
uint8_t gt32l_execute_cmd(uint8_t cmd)
{
	SPI_CS_LOW();
	gt_sed_rev_byte(cmd);			//����ҳд��ָ��
	SPI_CS_HIGH(); 
	return 0;
}

/*==================================================================================
* �� �� ���� write_enable
* ��    ���� None
* ��������:  дʹ��
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 121940
==================================================================================*/
uint8_t write_enable(void)
{
	SPI_CS_LOW();
	gt_sed_rev_byte(0x06);			//����ҳд��ָ��
	SPI_CS_HIGH(); 
	return 0;
}

/*==================================================================================
* �� �� ���� write_disable
* ��    ���� None
* ��������:  дʧ��
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 121940
==================================================================================*/
uint8_t write_disable(void)
{ 
	SPI_CS_LOW();
	gt_sed_rev_byte(0x06);			//����ҳд��ָ��
	SPI_CS_HIGH(); 
	return 0;
}

/*==================================================================================
* �� �� ���� gt32_read_status
* ��    ���� None
* ��������:  ��ȡоƬ״̬
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/
uint8_t gt32_read_status(void)
{
	uint8_t chip_status = 0;
	
	SPI_CS_LOW();
	
	gt_sed_rev_byte(READ_STATUS_CMD);			//����ҳд��ָ��
	
	//���͵�ַ 
	chip_status = gt_sed_rev_byte(0xFF);				//���͵�ַ 
	
	SPI_CS_HIGH();
	
	return chip_status;
}
  
/*==================================================================================
* �� �� ���� check_chip_status
* ��    ���� None
* ��������:  ���оƬ��״̬
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 145307
==================================================================================*/
void check_chip_status(void)
{
	uint8_t chip_status = 0,count = 0;
	do{
		count++;
		chip_status = gt32_read_status();
		if(count>1000){
			debug_print("flash write fail \r\n");
			break;
		}	
	}while(chip_status&0x01);
}

/*==================================================================================
* �� �� ���� page_program
* ��    ���� None
* ��������:  д����
* �� �� ֵ�� None
* ��    ע�� һ�����ֻ��д��256���ֽڣ�����ִ�п�ҳд����
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/
uint8_t page_program(uint32_t address, const uint8_t *buff, uint16_t length)
{
	uint16_t i = 0;

	//ʹ��д����
	write_enable();
	
	SPI_CS_LOW();
	gt_sed_rev_byte(WRITE_DATA_CMD);			//����ҳд��ָ��
	
	//���͵�ַ 
	gt_sed_rev_byte((uint8_t)((address>>16)&0x000000FF));				//���͵�ַ
	gt_sed_rev_byte((uint8_t)((address>>8)&0x000000FF));				//���͵�ַ
	gt_sed_rev_byte((uint8_t)((address>>0)&0x000000FF));						//���͵�ַ
	
	//��������
	for(i=0; i<length; i++)
	{
		gt_sed_rev_byte(buff[i]);
	}
	
	SPI_CS_HIGH();
	
	//����Ƿ�ִ�����
	check_chip_status();
	
	return 0x00; 
}

/*==================================================================================
* �� �� ���� sector_erase
* ��    ���� None
* ��������:  оƬ����
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/
uint8_t gt32_sector_erase(uint32_t address)
{
	//ʹ��д����
	write_enable();
	
	SPI_CS_LOW(); 
	gt_sed_rev_byte(0x20);			//����ҳд��ָ��

	//���͵�ַ 
	gt_sed_rev_byte((uint8_t)((address>>16)&0x000000FF));				//���͵�ַ
	gt_sed_rev_byte((uint8_t)((address>>8)&0x000000FF));				//���͵�ַ
	gt_sed_rev_byte((uint8_t)((address>>0)&0x000000FF));						//���͵�ַ
	
	SPI_CS_HIGH();
	
	//����Ƿ�ִ�����
	check_chip_status();
	return 0;
}

/*==================================================================================
* �� �� ���� gt32_chip_read
* ��    ���� None
* ��������:  оƬ��ȡ����
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/
uint8_t gt32_chip_read(uint32_t address, uint8_t* buff, uint32_t length)
{
	r_dat_bat(address,  length, buff);
	return 0;
}
 
/*==================================================================================
* �� �� ���� gt32_chip_write
* ��    ���� None
* ��������:  оƬд����
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/ 
uint8_t gt32_chip_write(uint32_t address, const uint8_t* buff, uint32_t length)
{
	uint16_t pageremain;	   
	
	pageremain=256-address%256; 	 	    
	if(length<=pageremain)pageremain=length;
	while(1)
	{	   
		page_program(address,buff,pageremain);
		if(length==pageremain)break;
	 	else //length>pageremain
		{
			buff+=pageremain;
			address+=pageremain;	

			length-=pageremain;			  
			if(length>256)pageremain=256; 
			else pageremain=length; 	  
		}
	};	

	return 0;
} 

/*==================================================================================
* �� �� ���� gt32_chip_write
* ��    ���� None
* ��������:  оƬд����
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-11-12 122209
==================================================================================*/
uint8_t gt32_erase_size(uint32_t address, uint32_t length)
{
	uint16_t erase_count = 0;
	
	if((address+length) > GT32L32_FLASH_MAX_ADDR)
	{
		return 0;
	}
	
	erase_count = (length/GT32L32_FLASH_SECTOR_SIZE)+1;
	
	for(int i=0; i<erase_count; i++)
	{
		gt32_sector_erase(address+i*GT32L32_FLASH_SECTOR_SIZE);
	}
	
	return 0;
}



 
#define  HZ_MODE00 0//���ú���(Y)
#define  HZ_MODE01 1//��������(Z)
#define  HZ_MODE10 2//���ú���(W)
#define  HZ_MODE11 3//��������(X)

#define  HZ_MODE4  4//Y-->W 
//����putdata������д��X��Y��LCD��ʾ����RAM��

void lcdram_map( uint8_t *getdate,uint8_t *putdata, uint8_t width, uint8_t high, uint8_t style )
{
  uint16_t i,j,hbyte,wbyte;
  unsigned char i_8,j_8;
  wbyte = (width+7)/8;
  hbyte = (high+7)/8;
   //--------------------------------------------------------------------------
   // Y--> W; Y-->X; Y-->Z;
   //--------------------------------------------------------------------------
  if( style == HZ_MODE4 )	//���ú��� ת ���ú��� ( Y-->W )
  {
	for( i = 0; i < high; i++ )
		for( j = 0; j < width; j++ )
		{
			i_8 = i/8;
			if((*(putdata+i_8*width+j)&(0x01<<(i%8))) > 0)
				getdate[wbyte*i+j/8] |= (0x80>>(j%8));
			else
				getdate[wbyte*i+j/8] &= (~(0x80>>(j%8)));
		}
  }	
   //--------------------------------------------------------------------------
   // W--> Y;  W-->Z;W-->X;
   //--------------------------------------------------------------------------
  
  if( style == HZ_MODE00 )	//���ú��� (W--> Y)
  {
	  for( i = 0; i < high; i++ )
		  for( j = 0; j < width; j++ )
		  {
			i_8 = i/8;
			if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
				getdate[i_8*width+j] |= (0x01<<(i%8));
			else
				getdate[i_8*width+j] &= (~(0x01<<(i%8)));
		  }
  }
  else if(style == HZ_MODE01)	//�������� (W-->Z)
  {
    for( i = 0; i < high; i++ )
      for( j = 0; j < width; j++ )
      {
        if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
					getdate[j*hbyte+i/8] |= (0x01<<(i%8));
        else
					getdate[j*hbyte+i/8]  &= (~(0x01<<(i%8)));
      }
  }
  else if(style == HZ_MODE11)//�������� (W-->X)
  {
    for( i = 0; i < high; i++ )
      for( j = 0; j < width; j++ )
      {
        j_8 = j/8;
        if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8))) > 0)
					getdate[j_8*high+i] |= (0x80>>(j%8));
        else
					getdate[j_8*high+i] &= (~(0x80>>(j%8)));
      }
  }
  else if(style == HZ_MODE10)//���ú��� ������(W-->W')
  {
    for( i = 0; i < high; i++)
      for( j = 0; j < width; j++)
      {
        if((*(putdata+wbyte*i+j/8)&(0x80>>(j%8)))>0 )
					*(getdate+wbyte*i+(width-j)/8) |=(0x80>>((width-j)%8));
        else
					*(getdate+wbyte*i+(width-j)/8) &=~(0x80>>((width-j)%8));
      }
  }
}

//��ֱ����ת���ú��� ZתW
void Z2W(unsigned char* getData, unsigned char* putData, unsigned char width, unsigned char height) {
	
	unsigned char line, lie;
	unsigned int i=0, count=0, mark = 0;	//i����putData���±�, count����getData���±�
	
	for(line=0; line<height; line++) {	//������
		for(lie=0; lie<width; lie++) {	//������
			getData[count] = (unsigned char)(getData[count]<<0x01);	//׼��������һ��
			if(putData[i]&(0x01<<(line%8))) 
				getData[count] += 1;	//��ȡ��λ, ����������
			else
				getData[count] += 0;
			i += height>>3;	//��һ��
			if(lie%8 == 7) {
				count++;
			}
		}
		if(line%8 == 7) {
			i = ++mark;
		}else
			i = mark;
	}
}

//��ֱ����ת���ú���(Z->Y)
void Z2Y(unsigned char* getData, unsigned char* putData, unsigned char width, unsigned char height) {
	unsigned char line=height>>3;
	unsigned int i=0, count=0, mark_count=0, sum = width*height>>3;

	for(i=0;i<sum;i++) {
		getData[count] = putData[i];
		count += width;
		if( ((i+1)%line) == 0)
			count = ++mark_count;
	}
}

//���ú���ת��ֱ����  YתZ
void Y2Z(unsigned char* getData, unsigned char* putData, unsigned char width, unsigned char height) {
	unsigned char line=height>>3;
	unsigned int i=0, count=0, mark_count=0, sum=width*height>>3;
	
	for(i=0; i<sum; i++) {
		getData[count] = putData[i];
		count += line;
		if( ((i+1)%width) == 0 ) {
			count = ++mark_count;
		}
	}
}

