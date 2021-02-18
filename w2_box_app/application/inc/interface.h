#ifndef  __INTERFACE_H
#define  __INTERFACE_H
#include <stdint.h>
#include "includes.h"

#define   APP_CONFIG_ADDR			(0x0800FC00)					//用户配置参数地址


void inter_init(void);


void can_rev_decode(void);
uint8_t write_config_param(void);
uint8_t read_config_param(void* c_param);
uint8_t can_sed_pkg_with_cache(void* can_msg); 
uint8_t config_mcan_addr(uint8_t cc_addr, uint8_t c_addr);
#endif
