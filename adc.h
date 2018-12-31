/******************************************************************

ADC.H  file
  
*******************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#include "my_type.h"

#define ADC_CHANNEL_TMP		7
#define ADC_CHANNEL_RES		6
#define ADC_CHANNEL_KEY		0

void InitADC(u8 ch);

u8 GetAdcUpdatedFlag(void);

u16 GetAdcData(void);
	
#endif

/**///////////////////////Pino Electronics////////////////////////**/







