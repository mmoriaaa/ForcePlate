/************************************************************************************
Copyright,  LCE STUDIO  
程序名称：串口1通讯函数（晶振频率18.432MHz）  
功能说明: 串口1初始化,字符发送与字符串发送函数,波特率9600bps
					串口2初始化,字符发送与字符串发送函数,波特率115200bps
程序版本：1.0 （2017/11）
阿里旺旺：rob1983
淘宝店铺：https://LCES.taobao.com 
************************************************************************************/ 

#include "stc15f2k60s2.h"
#include "intrins.h"
#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1
#define S2RB8 0x04              //S2CON.2
#define S2TB8 0x08              //S2CON.3
void SendData(unsigned char ch);
void SendString(char *s);
void UartInit(void);
void U2SendString(char *s);
void U2SendData(unsigned char ch);

#define LED2	P07

/*----------------------------
串口初始化
----------------------------*/

void UartInit(void)	
{
	SCON = 0x50;		//8位数据,可变波特率
	TMOD = 0x20;	//初始化定时器1工作在模式2（8位自动重装模式）
	TL1 = 0x20;	    	//设定定时初值
	TH1 = 0xFE;			//设定定时器重装值
	//PCON = 0x80;	//波特率加倍位打开
  ES = 1;				//打开串口中断
	TR1 = 1;			//启动定时器1
}


/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned char ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
			LED2=!LED2;
    }
}

/*----------------------------
通过串口2发送串口数据
----------------------------*/
void U2SendData(unsigned char ch)
{
    S2BUF = ch;                 //写数据到UART数据寄存器
		while(!(S2CON&S2TI));    
    S2CON&=~S2TI;
}

/*----------------------------
通过串口2发送字符串
----------------------------*/
void U2SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        U2SendData(*s++);         //发送当前字符
    }
}


