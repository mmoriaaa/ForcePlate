/**************************************************************************************
*功能：ADS1110模块连续读测试例程												               									*
*特别说明：VCC         2.7V-5.5V																												*
*					VIN+ VIN-   压差不超过4.096V																									*
*					满量程       -2.048V   +2.048V																								*
*作者：科睿研发中心																		  															*
*公司：科睿电子产品有限公司 																														*	
*电话：0530-2897239   15192459227			  										  												*
***************************************************************************************/

#include "stc15f2k60s2.h"
#include "I2C2.h"
#include "ADS1110.h"
#include "num.h"

#define ADS1110_ST  0x80	//ADS1110写入数据
#define ADS1110_DR  0x0C	//ADS1110转换速率设置  0x0C为15SPS  0x08为30SPS  0x04为60SPS  0x00为240SPS
#define ADS1110_PGA 0x00  //ADS1110增益倍数设置  0x00为x1  0x01为x2  0x02为x4  0x03为x8
#define ADS1110_SC  0x10 

#define VREF 2.048F				 //参考电压
unsigned int MinCode=32768;//最小码
unsigned int PGA_Num=1;    //放大倍数

/***************************************
函数名称：void Uart_Init(void)
函数说明：串口初始化函数
其他：波特率9600
***************************************/
void Uart_Init(void)
{
	TMOD=0x21;	 //设T0为方式1，GATE=1；
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TH0=0;
	TL0=0; 
	TR0=1;  
	ET0=1;        //允许T0中断
	TR1=1;			  //开启定时器
	TI=1;

	EA=1;			    //开启总中断
}

/***************************************
函数名称：void Delay100ms(void)
函数说明：软件延时函数
其他：延时时间100ms
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
函数名称：void Uart_SendData(unsigned char d)
函数说明：串口发送函数
其他：d为要发送的字节
***************************************/
void Uart_SendData(unsigned char d)
{
	
	SBUF=d;
	while(!TI);
	TI=0;
}

/***************************************
函数名称：void Uart_SendNum(int n)
函数说明：串口发送数字
其他：n为要发送的数字
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
函数名称：void ADS1110_Config(void)
函数说明：ADS1110参数配置函数
其他：根据实际需要修改程序最上方的#define宏定义
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
函数名称：void ADS1110_SetValue(void)
函数说明：输出码转换为电压值需要的几个关键参数设置
其他：与最上方的#define宏定义一致
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
函数名称：unsigned int ADS1110_ReadADValue(void)
函数说明：读取ADS1110的输出码
其他：无
***************************************/
u16 ADS1110_ReadADValue(void)
{
	u16 result;
	
	IIC_Start();
	IIC_WriteByte(0x93);//发送ED0地址和读操作命令,ED1为#93H、ED2为#95H、ED3为#97H
	while(IIC_RecAck());
	result=IIC_ReadByte();
	result<<=8;
	IIC_SendAck(0);
	result|=IIC_ReadByte();
	IIC_SendAck(1);
	IIC_Stop();
	return result;
}