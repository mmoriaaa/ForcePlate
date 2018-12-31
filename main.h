/******************************************************************

MAIN.H  file
  
*******************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "stc15f2k60s2.h"
#include "my_type.h"

#define SYSCLK 		22118400UL		//系统时钟频率为22118400Hz

#define S2		P10			// S ='0' 表示按键被按下
#define S3		P27			// S ='0' 表示按键被按下

#define LED2	P07	   	// LED ='0' 表示亮灯，LED ='1' 表示灭灯

/* 定义读取KEY2状态的宏命令 */
#define IsKey2Down()		S2==0	//读取按键2
#define IsKey3Down()		S3==0	//读取按键3

/* 定义控制LED2的宏命令 */
#define Led2Tog()			LED2=!LED2	//LED闪烁
#define Led2Off()			LED2=1			//LED关闭
#define Led2On()			LED2=0			//LED打开

#endif

/**///////////////////////Pino Electronics////////////////////////**/