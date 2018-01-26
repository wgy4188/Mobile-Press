#ifndef _IIC_H_
#define _IIC_H_

#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

#define   WriteDeviceIIC    				0xA0	   //Ð´ 1010 000 0
#define   ReadDeviceIIC    					0xA1	   //¶Á 1010 000 1

#define   GPIO_Pin_SDA 							GPIO_Pin_7
#define   GPIO_Pin_SCL							GPIO_Pin_6

#define   Set_IIC_SCL  							{GPIO_SetBits(GPIOB,GPIO_Pin_6);}
#define   Clr_IIC_SCL  							{GPIO_ResetBits(GPIOB,GPIO_Pin_6);} 

#define 	Set_IIC_SDA  							{GPIO_SetBits(GPIOB,GPIO_Pin_SDA);}
#define 	Clr_IIC_SDA  							{GPIO_ResetBits(GPIOB,GPIO_Pin_SDA);} 

#define 	READ_SDA     							GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_SDA)

#define   EE_TYPE    0
#define   AT24C02    1

void IIC_Configuration(void);
void IIC_WRITE_BYTE(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);
void IIC_Write_2Byte(unsigned int RomAddress, int udata);
int IIC_Read_2Byte(unsigned int RomAddress);
void IIC_Write_4Byte(unsigned int RomAddress,unsigned long udata);
unsigned long IIC_Read_4Byte(unsigned int RomAddress);
void IIC_Write_double(unsigned int RomAddress,double udata);
double IIC_Read_double(unsigned int RomAddress);

void IIC_Write_Data(unsigned int RomAddress,unsigned char data);
unsigned char IIC_Read_Data(unsigned int RomAddress);
void IIC_Write_Nbyte(unsigned char *pc,unsigned int Addr,unsigned char number);
void IIC_Read_Nbyte(unsigned char *pc,unsigned int Addr,unsigned char number); 

#endif

