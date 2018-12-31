/******************************************************************

NUM.H  file
  
*******************************************************************/

#ifndef __NUM_H__
#define __NUM_H__

#include "my_type.h"

void SetOneBit(u8 b);

void RefreshNumDisplay(void);

void SetNumDisplayDecimal(u16 number);

void SetNumDisplayHex8(u8 number);

void SetNumDisplayHex16(u16 number);

void SetNumDisplayBlank(void);

void SetNumDisplayDot(u8 x);

void InitTimer0(void);

void Timer0ISR(void);

u8 GetSecondFlag(void);
	
#endif

/**///////////////////////Pino Electronics////////////////////////**/











