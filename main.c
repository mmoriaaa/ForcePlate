/************************************************************************************
*
* ÊµÑéÆ½Ì¨£ºÆ·Åµµç×ÓSTC15W4KºËĞÄ°å + Æ·Åµµç×Ó"¿ªÍØÕß"À©Õ¹°å
* Ê±ÖÓÑ¡Ôñ£ºÄÚ²¿IRCÊ±ÖÓ22.1184M
* ¹¦ÄÜËµÃ÷£º¸ÃÊµÑéÍ¨¹ıADC¹¦ÄÜ£¬Ê¹ÓÃÀ©Õ¹°åÉÏµÄÎÂÃôµç×è²âÁ¿µ±Ç°ÎÂ¶È£¬ÎÂ¶ÈÖµÏÔÊ¾ÔÚÊı
						Âë¹ÜÉÏ¡£ÎÂÃôµç×èÏà¶ÔÓÚDS18B20ÎÂ¶È´«¸ĞÆ÷£¬¼Û¸ñµÍµÄ¶à¡£
* ×÷ÕßĞÅÏ¢£ºÆ·Åµµç×Ó(http://free-design.taobao.com)
*
************************************************************************************/

#include "stc15f2k60s2.h"
#include "my_type.h"
#include "main.h"
#include "num.h"
#include "adc.h"
#include "ntc_temp.h"
#include "i2c1.h"
#include "pcf8563.h"
#include "ADS1110.h"
// #include "uart.h"
#include  <stdio.h>   				// Keil library	

sbit SD_BUSY = P2^4;
sbit SD_STATUS = P2^5;

#define	BAUDRATE 	115200UL 

volatile u8 uart3SendingFlag=0;	 	//È«¾Ö±äÁ¿£¬±êÖ¾´®¿ÚÊÇ·ñÕı´¦ÓÚ·¢ËÍÖĞ(Ã¦×´Ì¬)
volatile u8 uart3ReceivedFlag=0;		//È«¾Ö±äÁ¿£¬±êÖ¾´®¿ÚÒÑ½ÓÊÕµ½Êı¾İ
volatile u8 uart3Buf;	//È«¾Ö±äÁ¿£¬ÓÃÓÚ´æ·Å´Ó´®¿Ú½ÓÊÕµ½µÄÊı¾İ

u8 xdata Data_Buffer[60];
u8 xdata Data_Buffer2[30];


/********************************************************************
º¯Êı¹¦ÄÜ£ºÑÓÊ±º¯Êı¡£
Èë¿Ú²ÎÊı£ºÎŞ¡£
·µ    »Ø£ºÎŞ¡£
±¸    ×¢£ºÎŞ¡£
********************************************************************/
void Delay20ms(void)   //Îó²î -0.000000000004us
{
    u8 a,b,c;
    for(c=191;c>0;c--)
        for(b=231;b>0;b--)
            for(a=1;a>0;a--);
}

void DelayForAD(void)   //Îó²î -0.000000000004us
{
    u8 a,b,c;
    for(c=191;c>0;c--)
        for(b=100;b>0;b--)
            for(a=1;a>0;a--);
}

void Delay(void) {
	u8 a,b,c;
  for(c=100;c>0;c--)
     for(a=1;a>0;a--);
}

/********************************************************************/
void InitUart3(void)
{
	/*³õÊ¼»¯¶¨Ê±Æ÷3*/	
	//TMOD &= 0xCF;
	T4T3M &= 0xFB;
	T4T3M |= 0x02;      //¶¨Ê±Æ÷3Îª1TÄ£Ê½
	T3H=(65536-(SYSCLK/4/BAUDRATE))>>8;		//³õÊ¼»¯Timer3ÖØÔØÖµ
	T3L=(65536-(SYSCLK/4/BAUDRATE));		//³õÊ¼»¯Timer3ÖØÔØÖµ
	T4T3M |= 0x08;      //¶¨Ê±Æ÷3¿ªÊ¼¼ÆÊ±
	
	/*³õÊ¼»¯´®¿Ú3*/
	//Ä¬ÈÏ¹¤×÷·½Ê½0£¬8Î»UARTÄ£Ê½£¬²¨ÌØÂÊ¿                                                                                                             É±ä£¬Ê¹ÓÃP00ºÍP01Òı½Å
	S3CON|=0x40;			//Ñ¡Ôñ¶¨Ê±Æ÷3×÷Îª²¨ÌØÂÊ·¢ÉúÆ÷
	S3CON|=0x10;				//´ò¿ª´®¿Ú3½ÓÊÕ
	IE2|=0x08;					//´ò¿ª´®¿Ú3ÖĞ¶Ï
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
º¯Êı¹¦ÄÜ£º·¢ËÍÒ»¸öbyteµÄÊı¾İ¡£
Èë¿Ú²ÎÊı£ºx:´ı·¢ËÍµÄÊı¾İ¡£
·µ    »Ø£ºÎŞ¡£
±¸    ×¢£ºÎŞ¡£
********************************************************************/
void Printc(unsigned char x)
{
	uart3SendingFlag=1;	 		//ÉèÖÃ·¢ËÍ±êÖ¾
	S3BUF=x; 				//½«Êı¾İĞ´Èëµ½´®¿Ú»º³å
	while(uart3SendingFlag); 	//µÈ´ı·¢ËÍÍê±Ï
}
void U3SendData(unsigned char ch)
{
	uart3SendingFlag=1;	 		//ÉèÖÃ·¢ËÍ±êÖ¾
  S3BUF = ch;                 //Ğ´Êı¾İµ½UARTÊı¾İ¼Ä´æÆ
	while(uart3SendingFlag); 	//µÈ´ı·¢ËÍÍê±Ï
}

void U3SendString(char *s)
{
    while (*s)                  //¼ì²â×Ö·û´®½áÊø±êÖ¾
    {
        U3SendData(*s++);         //·¢ËÍµ±Ç°×Ö·û
				Delay();
    }
}

/********************************************************************
º¯Êı¹¦ÄÜ£º¶ÔIAP15W4K£¬STC15W4KµÄIO¿Ú³õÊ¼»¯Îª×¼Ë«ÏòIO¿Ú¡£		  
Èë¿Ú²ÎÊı£ºÎŞ¡£
·µ    »Ø£ºÎŞ¡£
±¸    ×¢£ºSTC15W4KÏµÁĞµÄĞ¾Æ¬,ÉÏµçºóËùÓĞÓëPWMÏà¹ØµÄIO¿Ú£¨P0.6/P0.7
		  /P1.6/P1.7/P2.1/P2.2/P2.3/P2.7/P3.7/P4.2/P4.4/P4.5£©¾ùÎª¸ß
		  ×èÌ¬,Ğè½«ÕâĞ©¿ÚÉèÖÃÎª×¼Ë«Ïò¿Ú»òÇ¿ÍÆÍìÄ£Ê½·½¿ÉÕı³£Ê¹ÓÃ¡£
********************************************************************/
void InitIO(void)
{
	//³õÊ¼»¯ P0 P1 P2 P3 P4 Îª×¼Ë«ÏòIO¿Ú
	P0M0 = 0x00;
	P0M1 = 0x00;

	P1M0 = 0x00;
	P1M1 = 0x00;

	P2M0 = 0x00;
	P2M1 = 0x00;

	P3M0 = 0x00;
	P3M1 = 0x00;

	P4M0 = 0x00;
	P4M1 = 0x00;  
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
º¯Êı¹¦ÄÜ£ºÖ÷º¯Êı¡£
Èë¿Ú²ÎÊı£ºÎŞ¡£
·µ    »Ø£ºÎŞ¡£
±¸    ×¢£ºÎŞ¡£
********************************************************************/
void main (void)
{
	//Inner ADC Module
	u16 adcNum;
	//Time Module
	u16 dat;
	u8 second, minute;
	u8 secondDec, minuteDec;
	
	u8 key2DownFlag = 0;
	u8 key3DownFlag = 0;
	
	//Outer ADC Module
	u16 result;
	u16 result1, result2, result3, result4, result5, result6, result7, result8;

	u8 i;
	u8 j; //for dalay
	u16 times = 0;
	
	
	InitIO();
	InitTimer0();
	I2CInit();
	//InitADC(ADC_CHANNEL_TMP);
	//ADS
	IIC_Init();
	ADS1110_Config();
	ADS1110_SetValue();
	
	InitUart3();
	
	EA=1;				//ÔÊĞíËùÓĞÖĞ¶Ï

	while(1){
		if((key3DownFlag==0)&&(IsKey3Down())){//°´ÏÂS3ÖØÖÃÊ±ÖÓÎª0·Ö0Ãë
			Delay20ms();	//°´¼üÏû¶¶
			key3DownFlag=1;
			Pcf8563SetSecond(0);
			Pcf8563SetMinute(0);
		}else if((key3DownFlag==1)&&(!(IsKey3Down()))){
			Delay20ms();	//°´¼üÏûf¶¶
			key3DownFlag=0;
		}
		second=Pcf8563GetSecond();//¶ÁÈ¡Ãë
		minute=Pcf8563GetMinute();//¶ÁÈ¡·Ö
		//ÏÔÊ¾Îª¡°·ÖÖÓ.ÃëÖÖ¡±
		dat=(minute<<8)|second;
		//SetNumDisplayHex16(dat);
		minuteDec = (((minute&0xf0)>>4)*10 + (minute&0x0f));
		secondDec = (((second&0xf0)>>4)*10 + (second&0x0f));
		
		//P05:A2 1 P06:A1 2 P26:A0 4
		//ADS
		
		for(i = 0; i < 8; i++) {
			switch(i) {
				case 0:
					P05 = 0;
					P06 = 0;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result1=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 1:
					P05 = 1;
					P06 = 0;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result2=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 2:
					P05 = 0;
					P06 = 1;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result3=ADS1110_ReadADValue();
						DelayForAD();
					}
				  break;
				case 3:
					P05 = 1;
					P06 = 1;
					P26 = 0;
					for(j = 0; j < 10; j++){
						result4=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 4:
					P05 = 0;
					P06 = 0;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result5=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 5:
					P05 = 1;
					P06 = 0;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result6=ADS1110_ReadADValue();
						DelayForAD();
					}
					SetNumDisplayHex16(result6);
					break;
				case 6:
					P05 = 0;
					P06 = 1;
					P26 = 1;
					Delay20ms();
					for(j = 0; j < 10; j++){
						result7=ADS1110_ReadADValue();
						DelayForAD();
					}
					break;
				case 7:
					P05 = 1;
					P06 = 1;
					P26 = 1;
					for(j = 0; j < 10; j++){
						result8=ADS1110_ReadADValue();
						DelayForAD();
				  }
					break;
			}
		}
		
		
		/*
		for(i = 0; i < 5; i++) {
			Delay20ms();
		}
		*/
		/*
		P05 = 1;
		P06 = 0;
		P26 = 1;
		Delay20ms();
		result=ADS1110_ReadADValue();
		SetNumDisplayHex16(result);
		*/
		
		sprintf(Data_Buffer,"%02d-%02d-%02d-%02d-%02d-",result1,result2,result3,result4,result5);
		sprintf(Data_Buffer2,"%02d-%02d-%02d-%02d-%02d OK\r\n",result6,result7,result8,(int)minuteDec,(int)secondDec);
		
		
		if(!SD_STATUS&&SD_BUSY) {
			U3SendString(Data_Buffer);
			U3SendString(Data_Buffer2);
		}
		Led2Tog();                                                                                                                                                                                                                                                                                                                                                                                              
	}	
}

void Uart3ISR(void) interrupt 17
{
	if(S3CON&0x02){			//¼ì²é´®¿Ú3·¢ËÍ±êÖ¾Î»
		S3CON&=(~0x02);		//Çå³ı·¢ËÍ±êÖ¾Î»
		uart3SendingFlag=0;
	}else if(S3CON&0x01){	//¼ì²é´®¿Ú3½ÓÊÕ±êÖ¾Î»
		S3CON&=(~0x01);		//Çå³ı½ÓÊÕ±êÖ¾Î»
		uart3Buf=S3BUF;
		uart3ReceivedFlag=1;
	}
}
