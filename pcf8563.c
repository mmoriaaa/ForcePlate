/************************************************************************************
*
* ����˵�������ļ��ṩPCF8563�Ĳ����������ϡ�
* ������Ϣ��Ʒŵ����(http://free-design.taobao.com)
*
************************************************************************************/
#include "my_type.h"
#include "i2c1.h"

#define PCF8563_WRITE_ADDR 0xA2	//PCF8563��IICд��ַ

u8 Pcf8563GetSecond(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x02);//��ȡ��
	return dat&0x7f;
}

u8 Pcf8563GetMinute(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x03);//��ȡ��
	return dat&0x7f;
}

u8 Pcf8563GetHoure(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x04);//��ȡʱ
	return dat&0x3f;
}

u8 Pcf8563GetDay(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x05);//��ȡ��
	return dat&0x3f;
}

u8 Pcf8563GetWeek(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x06);//��ȡ����
	return dat&0x07;
}

u8 Pcf8563GetMonth(void){//����BCD�����ʱ���ʽ
	u8 dat;
	dat=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x07);//��ȡ��/����
	return dat&0x1f;
}

u16 Pcf8563GetYear(void){//����BCD�����ʱ���ʽ
	u16 dat;
	u8 tmp;
	tmp=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x07);//��ȡ��/����
	if(tmp&=0x80)
		dat=0x2000;
	else
		dat=0x1900;
	tmp=I2CReadByteFromDevice8(PCF8563_WRITE_ADDR,0x08);//��ȡ��
	dat|=tmp;
	return dat;
}

void Pcf8563SetSecond(u8 dat){
	u8 bcdDat;
	bcdDat=((dat/10)<<4)|(dat%10);
	I2CWriteByteToDevice8(PCF8563_WRITE_ADDR,0x02,bcdDat);//д����
}

void Pcf8563SetMinute(u8 dat){
	u8 bcdDat;
	bcdDat=((dat/10)<<4)|(dat%10);
	I2CWriteByteToDevice8(PCF8563_WRITE_ADDR,0x03,bcdDat);//д���
}


