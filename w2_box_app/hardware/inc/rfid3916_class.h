#ifndef __RFID_3916_CLASS_H
#define __RFID_3916_CLASS_H

#include <stdint.h>
#include <string.h>


void rfid_st25r3916_init(void);
uint8_t iso15693_find_tag(void);
uint8_t read_card_block(const uint8_t* uid, uint8_t* block_buff, uint16_t* read_block_len);
uint8_t get_rfid_status(void);
#endif
