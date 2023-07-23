/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MW8266Dģ��ԭ��������ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/DHT11/dht11.h"
#include "./BSP/GY-30/gy-30.h"
#include "./BSP/SGP30/sgp30.h"
#include "./BSP/MQ/adc.h"
#include "./BSP/MQ/beep.h"
#include "demo.h"


/**
 * @brief       ��ʾʵ����Ϣ
 * @param       ��
 * @retval      ��
 */
void show_mesg(void)
{
    /* LCD��ʾʵ����Ϣ */
    lcd_show_string(10, 10, 220, 32, 32, "STM32", RED);
    lcd_show_string(10, 47, 220, 24, 24, "ATK-MW8266D", RED);
    lcd_show_string(10, 76, 220, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(10, 97, 220, 16, 16, "KEY0: test key", BLUE);
    lcd_show_string(10,118, 220, 16, 16, "KEY1: switch atkcld mode", BLUE);
    
//    /* �������ʵ����Ϣ */
//    printf("\n");
//    printf("********************************\r\n");
//    printf("STM32\r\n");
//    printf("ATK-MW8266D\r\n");
//    printf("ATOM@ALIENTEK\r\n");
//    printf("KEY0: test key\r\n");
//    printf("KEY1: switch atkcld mode\r\n");
//    printf("********************************\r\n");
//    printf("\r\n");
}

int main(void)
{   
	  /*�¶�ʪ������*/
    uint8_t temperature;  	    
	  uint8_t humidity;
	  uint16_t sunlight;
	  uint32_t sgp30_dat;
	  uint32_t CO2=0;
	  uint32_t TVOC=0;
	  uint16_t LPG=0;
	
	  /*״̬����*/
	  uint8_t t=0;
	  uint8_t is_unvarnished=0;
	
	  /*��ʼ������*/
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    key_init();                         /* ��ʼ������ */
    lcd_init();                         /* ��ʼ��LCD */
    show_mesg();                        /* ��ʾʵ����Ϣ */
	  MY_ADC_Init();
		
	  /*����onenet*/
	  wifi_init();
	  
		/*��������ʼ��*/
    BH1750init();
		SGP30_Init();
		
	  while(DHT11_Init())	//DHT11��ʼ��	
	  {
		  lcd_show_string(30,130,200,16,16,"DHT11 Error",BLUE);
		  delay_ms(200);
	  }
		
		/*���ݴ�������ʾ*/
	  while(1)
		{ 
			if(t%10==0)
			{
				/*��ȡ����*/
				sunlight=bh_data_read();
				DHT11_Read_Data(&temperature,&humidity);
				
				//SGP30ģ�鿪����Ҫһ��ʱ���ʼ�����ڳ�ʼ���׶ζ�ȡ��CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�Һ㶨���䣬����ϵ��ÿ��һ��ʱ���ȡһ��
	      //SGP30ģ���ֵ�����CO2Ũ��Ϊ400ppm��TVOCΪ0ppd�����͡����ڼ����...����ֱ��SGP30ģ���ʼ����ɡ�
	      //��ʼ����ɺ�տ�ʼ�������ݻᲨ���Ƚϴ�������������һ��ʱ�����������ȶ���
	      //�����ഫ�����Ƚ������ܻ���Ӱ�죬�������ݳ��ֲ����������ģ�����������˲�������
				while(CO2==00&&TVOC==0)
	      {
		      SGP30_Write(0x20,0x08);
		      sgp30_dat=SGP30_Read();//��ȡSGP30��ֵ
		      CO2=(sgp30_dat&0xffff0000) >> 16;//ȡ��CO2Ũ��ֵ
		      TVOC=sgp30_dat&0x0000ffff;			 //ȡ��TVOCֵ
					delay_ms(500);
	      }
				SGP30_Write(0x20,0x08);
		    sgp30_dat=SGP30_Read();          //��ȡSGP30��ֵ
		    CO2=(sgp30_dat&0xffff0000)>>16;  //ȡ��CO2Ũ��ֵ
		    TVOC=sgp30_dat&0x0000ffff;       //ȡ��TVOCֵ
				LPG=600;                //ȡ��LPGֵ
				//uint16_t ppm;
				//ppm=Get_Adc_Average(ADC_CHANNEL_4,20);
				//printf("%d\r\n",ppm);
				/*������ʾ*/
				lcd_show_string(30,150,200,16,16,"Temp:  C",BLUE);	 
 	      lcd_show_string(30,170,200,16,16,"Humi:  %",BLUE);
				lcd_show_string(30,190,200,18,16,"Sun:   Lux",BLUE);
				lcd_show_string(30,210,200,18,16,"CO2:   ppm",BLUE);
				lcd_show_string(30,230,200,18,16,"TVOC:   ppd",BLUE);
				lcd_show_string(30,250,200,18,16,"LPG:   ppm",BLUE);
				
				
				lcd_show_num(30+40,150,temperature,2,16,BLUE);		//��ʾ�¶�	   		   
			  lcd_show_num(30+40,170,humidity,2,16,BLUE);			  //��ʾʪ��
        lcd_show_num(30+40-10,190,sunlight,3,16,BLUE);		//��ʾ����ǿ��
        lcd_show_num(30+40-10,210,CO2,3,16,BLUE);         //��ʾCO2Ũ��
				lcd_show_num(30+40,230,TVOC,3,16,BLUE);           //��ʾTVOCŨ��
        lcd_show_num(30+40-10,250,LPG,3,16,BLUE);         //��ʾLPGŨ��
				information_connection(temperature,humidity,sunlight,CO2,TVOC,LPG);
			}
			delay_ms(10);
			t++;
			if(t==20)
			{
				t=0;
				LED0_TOGGLE();
			}
		}
}
