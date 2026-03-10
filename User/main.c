#include "stm32f10x.h"                  // Device header
#include "AT24C02.h"
#include "Usart.h"
#include "main.h"
#include "Boot.h"

OTA_InfoCB OTA_Info;
UpDataA_CB UpDataA;
uint32_t BootStaFlag;

int main(void)
{
	uint8_t i;
	My_I2C_Init();
	MyUSART_Init(115200);
	AT24C02_ReadOTAFlag(); //读取OTA标志位和长度信息
	BootLoader_Branch();   //根据OTA标志位决定是跳转A
	//测试页写
	/* uint8_t data[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22}; // 要写入的数据
	uint8_t writeResult = AT24C02_WritePage(0x00, data, 8); // 写入数据到地址0x00
	Delay_ms(10);
	uint8_t readBuffer[8];
	AT24C02_ReadPage(0x00, readBuffer, 8);
	for(int i=0;i<8;i++)
	{
		U0_Printf("0x%02X ", readBuffer[i]);
	}
	U0_Printf("\r"); */

	//测试单个随机地址写
	// uint8_t data = 0xC0;
	// AT24C02_WriteByte(0x00, data);
	// Delay_ms(10);
	// uint8_t readData = AT24C02_ReadByte(0x00);
	// U0_Printf("Read Data: 0x%02X\r\n", readData);
	while(1){
		/*--------------------------------------------------*/
		/*        UPDATA_A_FLAG置位，表明需要更新A区        */
		/*--------------------------------------------------*/
		if(BootStaFlag && UPDATA_A_FLAG)
		{
			U0_Printf("长度%d字节\r\n",OTA_Info.Firelen[UpDataA.W25Q64_BlockNB]);      //输出长度信息
			if (OTA_Info.Firelen[UpDataA.W25Q64_BlockNB] % 4 == 0)
			{
				MyFlash_EraseFlash(A_SADDR,A_PAGE_NUM);                               //擦除A区
				for(i=0;i<OTA_Info.Firelen[UpDataA.W25Q64_BlockNB]/PAGE_SIZE;i++){    //每次读写一个扇区数据，使用for循环，写入整数个扇区
					W25Q64_ReadData(UpDataA.Updatabuff,i*1024 + UpDataA.W25Q64_BlockNB*64*1024 ,PAGE_SIZE);              //从W25Q64读取一个完整扇区的数据
					MyFlash_WriteWord(FLASH_SADDR + i*PAGE_SIZE,(uint32_t *)UpDataA.Updatabuff,PAGE_SIZE);           //然后写入单片机A区相应的扇区
				}
				if(OTA_Info.Firelen[UpDataA.W25Q64_BlockNB] % 1024 != 0){                  //如果长度不是整数个扇区，说明还有剩余数据需要写入，进入if
					W25Q64_ReadData(UpDataA.Updatabuff,i*1024 + UpDataA.W25Q64_BlockNB*64*1024 ,OTA_Info.Firelen[UpDataA.W25Q64_BlockNB] % 1024);         //从W25Q64读取不足一个完整扇区的数据
					MyFlash_WriteWord(FLASH_SADDR + i*PAGE_SIZE,(uint32_t *)UpDataA.Updatabuff,OTA_Info.Firelen[UpDataA.W25Q64_BlockNB] % 1024);      //然后写入单片机A区相应的扇区
				}
				if(UpDataA.W25Q64_BlockNB == 0){   //如果W25Q64_BlockNB是0，表示是OTA更新A区，进入if
					OTA_Info.OTA_flag = 0;         //设置OTA_flag，只要不是OTA_SET_FLAG定义的值即可
					M24C02_WriteOTAInfo();         //写入24C02内保存
				}
				NVIC_SystemReset();                //重启
			}
			else
			{
				U0_Printf("长度错误\r\n");         //串口输出信息
				BootStaFlag &=~ UPDATA_A_FLAG;     //清除UPDATA_A_FLAG标志位
			}
			
		}
		
	}
}
