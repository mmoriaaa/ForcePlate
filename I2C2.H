
#ifndef __IIC_H__
#define __IIC_H__


extern void IIC_Init(void)  ;  
extern void IIC_Start(void);  
extern void IIC_Stop(void);  
extern void IIC_SendAck(unsigned char);  
extern unsigned char IIC_RecAck(void);  
extern void IIC_WriteByte(unsigned char);  
extern unsigned char IIC_ReadByte();  
extern unsigned char write_add(unsigned char,unsigned char *,unsigned char);  
extern unsigned char read_add(unsigned char,unsigned char *,unsigned char);  
            
#endif
