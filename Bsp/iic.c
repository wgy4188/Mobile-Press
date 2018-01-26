
#include <iic.h>

void IIC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //时钟使能
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //SCL			
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB  

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA;   //SDA			
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 						  
}

void SDA_IN(void)  
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //定义变量
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA;   //SDA			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
}

void SDA_OUT(void) 
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //定义变量
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA;   //SDA			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
}

void IIC_Start(void)
{
	 SDA_OUT();     //sda线输出
	 Set_IIC_SDA;      
	 Set_IIC_SCL;
	 delay_Nus(5);
	 Clr_IIC_SDA; //START:when CLK is high,DATA change form high to low 
	 delay_Nus(5);
	 Clr_IIC_SCL; //钳住I2C总线，准备发送或接收数据 
} 

void IIC_Stop(void)
{
	 SDA_OUT();//sda线输出

	 Clr_IIC_SDA;//STOP:when CLK is high DATA change form low to high
	 Set_IIC_SCL;	 
	 delay_Nus(5);
	 Set_IIC_SDA;//发送I2C总线结束信号
	 delay_Nus(5);  
	 Set_IIC_SCL; 	          
}

 unsigned char IIC_Recelve_Ack(void)
{
	 unsigned int ucErrTime=0;
	
	 SDA_IN();      //SDA设置为输入  
	 Set_IIC_SDA;
	 Clr_IIC_SCL;
	 delay_Nus(5);    
	 Set_IIC_SCL;
	 delay_Nus(5);  
	 while(READ_SDA)  //6T=1us
	 {
	 	ucErrTime++;
	  	if(ucErrTime>=72000)   //600/6=100uS
	  	{
	   		Clr_IIC_SCL;//时钟输出0
	   		return 1;
	  	}
	 }
	 Clr_IIC_SCL;//时钟输出0     
	 return 0;  
} 

void IIC_Ack(void)
{
	 Clr_IIC_SCL;
	 SDA_OUT();
	 Clr_IIC_SDA;
	 delay_Nus(5);
	 Set_IIC_SCL;
	 delay_Nus(5);
	 Clr_IIC_SCL;
}

void IIC_Nack(void)
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
    Clr_IIC_SCL;//拉低时钟开始数据传输
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

	SDA_IN();//SDA设置为输入
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
	{IIC_Nack();}//发送NaCK    	
    else
    {IIC_Ack();} //发送ACK       
	return receive;
}


void IIC_Write_Data(unsigned int RomAddress,unsigned char data)
{
	unsigned char flag=0;
	
	do
	{
		IIC_Start();
		IIC_WRITE_BYTE(WriteDeviceIIC);	 //写操作   
		if(IIC_Recelve_Ack()==0)  		//判断ACK
		{
	      IIC_WRITE_BYTE((unsigned char)(RomAddress>>8));	  //写H地址
		  if(IIC_Recelve_Ack()==0)  //判断ACK
	  	  {
			IIC_WRITE_BYTE((unsigned char)RomAddress);	  //写L地址
			if(IIC_Recelve_Ack()==0)  //判断ACK
			{
				IIC_WRITE_BYTE(data);
			    if(IIC_Recelve_Ack()==0)  //判断ACK
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
	}while(flag==1);
	IIC_Stop();      //停止
//	delay_Nms(1);
//	OSTimeDlyHMSM(0, 0, 0, 10);	
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
		IIC_WRITE_BYTE(WriteDeviceIIC);	 //写操作 
		if(IIC_Recelve_Ack()==0)  		//判断ACK
		{
			IIC_WRITE_BYTE((unsigned char)(RomAddress>>8));		//写H地址
			if(IIC_Recelve_Ack()==0)  //判断ACK
			{
				IIC_WRITE_BYTE((unsigned char)RomAddress);		//写L地址
				if(IIC_Recelve_Ack()==0)  //判断ACK
				{
					IIC_Start();
					IIC_WRITE_BYTE(ReadDeviceIIC);   //读操作
					if(IIC_Recelve_Ack()==0)  //判断ACK
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
	}while(flag==1);
	IIC_Stop();   //停止
	return data;
}

void IIC_Write_2Byte(unsigned int RomAddress, int udata)
{
	unsigned char i;
		
	for(i=0;i<=8;i+=8)
	{
	  IIC_Write_Data(RomAddress,((unsigned char)(udata>>i))); 
	  RomAddress++; 
	}	  
}


int IIC_Read_2Byte(unsigned int RomAddress)
{
	unsigned char BufData,i;
	unsigned int data=0;
		
	for(i=0;i<=8;i+=8)
	{
	  BufData=IIC_Read_Data(RomAddress); 
	  RomAddress++; 
	  data|=((int)BufData)<<i;
	}
	
	return data;
}

void IIC_Write_4Byte(unsigned int RomAddress,unsigned long udata)
{
	unsigned char i;
		
	for(i=0;i<=24;i+=8)
	{
	  IIC_Write_Data(RomAddress,((unsigned char)(udata>>i))); 
	  RomAddress++; 
	}  
}

unsigned long IIC_Read_4Byte(unsigned int RomAddress)
{
	unsigned char BufData,i;
	unsigned long data=0;
		
	for(i=0;i<=24;i+=8)
	{
	  BufData=IIC_Read_Data(RomAddress); 
	  RomAddress++; 
	  data|=((unsigned long)BufData)<<i;
	}
	return data;
}

void IIC_Write_double(unsigned int RomAddress,double udata)
{
	unsigned char i;
	void *p;

	p=&udata;	
	for(i=0;i<sizeof(double);i++)
	{IIC_Write_Data(RomAddress+i,*((char *)p+i));}
  
}

double IIC_Read_double(unsigned int RomAddress)
{
	

	unsigned char i;
	double data=0.0;
	void *p;

	p=&data;
	for(i=0;i<sizeof(double);i++)
	{*((char *)p+i)=IIC_Read_Data(RomAddress+i);}	
	return data;
}
