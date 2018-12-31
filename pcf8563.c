/************************************************************************************
*
* 功能说明：本文件提供PCF8563的操作函数集合。
* 作者信息：品诺电子(http://free-design.taobao.com)
*
************************************************************************************/
#include "my_type.h"
#include "i2c1.h"

#define PCF8563_WRITE_ADDR 0xA2	//PCF8563的IIC写地址

u8 Pcf8563GetSecond(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x02);//读取秒
	return dat&0x7f;
}

u8 Pcf8563GetMinute(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x03);//读取分
	return dat&0x7f;
}

u8 Pcf8563GetHoure(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x04);//读取时
	return dat&0x3f;
}

u8 Pcf8563GetDay(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x05);//读取日
	return dat&0x3f;
}

u8 Pcf8563GetWeek(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x06);//读取星期
	return dat&0x07;
}

u8 Pcf8563GetMonth(void){//返回BCD编码的时间格式
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x07);//读取月/世纪
	return dat&0x1f;
}

u16 Pcf8563GetYear(void){//返回BCD编码的时间格式
	u16 dat;
	u8 tmp;
	tmp=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x07);//读取月/世纪
	if(tmp&=0x80)
		dat=0x2000;
	else
		dat=0x1900;
	tmp=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x08);//读取年
	dat|=tmp;
	return dat;
}

void Pcf8563SetSecond(u8 dat){
	u8 bcdDat;
	bcdDat=((dat/10)<<4)|(dat%10);
	I2CWriteByteToDevice8(PCF8563_WRITE_ADDR,0x02,bcdDat);//写入秒
}

void Pcf8563SetMinute(u8 dat){
	u8 bcdDat;
	bcdDat=((dat/10)<<4)|(dat%10);
	I2CWriteByteToDevice8(PCF8563_WRITE_ADDR,0x03,bcdDat);//写入分
}


