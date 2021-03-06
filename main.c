/************************************************************************************
*
* 实验平台：品诺电子STC15W4K核心板 + 品诺电子"开拓者"扩展板
* 时钟选择：内部IRC时钟22.1184M
* 功能说明：该实验通过ADC功能，使用扩展板上的温敏电阻测量当前温度，温度值显示在数
						码管上。温敏电阻相对于DS18B20温度传感器，价格低的多。
* 作者信息：品诺电子(http://free-design.taobao.com)
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

volatile u8 uart3SendingFlag=0;	 	//全局变量，标志串口是否正处于发送中(忙状态)
volatile u8 uart3ReceivedFlag=0;		//全局变量，标志串口已接收到数据
volatile u8 uart3Buf;	//全局变量，用于存放从串口接收到的数据

u8 xdata Data_Buffer[60];
u8 xdata Data_Buffer2[30];


/********************************************************************
函数功能：延时函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void Delay20ms(void)   //误差 -0.000000000004us
{
    u8 a,b,c;
    for(c=191;c>0;c--)
        for(b=231;b>0;b--)
            for(a=1;a>0;a--);
}

void DelayForAD(void)   //误差 -0.000000000004us
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
	/*初始化定时器3*/	
	//TMOD &= 0xCF;
	T4T3M &= 0xFB;
	T4T3M |= 0x02;      //定时器3为1T模式
	T3H=(65536-(SYSCLK/4/BAUDRATE))>>8;		//初始化Timer3重载值
	T3L=(65536-(SYSCLK/4/BAUDRATE));		//初始化Timer3重载值
	T4T3M |= 0x08;      //定时器3开始计时
	
	/*初始化串口3*/
	//默认工作方式0，8位UART模式，波特率�                                                                                                             杀洌褂肞00和P01引脚
	S3CON|=0x40;			//选择定时器3作为波特率发生器
	S3CON|=0x10;				//打开串口3接收
	IE2|=0x08;					//打开串口3中断
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：发送一个byte的数据。
入口参数：x:待发送的数据。
返    回：无。
备    注：无。
********************************************************************/
void Printc(unsigned char x)
{
	uart3SendingFlag=1;	 		//设置发送标志
	S3BUF=x; 				//将数据写入到串口缓冲
	while(uart3SendingFlag); 	//等待发送完毕
}
void U3SendData(unsigned char ch)
{
	uart3SendingFlag=1;	 		//设置发送标志
  S3BUF = ch;                 //写数据到UART数据寄存�
	while(uart3SendingFlag); 	//等待发送完毕
}

void U3SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        U3SendData(*s++);         //发送当前字符
				Delay();
    }
}

/********************************************************************
函数功能：对IAP15W4K，STC15W4K的IO口初始化为准双向IO口。		  
入口参数：无。
返    回：无。
备    注：STC15W4K系列的芯片,上电后所有与PWM相关的IO口（P0.6/P0.7
		  /P1.6/P1.7/P2.1/P2.2/P2.3/P2.7/P3.7/P4.2/P4.4/P4.5）均为高
		  阻态,需将这些口设置为准双向口或强推挽模式方可正常使用。
********************************************************************/
void InitIO(void)
{
	//初始化 P0 P1 P2 P3 P4 为准双向IO口
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
函数功能：主函数。
入口参数：无。
返    回：无。
备    注：无。
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
	
	EA=1;				//允许所有中断

	while(1){
		if((key3DownFlag==0)&&(IsKey3Down())){//按下S3重置时钟为0分0秒
			Delay20ms();	//按键消抖
			key3DownFlag=1;
			Pcf8563SetSecond(0);
			Pcf8563SetMinute(0);
		}else if((key3DownFlag==1)&&(!(IsKey3Down()))){
			Delay20ms();	//按键消f抖
			key3DownFlag=0;
		}
		second=Pcf8563GetSecond();//读取秒
		minute=Pcf8563GetMinute();//读取分
		//显示为“分钟.秒种”
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
	if(S3CON&0x02){			//检查串口3发送标志位
		S3CON&=(~0x02);		//清除发送标志位
		uart3SendingFlag=0;
	}else if(S3CON&0x01){	//检查串口3接收标志位
		S3CON&=(~0x01);		//清除接收标志位
		uart3Buf=S3BUF;
		uart3ReceivedFlag=1;
	}
}
