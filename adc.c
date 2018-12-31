/************************************************************************************
*
* ����˵����ADC�Ĳ���������
* ������Ϣ��Ʒŵ����(http://free-design.taobao.com)
*
************************************************************************************/
#include "stc15f2k60s2.h"
#include "my_type.h"
#include "adc.h"

/*Define ADC operation const for ADC_CONTR*/ 
#define ADC_POWER   0x80            //ADC power control bit ģ��ת����Դ����λ 
#define ADC_FLAG    0x10            //ADC complete flag 	ģ��ת��������־λ 
#define ADC_START   0x08            //ADC start control bit ģ��ת����������λ 

//ת���ٶȿ���λSPEED0��SPEED1��������״̬����Ӧ����ת���ٶ� 
#define ADC_SPEEDLL 0x00            //540 ��ʱ��  
#define ADC_SPEEDL  0x20            //360 ��ʱ�� 
#define ADC_SPEEDH  0x40            //180 ��ʱ�� 
#define ADC_SPEEDHH 0x60            //90 ��ʱ��  

volatile u16 adcData=0;
volatile u8 adcUpdatedFlag=0;

/********************************************************************
�������ܣ���ʱ������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Delay500us(void)   //��� -0.01439525463us
{
    unsigned char a,b,n;
    for(b=11;b>0;b--)
        for(a=250;a>0;a-- );
    for(n=1;n>0;n--);
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ���ʼ��ADC��		  
��ڲ�����ch:ѡ��Ҫ��ʼ����ADCͨ����
��    �أ��ޡ�
��    ע���ޡ� 
********************************************************************/
void InitADC(u8 ch)
{
	EADC=0;				//�ر�ADC�ж�
	P1ASF=0xC1;		//����P1.0,P1,6,P1.7ΪADC���ţ�P1ASFΪֻд�Ĵ�������ȡ��Ч
	ADC_RES=0;		//�������Ĵ���
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ADC_START|ch;
	Delay500us();		//�ȴ��ȶ�
	EADC=1;				//����ADC�ж�
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ�ADC���жϷ������
��ڲ������ޡ�
��    �أ��ޡ�
��    ע��ע���ֶ������־λ��
********************************************************************/
void AdcISR (void) interrupt 5
{
	static u32 accumulator = 0; 	//ADת������ۼӺ�
	static u16 measurements = 0;	//ADת����������
	
	ADC_CONTR&=(~ADC_FLAG);					//���ADCת����ɱ�־λ
	//accumulator += ((ADC_RES<<2)+ADC_RESL);		//ADת������ۼ�
	accumulator += ADC_RES;
	measurements++;
	
	if(measurements == 10){  	//1000��ADת����ɺ�ȡƽ��ֵ
		measurements = 0;
		adcData =  accumulator/10;	//��ƽ��ֵд��adcData
		adcUpdatedFlag=1;						//����ADC������±�־λ
		accumulator=0;
	}else{
		ADC_CONTR|=ADC_START;						//������һ��ADת��
	}
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ���ѯadcUpdatedFlag��־λ��
��ڲ������ޡ�
��    �أ�u8:��־λ��Ч���򷵻�1�����򷵻�0��
��    ע���ޡ�
********************************************************************/
u8 GetAdcUpdatedFlag(void)
{
	if(adcUpdatedFlag){
		adcUpdatedFlag=0;//�����־λ
		return 1;
	}else{
		return 0;
	}
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
�������ܣ���ѯadcData��
��ڲ������ޡ�
��    �أ�adcData��
��    ע���ޡ�
********************************************************************/
u16 GetAdcData(void)
{
		return adcData;
}
/**///////////////////////Pino Electronics////////////////////////**/
