#include "MyI2C.h"

void My_I2C_Init(void)
{
        // Enable clocks for I2C and GPIO
    RCC_APB2PeriphClockCmd(I2C_GPIO_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    // Configure I2C SCL and SDA pins
    GPIO_InitStructure.GPIO_Pin = I2C_SCL | I2C_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);	
}

void My_I2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(I2C_GPIO_PORT, I2C_SCL, (BitAction)BitValue);
    Delay_us(10);
}

void My_I2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(I2C_GPIO_PORT, I2C_SDA, (BitAction)BitValue);
    Delay_us(10);
}

uint8_t My_I2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA);
    Delay_us(10);
    return BitValue;
    
}

void I2C_Start(void)
{
    My_I2C_W_SCL(Bit_SET);
    My_I2C_W_SDA(Bit_SET);
    My_I2C_W_SDA(Bit_RESET);
    My_I2C_W_SCL(Bit_RESET);
}
void I2C_Stop(void)
{
    My_I2C_W_SDA(Bit_RESET);
    My_I2C_W_SCL(Bit_SET);
    My_I2C_W_SDA(Bit_SET);
}

void I2C_WriteByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        My_I2C_W_SDA((Byte & (0x80 >> i)));
        
        My_I2C_W_SCL(Bit_SET);
      
        My_I2C_W_SCL(Bit_RESET);
        
    }

}

uint8_t I2C_ReadByte(void)
{
    uint8_t i;
    uint8_t Byte = 0x00;
    My_I2C_W_SDA(Bit_SET);              // Release SDA for reading
    for (i = 0; i < 8; i++)
    {
        My_I2C_W_SCL(Bit_SET);
        if (My_I2C_R_SDA())
            Byte |= (0x80 >> i);
        My_I2C_W_SCL(Bit_RESET);
    }
    return Byte;
}

void I2C_Send_Ack(uint8_t AckBit)
{
    My_I2C_W_SDA(AckBit);
    My_I2C_W_SCL(Bit_SET);
    My_I2C_W_SCL(Bit_RESET);
}

uint8_t I2C_Recieve_Ack(void)
{
    uint8_t AckBit;
    My_I2C_W_SDA(Bit_SET);              // Release SDA for reading
    My_I2C_W_SCL(Bit_SET);
    AckBit = My_I2C_R_SDA();
    My_I2C_W_SCL(Bit_RESET);
    return AckBit;
}

uint8_t IIC_Wait_Ack(int16_t timeout){
	do{
		timeout--;
		Delay_us(2);
	}while((My_I2C_R_SDA())&&(timeout>=0));
	if(timeout<0) return 1;
	My_I2C_W_SCL(Bit_SET);
	Delay_us(2);
	if(My_I2C_R_SDA()!=0) return 2;
	My_I2C_W_SCL(Bit_RESET);
	Delay_us(2);
	return 0;
}