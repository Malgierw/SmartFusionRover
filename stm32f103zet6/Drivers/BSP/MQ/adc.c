#include "./BSP/MQ/adc.h"
#include "./SYSTEM/delay/delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//ADC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

ADC_HandleTypeDef ADC3_Handler;		//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
	RCC_PeriphCLKInitTypeDef ADC_CLKInit;
	
	ADC_CLKInit.PeriphClockSelection=RCC_PERIPHCLK_ADC;			//ADC����ʱ��
	ADC_CLKInit.AdcClockSelection=RCC_ADCPCLK2_DIV6;			//��Ƶ����6ʱ��Ϊ72M/6=12MHz
	HAL_RCCEx_PeriphCLKConfig(&ADC_CLKInit);					//����ADCʱ��
	
	ADC3_Handler.Instance=ADC3;
	ADC3_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
	ADC3_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
	ADC3_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
	ADC3_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC3_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
	ADC3_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
	ADC3_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
	HAL_ADC_Init(&ADC3_Handler);                                 //��ʼ�� 
	
	HAL_ADCEx_Calibration_Start(&ADC3_Handler);					 //У׼ADC
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC3_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6;            //PA1
    GPIO_Initure.Mode=GPIO_MODE_INPUT;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
uint16_t Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC3_ChanConf;
    
    ADC3_ChanConf.Channel=ch;                                   //ͨ��
    ADC3_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC3_ChanConf.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;      //����ʱ��               
    HAL_ADC_ConfigChannel(&ADC3_Handler,&ADC3_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC3_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC3_Handler,10);                //��ѯת��
 
	return (uint16_t)HAL_ADC_GetValue(&ADC3_Handler);	        	//�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
