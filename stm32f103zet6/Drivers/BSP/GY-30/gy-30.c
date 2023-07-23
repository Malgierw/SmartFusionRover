#include "./BSP/GY-30/gy-30.h"
#include "./SYSTEM/delay/delay.h"

/*********IIC��ʼ��************
**Ӳ���ӿڣ�IIC_SCL -- PF10
**					IIC_SDA -- PF11
**
**
*******************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOF_CLK_ENABLE();			//����GPIOFʱ��
	
  GPIO_Initure.Pin=GPIO_PIN_10;           //PF10
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //��ʼ��
}
//������ʼ�ź�
void IIC_Start(void)
{
	IIC_SDAout_Mode();//���ģʽ
	IIC_SCL=1;
	IIC_SDAout=1;
	delay_us(2);
	IIC_SDAout=0;
	
	IIC_SCL=0;//������������շ�
}
//ֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDAout_Mode();//���ģʽ
	IIC_SCL=0;
	IIC_SDAout=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SDAout=1;
}
/***********����Ӧ���ź�**************************
**
**�βΣ�u8 ack -- 0Ӧ��1��Ӧ��
**
***************************************************/
void IIC_SendAck(uint8_t ack)
{
	IIC_SDAout_Mode();//���ģʽ	
	IIC_SCL=0;//���ߴӻ���������ʼ��������
	IIC_SDAout=ack&0x01;
	delay_us(2);
	IIC_SCL=1;//���ߴӻ����������ݷ������
	//������һ�������շ�
	delay_us(2);
	IIC_SCL=0;
}
//��ȡӦ���ź�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t cnt=0;
	IIC_SDAIN_Mode();//����Ϊ����ģʽ
	IIC_SDAout=1;	
	IIC_SCL=0;//���ߴӻ���������Ҫ��ȡ����
	delay_us(2);
	IIC_SCL=1;//�ӻ����ݷ�����ɣ�������ʼ��ȡ����
	while(IIC_SDAin)
	{
		cnt++;
		delay_us(1);
		if(cnt>=100)return 1;
	}
	delay_us(2);
	IIC_SCL=0;//������һ�������շ�
	return 0;
}
//����һ���ֽ�����
void IIC_Send_Byte(uint8_t data)
{
	uint8_t i=0;
	IIC_SDAout_Mode();//���ģʽ
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;//���ߴӻ���������ʼ��������
		if(data&0x80)IIC_SDAout=1;
		else IIC_SDAout=0;
		delay_us(2);
		IIC_SCL=1;//�������ݷ������
		data<<=1;
		delay_us(2);
	}
	IIC_SCL=0;//������һ�������շ�
}
//��ȡһ���ֽ�����
uint8_t IIC_Read_Byte(void)
{
	uint8_t i=0;
	uint8_t data=0;
	IIC_SDAIN_Mode();//����Ϊ����ģʽ
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;//���ߴӻ���������Ҫ��ȡ����
		delay_us(2);
		IIC_SCL=1;//��ʼ��ȡ����
		data<<=1;//Ĭ���յ�0
		if(IIC_SDAin)data|=0x01;
		delay_us(2);
	}
	IIC_SCL=0;
	return data;
}

void bh_data_send(uint8_t command)  
{  
    do{  
    IIC_Start();                      //iic��ʼ
    IIC_Send_Byte(BHAddWrite);       //д��ַ  
    }while(IIC_Wait_Ack());           //�ȴ���Ӧ 
    IIC_Send_Byte(command);          //��������  
    IIC_Wait_Ack();                   //�ȴ���Ӧ 
    IIC_Stop();                       //iicֹͣ  
}   


uint16_t bh_data_read(void)  
{  
    uint16_t buf;  
    IIC_Start();                       //iic��ʼ  
    IIC_Send_Byte(BHAddRead);         //���Ͷ���ַ
    IIC_Wait_Ack();                     //�ȴ���Ӧ  
    buf=IIC_Read_Byte();              //��ȡ����  
		IIC_SendAck(0);
    buf=buf<<8;                        //��ȡ������߰�λ����
    buf+=0x00ff&IIC_Read_Byte();      //��ȡ������ڰ�λ����
		IIC_SendAck(1);
    IIC_Stop();                        //����ֹͣ�ź�
    return buf;   
}  

void BH1750init(void)
{
	IIC_Init();//GPIO��ʼ��
	bh_data_send(BHPowOn);    //���������ź�
	bh_data_send(BHReset);    //����Ĵ���  
  bh_data_send(BHModeH1);   //����Ϊģʽ2
  delay_ms(180);            //�����ʱ180ms
}
