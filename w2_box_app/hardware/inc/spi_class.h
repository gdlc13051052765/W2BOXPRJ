#ifndef  __SPI_CLASS_H
#define  __SPI_CLASS_H
#include "main.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define ST25R_SPI				SPI1

#define pSPI_TYPE				SPI_TypeDef *	
#define pGPIO_TYPE				GPIO_TypeDef *	

#define BUS_SPI1_TIMEOUT        2000U /* baud rate of SPI1 = 5 Mbps*/

typedef enum
{
	st25r = 0,
	oled_1 = 1,	
	oled_2 = 2,
	gt32l32 = 3,
}_Spi_Type;


typedef struct
{
	void*				 spi;
	void*        cs_port;
	void*        res_port;
	uint16_t 		 cs_pin;
	uint16_t     res_pin;

}_Context,*_pContext;


typedef struct
{
	uint8_t irq_pin;
	void *pContext;
	uint8_t (*cs_set)(void*, uint8_t);					//pipe   level
	uint8_t (*sdn_set)(void*, uint8_t);					//pipe   level
	uint8_t (*sed_rev_byte)(void*, uint8_t);			//发送接收一个字节
	uint8_t (*sed_rev_buf)(void*, uint8_t*, uint8_t*, uint16_t);		//发送 接收 长度
}_Spi_Class,*_pSpi_Class;

extern  _Spi_Class mSpi_Class[];

void si_spi_init(void);
uint8_t custom_sed_rev_buff(const uint8_t *sed_buff, uint8_t *rev_buff, uint16_t length);
uint8_t SpiTxRx(const uint8_t *txData, uint8_t *rxData, uint8_t length);
#endif
