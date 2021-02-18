#include "adc_drv.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;


typedef struct
{
	uint8_t is_complete;		//是否扫描完成  0：未完成   1：完成一次扫描
	uint32_t temp_adc_value;		//温度adc值
	uint32_t ev_adc_value;			//电流值
	
	uint32_t adc_buff[MAX_ACD_NUM];		//最大采集缓存，两个通道，每个通道采集10次，求平均
}_Adc_Info, *_pAdc_Info;

 
_Adc_Info mAdc_Info = 
{
	.is_complete			= 0,
};

void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)																			//4
{ 
	//关闭中断
	 HAL_ADC_Stop_DMA(hadc);
	
	 mAdc_Info.is_complete = 0x01;	//完成一次扫描
}

/*==================================================================================
* 函 数 名： start_once_conv
* 参    数： None
* 功能描述:  使能数据采集
* 返 回 值： None
* 备    注： 100ms采集一次
* 作    者： xiaozh
* 创建时间： 2019-09-23 185058
==================================================================================*/
void start_once_conv(void)
{ 
  HAL_ADC_Start_DMA(&hadc1, mAdc_Info.adc_buff, MAX_CONV_NUM);
}

/*==================================================================================
* 函 数 名： adc_filter_conver
* 参    数： None
* 功能描述:  ADC滤波算法
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2017-2-20 
==================================================================================*/
uint16_t adc_filter_conver(uint16_t* AdcBuff,uint8_t Len)
{
	uint8_t count,i,j;
	uint64_t  sum=0;
	uint16_t temp  = 0;
	
   for (j=0;j<Len-1;j++)
   {
      for (i=0;i<Len-j;i++)
      {
         if (AdcBuff[i] < AdcBuff[i+1])
         {
            temp = AdcBuff[i];
            AdcBuff[i] = AdcBuff[i+1]; 
             AdcBuff[i+1] = temp;
         }
      }
   }
	 
   for(count=1;count<Len-1;count++)
      sum += AdcBuff[count];
	 
   return (uint16_t)(sum/(Len-2));
}

/*==================================================================================
* 函 数 名： adc_conver_refresh
* 参    数： None
* 功能描述:  ADC转换值更新
* 返 回 值： None
* 备    注： 
* 作    者： xiaozh
* 创建时间： 2019-09-24 105740
==================================================================================*/
void adc_conver_refresh(void)
{
	uint16_t temp_adc_buff[16] = {0};
	uint16_t ev_adc_buff[16] = {0};
	
	for(int i=0,j=0; i<MAX_CONV_NUM/2; i++)
	{  
		temp_adc_buff[i] = mAdc_Info.adc_buff[j++];
		ev_adc_buff[i] = mAdc_Info.adc_buff[j++];
	}
	
	//软件滤波
	mAdc_Info.temp_adc_value = adc_filter_conver(temp_adc_buff, MAX_CONV_NUM/2);
	mAdc_Info.ev_adc_value = adc_filter_conver(ev_adc_buff, MAX_CONV_NUM/2);
}
 
/*==================================================================================
* 函 数 名： adc_scan_loop_task
* 参    数： None
* 功能描述:  ADC采集更新
* 返 回 值： None
* 备    注： 间隔200ms扫描一次
* 作    者： xiaozh
* 创建时间： 2019-09-24 105740
==================================================================================*/
void adc_scan_loop_task(void* argv)
{
	//如果完成一次扫描，则进行数据解析
	if(mAdc_Info.is_complete)
	{
		mAdc_Info.is_complete = 0;
		
		//进行数据转换
		adc_conver_refresh();
	}
	else
	{
		//开启一次ADC转换
		start_once_conv();
	}
}

/*==================================================================================
* 函 数 名： get_adc_temp
* 参    数： None
* 功能描述:  获取温度的ADC值
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-09-24 105740
==================================================================================*/
uint16_t get_adc_temp(void)
{
	g_sys_info.sys_temp = mAdc_Info.temp_adc_value;
	return mAdc_Info.temp_adc_value;
}

/*==================================================================================
* 函 数 名： get_adc_ev
* 参    数： None
* 功能描述:  获取电流的ADC值
* 返 回 值： None
* 备    注： None
* 作    者： xiaozh
* 创建时间： 2019-09-24 105740
==================================================================================*/
uint16_t get_adc_ev(void)
{
	g_sys_info.sys_ev = mAdc_Info.ev_adc_value;
	return mAdc_Info.ev_adc_value;
}

