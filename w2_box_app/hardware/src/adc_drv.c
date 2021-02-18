#include "adc_drv.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;


typedef struct
{
	uint8_t is_complete;		//�Ƿ�ɨ�����  0��δ���   1�����һ��ɨ��
	uint32_t temp_adc_value;		//�¶�adcֵ
	uint32_t ev_adc_value;			//����ֵ
	
	uint32_t adc_buff[MAX_ACD_NUM];		//���ɼ����棬����ͨ����ÿ��ͨ���ɼ�10�Σ���ƽ��
}_Adc_Info, *_pAdc_Info;

 
_Adc_Info mAdc_Info = 
{
	.is_complete			= 0,
};

void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)																			//4
{ 
	//�ر��ж�
	 HAL_ADC_Stop_DMA(hadc);
	
	 mAdc_Info.is_complete = 0x01;	//���һ��ɨ��
}

/*==================================================================================
* �� �� ���� start_once_conv
* ��    ���� None
* ��������:  ʹ�����ݲɼ�
* �� �� ֵ�� None
* ��    ע�� 100ms�ɼ�һ��
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-23 185058
==================================================================================*/
void start_once_conv(void)
{ 
  HAL_ADC_Start_DMA(&hadc1, mAdc_Info.adc_buff, MAX_CONV_NUM);
}

/*==================================================================================
* �� �� ���� adc_filter_conver
* ��    ���� None
* ��������:  ADC�˲��㷨
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2017-2-20 
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
* �� �� ���� adc_conver_refresh
* ��    ���� None
* ��������:  ADCת��ֵ����
* �� �� ֵ�� None
* ��    ע�� 
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 105740
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
	
	//����˲�
	mAdc_Info.temp_adc_value = adc_filter_conver(temp_adc_buff, MAX_CONV_NUM/2);
	mAdc_Info.ev_adc_value = adc_filter_conver(ev_adc_buff, MAX_CONV_NUM/2);
}
 
/*==================================================================================
* �� �� ���� adc_scan_loop_task
* ��    ���� None
* ��������:  ADC�ɼ�����
* �� �� ֵ�� None
* ��    ע�� ���200msɨ��һ��
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 105740
==================================================================================*/
void adc_scan_loop_task(void* argv)
{
	//������һ��ɨ�裬��������ݽ���
	if(mAdc_Info.is_complete)
	{
		mAdc_Info.is_complete = 0;
		
		//��������ת��
		adc_conver_refresh();
	}
	else
	{
		//����һ��ADCת��
		start_once_conv();
	}
}

/*==================================================================================
* �� �� ���� get_adc_temp
* ��    ���� None
* ��������:  ��ȡ�¶ȵ�ADCֵ
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 105740
==================================================================================*/
uint16_t get_adc_temp(void)
{
	g_sys_info.sys_temp = mAdc_Info.temp_adc_value;
	return mAdc_Info.temp_adc_value;
}

/*==================================================================================
* �� �� ���� get_adc_ev
* ��    ���� None
* ��������:  ��ȡ������ADCֵ
* �� �� ֵ�� None
* ��    ע�� None
* ��    �ߣ� xiaozh
* ����ʱ�䣺 2019-09-24 105740
==================================================================================*/
uint16_t get_adc_ev(void)
{
	g_sys_info.sys_ev = mAdc_Info.ev_adc_value;
	return mAdc_Info.ev_adc_value;
}

