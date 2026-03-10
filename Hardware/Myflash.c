#include "stm32f10x.h"
#include "stm32f10x_flash.h"

void MyFlash_EraseFlash(uint16_t start , uint16_t num)
{
    uint16_t i;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    for(i = 0; i < num; i++)
    {
        FLASH_ErasePage(0x08000000 + (start + i) * 1024);
    }
    
    FLASH_Lock();
}

void MyFlash_WriteWord(uint32_t address, uint32_t *data, uint16_t num)
{

    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    while (num)
    {
        FLASH_ProgramWord(address , *data);
        address += 4;
        data++;
        num -= 4;
    }
    FLASH_Lock();
}
