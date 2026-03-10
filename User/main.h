#ifndef MAIN_H
#define MAIN_H

#define  FLASH_SADDR   0x08000000                                  //flash起始地址           
#define  PAGE_SIZE     1024                                        //falsh页大小          
#define  PAGE_NUM      64                                          //falsh页数           
#define  B_PAGE_NUM    20                                          //B区页数          
#define  A_PAGE_NUM    PAGE_NUM - B_PAGE_NUM                       //A区页数
#define  A_START_PAGE  B_PAGE_NUM                                  //A区起始页数     
#define  A_SADDR       FLASH_SADDR + A_START_PAGE * PAGE_SIZE      //A区起始地址

#define  UPDATA_A_FLAG      0x00000001        //

#define OTA_SET_FLAG        0xAABB1122        //OTA升级标志
typedef struct{
	uint32_t OTA_flag;                        //
	uint32_t Firelen[11];                     //
}OTA_InfoCB;                                  //
#define OTA_INFOCB_SIZE sizeof(OTA_InfoCB)    //结构体大小
	
typedef struct{
	uint8_t  Updatabuff[PAGE_SIZE];      //
	uint32_t W25Q64_BlockNB;                  //
}UpDataA_CB;                                  //

extern OTA_InfoCB OTA_Info;   //
extern UpDataA_CB UpDataA;    //
extern uint32_t BootStaFlag;  //
#endif // !MAIN_H
