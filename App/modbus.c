
#include "modbus.h"

volatile uint8_t receTimeOut = 0;
volatile uint8_t checkoutError = 0; 

uint8_t	  localAddr =1;	

uint8_t   CoilData[800]; 
int16_t   RegData[200];	 

Coil_Dis   Coil;
Regis_Dis  Rdis;

const uint8_t  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

const uint8_t  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

uint8_t disposeFlag=0;

void ReceiveOver(void)
{
		if(receTimeOut>0)
		{
				receTimeOut--;
				if(receTimeOut==0 && receCount>0)//receive a frame, interval 20ms
				{
						receCount = 0;
						checkoutError = 0;
						disposeFlag = 1; //dispose flag
				}
		}
		
		return;
} 

uint16_t crc16(uint8_t *puchMsg, uint16_t usDataLen) 
{ 
		uint8_t  uchCRCHi = 0xFF ;
		uint8_t  uchCRCLo = 0xFF ; 
		uint32_t uIndex = 0;

		while (usDataLen--) 
		{ 
				uIndex = uchCRCHi ^ *puchMsg++ ;
				uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
				uchCRCLo = auchCRCLo[uIndex] ; 
		} 
		
		return (uchCRCHi << 8 | uchCRCLo) ; 
}

void beginSend(uint16_t sendCount)
{
		uint16_t i;

    for(i=0;i<=sendCount;i++)
    {
				USART1_SendData(sendBuf[i]);      	
    }
		receCount = 0;
		checkoutError = 0;
		
		return;
}


int16_t getCoilVal(uint16_t addr, int16_t *tempData)//get a value from cpu;
{
		int16_t result = 0;
		uint16_t tempAddr;
    uint8_t  *p;
    
		tempAddr = addr & 0xfff;
    p=CoilData;
    *tempData = *(p+tempAddr);

		return result;
}

void readCoil(void)/*read mean from MCU to HMI*/
{
    uint8_t  i,k,byteCount,exit=0;
    uint16_t addr,tempAddr,CoilNum,crcData;
		int16_t tempData=0;
		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);/*16bit addr*/
    tempAddr = addr;
    CoilNum = ((uint16_t)(receBuf[4]<<8))|((uint16_t)receBuf[5]);/*coin num*/
    byteCount = (uint8_t)((CoilNum+7)/8);
										
		for(k=0;k<byteCount;k++)
		{
				sendBuf[k+3] = 0; /*INIT*/
				for(i=0;i<8;i++)
				{
						getCoilVal(tempAddr,&tempData);			
						sendBuf[k+3] |= (uint8_t)(tempData << i);
						tempAddr++;
						if(tempAddr >= (addr+CoilNum))
						{
								exit = 1;
								break;
						}	
				}
				if(exit == 1)break;
		}
	
		sendBuf[0] = localAddr;
		sendBuf[1] = 0x01;	
		sendBuf[2] = byteCount;
		byteCount += 3;
		crcData = crc16(sendBuf,byteCount);
		sendBuf[byteCount] = crcData >> 8;
		byteCount++;
		sendBuf[byteCount] = crcData & 0xff;
		beginSend(byteCount);
}


int16_t getRegisterVal(uint16_t addr,int16_t *tempData)//get a value from cpu;
{
		uint16_t tempAddr;
		int16_t  *p,result = 0;
		
		tempAddr = addr & 0xfff;
		p = RegData;
		*tempData=*(p+tempAddr);
		
		return result;
}

void readRegisters(void)/*read mean from MCU to HMI*/
{
		uint16_t readCount,byteCount;
		uint16_t addr,tempAddr,crcData;
		uint16_t i;
		int16_t  tempData = 0;	
	
		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);
		tempAddr = addr;
		readCount = ((uint16_t)(receBuf[4]<<8))|((uint16_t)receBuf[5]);
		byteCount = readCount * 2;
		for(i=0;i<byteCount;i+=2,tempAddr++)
		{
				getRegisterVal(tempAddr,&tempData);				
				sendBuf[i+3] = (uint8_t)(tempData >> 8);				   		
				sendBuf[i+4] = (uint8_t)(tempData & 0xff);			
		}
		
		sendBuf[0] = localAddr;
		sendBuf[1] = 3;
		sendBuf[2] = byteCount;
		byteCount += 3;
		crcData = crc16(sendBuf,byteCount);
		sendBuf[byteCount] = crcData >> 8;
		byteCount++;
		sendBuf[byteCount] = crcData & 0xff;
		beginSend(byteCount);
}


int16_t setCoilVal(uint16_t addr,int16_t tempData)/*set a value to cpu*/ 
{
		int16_t  result = 0;
		uint16_t tempAddr;
		uint8_t  *p;

		p = CoilData;
		tempAddr = addr & 0xfff;
		*(p+tempAddr)=(uint8_t)tempData;
		if(tempAddr<400)
		{
				//IIC_Write_Data(ModbusCoilAdd+tempAddr,(uint8_t)tempData);
		}
		
		return result;
}

void ForceSingleCoil(void)/*from HMI to MCU*/
{
		uint16_t addr,tempAddr;
		int16_t  tempData=0;
		uint16_t onOff;
		uint8_t  i;
		
		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);
		tempAddr = addr;
		onOff = ((uint16_t)(receBuf[4]<<8)) | ((uint16_t)receBuf[5]);
		if(onOff == 0xff00) 
		{
				tempData = 1;
		} 
		else if(onOff == 0x0000)
		{
				tempData = 0;
		}
		Coil.Add  = tempAddr;
		Coil.num  = 1;
		setCoilVal(tempAddr,tempData);
		for(i=0;i<receCount;i++)
		{
				sendBuf[i] = receBuf[i];
		}
		if(receCount>0)receCount--;
		beginSend(receCount);
}


int16_t SetCoilReg(uint16_t addr,int16_t tempData)
{
		int16_t  result = 0,*p;
		uint16_t tempAddr;
	  
    p = RegData;
		tempAddr = addr & 0xfff;
		*(p+tempAddr)=tempData;
		if(tempAddr<100)
		{
				//IIC_Write_2Byte(OperateAdd(ModbusRegAdd,tempAddr),tempData);
		}

		return result;
}

void WritetSingleRegister(void)
{
		uint16_t addr,tempAddr;
		int16_t tempData=0;
		int16_t ReVal;
		uint8_t  i;
		
		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);
		tempAddr = addr;

		ReVal = ((int16_t)(receBuf[4]<<8)) | ((int16_t)receBuf[5]);
		tempData = ReVal;

		Rdis.Add = tempAddr;
		Rdis.num = 1; 

		SetCoilReg(tempAddr,tempData);	
		
		for(i=0;i<receCount;i++)
		{
			sendBuf[i] = receBuf[i];
		}

		if(receCount>0)
		{receCount--;}
		beginSend(receCount);
}

uint16_t SetMultipleCoilVal(uint16_t addr,uint8_t tempData)
{
		uint16_t  result = 0;
		uint16_t  tempAddr;
    uint8_t   *p;

		p = CoilData;
		tempAddr = addr & 0xfff;
		*(p+tempAddr)=tempData;

		if(tempAddr<400)
		{
				//IIC_Write_Data(ModbusCoilAdd+tempAddr,(uint8_t)tempData);
		}
	
		return result;
}

void ForceMultipleCoils(void)
{
		uint16_t addr,tempAddr,crcData;
		uint16_t setCount;
		uint8_t  byteCount,i,j=0,k,w;

		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);
		tempAddr = addr;
		setCount = ((uint16_t)(receBuf[4]<<8))|((uint16_t)receBuf[5]);	
    byteCount = receBuf[6];
		w = byteCount + 7;
    Coil.Add  = tempAddr;
		Coil.num  = setCount;
		for(i=0;i<setCount;i++)
		{				
				k=(receBuf[w]>>j)&0x01;
				SetMultipleCoilVal(tempAddr,k);
				tempAddr++;
				j++;
				if(j>=7)
				{
					j=0;
					w--;
				}
		}

		sendBuf[0] = localAddr;
		sendBuf[1] = 15;
		sendBuf[2] = addr >> 8;
		sendBuf[3] = addr & 0xff;
		sendBuf[4] = setCount >> 8;
		sendBuf[5] = setCount & 0xff;
		crcData = crc16(sendBuf,6);
		sendBuf[6] = crcData >> 8;
		sendBuf[7] = crcData & 0xff;
		beginSend(7);	
}

int16_t setRegisterVal(uint16_t addr,int16_t tempData)
{
		int16_t result = 0,*p;
		uint16_t tempAddr;
	 

		p=RegData;
		tempAddr = addr & 0xfff;

		*(p+tempAddr)=tempData;

		if(tempData<100)
		{
				//IIC_Write_2Byte(ModbusRegAdd+tempAddr,tempData);
		}

		return result;
}

void presetMultipleRegisters(void)
{
		uint16_t addr,tempAddr,crcData;
		int16_t  tempData;
		uint16_t setCount,byteCount;
		uint16_t i;

		addr = ((uint16_t)(receBuf[2]<<8)) | ((uint16_t)receBuf[3]);
		tempAddr = addr;

		setCount = ((uint16_t)(receBuf[4]<<8))|((uint16_t)receBuf[5]);
		
    byteCount = receBuf[6];
    byteCount = byteCount;

		Rdis.Add = tempAddr;
		Rdis.num = setCount;

		for(i=0;i<setCount;i++)
		{
				tempData = (((int16_t)(receBuf[i*2+7]<<8)) | ((int16_t)receBuf[i*2+8]));	
				setRegisterVal(tempAddr,tempData);	
				tempAddr++;		
		}

		sendBuf[0] = localAddr;
		sendBuf[1] = 16;
		sendBuf[2] = addr >> 8;
		sendBuf[3] = addr & 0xff;
		sendBuf[4] = setCount >> 8;
		sendBuf[5] = setCount & 0xff;
		crcData = crc16(sendBuf,6);
		sendBuf[6] = crcData >> 8;
		sendBuf[7] = crcData & 0xff;
		beginSend(7);	
}

void checkComm0Modbus(void)
{
		uint16_t crcData,TcrcD;
		uint16_t tempData;
	
		if(receCount > 4)
		{
				switch(receBuf[1])/*sort*/
				{
						case 1:
						case 3:
						case 5:
						case 6:
						{
								if(receCount >= 8)
								{						
										if(receBuf[0]==localAddr && checkoutError==0)/*data is correct*/
										{
												crcData = crc16(receBuf,6);
												TcrcD = (uint16_t)(receBuf[6]<<8) | (uint16_t)receBuf[7];/*crc check*/
												if(crcData == TcrcD)
												{																			
														if(receBuf[1] == 1)
														{
																readCoil();								
														}
														else if(receBuf[1] == 3)
														{
																readRegisters(); 
														}
														else if(receBuf[1] == 5)
														{
																ForceSingleCoil();								
														}
														else if(receBuf[1] == 6)
														{
																WritetSingleRegister();								
														}
												}
										}						
										
										receCount = 0;/*clear check state*/
										checkoutError = 0;
								}
						}
						break;
						
						case 15:
						{
								tempData = receBuf[6]; 
								tempData += 9;
								if(receCount >= tempData)
								{
										if(receBuf[0]==localAddr && checkoutError==0)
										{
												crcData = crc16(receBuf,tempData-2);
												if(crcData == (receBuf[tempData-2]<<8)+ receBuf[tempData-1])
												{
													ForceMultipleCoils();			
												}
										}	
										receCount = 0;
										checkoutError = 0;
								}
						}
						break;
						
						case 16:
						{
								tempData = ((uint16_t)receBuf[4]<<8) | (uint16_t)receBuf[5];
								tempData = tempData * 2;
								tempData += 9;
								if(receCount >= tempData)
								{
										if(receBuf[0]==localAddr && checkoutError==0)
										{
												crcData = crc16(receBuf,tempData-2);
												if( crcData == (((uint16_t)receBuf[tempData-2]<<8) | (uint16_t)receBuf[tempData-1]) )
												{
														presetMultipleRegisters();			
												}
										}	
										receCount = 0;
										checkoutError = 0;
								}
						}		
						break;
						
						default:
								break;			
				}
		}
}




