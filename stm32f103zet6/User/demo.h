/**
 ****************************************************************************************************
 * @file        demo.h
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

#ifndef __DEMO_H
#define __DEMO_H

#include "stm32f1xx.h"

static void demo_show_ip(char *buf);
void OneNet_FillBuf(char *buf,uint8_t temperature,uint8_t humidity,uint16_t sunlight,uint32_t C02,uint32_t TVOC,uint16_t LPG);
void wifi_init(void);
void information_connection(uint8_t temperature,uint8_t humidity,uint16_t sunlight,uint32_t C02,uint32_t TVOC,uint16_t LPG);

#endif
