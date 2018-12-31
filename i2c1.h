/******************************************************************

I2C.H  file
  
*******************************************************************/
#ifndef __I2C_H__
#define __I2C_H__

#include "my_type.h"

void I2CInit(void);

void I2CDelay(void);

void I2CStart(void);

void I2CStop(void);

void I2CAck(void);
	
void I2CNoAck(void);

u8 I2CWaitAck(void);

void I2CWriteByte(u8 dat);

u8 I2CReadByte(void);

void I2CWriteAddr16(u8 deviceWriteAddr,u16 registerAddr);

void I2CWriteAddr8(u8 deviceWriteAddr,u8 registerAddr);

void I2CWriteByteToDevice16(u8 deviceWriteAddr,u16 registerAddr,u8 dat);

void I2CWriteByteToDevice8(u8 deviceWriteAddr,u8 registerAddr,u8 dat);

u8 I2CReadByteFromDevice16(u8 deviceWriteAddr,u16 registerAddr);

u8 I2CReadByteFromDevice8(u8 deviceWriteAddr,u8 registerAddr);

#endif
/**///////////////////////Pino Electronics////////////////////////**/



