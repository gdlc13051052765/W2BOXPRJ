#include "spi_class.h"


_Context mSpi_Context[] = 
{
	{
		.spi			= SPI1,
		.cs_port	= ST25R_CS_GPIO_Port,
		.cs_pin		= ST25R_CS_Pin,
		.res_port	= 0,
		.res_pin	= 0, 
	},
	{
		.spi			= SPI2,
		.cs_port	= GPIOB,
		.cs_pin		= OLED_CS1_Pin,
		.res_port	= OLED_RES1_GPIO_Port,
		.res_pin		= OLED_RES1_Pin, 
	},
	{
		.spi			= SPI2,
		.cs_port	= GPIOB,
		.cs_pin		= OLED_CS2_Pin,
		.res_port	= OELD_RES2_GPIO_Port,
		.res_pin	= OELD_RES2_Pin, 
	},
	{
		.spi			= SPI2,
		.cs_port	= CS_FLASH_GPIO_Port,
		.cs_pin		= CS_FLASH_Pin,
		.res_port	= 0,
		.res_pin	= 0, 
	}
};


static uint8_t cs_gpio_set(void* context, uint8_t level);
static uint8_t res_gpio_set(void* context, uint8_t level);
static uint8_t sed_rev_byte(void* context, uint8_t value);
static uint8_t sed_rev_buff(void* context, uint8_t *s_buff, uint8_t *r_buff, uint16_t len);
static uint8_t sed_byte(void* context, uint8_t value);


_Spi_Class mSpi_Class[] = 
{
	{	//ST25
		.irq_pin		= ST25R_IRQ_Pin,
	  .pContext   = &mSpi_Context[0],
	  .cs_set	  	= cs_gpio_set,
		.sdn_set		= res_gpio_set,
	  .sed_rev_byte = sed_rev_byte,
	  .sed_rev_buf  = sed_rev_buff,
	},
	
	{//OLED_L
		.irq_pin		= NULL,
	  .pContext   = &mSpi_Context[1],
	  .cs_set	  	= cs_gpio_set,
		.sdn_set		= res_gpio_set,
	  .sed_rev_byte = sed_byte,
	  .sed_rev_buf  = sed_rev_buff,
	},
	
	{//OLED_L
		.irq_pin		= NULL,
	  .pContext   = &mSpi_Context[2],
	  .cs_set	  	= cs_gpio_set,
		.sdn_set		= res_gpio_set,
	  .sed_rev_byte = sed_byte,
	  .sed_rev_buf  = sed_rev_buff,
	},
	{//OLED_L
		.irq_pin		= NULL,
	  .pContext   = &mSpi_Context[3],
	  .cs_set	  	= cs_gpio_set,
		.sdn_set		= res_gpio_set,
	  .sed_rev_byte = sed_rev_byte,
	  .sed_rev_buf  = sed_rev_buff,
	}
};

/*==================================================================================
* 函 数 名： spi1_sed_rev
* 参    数： None
* 功能描述:  SPI1接口发送数据
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 143321
==================================================================================*/
static uint8_t sed_rev_byte(void* context, uint8_t value)
{
	_pContext pthis = (_pContext)context;
	if(pthis->spi == 0)
	{
		return 0;
	}
	
	uint8_t ret = 0; 
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_TXE) == RESET);	//等待发送缓冲区空

	((pSPI_TYPE)pthis->spi)->DR = value; 	//发送一个字节  

	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_RXNE) == RESET);	 //等待是否接收到一个字节  
	
	ret = ((((pSPI_TYPE)pthis->spi)->DR)); 													//获得该字节

	return ret; //返回收到的字节
}

/*==================================================================================
* 函 数 名： spi1_sed_rev
* 参    数： None
* 功能描述:  SPI1接口发送数据
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 143321
==================================================================================*/
static uint8_t sed_byte(void* context, uint8_t value)
{
	_pContext pthis = (_pContext)context;
	if(pthis->spi == 0)
	{
		return 0;
	}
	
	uint8_t ret = 0; 
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_TXE) == RESET);	//等待发送缓冲区空
	((pSPI_TYPE)pthis->spi)->DR = value; 	//发送一个字节  
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_RXNE) == RESET);	//等待发送缓冲区空
	ret = ((((pSPI_TYPE)pthis->spi)->DR)); 													//获得该字节
	return ret; //返回收到的字节
}

/*==================================================================================
* 函 数 名： sed_rev_buff
* 参    数： None
* 功能描述:  SPI发送数组
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 190852
==================================================================================*/
static uint8_t sed_rev_buff(void* context, uint8_t *s_buff, uint8_t *r_buff, uint16_t len)
{
	for(int i=0; i<len; i++)
	{
		r_buff[i] = sed_rev_byte(context, s_buff[i]);
	}
	
	return 0x01;
}

/*==================================================================================
* 函 数 名： cs_gpio_set
* 参    数： None
* 功能描述:  设置cs引脚电平
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 190852
==================================================================================*/
static uint8_t cs_gpio_set(void* context, uint8_t level)
{
	_pContext pthis = (_pContext)context;

	if(pthis->cs_port == 0)
	{
		return 0;
	}
	
	HAL_GPIO_WritePin(((pGPIO_TYPE)pthis->cs_port), pthis->cs_pin, (GPIO_PinState)level);

	return 0x01;
}

/*==================================================================================
* 函 数 名： sdn_gpio_set
* 参    数： None
* 功能描述:  设置sdn引脚电平
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 190852
==================================================================================*/
static uint8_t res_gpio_set(void* context, uint8_t level)
{
	_pContext pthis = (_pContext)context;

	if(pthis->res_port == 0)
	{
		return 0;
	}
	
	HAL_GPIO_WritePin(((pGPIO_TYPE)pthis->res_port), pthis->res_pin, (GPIO_PinState)level);

	return 0x01;
}

/*==================================================================================
* 函 数 名： csed_rev_byte
* 参    数： None
* 功能描述:  自定义
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-18 192258
==================================================================================*/
static uint8_t csed_rev_byte(uint8_t value)
{	
	uint8_t ret = 0; 
	while((ST25R_SPI->SR & SPI_FLAG_TXE) == RESET);	//等待发送缓冲区空

	ST25R_SPI->DR = value; 	//发送一个字节  

	while((ST25R_SPI->SR & SPI_FLAG_RXNE) == RESET);	 //等待是否接收到一个字节  
	
	ret = ST25R_SPI->DR; 													//获得该字节

	return ret; //返回收到的字节
}

/*==================================================================================
* 函 数 名： custom_sed_rev_buff
* 参    数： None
* 功能描述:  发送接收，针对st25r3911
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 190852
==================================================================================*/
uint8_t custom_sed_rev_buff(const uint8_t *sed_buff, uint8_t *rev_buff, uint16_t length)
{
	//判断数据合法性
	if((sed_buff == NULL) || (rev_buff == NULL))
	{
		return 0x81;
	}
	
	//数据发送接收
	for(int i=0; i<length; i++)
	{
	 rev_buff[i]	= csed_rev_byte(sed_buff[i]);
	}
	
	return 0;
}



extern SPI_HandleTypeDef hspi1;


SPI_HandleTypeDef *pSpi = &hspi1;

/*==================================================================================
* 函 数 名： custom_sed_rev_buff
* 参    数： None
* 功能描述:  发送接收，针对st25r3911
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-05 190852
==================================================================================*/
uint8_t SpiTxRx(const uint8_t *txData, uint8_t *rxData, uint8_t length)
{  
  if(pSpi == 0)
    return 0x81;

	uint8_t   tx[256];
	static  uint8_t   rx[256];
  if(txData != NULL)
	{
    memcpy(tx, txData, length);
  }
  return HAL_SPI_TransmitReceive(pSpi, tx, (rxData != NULL) ? rxData : rx, length, BUS_SPI1_TIMEOUT);
}
