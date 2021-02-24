#ifndef __OLED_INTERFACE_H
#define __OLED_INTERFACE_H

#include <stdint.h>
#include <string.h>
#include "ex_flash.h"

#define MAX_DISP_LEN 128//最大显示的数据长度



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
	uint8_t dispLen;//显示字节长度
	uint8_t data[MAX_DISP_LEN];//现实的字节数据或者显示的字库flash地址
	uint32_t dispAddr;//图片显示的flash存储地址
}_Disp_Param, *_disp_Param;

typedef struct
{
	void (*init)(void);
	uint8_t (*updataPic_opt)(uint8_t *, uint16_t *);		//更新图像库里面的自定义图像
	void (*dispPic_opt)(_Disp_Param);//显示图像或字符串
	void (*directlyDisp_opt)(_Disp_Param);//直接显示安卓发过来的数据

}_Oled_Func, *_pOled_Func;

extern _pOled_Func pOled_Func;
#endif
