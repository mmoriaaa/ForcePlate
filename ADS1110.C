/**************************************************************************************
*���ܣ�ADS1110ģ����������������												               									*
*�ر�˵����VCC         2.7V-5.5V																												*
*					VIN+ VIN-   ѹ�����4.096V																									*
*					������       -2.048V   +2.048V																								*
*���ߣ�����з�����																		  															*
*��˾������Ӳ�Ʒ���޹�˾ 																														*	
*�绰��0530-2897239   15192459227			  										  												*
***************************************************************************************/

#include "stc15f2k60s2.h"
#include "I2C2.h"
#include "ADS1110.h"
#include "num.h"

#define ADS1110_ST  0x80	//ADS1110д������
#define ADS1110_DR  0x0C	//ADS1110ת����������  0x0CΪ15SPS  0x08Ϊ30SPS  0x04Ϊ60SPS  0x00Ϊ240SPS
#define ADS1110_PGA 0x00  //ADS1110���汶������  0x00Ϊx1  0x01Ϊx2  0x02Ϊx4  0x03Ϊx8
#define ADS1110_SC  0x10 

#define VREF 2.048F				 //�ο���ѹ
unsigned int MinCode=32768;//��С��
unsigned int PGA_Num=1;    //�Ŵ���

/***************************************
�������ƣ�void Uart_Init(void)
����˵�������ڳ�ʼ������
������������9600
***************************************/
void Uart_Init(void)
{
	TMOD=0x21;	 //��T0Ϊ��ʽ1��GATE=1��
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;        //����T0�ж�
	TR1=1;			  //������ʱ��
	TI=1;

	EA=1;			    //�������ж�
}

/***************************************
�������ƣ�void Delay100ms(void)
����˵����������ʱ����
��������ʱʱ��100ms
***************************************/
void Delay100ms(void)	
{
	unsigned char i, j;

	i = 180;
	j = 73;
	do
	{
		while (--j);
	} while (--i);
}

/***************************************
�������ƣ�void Uart_SendData(unsigned char d)
����˵�������ڷ��ͺ���
������dΪҪ���͵��ֽ�
***************************************/
void Uart_SendData(unsigned char d)
{
	
	SBUF=d;
	while(!TI);
	TI=0;
}

/***************************************
�������ƣ�void Uart_SendNum(int n)
����˵�������ڷ�������
������nΪҪ���͵�����
***************************************/
void Uart_SendNum(int n)
{
	if (n<0)
	{
		Uart_SendData('-');
		n*=-1;
	}
	Uart_SendData(n/1000+0x30);
	Uart_SendData('.');
	Uart_SendData(n%1000/100+0x30);
	Uart_SendData(n%100/10+0x30);
	Uart_SendData(n%10+0x30);
	Uart_SendData('V');
	Uart_SendData('\r');
	Uart_SendData('\n');
}

/***************************************
�������ƣ�void ADS1110_Config(void)
����˵����ADS1110�������ú���
����������ʵ����Ҫ�޸ĳ������Ϸ���#define�궨��
***************************************/
void ADS1110_Config(void)
{
	IIC_Start();
	IIC_WriteByte(0x90);
	IIC_SendAck(1);
	IIC_WriteByte(ADS1110_ST|ADS1110_DR|ADS1110_PGA);
	IIC_SendAck(1);
	IIC_Stop();	
}

/***************************************
�������ƣ�void ADS1110_SetValue(void)
����˵���������ת��Ϊ��ѹֵ��Ҫ�ļ����ؼ���������
�����������Ϸ���#define�궨��һ��
***************************************/
void ADS1110_SetValue(void)
{
	switch (ADS1110_DR)
	{
		case 0x0C://15SPS
		{
			MinCode=32768;
		}break;
		case 0x08://30SPS
		{
			MinCode=16384;
		}break;
		case 0x04://60SPS
		{
			MinCode=8192;
		}break;
		case 0x00://240SPS
		{
			MinCode=2048;
		}break;
		default:break;
	}
	switch (ADS1110_PGA)
	{
		case 0x00:
		{
			PGA_Num=1;
		}break;
		case 0x01:
		{
			PGA_Num=2;
		}break;
		case 0x02:
		{
			PGA_Num=4;
		}break;
		case 0x03:
		{
			PGA_Num=8;
		}break;
		default:break;
	}
}

/***************************************
�������ƣ�unsigned int ADS1110_ReadADValue(void)
����˵������ȡADS1110�������
��������
***************************************/
u16 ADS1110_ReadADValue(void)
{
	u16 result;
	
	IIC_Start();
	IIC_WriteByte(0x93);//����ED0��ַ�Ͷ���������,ED1Ϊ#93H��ED2Ϊ#95H��ED3Ϊ#97H
	while(IIC_RecAck());
	result=IIC_ReadByte();
	result<<=8;
	IIC_SendAck(0);
	result|=IIC_ReadByte();
	IIC_SendAck(1);
	IIC_Stop();
	return result;
}