/************************************************************************************
*
* 功能说明：本文件提供了用普通IO端口模拟I2C总线行为的函数集合。
* 作者信息：品诺电子(http://free-design.taobao.com)
*
************************************************************************************/
#include "stc15f2k60s2.h"
#include "i2c1.h"

#define PCF8563_WRITE_ADDR 0xA2	//PCF8563的IIC写地址
/* 定义I2C总线连接的GPIO端口, 只需要修改下面宏定义即可改变SCL和SDA的引脚定义 */
#define I2C_SCL		P43			/* 连接到SCL时钟线的GPIO */
#define I2C_SDA		P44			/* 连接到SDA数据线的GPIO */

/* 定义读写SCL和SDA的宏，增加代码的可移植性和可阅读性 */
#define I2C_SCL_1()  		I2C_SCL=1		/* SCL = 1 */
#define I2C_SCL_0()  		I2C_SCL=0		/* SCL = 0 */
	
#define I2C_SDA_1()  		I2C_SDA=1		/* SDA = 1 */
#define I2C_SDA_0()  		I2C_SDA=0		/* SDA = 0 */
	
#define I2C_SDA_READ()  I2C_SDA			/* 读SDA线状态 */


/********************************************************************
函数功能：I2C总线位延迟函数，总线频率最快400KHz。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void I2CDelay(void)//延时4us，误差 -0.021412037037us
{
    u8 a;
    for(a=19;a>0;a--);
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C总线初始化。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void I2CInit(void)
{
    I2CStop();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C开始信号。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void I2CStart(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	I2CDelay();
	I2C_SDA_0();
	I2CDelay();
	I2C_SCL_0();
	I2CDelay();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C结束信号。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void I2CStop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	I2CDelay();
	I2C_SDA_1();
	I2CDelay();
	I2C_SCL_0();
	I2CDelay();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C应答信号。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void I2CAck(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	I2CDelay();
	I2C_SCL_0();
	I2CDelay();
	I2C_SDA_1();		//因为引脚配置为漏极开路加上拉电阻
	I2CDelay();//最后应该保持数据线在高电平状态，使总线上其它器件可以进行数据发送
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C非应答信号。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/	
void I2CNoAck(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	I2CDelay();
	I2C_SCL_0();
	I2CDelay();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：I2C等待应答信号。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
u8 I2CWaitAck(void)
{
	u8 ack=0;
	I2C_SCL_1();
	I2CDelay();
	if(I2C_SDA_READ())
		ack=1;
	I2C_SCL_0();
	I2CDelay();
	return ack;
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：在I2C总线上，写入一个字节。
入口参数：dat:要写入的字节。
返    回：无。
备    注：无。
********************************************************************/
void I2CWriteByte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++){
		if(dat&0x80)
			I2C_SDA_1();
		else
			I2C_SDA_0();
		I2C_SCL_1();
		I2CDelay();
		I2C_SCL_0();
		I2CDelay();
		dat<<=1;
	}
	I2C_SDA_1();//最后应该保持数据线在高电平状态
	I2CDelay();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：在I2C总线上，读取一个字节。
入口参数：无。
返    回：读取的字节。
备    注：无。
********************************************************************/
u8 I2CReadByte(void)
{
	u8 i,dat;
	for(i=0;i<8;i++){
		I2C_SCL_1();
		I2CDelay();
		dat<<=1;	
		if (I2C_SDA_READ())
			dat|=0x01;	
		I2C_SCL_0();
		I2CDelay();
	}
	return dat;
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：往I2C总线上写入设备地址和16bit的寄存器地址。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 16bit的设备寄存器地址；
返    回：无。
备    注：如AT24C64的寄存器地址是16bit的。
********************************************************************/
void I2CWriteAddr16(u8 deviceWriteAddr,u16 registerAddr)
{
	u8 tmp;
	I2CStart();
	I2CWriteByte(deviceWriteAddr);	//设备写地址
	I2CWaitAck();
	tmp=(registerAddr>>8);		//先写高8位地址
	I2CWriteByte(tmp);			
	I2CWaitAck();
	tmp=(registerAddr&0x00ff);			//再写低8位地址
	I2CWriteByte(tmp);			
	I2CWaitAck();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：往I2C总线上写入设备地址和8bit的寄存器地址。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 8bit的设备寄存器地址；
返    回：无。
备    注：如PCF8563的寄存器地址是8bit的。
********************************************************************/
void I2CWriteAddr8(u8 deviceWriteAddr,u8 registerAddr)
{
	I2CStart();
	I2CWriteByte(deviceWriteAddr);	//设备写地址
	I2CWaitAck();
	I2CWriteByte(registerAddr);			
	I2CWaitAck();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：往I2C总线上给定的设备地址和16bit的寄存器地址写入一个BYTE数据。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 16bit的设备寄存器地址；
		  dat: 要写入的一个BYTE数据
返    回：无。
备    注：如AT24C64的寄存器地址是16bit的。
********************************************************************/
void I2CWriteByteToDevice16(u8 deviceWriteAddr,u16 registerAddr,u8 dat)
{
	I2CWriteAddr16(deviceWriteAddr,registerAddr);
	I2CWriteByte(dat);
	I2CWaitAck();
	I2CStop();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：往I2C总线上给定的设备地址和8bit的寄存器地址写入一个BYTE数据。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 8bit的设备寄存器地址；
		  dat: 要写入的一个BYTE数据
返    回：无。
备    注：如PCF8563的寄存器地址是8bit的。
********************************************************************/
void I2CWriteByteToDevice8(u8 deviceWriteAddr,u8 registerAddr,u8 dat)
{
	I2CWriteAddr8(deviceWriteAddr,registerAddr);
	I2CWriteByte(dat);
	I2CWaitAck();
	I2CStop();
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：从I2C总线上给定的设备地址和16bit的寄存器地址读取一个BYTE数据。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 16bit的设备寄存器地址；
返    回：读取的字节。
备    注：如AT24C64的寄存器地址是16bit的。
********************************************************************/
u8 I2CReadByteFromDevice16(u8 deviceWriteAddr,u16 registerAddr)
{
	u8 dat;
	I2CWriteAddr16(deviceWriteAddr,registerAddr);
	I2CStart();
	I2CWriteByte(deviceWriteAddr|0x01);	//修改设备地址的读写标志位
	I2CWaitAck();
	dat=I2CReadByte();
	I2CNoAck();
	I2CStop();
	return dat;
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：从I2C总线上给定的设备地址和8bit的寄存器地址读取一个BYTE数据。
入口参数：deviceWriteAddr: 设备的I2C写地址；
 		  registerAddr: 8bit的设备寄存器地址；
返    回：读取的字节。
备    注：如PCF8563的寄存器地址是8bit的。
********************************************************************/
u8 I2CReadByteFromDevice8(u8 deviceWriteAddr,u8 registerAddr)
{
	u8 dat;
	I2CWriteAddr8(deviceWriteAddr,registerAddr);
	I2CStart();
	I2CWriteByte(deviceWriteAddr|0x01);	//修改设备地址的读写标志位
	I2CWaitAck();
	dat=I2CReadByte();
	I2CNoAck();
	I2CStop();
	return dat;
}
/**///////////////////////Pino Electronics////////////////////////**/

