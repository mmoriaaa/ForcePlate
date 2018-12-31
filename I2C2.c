/**************************************************************************************
*功能：IIC协议的通信代码        												               									*
*特别说明：无																																					*
*作者：科睿研发中心																		  															*
*公司：科睿电子产品有限公司 																														*	
*电话：0530-2897239   15192459227			  										  												*
***************************************************************************************/

#include "stc15f2k60s2.h"
#include "I2C2.h"

#define IIC_SCL		P40			/* 连接到SCL时钟线的GPIO */
#define IIC_SDA		P41			/* 连接到SDA数据线的GPIO */

/***************************************
函数名称：void IIC_NOP(void)
函数说明：延时函数
其他：无
***************************************/ 
void IIC_NOP(void)  
{;;}  
  
	
/***************************************
函数名称：void IIC_DelayXms(unsigned char x)
函数说明：延时函数
其他：延时1-255ms
***************************************/ 
void IIC_DelayXms(unsigned char x)  
{  
	unsigned char y,z;  
	for(y=x;y>0;y--)  
			for(z=110;z>0;z--);  
}  	

/***************************************
函数名称：void IIC_Init(void)  
函数说明：IIC端口初始化函数
其他：无
***************************************/ 
void IIC_Init(void)  
{  
	IIC_SDA = 1;  
	IIC_SCL = 1;  
	IIC_NOP();  
}  


/***************************************
函数名称：void IIC_Start(void)  
函数说明：IIC起始信号
其他：无
***************************************/ 
void IIC_Start(void)  
{  
	IIC_SDA = 1;  
	IIC_NOP();  
	IIC_SCL = 1;  
	IIC_NOP();  
	IIC_SDA = 0;  
	IIC_NOP();  
}  

/***************************************
函数名称：void IIC_Stop(void)  
函数说明：IIC终止信号
其他：无
***************************************/ 
void IIC_Stop(void)  
{  
	IIC_SDA = 0;  
	IIC_NOP();  
	IIC_SCL = 1;  
	IIC_NOP();  
	IIC_SDA = 1;  
	IIC_NOP();  
}  

/***************************************
函数名称void IIC_SendAck(unsigned char ack)   
函数说明：IIC主机发送应答信号
其他： ack=0为连续读  ack=1为不连续读
***************************************/ 
void IIC_SendAck(unsigned char ack)  
{  
	IIC_SCL = 0;  
	IIC_NOP();  
	IIC_SDA = ack;  
	IIC_NOP();  
	IIC_SCL = 1;  
	IIC_NOP();  
	IIC_SCL = 0;  
	IIC_NOP();  
	IIC_SDA = 1;  
	IIC_NOP();  
}  

/***************************************
函数名称：unsigned char IIC_RecAck(void)   
函数说明：IIC主机接收应答信号  
其他：无
***************************************/ 
unsigned char IIC_RecAck(void)  
{  
    IIC_SCL = 1;  
    IIC_NOP();  
    if(IIC_SDA == 1)  
    {  
			IIC_SCL = 0;  
			IIC_DelayXms(1);  
			IIC_SCL = 1;  
			if(IIC_SDA == 1)  
			{
				return 1; 
			}					
    }  
    else  
		{
			return 0;  
		}
}  
/***************************************
函数名称：void IIC_WriteByte(unsigned char dat)   
函数说明：IIC写一个字节  
其他：无
***************************************/ 
void IIC_WriteByte(unsigned char dat)  
{  
    unsigned char i,a;  
    a = dat;  
    for(i=0;i<8;i++)  
    {  
        a = a<<1;  
        IIC_SCL = 0;  
        IIC_NOP();  
        IIC_SDA = CY;  //具体请查阅51单片机的CY位
        IIC_NOP();  
        IIC_SCL = 1;  
        IIC_NOP();  
    }  
    IIC_SCL = 0;  
    IIC_NOP();  
    IIC_SDA = 1;  
    IIC_NOP();  
}  

/***************************************
函数名称：unsigned char IIC_ReadByte(void)    
函数说明：IIC读一个字节  
其他：无
***************************************/ 
unsigned char IIC_ReadByte(void)  
{  
    unsigned char i,a;  
    IIC_SCL = 0;  
    IIC_NOP();  
    IIC_SDA = 1;  
    IIC_NOP();  
    for(i=0;i<8;i++)  
    {  
        IIC_SCL = 1;  
        IIC_NOP();  
        a = (a<<1)|IIC_SDA;  
        IIC_SCL = 0;  
        IIC_NOP();  
    }  
    return a;  
}  


