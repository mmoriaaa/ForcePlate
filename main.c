/************************************************************************************
*
* ʵ��ƽ̨��Ʒŵ����STC15W4K���İ� + Ʒŵ����"������"��չ��
* ʱ��ѡ���ڲ�IRCʱ��22.1184M
* ����˵������ʵ��ͨ��ADC���ܣ�ʹ����չ���ϵ��������������ǰ�¶ȣ��¶�ֵ��ʾ����
						����ϡ��������������DS18B20�¶ȴ��������۸�͵Ķࡣ
* ������Ϣ��Ʒŵ����(http://free-design.taobao.com)
*
************************************************************************************/

#include "stc15f2k60s2.h"
#include "my_type.h"
#include "main.h"
#include "num.h"
#include "adc.h"
#include "ntc_temp.h"
#include "i2c1.h"
#include "pcf8563.h"
#include "ADS1110.h"
// #include "uart.h"
#include  <stdio.h>   				// Keil library	

sbit SD_BUSY = P2^4;
sbit SD_STATUS = P2^5;

#define	BAUDRATE 	115200UL 

volatile u8 uart3SendingFlag=0;	 	//ȫ�ֱ�������־�����Ƿ������ڷ�����(æ״̬)
volatile u8 uart3ReceivedFlag=0;		//ȫ�ֱ�������־�����ѽ��յ�����
volatile u8 uart3Buf;	//ȫ�ֱ��������ڴ�ŴӴ��ڽ��յ�������

u8 xdata Data_Buffer[60];
u8 xdata Data_Buffer2[30];


/********************************************************************
�������ܣ���ʱ������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Delay20ms(void)   //��� -0.000000000004us
{
    u8 a,b,c;
    for(c=191;c>0;c--)
        for(b=231;b>0;b--)
            for(a=1;a>0;a--);
}

void DelayForAD(void)   //��� -0.000000000004us
{
    u8 a,b,c;
    for(c=191;c>0;c--)
        for(b=100;b>0;b--)
            for(a=1;a>0;a--);
}

void Delay(void) {
	u8 a,b,c;
  for(c=100;c>0;c--)
     for(a=1;a>0;a--);
}

/********************************************************************/
void InitUart3(void)
{
	/*��ʼ����ʱ��3*/	
	//TMOD &= 0xCF;
	T4T3M &= 0xFB;
	T4T3M |= 0x02;      //��ʱ��3Ϊ1Tģʽ
	T3H=(65536-(SYSCLK/4/BAUDRATE))>>8;		//��ʼ��Timer3����ֵ
	T3L=(65536-(SYSCLK/4/BAUDRATE));		//��ʼ��Timer3����ֵ
	T4T3M |= 0x08;      //��ʱ��3��ʼ��ʱ
	
	/*��ʼ������3*/
	//Ĭ�Ϲ�����ʽ0��8λUARTģʽ�������ʿ                                                                                                             ɱ䣬ʹ��P00��P01����
	S3CON|=0x40;			//ѡ��ʱ��3��Ϊ�����ʷ�����
	S3CON|=0x10;				//�򿪴���3����
	IE2|=0x08;					//�򿪴���3�ж�
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ�����һ��byte�����ݡ�
��ڲ�����x:�����͵����ݡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Printc(unsigned char x)
{
	uart3SendingFlag=1;	 		//���÷��ͱ�־
	S3BUF=x; 				//������д�뵽���ڻ���
	while(uart3SendingFlag); 	//�ȴ��������
}
void U3SendData(unsigned char ch)
{
	uart3SendingFlag=1;	 		//���÷��ͱ�־
  S3BUF = ch;                 //д���ݵ�UART���ݼĴ��
	while(uart3SendingFlag); 	//�ȴ��������
}

void U3SendString(char *s)
{
    while (*s)                  //����ַ���������־
    {
        U3SendData(*s++);         //���͵�ǰ�ַ�
				Delay();
    }
}

/********************************************************************
�������ܣ���IAP15W4K��STC15W4K��IO�ڳ�ʼ��Ϊ׼˫��IO�ڡ�		  
��ڲ������ޡ�
��    �أ��ޡ�
��    ע��STC15W4Kϵ�е�оƬ,�ϵ��������PWM��ص�IO�ڣ�P0.6/P0.7
		  /P1.6/P1.7/P2.1/P2.2/P2.3/P2.7/P3.7/P4.2/P4.4/P4.5����Ϊ��
		  ��̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ�á�
********************************************************************/
void InitIO(void)
{
	//��ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��
	P0M0 = 0x00;
	P0M1 = 0x00;

	P1M0 = 0x00;
	P1M1 = 0x00;

	P2M0 = 0x00;
	P2M1 = 0x00;

	P3M0 = 0x00;
	P3M1 = 0x00;

	P4M0 = 0x00;
	P4M1 = 0x00;  
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ���������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void main (void)
{
	//Inner ADC Module
	u16 adcNum;
	//Time Module
	u16 dat;
	u8 second, minute;
	u8 secondDec, minuteDec;
	
	u8 key2DownFlag = 0;
	u8 key3DownFlag = 0;
	
	//Outer ADC Module
	u16 result;
	u16 result1, result2, result3, result4, result5, result6, result7, result8;

	u8 i;
	u8 j; //for dalay
	u16 times = 0;
	
	
	InitIO();
	InitTimer0();
	I2CInit();
	//InitADC(ADC_CHANNEL_TMP);
	//ADS
	IIC_Init();
	ADS1110_Config();
	ADS1110_SetValue();
	
	InitUart3();
	
	EA=1;				//���������ж�

	while(1){
		if((key3DownFlag==0)&&(IsKey3Down())){//����S3����ʱ��Ϊ0��0��
			Delay20ms();	//��������
			key3DownFlag=1;
			Pcf8563SetSecond(0);
			Pcf8563SetMinute(0);
		}else if((key3DownFlag==1)&&(!(IsKey3Down()))){
			Delay20ms();	//������f��
			key3DownFlag=0;
		}
		second=Pcf8563GetSecond();//��ȡ��
		minute=Pcf8563GetMinute();//��ȡ��
		//��ʾΪ������.���֡�
		dat=(minute<<8)|second;
		//SetNumDisplayHex16(dat);
		minuteDec = (((minute&0xf0)>>4)*10 + (minute&0x0f));
		secondDec = (((second&0xf0)>>4)*10 + (second&0x0f));
		
		//P05:A2 1 P06:A1 2 P26:A0 4
		//ADS
		
		for(i = 0; i < 8; i++) {
			switch(i) {
				case 0:
					P05 = 0;
					P06 = 0;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result1=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 1:
					P05 = 1;
					P06 = 0;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result2=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 2:
					P05 = 0;
					P06 = 1;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result3=ADS1110_ReadADValue();
						DelayForAD();
					}
				  break;
				case 3:
					P05 = 1;
					P06 = 1;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result4=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 4:
					P05 = 0;
					P06 = 0;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result5=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 5:
					P05 = 1;
					P06 = 0;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result6=ADS1110_ReadADValue();
						DelayForAD();
					}
					SetNumDisplayHex16(result6);
					break;
				case 6:
					P05 = 0;
					P06 = 1;
					P26 = 1;
					Delay20ms();
					for(j = 0; j < 10; j++){
						result7=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 7:
					P05 = 1;
					P06 = 1;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result8=ADS1110_ReadADValue();
						DelayForAD();
				  }
					break;
			}
		}
		
		
		/*
		for(i = 0; i < 5; i++) {
			Delay20ms();
		}
		*/
		/*
		P05 = 1;
		P06 = 0;
		P26 = 1;
		Delay20ms();
		result=ADS1110_ReadADValue();
		SetNumDisplayHex16(result);
		*/
		
		sprintf(Data_Buffer,"%02d-%02d-%02d-%02d-%02d-",result1,result2,result3,result4,result5);
		sprintf(Data_Buffer2,"%02d-%02d-%02d-%02d-%02d OK\r\n",result6,result7,result8,(int)minuteDec,(int)secondDec);
		
		
		if(!SD_STATUS&&SD_BUSY) {
			U3SendString(Data_Buffer);
			U3SendString(Data_Buffer2);
		}
		Led2Tog();                                                                                                                                                                                                                                                                                                                                                                                              
	}	
}

void Uart3ISR(void) interrupt 17
{
	if(S3CON&0x02){			//��鴮��3���ͱ�־λ
		S3CON&=(~0x02);		//������ͱ�־λ
		uart3SendingFlag=0;
	}else if(S3CON&0x01){	//��鴮��3���ձ�־λ
		S3CON&=(~0x01);		//������ձ�־λ
		uart3Buf=S3BUF;
		uart3ReceivedFlag=1;
	}
}
