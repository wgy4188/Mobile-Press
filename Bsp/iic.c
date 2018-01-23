#include "iic.h"

void IIC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //�������

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //SCL			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;   //SDA			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
}

void SDA_IN(void)  
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //�������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA;   //SDA			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
}

static void SDA_OUT(void) 
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //�������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA;   //SDA			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
}

static void IIC_Start(void)
{
	 SDA_OUT();     //sda�����
	 Set_IIC_SDA;      
	 Set_IIC_SCL;
	 delay_Nus(5);
	 Clr_IIC_SDA; //START:when CLK is high,DATA change form high to low 
	 delay_Nus(5);
	 Clr_IIC_SCL; //ǯסI2C���ߣ�׼�����ͻ�������� 
} 


static void IIC_Stop(void)
{
	 SDA_OUT();//sda�����

	 Clr_IIC_SDA;//STOP:when CLK is high DATA change form low to high
	 Set_IIC_SCL;	 
	 delay_Nus(5);
	 Set_IIC_SDA;//����I2C���߽����ź�
	 delay_Nus(5);  
	 Set_IIC_SCL; 	          
}


static uint8_t IIC_Receive_Ack(void)
{
	 uint8_t ucErrTime=0;
	
	 SDA_IN();      //SDA����Ϊ����  
	 Set_IIC_SDA;
	 Clr_IIC_SCL;
	 delay_Nus(5);    
	 Set_IIC_SCL;
	 delay_Nus(5);  
	 while(READ_SDA)  //6T=1us
	 {
			ucErrTime++;
	  	if(ucErrTime>=250)   //600/6=100uS
	  	{
	   		Clr_IIC_SCL;//ʱ�����0
	   		return 1;
	  	}
	 }
	 Clr_IIC_SCL;//ʱ�����0     
	 return 0;  
} 

static void IIC_Ack(void)
{
	 Clr_IIC_SCL;
	 SDA_OUT();
	 Clr_IIC_SDA;
	 delay_Nus(5);
	 Set_IIC_SCL;
	 delay_Nus(5);
	 Clr_IIC_SCL;
}

static void IIC_Nack(void)
{
	 Clr_IIC_SCL;
	 SDA_OUT();
	 Set_IIC_SDA;
	 delay_Nus(5);
	 Set_IIC_SCL;
	 delay_Nus(5);
	 Clr_IIC_SCL;
}

void IIC_WRITE_BYTE(unsigned char txd)
{
    unsigned char  t;  
	 
    SDA_OUT();      
    Clr_IIC_SCL;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              	        
	  if(((txd&0x80)>>7)==1)  
	  {Set_IIC_SDA;}
	  else 
	  {Clr_IIC_SDA;}
	  txd<<=1;    
	  delay_Nus(5);   
	  Set_IIC_SCL;
	  delay_Nus(5); 
	  Clr_IIC_SCL; 
	  delay_Nus(5);
    }  
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;

	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
	  Clr_IIC_SCL; 
	  delay_Nus(5);
	  Set_IIC_SCL;
	  delay_Nus(5);
	  receive<<=1;
	  if(READ_SDA==1)
	  {receive|=0x01;}   
	  delay_Nus(5); 
	}  
  if (!ack)
	{IIC_Nack();}//����NaCK    	
    else
    {IIC_Ack();} //����ACK       
	return receive;
}


void IIC_Write_Data(unsigned int RomAddress,unsigned char data)
{
	unsigned char flag=0;	
	
	do
	{
		IIC_Start();		
		if(EE_TYPE==AT24C02)
		{
			IIC_WRITE_BYTE(0XA0+((RomAddress/256)<<1));
		  if(IIC_Receive_Ack()==0)  //�ж�ACK
	  	{
			IIC_WRITE_BYTE((unsigned char)RomAddress);	  //дL��ַ
			if(IIC_Receive_Ack()==0)  //�ж�ACK
			{
				IIC_WRITE_BYTE(data);
			    if(IIC_Receive_Ack()==0)  //�ж�ACK
			    {flag=0;}
			    else
			    {flag=1;}
			}
			else
			{flag=1;}			
		  }	
		  else
		  {flag=1;}	
    }
    else
		{
		 IIC_WRITE_BYTE(WriteDeviceIIC);	 //д����   
		 if(IIC_Receive_Ack()==0)  		//�ж�ACK
		  {
	     IIC_WRITE_BYTE((unsigned char)(RomAddress>>8));	  //дH��ַ
		   if(IIC_Receive_Ack()==0)  //�ж�ACK
	  	  {
			    IIC_WRITE_BYTE((unsigned char)RomAddress);	  //дL��ַ
			    if(IIC_Receive_Ack()==0)  //�ж�ACK
			      {
				     IIC_WRITE_BYTE(data);
			       if(IIC_Receive_Ack()==0)  //�ж�ACK
			        {flag=0;}
			       else
			       {flag=1;}
			      }
			    else
			     {flag=1;}			
		    }	
		   else
		   {flag=1;}
		  }
		  else
		  {flag=1;}
		}			
	}while(flag==1);
	
	IIC_Stop();      //ֹͣ
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);
}

unsigned char IIC_Read_Data(unsigned int RomAddress)
{
	unsigned char flag,data=0;

	do
	{
		IIC_Start();		
		if(EE_TYPE==AT24C02)
		{		
		  IIC_WRITE_BYTE(0XA0+((RomAddress/256)<<1));	 //д���� 		
		  if(IIC_Receive_Ack()==0)  //�ж�ACK
			 {
				IIC_WRITE_BYTE((unsigned char)RomAddress);		//дL��ַ
				if(IIC_Receive_Ack()==0)  //�ж�ACK
				{
					IIC_Start();
					IIC_WRITE_BYTE(ReadDeviceIIC);   //������
					if(IIC_Receive_Ack()==0)  //�ж�ACK
					{
						data=IIC_Read_Byte(0);
						flag=0;				
					}
					else
					{flag=1;}  
				}
				else
				{flag=1;} 	
			 }
			else
			{flag=1;}
		}	
    else
		{
		 	IIC_WRITE_BYTE(WriteDeviceIIC);	 //д���� 
	  	if(IIC_Receive_Ack()==0)  		//�ж�ACK
		   {
			  IIC_WRITE_BYTE((unsigned char)(RomAddress>>8));		//дH��ַ
			  if(IIC_Receive_Ack()==0)  //�ж�ACK
			   {
				  IIC_WRITE_BYTE((unsigned char)RomAddress);		//дL��ַ
				  if(IIC_Receive_Ack()==0)  //�ж�ACK
				   {
					  IIC_Start();
					  IIC_WRITE_BYTE(ReadDeviceIIC);   //������
					  if(IIC_Receive_Ack()==0)  //�ж�ACK
					    {
						   data=IIC_Read_Byte(0);
						   flag=0;				
					    }
					  else
					   {flag=1;}  
				   }
				 else
				  {flag=1;} 	
			  }
			  else
			  {flag=1;}
		   }
		  else
	     {flag=1;}	
		 }			
	}while(flag==1);
	IIC_Stop();   //ֹͣ
	return data;
}

void IIC_Write_Nbyte(unsigned char *pc,unsigned int Addr,unsigned char number) 
{
	unsigned char i;		
    
	for(i=0;i<=number;i++)
	{
		IIC_Write_Data(Addr,pc[i]);
		Addr++;
	}
}

void IIC_Read_Nbyte(unsigned char *pc,unsigned int Addr,unsigned char number) 
{
	unsigned char i;
	
	for(i=0;i<number;i++)
	{
		pc[i]=IIC_Read_Data(Addr);
		Addr++;
	}		
}


