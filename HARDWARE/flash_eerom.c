#include "pbdata.h"
u32 GROUP_ADD[]={STORE_M1,STORE_M2,STORE_M3,STORE_M4,STORE_M5};
//void Store_set_flash(void)
//{
////	u16 i;
////	u32 addr;
//	Jk516save_TypeDef *ptt;
//    ptt=&Jk516save;
//    SPI_FLASH_SectorErase(0);
//    delay_ms(20);
//    SPI_FLASH_BufferWrite((u8 *)ptt, 0, sizeof(Jk516save));
//}

//void Store_cal_flash(void)
//{
////	u16 i;
////	u32 addr;
//	Jk516cal_TypeDef *ptt;
//    ptt=&Jk516cal;
//    SPI_FLASH_SectorErase(1);
//    delay_ms(20);
//    SPI_FLASH_BufferWrite((u8 *)ptt, 4096*1, sizeof(Jk516cal));
//}
//void Read_compvalue(u8 data)
//{
//	u32 addr;
//	u16 i;
//	System_TypeDef *pt;
//	pt= &System;
//	Parameter_valuecomp();
//	addr=GROUP_ADD[data];
//	for(i=0;i<sizeof(SaveData);i++)
//	{
//		*((u8*)pt+i)=*(((u8*)(addr))+i);//FMC_Read(addr+i);
//			
//	}

//}
//void Read_set_flash(void)
//{
////	u16 i;
//	u32 addr;
//	Jk516save_TypeDef *ptt;
//	ptt=&Jk516save;
//    addr=0;

//    SPI_FLASH_BufferRead((u8*)ptt, addr, sizeof(Jk516save));
//	//
//	

//}

//void Read_cal_flash(void)
//{
////	u16 i;
//	u32 addr;
//	Jk516cal_TypeDef *ptt;
//    ptt=&Jk516cal;
//    addr=4096;

//    SPI_FLASH_BufferRead((u8*)ptt, addr, sizeof(Jk516save));
//	//
//	

//}