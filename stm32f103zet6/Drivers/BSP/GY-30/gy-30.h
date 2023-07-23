#ifndef __SHT30_H__
#define __SHT30_H__
#include "./SYSTEM/sys/sys.h"

#define IIC_SCL PFout(10)


//IO��������
#define IIC_SDAIN_Mode()  {GPIOF->CRH&=0XFFFF0FFF;GPIOF->CRH|=8<<12;}
#define IIC_SDAout_Mode() {GPIOF->CRH&=0XFFFF0FFF;GPIOF->CRH|=3<<12;}

//IO��������											   
#define	IIC_SDAout    PFout(11)//���ݶ˿�	PF11
#define	IIC_SDAin     PFin(11) //���ݶ˿�	PF11

//����

#define BHAddWrite     0x46      //д��ַ
#define BHAddRead      0x47      //����ַ
#define BHPowDown      0x00      //�ر�ģ��
#define BHPowOn        0x01      //��ģ��ȴ�ָ�� 
#define BHReset        0x07      //�������ݼĴ�������BHpowOnģʽ����Ч
#define BHModeH1       0x10      //�߷ֱ��� ģʽ1 ��λ 11X ����ʱ�� 120ms 
#define BHModeH2       0x11      //�߷ֱ��� ģʽ2 ��λ 11X ����ʱ�� 120ms 
#define BHModeL        0x13      //�ͷֱ��� ��λ4lx ����ʱ��16ms  
#define BHSigModeH     0x20      //һ�β����߷ֱ���ģʽ1��Ȼ��ת��powerdownģʽ
#define BHSigModeH2    0x21      //ͬ������
#define BHSigModeL     0x23      //ͬ������  



void bh_data_send(unsigned char command);
unsigned short bh_data_read(void);
void BH1750init(void);
#endif 
