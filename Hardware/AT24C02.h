#ifndef AT24C02_H
#define AT24C02_H
#include "MyI2C.h"


#define AT24C02_ADDRESS_WRITE  0xA0
#define AT24C02_ADDRESS_READ   0xA1


uint8_t AT24C02_WriteByte(uint8_t MemAddress, uint8_t Data);
uint8_t AT24C02_ReadByte(uint8_t MemAddress);

uint8_t AT24C02_WritePage(uint8_t MemAddress, uint8_t* pData, uint8_t Length);
uint8_t AT24C02_ReadPage(uint8_t MemAddress, uint8_t* pData, uint8_t Length);

void AT24C02_ReadOTAFlag(void);
void M24C02_WriteOTAInfo(void);
#endif // AT24C02_H