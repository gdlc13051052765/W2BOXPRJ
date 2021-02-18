#include "includes.h" 
#include "rfal_nfc.h"
#include "st25r3916.h"

#define EXAMPLE_RFAL_POLLER_DEVICES      10 
static uint8_t gDevCnt;  
static int8_t rfidStatus = -1;//rfid的初始化状态
rfalNfcvListenDevice nfcvDevList[EXAMPLE_RFAL_POLLER_DEVICES];      

rfalNfcvInventoryRes invRes;
ReturnCode           err;

uint8_t block_buff[16];  /* Flags + Block Data + CRC */
uint16_t read_block_len = 0;
uint8_t txData[64] = {0};



void rfid_st25r3916_init(void)
{
	rfalAnalogConfigInitialize();
	if(rfalInitialize() != ERR_NONE)                                                  /* Initialize RFAL */
  {
		rfidStatus = 0;//初始化失败
	}
	else
		rfidStatus = 1;
}
//获取rfid的初始化状态
uint8_t get_rfid_status(void)
{
	return rfidStatus;
}
uint8_t iso15693_find_tag(void)
{
	uint8_t dev_cnt = 0;

	rfalNfcvPollerInitialize();        
	rfalFieldOnAndStartGT();                                                      /* Ensure GT again as other technologies have also been polled */       
	err = rfalNfcvPollerCollisionResolution(RFAL_COMPLIANCE_MODE_ISO , \
		 (EXAMPLE_RFAL_POLLER_DEVICES - gDevCnt), nfcvDevList, &dev_cnt );
	if((err == ERR_NONE) && (dev_cnt != 0)) {
		return dev_cnt;
	} else {
		return 0;
	}
}

uint8_t read_card_block(const uint8_t* uid, uint8_t* block_buff, uint16_t* read_block_len)
{
	uint16_t len1 = 0;
	uint16_t len2 = 0;
	uint8_t block_buff1[8];
	uint8_t block_buff2[8];

	err = rfalNfcvPollerSelect( RFAL_NFCV_REQ_FLAG_DEFAULT , uid);
	err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, 2, block_buff1, 8, &len1);
	err = rfalNfcvPollerReadSingleBlock(RFAL_NFCV_REQ_FLAG_DEFAULT, uid, 3, block_buff2, 8, &len2);
	if((5 == len1) && (5 == len2))//读出有效数据
	{
		memcpy(block_buff, block_buff1 + 1, len1 - 1);
		memcpy(block_buff + 4, block_buff2 + 1, len2 - 1);
		(*read_block_len) = len1 + len2 - 2;
		printf("block:");
		print_hex(block_buff, *read_block_len);
		printf("\n");
		err = ERR_NONE;
	} else {
		printf("rfalNfcvPollerReadSingleBlock() ==> error\n");
		err = !ERR_NONE;
	}
	return err;
}

