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
* �� �� ���� spi1_sed_rev
* ��    ���� None
* ��������:  SPI1�ӿڷ�������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 143321
==================================================================================*/
static uint8_t sed_rev_byte(void* context, uint8_t value)
{
	_pContext pthis = (_pContext)context;
	if(pthis->spi == 0)
	{
		return 0;
	}
	
	uint8_t ret = 0; 
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_TXE) == RESET);	//�ȴ����ͻ�������

	((pSPI_TYPE)pthis->spi)->DR = value; 	//����һ���ֽ�  

	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_RXNE) == RESET);	 //�ȴ��Ƿ���յ�һ���ֽ�  
	
	ret = ((((pSPI_TYPE)pthis->spi)->DR)); 													//��ø��ֽ�

	return ret; //�����յ����ֽ�
}

/*==================================================================================
* �� �� ���� spi1_sed_rev
* ��    ���� None
* ��������:  SPI1�ӿڷ�������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 143321
==================================================================================*/
static uint8_t sed_byte(void* context, uint8_t value)
{
	_pContext pthis = (_pContext)context;
	if(pthis->spi == 0)
	{
		return 0;
	}
	
	uint8_t ret = 0; 
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_TXE) == RESET);	//�ȴ����ͻ�������
	((pSPI_TYPE)pthis->spi)->DR = value; 	//����һ���ֽ�  
	while((((pSPI_TYPE)pthis->spi)->SR & SPI_FLAG_RXNE) == RESET);	//�ȴ����ͻ�������
	ret = ((((pSPI_TYPE)pthis->spi)->DR)); 													//��ø��ֽ�
	return ret; //�����յ����ֽ�
}

/*==================================================================================
* �� �� ���� sed_rev_buff
* ��    ���� None
* ��������:  SPI��������
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 190852
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
* �� �� ���� cs_gpio_set
* ��    ���� None
* ��������:  ����cs���ŵ�ƽ
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 190852
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
* �� �� ���� sdn_gpio_set
* ��    ���� None
* ��������:  ����sdn���ŵ�ƽ
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 190852
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
* �� �� ���� csed_rev_byte
* ��    ���� None
* ��������:  �Զ���
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-18 192258
==================================================================================*/
static uint8_t csed_rev_byte(uint8_t value)
{	
	uint8_t ret = 0; 
	while((ST25R_SPI->SR & SPI_FLAG_TXE) == RESET);	//�ȴ����ͻ�������

	ST25R_SPI->DR = value; 	//����һ���ֽ�  

	while((ST25R_SPI->SR & SPI_FLAG_RXNE) == RESET);	 //�ȴ��Ƿ���յ�һ���ֽ�  
	
	ret = ST25R_SPI->DR; 													//��ø��ֽ�

	return ret; //�����յ����ֽ�
}

/*==================================================================================
* �� �� ���� custom_sed_rev_buff
* ��    ���� None
* ��������:  ���ͽ��գ����st25r3911
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 190852
==================================================================================*/
uint8_t custom_sed_rev_buff(const uint8_t *sed_buff, uint8_t *rev_buff, uint16_t length)
{
	//�ж����ݺϷ���
	if((sed_buff == NULL) || (rev_buff == NULL))
	{
		return 0x81;
	}
	
	//���ݷ��ͽ���
	for(int i=0; i<length; i++)
	{
	 rev_buff[i]	= csed_rev_byte(sed_buff[i]);
	}
	
	return 0;
}



extern SPI_HandleTypeDef hspi1;


SPI_HandleTypeDef *pSpi = &hspi1;

/*==================================================================================
* �� �� ���� custom_sed_rev_buff
* ��    ���� None
* ��������:  ���ͽ��գ����st25r3911
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-05 190852
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
