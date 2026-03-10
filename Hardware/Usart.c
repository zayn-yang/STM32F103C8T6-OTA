#include "Usart.h"



UCB_CB  U0CB;
uint8_t U0_RxBuff[U0_RX_SIZE];
uint8_t U0_TxBuff[U0_TX_SIZE];

void MyUSART_Init(uint32_t baudrate)
{
    // Initialize GPIO pins for USART
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Initialize USART hardware
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);

    U0Rx_PtrInit();
    MyDMA_Init((uint32_t)U0_RxBuff, U0_RX_MAX);
    
}

void MyDMA_Init(uint32_t RxBuffAddr, uint16_t Size)
{
    // Initialize DMA for USART reception
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffAddr; 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = Size + 1; 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel5, &DMA_InitStructure); 
    DMA_Cmd(DMA1_Channel5, ENABLE);
}

void U0Rx_PtrInit(void)
{
    U0CB.URxCounter = 0;
    U0CB.URxDataIN = &U0CB.URxDataPtr[0];
    U0CB.URxDataOUT = &U0CB.URxDataPtr[0];
    U0CB.URxDataEND = &U0CB.URxDataPtr[NUM - 1];
    U0CB.URxDataIN->start = U0_RxBuff;
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        volatile uint32_t temp;
        temp = USART1->SR;
        temp = USART1->DR;
        (void)temp; // Prevent compiler optimization

        U0CB.URxCounter += U0_RX_MAX + 1 - DMA_GetCurrDataCounter(DMA1_Channel5);
        U0CB.URxDataIN->end = &U0_RxBuff[U0CB.URxCounter - 1];
        U0CB.URxDataIN++;
        if (U0CB.URxDataIN == U0CB.URxDataEND)
        {
            U0CB.URxDataIN = &U0CB.URxDataPtr[0];
            U0CB.URxDataIN->start = U0_RxBuff;
        }
        if(U0_RX_SIZE - U0CB.URxCounter >= U0_RX_MAX){
			U0CB.URxDataIN->start = &U0_RxBuff[U0CB.URxCounter];
		}else{
			U0CB.URxDataIN->start = U0_RxBuff;
			U0CB.URxCounter = 0;
		}
        DMA_Cmd(DMA1_Channel5, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel5, U0_RX_MAX + 1);
        DMA1_Channel5->CMAR = (uint32_t)U0CB.URxDataIN->start;
        DMA_Cmd(DMA1_Channel5, ENABLE);
    }
}

void U0_Printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf((char *)U0_TxBuff, U0_TX_SIZE, fmt, args);
    va_end(args);

    for (int i = 0; i < len; i++)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, U0_TxBuff[i]);
    }
    while (USART_GetFlagStatus(USART1,USART_FLAG_TC) != 1);
   
    
}
