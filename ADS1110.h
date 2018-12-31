/******************************************************************

ADS1110.H  file
  
*******************************************************************/
#ifndef __ADS1110_H__
#define __ADS1110_H__

#include "my_type.h"

void Uart_Init(void);

void Delay100ms(void);

void Uart_SendData(u8 d);

void Uart_SendNum(int n);

void ADS1110_Config(void);

void ADS1110_SetValue(void);

unsigned int ADS1110_ReadADValue(void);

#endif