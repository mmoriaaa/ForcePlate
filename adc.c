/************************************************************************************
*
* 功能说明：ADC的操作函数。
* 作者信息：品诺电子(http://free-design.taobao.com)
*
************************************************************************************/
#include "stc15f2k60s2.h"
#include "my_type.h"
#include "adc.h"

/*Define ADC operation const for ADC_CONTR*/ 
#define ADC_POWER   0x80            //ADC power control bit 模数转换电源控制位 
#define ADC_FLAG    0x10            //ADC complete flag 	模数转换结束标志位 
#define ADC_START   0x08            //ADC start control bit 模数转换启动控制位 

//转换速度控制位SPEED0和SPEED1，共四种状态，对应四种转换速度 
#define ADC_SPEEDLL 0x00            //540 个时钟  
#define ADC_SPEEDL  0x20            //360 个时钟 
#define ADC_SPEEDH  0x40            //180 个时钟 
#define ADC_SPEEDHH 0x60            //90 个时钟  

volatile u16 adcData=0;
volatile u8 adcUpdatedFlag=0;

/********************************************************************
函数功能：延时函数。
入口参数：无。
返    回：无。
备    注：无。
********************************************************************/
void Delay500us(void)   //误差 -0.01439525463us
{
    unsigned char a,b,n;
    for(b=11;b>0;b--)
        for(a=250;a>0;a-- );
    for(n=1;n>0;n--);
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：初始化ADC。		  
入口参数：ch:选择要初始化的ADC通道。
返    回：无。
备    注：无。 
********************************************************************/
void InitADC(u8 ch)
{
	EADC=0;				//关闭ADC中断
	P1ASF=0xC1;		//设置P1.0,P1,6,P1.7为ADC引脚，P1ASF为只写寄存器，读取无效
	ADC_RES=0;		//清除结果寄存器
	ADC_CONTR=ADC_POWER|ADC_SPEEDLL|ADC_START|ch;
	Delay500us();		//等待稳定
	EADC=1;				//允许ADC中断
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：ADC的中断服务程序。
入口参数：无。
返    回：无。
备    注：注意手动清除标志位。
********************************************************************/
void AdcISR (void) interrupt 5
{
	static u32 accumulator = 0; 	//AD转换结果累加和
	static u16 measurements = 0;	//AD转换次数计数
	
	ADC_CONTR&=(~ADC_FLAG);					//清除ADC转换完成标志位
	//accumulator += ((ADC_RES<<2)+ADC_RESL);		//AD转换结果累加
	accumulator += ADC_RES;
	measurements++;
	
	if(measurements == 10){  	//1000次AD转换完成后取平均值
		measurements = 0;
		adcData =  accumulator/10;	//求平均值写入adcData
		adcUpdatedFlag=1;						//设置ADC结果更新标志位
		accumulator=0;
	}else{
		ADC_CONTR|=ADC_START;						//开启下一次AD转换
	}
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：查询adcUpdatedFlag标志位。
入口参数：无。
返    回：u8:标志位有效，则返回1，否则返回0。
备    注：无。
********************************************************************/
u8 GetAdcUpdatedFlag(void)
{
	if(adcUpdatedFlag){
		adcUpdatedFlag=0;//清除标志位
		return 1;
	}else{
		return 0;
	}
}
/**///////////////////////Pino Electronics////////////////////////**/

/********************************************************************
函数功能：查询adcData。
入口参数：无。
返    回：adcData。
备    注：无。
********************************************************************/
u16 GetAdcData(void)
{
		return adcData;
}
/**///////////////////////Pino Electronics////////////////////////**/
