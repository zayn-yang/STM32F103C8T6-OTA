#ifndef MYI2C_H
#define MYI2C_H
#include "stm32f10x.h"
#include "Delay.h"

#define I2C_SPEED       100000
#define I2C_OWN_ADDRESS 0x30

#define I2C_SCL      GPIO_Pin_10
#define I2C_SDA      GPIO_Pin_11
#define I2C_GPIO_PORT GPIOB
#define I2C_GPIO_CLK  RCC_APB2Periph_GPIOB


void My_I2C_Init(void);
void My_I2C_W_SCL(uint8_t BitValue);
void My_I2C_W_SDA(uint8_t BitValue);
uint8_t My_I2C_R_SDA(void);

void I2C_Start(void);
void I2C_Stop(void);
void I2C_WriteByte(uint8_t Byte);
uint8_t I2C_ReadByte(void);
uint8_t I2C_Recieve_Ack(void);
void I2C_Send_Ack(uint8_t AckBit);
uint8_t IIC_Wait_Ack(int16_t timeout);

#endif // MYI2C_H