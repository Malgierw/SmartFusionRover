/**
 ****************************************************************************************************
 * @file        demo.c
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

#include "demo.h"
#include "./BSP/ATK_MW8266D/atk_mw8266d.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define DEMO_WIFI_SSID          "nova 7 5G"
#define DEMO_WIFI_PWD           "greer6666"
#define DEMO_TCP_SERVER_IP      "183.230.40.33"
#define DEMO_TCP_SERVER_PORT    "80"
#define DEMO_DEVID              "1055107638"
#define DEMO_API_KEY            "ba1N42BvFylMutHjcMVZmJYKUsg="


/**
 * @brief       ��ʾIP��ַ
 * @param       ��
 * @retval      ��
 */
static void demo_show_ip(char *buf)
{
    printf("IP: %s\r\n", buf);
    lcd_show_string(60, 151, 128, 16, 16, buf, BLUE);
}

/**
 * @brief       ��װ���ݺ���������Ҫ�ϴ������ݣ����ϵ�������
 * @param       ��
 * @retval      ��
 */
void OneNet_FillBuf(char *buf,uint8_t temperature,uint8_t humidity,uint16_t sunlight,uint32_t CO2,uint32_t TVOC,uint16_t LPG)
{
	char text[24];//��ʱ����
	char buf1[256];//��������
	
	memset(text, 0, sizeof(text));
	memset(buf1, 0, sizeof(buf1));
	
	
	strcpy(buf1, "{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"temp\":%d,",temperature);//
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"hum\":%d,", humidity);//
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"sun\":%d,", sunlight);//
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"co2\":%d,", CO2);//
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"tvoc\":%d,", TVOC);//
	strcat(buf1, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"LPG\":%d", LPG);//
	strcat(buf1, text);
	
	strcat(buf1, "}");
	
	sprintf(buf, "POST /devices/%s/datapoints?type=3 HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\n"
					"Content-Length:%d\r\n\r\n",DEMO_DEVID, DEMO_API_KEY, strlen(buf1));			
	strcat(buf, buf1);

	printf("pub_buf=%s\r\n",buf);

}


void wifi_init(void)
{   
	  /*ESP��������*/
    uint8_t ret;
    char ip_buf[16];
    uint8_t key;
    uint8_t is_atkcld = 0;
	
    /* ��ʼ��ATK-MW8266D */
    ret = atk_mw8266d_init(115200);
    if (ret != 0)
    {
        printf("ATK-MW8266D init failed!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    
    printf("Joining to AP...\r\n");
    ret  = atk_mw8266d_restore();                               /* �ָ��������� */
    ret += atk_mw8266d_at_test();                               /* AT���� */
    ret += atk_mw8266d_set_mode(1);                             /* Stationģʽ */
    ret += atk_mw8266d_sw_reset();                              /* �����λ */
    ret += atk_mw8266d_ate_config(0);                           /* �رջ��Թ��� */	
    ret += atk_mw8266d_join_ap(DEMO_WIFI_SSID, DEMO_WIFI_PWD);  /* ����WIFI */
		if (ret != 0)
    {
        printf("Error to join ap6!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
    ret += atk_mw8266d_get_ip(ip_buf);                          /* ��ȡIP��ַ */
    if (ret != 0)
    {
        printf("Error to join ap7!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
				
		ret +=atk_mw8266d_single_connection();		
		ret +=atk_mw8266d_connect_tcp_server(DEMO_TCP_SERVER_IP,DEMO_TCP_SERVER_PORT);
		if (ret != 0)
    {
        printf("Error to join OneNet!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
		else
		{
			printf("success to join OneNet!\r\n");
		}
		
		ret +=atk_mw8266d_enter_unvarnished();
		if (ret != 0)
    {
        printf("Error to open unvarnished!\r\n");
        while (1)
        {
            LED0_TOGGLE();
            delay_ms(200);
        }
    }
		else
		{
			printf("success to open unvarnished!\r\n");
		}
		
		atk_mw8266d_uart_rx_restart();
}

/**
 * @brief       ������ʾ��ں���
 * @param       ��
 * @retval      ��
 */
void information_connection(uint8_t temperature,uint8_t humidity,uint16_t sunlight,uint32_t CO2,uint32_t TVOC,uint16_t LPG)
{   
    char buf[400];
	  memset(buf,0,sizeof(buf));
	  OneNet_FillBuf(buf,temperature,humidity,sunlight,CO2,TVOC,LPG);
    atk_mw8266d_uart_printf("%s\r\n", buf);
}
