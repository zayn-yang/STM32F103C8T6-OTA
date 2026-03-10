#include "AT24C02.h"
#include "main.h"
#include "Delay.h"
#include <string.h>

uint8_t AT24C02_WriteByte(uint8_t MemAddress, uint8_t Data)
{
    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_WRITE);
    if(IIC_Wait_Ack(100) != 0) return 1;
    I2C_WriteByte(MemAddress);
    if(IIC_Wait_Ack(100) != 0) return 2;
    I2C_WriteByte(Data);
    if(IIC_Wait_Ack(100) != 0) return 3;

    I2C_Stop();
    return 0;
   
}

uint8_t AT24C02_ReadByte(uint8_t MemAddress)
{
    uint8_t Data;
    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_WRITE);
    if(IIC_Wait_Ack(100) != 0) return 1;
    I2C_WriteByte(MemAddress);
    if(IIC_Wait_Ack(100) != 0) return 2;
    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_READ);
    if(IIC_Wait_Ack(100) != 0) return 3;
    Data = I2C_ReadByte();

    Delay_ms(10);
	I2C_Send_Ack(1);
    I2C_Stop();
    return Data;
}

uint8_t AT24C02_WritePage(uint8_t MemAddress, uint8_t *pData, uint8_t Length)
{

    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_WRITE);
    if(IIC_Wait_Ack(100) != 0) return 1;
    I2C_WriteByte(MemAddress);
    if(IIC_Wait_Ack(100) != 0) return 2;

    for (uint8_t i = 0; i < Length; i++)
    {
        I2C_WriteByte(pData[i]);
        if(IIC_Wait_Ack(100) != 0) return 3;
    }

    I2C_Stop();
    return 0;
}

uint8_t AT24C02_ReadPage(uint8_t MemAddress, uint8_t *pData, uint8_t Length)
{
    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_WRITE);
    if(IIC_Wait_Ack(100) != 0) return 1;
    I2C_WriteByte(MemAddress);
    if(IIC_Wait_Ack(100) != 0) return 2;

    I2C_Start();
    I2C_WriteByte(AT24C02_ADDRESS_READ);
    if(IIC_Wait_Ack(100) != 0) return 3;

    for (uint8_t i = 0; i < Length; i++)
    {
        pData[i] = I2C_ReadByte();
        if (i != Length - 1)
            I2C_Send_Ack(0); // Acknowledge for all but last byte
        else
            I2C_Send_Ack(1); // No acknowledge for last byte
    }

    I2C_Stop();
    return 0;
}

void AT24C02_ReadOTAFlag(void)
{
    memset(&OTA_Info,0,OTA_INFOCB_SIZE);
    OTA_Info.OTA_flag = AT24C02_ReadPage(0x00, (uint8_t*)&OTA_Info, OTA_INFOCB_SIZE);
}

void M24C02_WriteOTAInfo(void)
{
	uint8_t i;                               
	uint8_t *wptr;                           
	
	wptr = (uint8_t *)&OTA_Info;             
	for(i=0;i<OTA_INFOCB_SIZE/16;i++){       
		AT24C02_WritePage(0x00 + i*16, wptr + i*16, 16);    
		Delay_ms(5);                         
	}		
}