/******************************************************************

PCF8563.H  file
  
*******************************************************************/
#ifndef __PCF8563_H__
#define __PCF8563_H__

#include "my_type.h"

u8 Pcf8563GetSecond(void);

u8 Pcf8563GetMinute(void);

u8 Pcf8563GetHoure(void);

u8 Pcf8563GetDay(void);

u8 Pcf8563GetWeek(void);

u8 Pcf8563GetMonth(void);

u16 Pcf8563GetYear(void);

void Pcf8563SetSecond(u8 dat);

void Pcf8563SetMinute(u8 dat);



#endif
/**///////////////////////Pino Electronics////////////////////////**/





