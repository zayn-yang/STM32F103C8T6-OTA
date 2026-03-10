#ifndef MYFLASH_H
#define MYFLASH_H

void MyFlash_EraseFlash(uint16_t start , uint16_t num);
void MyFlash_WriteWord(uint32_t address, uint32_t *data, uint16_t num);


#endif