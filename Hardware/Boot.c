#include "stm32f10x.h"
#include "Boot.h"
#include "AT24C02.h"
#include "W25Q64.h"
#include "Usart.h"
#include "main.h"

typedef void (*load_a)(void);


void BootLoader_Branch(void)
{
    if(OTA_Info.OTA_flag == OTA_SET_FLAG)
    {
        U0_Printf("OTA升级\r\n");
        BootStaFlag |= UPDATA_A_FLAG;            //置位标志位，表明需要更新A区
		UpDataA.W25Q64_BlockNB = 0;              //W25Q64_BlockNB等于0，表明是OTA要更新A区,等于其他值，表示是从W25Q64中拿对应位的程序来更新
    }
    else
    {
        U0_Printf("跳转A分区\r\n");
        Load_A(A_SADDR);
    }
}

void MSR_SP(uint32_t addr)
{
    __set_MSP(addr);
}

void Load_A(uint32_t addr)
{
    if (*(uint32_t *)addr >= 0x20000000 && (*(uint32_t *)addr < 0x20004FFF))
    {
        MSR_SP(*(uint32_t*)addr);
        load_a Jump_To_Application = (load_a)(*(uint32_t*)(addr + 4));
        Jump_To_Application();
    }
}

void BootLoader_Clear(void)
{
    USART_DeInit(USART1);
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);

}