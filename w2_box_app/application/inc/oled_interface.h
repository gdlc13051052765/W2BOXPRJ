#ifndef __OLED_INTERFACE_H
#define __OLED_INTERFACE_H

#include <stdint.h>
#include <string.h>
#include "ex_flash.h"

#define MAX_DISP_LEN 128//最大显示的数据长度


/**
	*宏：FLASH地址索引
	*/
#define ADDRESS_BASE_CUSTOM						0x000000
#define ADDRESS_BASE_CUSTOM_NULL			0x0F0000

#define ADDRESS_BASE_ASCII_12					0x000000
#define ADDRESS_BASE_ASCII_16					0x002000
#define ADDRESS_BASE_ASCII_24					0x006000
#define ADDRESS_BASE_ASCII_32					0x00E000

#define ADDRESS_BASE_GB_16						0x194FDE
#define ADDRESS_BASE_GB_24						0x2743DE
#define ADDRESS_BASE_GB_32						0x47AE10

typedef struct
{

	uint16_t frame_step;				//当前接收帧的ID号

	const _Flash_Func *w_flash;	//写flash接口
	const _Flash_Func *r_flash;	//读取flash接口
}_Oled_Param, *_oled_Param;

typedef struct
{
	uint8_t id;//需要显示那个屏幕
	uint8_t cmd;//命令字
	uint8_t startRow;//起始行
	uint8_t startCol;//起始列
	uint8_t endRow;//终止行
	uint8_t endCol;//终止列
	uint8_t fontSize;//字号
//	uint8_t *pString;//字符串
	uint8_t dispLen;//显示字节长度
	uint8_t data[128];//现实的字节数据或者显示的字库flash地址
//	uint16_t toalNum;//字库总包号
//	uint16_t lostPack[100];//丢失包号
	uint32_t dispAddr;//图片显示的flash存储地址
}_Disp_Param, *_disp_Param;

typedef struct
{
	void (*init)(void);
	uint8_t (*updataPic_opt)(const uint8_t *, uint16_t *);		//更新图像库里面的自定义图像
	uint8_t (*dispPic_opt)(_Disp_Param);//显示图像或字符串
	uint8_t (*directlyDisp_opt)(_Disp_Param);//直接显示安卓发过来的数据
	uint8_t (*toalNumber_opt)(uint16_t);//总的字库包号
	uint16_t (*checkData_opt)(uint16_t *);//字库数据效验
	uint8_t (*repairPackage_opt)(uint16_t *num,uint8_t *data,uint16_t *);//补包

}_Oled_Func, *_pOled_Func;

extern _pOled_Func pOled_Func;

#endif
