#ifndef _ADC_H_
#define _ADC_H_

#include "./SYSTEM/sys/sys.h"

void MY_ADC_Init(void); 				//ADCͨ����ʼ��
uint16_t  Get_Adc(uint32_t ch); 		        //���ĳ��ͨ��ֵ 
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times);//�õ�ĳ��ͨ����������������ƽ��ֵ
#endif 

