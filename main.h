/******************************************************************

MAIN.H  file
  
*******************************************************************/

#ifndef __MAIN_H__
#define __MAIN_H__

#include "stc15f2k60s2.h"
#include "my_type.h"

#define SYSCLK 		22118400UL		//ϵͳʱ��Ƶ��Ϊ22118400Hz

#define S2		P10			// S ='0' ��ʾ����������
#define S3		P27			// S ='0' ��ʾ����������

#define LED2	P07	   	// LED ='0' ��ʾ���ƣ�LED ='1' ��ʾ���

/* �����ȡKEY2״̬�ĺ����� */
#define IsKey2Down()		S2==0	//��ȡ����2
#define IsKey3Down()		S3==0	//��ȡ����3

/* �������LED2�ĺ����� */
#define Led2Tog()			LED2=!LED2	//LED��˸
#define Led2Off()			LED2=1			//LED�ر�
#define Led2On()			LED2=0			//LED��

#endif

/**///////////////////////Pino Electronics////////////////////////**/