/**************************************************************************************
*���ܣ�IICЭ���ͨ�Ŵ���        												               									*
*�ر�˵������																																					*
*���ߣ�����з�����																		  															*
*��˾������Ӳ�Ʒ���޹�˾ 																														*	
*�绰��0530-2897239   15192459227			  										  												*
***************************************************************************************/

#include "stc15f2k60s2.h"
#include "I2C2.h"

#define IIC_SCL		P40			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define IIC_SDA		P41			/* ���ӵ�SDA�����ߵ�GPIO */

/***************************************
�������ƣ�void IIC_NOP(void)
����˵������ʱ����
��������
***************************************/ 
void IIC_NOP(void)  
{;;}  
  
	
/***************************************
�������ƣ�void IIC_DelayXms(unsigned char x)
����˵������ʱ����
��������ʱ1-255ms
***************************************/ 
void IIC_DelayXms(unsigned char x)  
{  
	unsigned char y,z;  
	for(y=x;y>0;y--)  
			for(z=110;z>0;z--);  
}  	

/***************************************
�������ƣ�void IIC_Init(void)  
����˵����IIC�˿ڳ�ʼ������
��������
***************************************/ 
void IIC_Init(void)  
{  
	IIC_SDA = 1;  
	IIC_SCL = 1;  
	IIC_NOP();  
}  


/***************************************
�������ƣ�void IIC_Start(void)  
����˵����IIC��ʼ�ź�
��������
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
�������ƣ�void IIC_Stop(void)  
����˵����IIC��ֹ�ź�
��������
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
��������void IIC_SendAck(unsigned char ack)   
����˵����IIC��������Ӧ���ź�
������ ack=0Ϊ������  ack=1Ϊ��������
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
�������ƣ�unsigned char IIC_RecAck(void)   
����˵����IIC��������Ӧ���ź�  
��������
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
�������ƣ�void IIC_WriteByte(unsigned char dat)   
����˵����IICдһ���ֽ�  
��������
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
        IIC_SDA = CY;  //���������51��Ƭ����CYλ
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
�������ƣ�unsigned char IIC_ReadByte(void)    
����˵����IIC��һ���ֽ�  
��������
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


