#ifndef USART_H
#define USART_H
#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define U0_RX_SIZE 2048
#define U0_TX_SIZE 2048
#define U0_RX_MAX  256
#define NUM        10

typedef struct{
	uint8_t *start;
	uint8_t *end;
}UCB_URxBuffptr;

typedef struct{
	uint16_t URxCounter;
	UCB_URxBuffptr URxDataPtr[NUM];
	UCB_URxBuffptr *URxDataIN;
	UCB_URxBuffptr *URxDataOUT;
	UCB_URxBuffptr *URxDataEND;
}UCB_CB;

void MyUSART_Init(uint32_t baudrate);
void MyDMA_Init(uint32_t RxBuffAddr, uint16_t Size);
void U0Rx_PtrInit(void);
void USART1_IRQHandler(void);
void U0_Printf(const char *fmt, ...);

extern UCB_CB  U0CB;
extern uint8_t U0_RxBuff[U0_RX_SIZE];
extern uint8_t U0_TxBuff[U0_TX_SIZE];

#endif // USART_H