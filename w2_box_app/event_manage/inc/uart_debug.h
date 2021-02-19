#ifndef  __UART_DEBUG_H
#define  __UART_DEBUG_H
#include "stm32f1xx_hal.h"
#include <stdio.h>
 
#define DEBUG_UART   USART1

#define USE_LOGGER  LOGGER_ON

void print_hex(uint8_t *buff, uint16_t len);
#endif