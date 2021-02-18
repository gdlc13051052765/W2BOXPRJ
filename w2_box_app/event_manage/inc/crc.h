#ifndef  __CRC_H
#define  __CRC_H
#include <stdint.h>

uint8_t crc4_itu(uint8_t *data, uint32_t length);
uint8_t crc8(const uint8_t* data, uint32_t length);
uint16_t crc16_ccitt(const uint8_t* data, uint32_t length);
uint32_t crc32(const uint8_t* data, uint32_t length);
 

#endif
