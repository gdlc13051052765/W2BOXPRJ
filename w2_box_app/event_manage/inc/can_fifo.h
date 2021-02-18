#ifndef __CAN_FIFO_H
#define __CAN_FIFO_H

#include <stdint.h>
#include <string.h>
#include "can_drv.h"

#define CAN_MAX_CACHE_LEN			32	//CAN发送缓存

typedef struct send_queue {
    _Can_Msg queue[CAN_MAX_CACHE_LEN];
    char wp;
    char rd;
    char count;
}send_queue_t, * p_send_queue_t;

void msg_queue_init(p_send_queue_t p_queue_buff);
unsigned int msg_queue_num(p_send_queue_t p_queue_buff);
void msg_queue_push(p_send_queue_t p_queue_buff, _Can_Msg *data);
void msg_queue_pop(p_send_queue_t p_queue_buff, uint8_t msg_id);


#endif
