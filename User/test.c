#include "pbdata.h"
#include <math.h>
#include <stdlib.h>
#include "ff.h"
#include "cpld.h"
//#include "./APP/usbh_bsp.h"
#include "./RTC/bsp_rtc.h"
#include "bsp_exti.h"
//#include "touchscreen.h"
#include "./touch/touch.h"
#include "./ch376/ch376.h"
#include "FILESYS.h"
#include "./beep/bsp_beep.h"
#include "open.h"
#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"
#include "flash_if.h"
#include "./internalFlash/bsp_internalFlash.h"
#include "sdio/bsp_sdio_sd.h"
//反馈切换
// 输入0 1 2 3  U16_4094
// 0  1000V
//1   100V
//2 10V 
//==========================================================
#define POWERON_DISP_TIME (20)	//开机显示界面延时20*100mS=2s

#define SLAVE_REG_P00		0x0000       //R_VOLU
#define SLAVE_REG_P01		0x0001      //Load_Voltage
#define SLAVE_REG_P02		0x0002      //Load_Current
#define SLAVE_REG_P03		0x0003		//Change_Voltage
#define SLAVE_REG_P04		0x0004		//Change_Current
#define SLAVE_REG_P05		0x0005		//Load_OCP
#define SLAVE_REG_P06		0x0006		//Change_OCP
#define SLAVE_REG_P07		0x0007		//Short_Time
#define SLAVE_REG_P08		0x0008		//Leak_Current
#define SLAVE_REG_P09		0x0009		//R1_Volu
#define SLAVE_REG_P10		0x000A		//R2_Volu
#define SLAVE_REG_P11		0x000B		//Temper
#define SLAVE_REG_P12		0x000C		//DHQ_Voltage
#define SLAVE_REG_P13		0x000D		//MODE
#define SLAVE_REG_P14		0x000E		//Load_Mode
#define SLAVE_REG_P15		0x000F		//Load_SET_Voltage
#define SLAVE_REG_P16		0x0010		//Load_SET_Current
#define SLAVE_REG_P17		0x0011		//Change_SET_Voltage
#define SLAVE_REG_P18		0x0012		//Change_SET_Current

/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

 // CRC 高位字节值表
static const uint8_t s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
// CRC 低位字节值表
const uint8_t s_CRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

u8 U15_4094,U16_4094;

u8 result,fileflag;
FATFS fs;
FIL file;
u8 usbstatus = UNKNOWN;
u8 usbbuf[0x40];
u8 send_usbbuf[0x40];
u8 powerstat;
u8 sdstatus;
u8 holdflag;
u16 foldernum;
char indexname[20];
u16 hispage;
u16 hispagestart;
u16 hispageend;
u16 hiscursor;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
//const u8 RANGE_UNIT[11]=
//{
//	4,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3
//	
extern char UsbHidReceiveComplete;
uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value);
uint16_t BEBufToUint16(uint8_t *_pBuf);
void MODS_03H(void);
uint16_t CRC16(uint8_t *_pBuf, uint16_t _usLen);
SAVE_DATA SDSAVE;
BLOCK_REC BlockNum;
SAVE_INDEX HisIndex;
u8 recordflag;
u8 sdcount;
u8 indexflag;

//SD卡储存相关
void Read_Block_Rec(void)
{
	SD_ReadBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
}

void Write_Block_Rec(void)
{
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
}

void Read_His_Data(u32 block)
{	
//	sizewatch =(sizeof(SaveBuffer)*block);
	SD_ReadMultiBlocks((uint8_t *)&SDSAVE,61952+(sizeof(SDSAVE)*block),512,sizeof(SDSAVE)/512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
}

void Write_His_Data(void)
{
	u16 i;
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
	SD_WriteMultiBlocks((uint8_t *)&SDSAVE,61952+sizeof(SDSAVE)*BlockNum.Num[0],512,sizeof(SDSAVE)/512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);	
	BlockNum.Num[0]++;
//	if(BlockNum.Num[1] > 4800)
//	{
//		BlockNum.Num[0] = 0;
//	}
//	if(BlockNum.Num[0] > SD_MAX_BLOCK)
//	{
//		BlockNum.Num[0] = 0;
//	}
//	memset(SaveBuffer.Time,0,sizeof(SaveBuffer.Time));
//	memset(SaveBuffer.Temp,0,sizeof(SaveBuffer.Temp));
	memset(SDSAVE.SAVEBUFF,0,sizeof(SDSAVE.SAVEBUFF));
}

void Write_His_Data_Man(void)
{
	u16 i;
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
	SD_WriteMultiBlocks((uint8_t *)&SDSAVE,61952+sizeof(SDSAVE)*BlockNum.Num[0],512,sizeof(SDSAVE)/512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);	
	if(indexflag == 1)
	{
		strcpy(HisIndex.Date[BlockNum.Num[1]%40],usbreadtime);
		HisIndex.Index[BlockNum.Num[1]%40] = BlockNum.Num[0];
//		for(i=0;i<8;i++)
//		{
//			HisIndex.Date[BlockNum.Num[1]][i]=SaveBuffer.Time[0][i]; 
//		}
		SD_WriteBlock((uint8_t *)&HisIndex,512+512*(BlockNum.Num[1]/40),512);
		SD_WaitWriteOperation();	
		while(SD_GetStatus() != SD_TRANSFER_OK);
		BlockNum.Num[1] ++;
		indexflag = 0;
	}
	BlockNum.Num[0]++;
	if(BlockNum.Num[1] > 4800)
	{
		BlockNum.Num[0] = 0;
	}
	if(BlockNum.Num[0] > SD_MAX_BLOCK)
	{
		BlockNum.Num[0] = 0;
	}
	memset(SDSAVE.SAVEBUFF,0,sizeof(SDSAVE.SAVEBUFF));
//	memset(SaveBuffer.Temp,0,sizeof(SaveBuffer.Temp));
}

void Read_Index(u32 indexnum)
{
	SD_ReadBlock((uint8_t *)&HisIndex,512+(512*indexnum),512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
}


void Draw_His_Index(u8 page,u8 keynum)
{
	u16 i,j;
	char buf[14],num[5],indexpages[10];
	Read_Index(page/4);
	
	Colour.black = LCD_COLOR_TEST_BACK;
	Colour.Fword = LCD_COLOR_TEST_BACK;
	LCD_DrawFullRect(10,40,400,370);
	
//	LCD_SetColors(LCD_COLOR_HLT,LCD_COLOR_BACK);
	Colour.black = LCD_COLOR_TEST_BACK;
	Colour.Fword = White;
	sprintf(indexpages,"%0.4d/%0.4d",page+1,BlockNum.Num[1]/10+1);
	WriteString_16(480,40+9*35,(uint8_t *)indexpages,0);
	
	if(page < BlockNum.Num[1]/10)
	{
		foldernum = 10;
	}else{
		foldernum = BlockNum.Num[1]%10;
	}
	if(keynum == 0 && page == 0)
	{
		hispage = 1;
	}else{
		hispage = HisIndex.Index[keynum+10*page-1]+1;
	}
	hispagestart = hispage;
	hispageend = HisIndex.Index[keynum+10*page];
	for(i=0;i<foldernum;i++)
	{
		
		sprintf(indexname,"20%0.2d-%0.2d-%0.2d-%0.2d:%0.2d:%0.2d",
																		  HisIndex.Date[i+10*page][1],
																		  HisIndex.Date[i+10*page][2],
																		  HisIndex.Date[i+10*page][3],
																		  HisIndex.Date[i+10*page][4],
																		  HisIndex.Date[i+10*page][5],
																		  HisIndex.Date[i+10*page][6]);
//		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		Colour.black = LCD_COLOR_TEST_BACK;
		Colour.Fword = White;
		Colour.black = LCD_COLOR_TEST_BACK;
		sprintf(num,"%d",i+1);
		WriteString_16(10,40+i*35,(uint8_t *)num,0);
		WriteString_16(40,40+i*35,(uint8_t *)".",0);
		if(i == keynum)
		{
			Colour.black = LCD_COLOR_SELECT;
		}else{
			Colour.black = LCD_COLOR_TEST_BACK;
		}
		WriteString_16(60,40+i*35,(uint8_t *)indexname,0);
	}
}

void Draw_His_Data(void)
{
	u8 i;
	u16 j;
	u16 pages;
	u16 cpage;
	char dispbuf[10];
	char pagebuf[10];
	
	Read_His_Data(hispage);	
	
	pages = hispageend - hispagestart + 1;
	cpage = hispage - hispagestart + 1;
	
	LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_SD_VIEW_Item();
	Colour.black = LCD_COLOR_TEST_BACK;
	Colour.Fword = White;
	for(i=0;i<10;i++)
	{
		LCD_DrawLine(0,55+40*i,640,LCD_DIR_HORIZONTAL);
	}
	LCD_DrawUniLine(178,55,178,414);
	LCD_DrawUniLine(338,55,338,414);
	LCD_DrawUniLine(530,55,530,414);
	sprintf(pagebuf,"%0.4d/%0.4d",cpage,pages);
	WriteString_16(400,20,(uint8_t *)pagebuf,0);
	for(i=0;i<4;i++)
	{
		WriteString_16(80+i*160,60,(uint8_t *)HIS_TOP[i],0);
	}
	for(j=0;j<8;j++)
	{
		for(i=0;i<10;i++)
		{
			dispbuf[i] = SDSAVE.SAVEBUFF[j].No[i];
		}
		WriteString_16(0,100+40*j,(uint8_t *)dispbuf,0);
		memset(dispbuf,0,sizeof(dispbuf));
		for(i=0;i<9;i++)
		{
			dispbuf[i] = SDSAVE.SAVEBUFF[j].Send_res[i];
		}
		WriteString_16(180,100+40*j,(uint8_t *)dispbuf,0);
		for(i=0;i<9;i++)
		{
			dispbuf[i] = SDSAVE.SAVEBUFF[j].Send_V[i];
		}
		WriteString_16(340,100+40*j,(uint8_t *)dispbuf,0);
		memset(dispbuf,0,sizeof(dispbuf));
		for(i=0;i<5;i++)
		{
			dispbuf[i] = SDSAVE.SAVEBUFF[j].comp[i];
		}
		WriteString_16(540,100+40*j,(uint8_t *)dispbuf,0);
		memset(dispbuf,0,sizeof(dispbuf));
	}
}

//关机检测
u8 PowerOffDetect(void)
{
	return GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3);
}

//检测到关机后的处理
void PowerOffHandle(void)
{
//	page_flag = poweroff;
//	static u8 offflag;
//	DrawPowOff();
	GPIO_ResetBits(GPIOH,GPIO_Pin_4);
	
}

void JumpBoot(u8 flag)
{
  	void (*pUserApp)(void);
  uint32_t JumpAddress;
	if(flag==55)
  {		
	__asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(BASIC_TIM, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_Cmd(TIM2,DISABLE);
		TIM_DeInit(BASIC_TIM);
		TIM_ITConfig(BASIC_TIM,TIM_IT_Update,DISABLE);
		TIM_Cmd(BASIC_TIM, DISABLE);	
		USART_DeInit(DEBUG_USART);
		USART_ITConfig(DEBUG_USART, USART_IT_RXNE, DISABLE);		
		USART_Cmd(DEBUG_USART,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//关闭WAKE UP 定时器中断
		RTC_WakeUpCmd( DISABLE);//关闭WAKE UP 定时器　
//		Disable_Extiint();
//		USBH_DeInit(&USB_OTG_Core,&USB_Host);
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
	}
}

//U盘检测
u8 udisk_scan(void)
{
	static u8 res;
	
//	u8 i;
//	char str[64];
	if(usbstatus != UNCONNECTED)
	{
		res = CH376DiskConnect( );
		if( res != USB_INT_SUCCESS )/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		{  
//			DrawUdisk1();
			
			usbstatus = UNCONNECTED;
			Disp_Usbflag(usbstatus);
			return NO_CONNECTION;
		}
	}
//	Delay(200);
	if(usbstatus != CONNECTED)
	{		
		res = CH376DiskMount( );
		if(res == USB_INT_SUCCESS)
		{
//			DrawUdisk2();
			
			usbstatus = CONNECTED;
			Disp_Usbflag(usbstatus);
			return UDISK_READY;
		}

	}
	return UDISK_NOTREADY;
}

//将缓冲区指定长度数据写到MCU flash 指定的地址
void FlashWrite(u32*Buffer,u32 Address,u16 length)
{
    u16  i;
    
    FLASH_Unlock(); 
	for(i = 0; i < length; i ++){
		FLASH_ProgramWord(Address+ (i*4), Buffer[i]);
	}	
    FLASH_Lock();
        
}

/****************************************************************************
* 功    能: 读取长度为length的32位数据
* 入口参数：address：地址
length： 数据长度
data_32  指向读出的数据
* 出口参数：无
* 说    明：无
* 调用方法：无
****************************************************************************/
void Flash_Read32BitDatas(uint32_t address, uint16_t length, u32* data_32)
{
	uint16_t i;
	for(i=0; i<length; i++)
	{
		data_32[i]=*(__IO int32_t*)address;
		address=address + 4;
	}
}

u8 CAL_ERASE(void)
{
	u8 state;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	state=FLASH_EraseSector(FLASH_Sector_12, VoltageRange_3);
    return state;
}

u8 SET_ERASE(void)
{
	u8 state;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	state=FLASH_EraseSector(FLASH_Sector_13, VoltageRange_3);
    return state;
}

void Store_cal_flash(void)
{
	while(CAL_ERASE() != FLASH_COMPLETE);
	FlashWrite((u32*)&Jk516cal,ADDR_CAL_SECTOR,sizeof(Jk516cal));
}


void Read_cal_flash(void)
{
	Flash_Read32BitDatas(ADDR_CAL_SECTOR,sizeof(Jk516cal),(u32*)&Jk516cal);
}

void Store_set_flash(void)
{
	while(SET_ERASE() != FLASH_COMPLETE);
//	SET_ERASE();
	FlashWrite((u32*)&Jk516save,ADDR_SET_SECTOR,sizeof(Jk516save));
}

void Read_set_flash(void)
{
	Flash_Read32BitDatas(ADDR_SET_SECTOR,sizeof(Jk516save),(u32*)&Jk516save);
}

void Power_Process(void)
{
		u16 i;
    u8 j;
    Disp_Coordinates_Typedef Debug_Cood;
		u8 key;
    Int_Pe3flag = 0;
    RTC_CLK_Config();
    RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次
    GPIO_Configuration();//端口初始化
	ReadBatGPIOInit();
    Ads1251_Int();
    if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
    {
        /* 设置时间和日期 */
        RTC_TimeAndDate_Set();
    }
    else
    {
        /* 检查是否电源复位 */   
        /* 使能 PWR 时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
        /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
        PWR_BackupAccessCmd(ENABLE);
         /* 等待 RTC APB 寄存器同步 */
        RTC_WaitForSynchro();   
    } 
    SysTick_Init();
	  set_mid(); 
    delay_ms(10);
    
//    Keyboard_Init();//按键初始化
    TIM6_Configuration();
	  
	  Beep_GPIO_Config();
    //////////////////////////////////////////液晶初始化
    LCD_Init();
    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	  /*把背景层刷黑色*/
    LCD_SetLayer(LCD_BACKGROUND_LAYER);  
    LCD_Clear(LCD_COLOR_RED);
	
		/*初始化后默认使用前景层*/
		LCD_SetLayer(LCD_FOREGROUND_LAYER); 
		//	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
    LCD_SetTransparency(0xff);
		LCD_Clear(LCD_COLOR_BLACK);
		lcd_image((uint8_t *)gImage_open);
    TIM_PWMOUTPUT_Config(50);
//    SPI_FLASH_Init();
//		InitGlobalValue();//初始化全局变量
		
    TIM6_Configuration();//定时器6定时10ms
		USBD_Init(&USB_OTG_dev,USB_OTG_HS_CORE_ID,
           &USR_desc,&USBD_HID_cb,&USR_cb);
		   
		MenuIndex=0;//待机菜单项
		i=0;//显示延时
	
	
	
	
	Parameter_valuecomp();//比较读出的数据
    open_flag=0;
    Range=Jk516save.Set_Data.Range;
    Range_Control(Range);
    RangeChange_Flag=1;
	if(SD_Init() == SD_OK)
	{
		sdstatus = 1;
		Read_Block_Rec();
		Read_Index(BlockNum.Num[1]/40);
	}
//		Touch_GPIO_Config();
//		tp_dev.init();				//触摸屏初始化
	
		while( GetSystemStatus() == SYS_STATUS_POWER )
		{
				i++;
				j++;
				if( i > POWERON_DISP_TIME )//延时20*100mS=2s后自动退出
							SetSystemStatus(SYS_STATUS_TEST);//待测状态
				delay_ms(100);
				key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)
				if(key!=KEY_NONE)
				{
						switch(key)
						{
								case	Key_F1:
											Trip_On();
											break;
								case		Key_F2:
											Trip_Off();
											break;
								case 	    Key_F3:
											Lock_On();
											break;
								case 	    Key_F4:
											Lock_Off();
											break;
								case		Key_F5:
											break;
								case		Key_Disp:
											Turnon_Led();
											break;
								case		Key_SETUP:
											Turnoff_Led();
											break;
								case		Key_FAST:
											LCD_Clear(LCD_COLOR_TEST_BACK);
											Debug_Cood.xpos=180;
											Debug_Cood.ypos =120;
											Debug_Cood.lenth=120;
											input_password(&Debug_Cood);
									//SetSystemStatus(SYS_STATUS_DEBUG);
											break;
								case		Key_LEFT:
											break;
								case		Key_RIGHT:
											break;
								case		Key_UP:
											Led_Pass_On();
											break;
								case		Key_DOWN:
											Led_Fail_On();
											break;
								case		Key_NUM7:
											break;
								case		Key_NUM8:
											break;
								case		Key_NUM9:
											break;
								case		Key_NUM4:
											break;
								case		Key_NUM5:
											break;
								case		Key_NUM6:
											break;
								case		Key_NUM1:
											break;
								case		Key_NUM2:
											break;
								case		Key_NUM3:
											break;
								case		Key_NUM0:
											break;
								case		Key_DOT:
											break;
								case		Key_BACK:
											break;
								case		Key_LOCK:
											break;
								case		Key_BIAS:
											break;
								case		Key_REST:
											break;
								case		Key_TRIG:
											break;
								case		Key_POWER:
											break;
						}
				}
		}
}

//void Read_Index(u32 indexnum)
//{
//	SD_ReadBlock((uint8_t *)&HisIndex,512+(512*indexnum),512);
//	SD_WaitWriteOperation();
//	while(SD_GetStatus() != SD_TRANSFER_OK);
//}

//==========================================================
//函数名称：SD_Process
//函数功能：读取查看SD卡数据
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.26 08:59
//备注说明：无
//==========================================================
void SD_Process(void)
{
	u8 keynum=0,i;
	u16 sdpage;
	static vu32 uwatch;
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key=0;
	vu8 Disp_Flag=1;
	vu8 folder = 0;;
	keynum=0;
	sdpage = 0;
	LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_SD_VIEW_Item();
	if(SD_Init() == SD_OK)
	{
		sdstatus = 1;
	}
	while(GetSystemStatus()==SYS_STATUS_SDFILE)
	{
		uwatch = sizeof(SDSAVE);
		powerstat = PowerOffDetect();
		if(powerstat == 0)
		{
			PowerOffHandle();
		}
		if(Disp_Flag==1)
		{
			if(folder == 0)
			{
				Draw_His_Index(sdpage,keynum);
			}else if(folder == 1){
				Draw_His_Data();
			}
           
			Disp_Flag=0;
		
		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        if(key!=KEY_NONE)
		{	Disp_Flag=1;
			ButtonSound();
			switch(key)
			{
				case Key_F1:

//					switch(keynum)
//					{
//						case 0:
//							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);//
//							else
//								SetSystemStatus(SYS_STATUS_FILE);
								
//							break;
//							
//						default:
//							break;
					
					
//					}

				break;
				case Key_F2:
					
					BlockNum.Num[0] = 0;
					BlockNum.Num[1] = 0;
					Write_Block_Rec();
					SD_Erase(0,61952+sizeof(SDSAVE));

//					switch(keynum)
//					{
//						case 0:
//								SetSystemStatus(SYS_STATUS_SETUP);
//							break;
//						default:
//							break;
//					
//					
//					}				
				

				break;
				case Key_F3:
					if(folder == 0)
					{
						if(sdpage > 0)
						{
							sdpage --;
							keynum = 0;
						}
					}else if(folder == 1){
						if(hispage > hispagestart)
						{
							hispage --;
						}
					}
				break;
				case Key_F4:
					if(folder == 0)
					{
						if(sdpage < BlockNum.Num[1]/10)
						{
							sdpage ++;
							keynum = 0;
						}
					}else if(folder == 1){
						if(hispage < hispageend)
						{
							hispage ++;
						}
					}
					
				
				break;
				case Key_F5:
					if(folder == 1){
						folder = 0;
						LCD_Clear(LCD_COLOR_TEST_BACK);
						Disp_SD_VIEW_Item();
					}
				break;
				case Key_Disp:
					if(folder == 0)
					{
						SetSystemStatus(SYS_STATUS_TEST);
					}else if(folder == 1){
						folder = 0;
						LCD_Clear(LCD_COLOR_TEST_BACK);
						Disp_SD_VIEW_Item();
					}
				break;
				case Key_SETUP:
					if(folder == 0)
						folder = 1;
                        //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				
				case Key_LEFT:
//					if(keynum==0)
//						keynum=12;
//					else
//					if(keynum>6)
//						keynum-=6;
//					else
//						keynum+=5;
					
						
				break;
				case Key_RIGHT:
//					if(keynum==0)
//						keynum=1;
//					else
//					if(keynum<=6)
//						keynum+=6;
//					else
//						keynum-=5;
					
						
				break;
				case Key_DOWN:
					if(folder == 0)
					{
						if(keynum < foldernum-1)
						{
							keynum ++;
						}
					}
					
					
				break;
				case Key_UP:
					if(folder == 0)
					{
						if(keynum > 0)
						{
							keynum --;
						}
					}
					
				break;
				case Key_DOT:

					break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				break;
                case Key_FAST:
				
					keynum=0;
																							
					break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
		Store_set_flash();
	
	}
	
	
}

                                                                                                                                                                                              
//==========================================================
//函数名称：Setup_Process
//函数功能：设置程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.28
//修改日期：2015.10.28 10:45
//备注说明：无
//==========================================================
void Setup_Process(void)
{

	u8 keynum=0,i;
	
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key=0;
	vu8 Disp_Flag=1;
	keynum=0;
  LCD_Clear(LCD_COLOR_TEST_BACK);
  
	Disp_Test_Set_Item();
	 
 	while(GetSystemStatus()==SYS_STATUS_SETUP)
	{
	    powerstat = PowerOffDetect();
		if(powerstat == 0)
		{
			PowerOffHandle();
		}
		if(Disp_Flag==1)
		{
			
			DispSet_value(keynum);
           
			Disp_Flag=0;
		
		}
//		tp_dev.scan(0); 		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
//		{
//			
//			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
//			{	

//				for(i=0;i<5;i++)
//				{
//					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
//					{
//						touch_value=i;
//						Disp_Flag=1;
//						break;
//						
//					}
//					else
//						touch_value=0xff;
//				}
//				if(touch_value==0xff)//没有检测到触摸屏 接着往下检测
//				{
//					for(i=0;i<16;i++)
//					{
//						if(i<16/2)
//						{
//							if(((tp_dev.x[0]>DISPX1)&&(tp_dev.x[0]<DISPX1+RECT))&&(tp_dev.y[0]<(FIRSTLINE+HIGH1*(i+1)))&&(tp_dev.y[0]>(FIRSTLINE+HIGH1*i)))
//							{	
//								touch_value=i+5;
//								Disp_Flag=1;
//								break;
//							}
//							
//						}
//						else
//						{
//							if(((tp_dev.x[0]>DISPX2)&&(tp_dev.x[0]<DISPX2+RECT))&&(tp_dev.y[0]<(FIRSTLINE+HIGH1*(i-8+1)))&&(tp_dev.y[0]>(FIRSTLINE+HIGH1*(i-8))))
//							{	
//								touch_value=i+5;
//								Disp_Flag=1;
//								break;
//							}
//						
//						}
//										
//					}
//				
//				}	
//				
//				switch(touch_value)
//					{
//						case 0:
//							switch(keynum)
//							{
//								case 0:
//									SetSystemStatus(SYS_STATUS_TEST);
//									ButtonSound();
//								break;
//								case 1:
//									Jk516save .Set_Data.trip=0;
//									ButtonSound();
//									break;
//								case 2:
//									Jk516save .Set_Data.speed=0;
//									ButtonSound();
//									break;
//								case 3:
//									Jk516save .Set_Data.Res_comp=0;
//									ButtonSound();
//									break;
//								case 4:
//									break;
//								case 5:
//									Jk516save .Set_Data.V_comp=0;
//									ButtonSound();
//									break;
//								case 6:
//									break;
//								case 7:
//									Jk516save.Set_Data.Range_Set=0;
//									Jk516save.Set_Data.Range=0;//自动
//									ButtonSound();
//									break;
//								case 8:
//									Jk516save .Set_Data.beep=0;
//									ButtonSound();
//									break;
//								case 9:
//									break;
//								case 10:
//									break;
//								case 11:
//									break;
//								case 12:
//									break;
//								
//								default :
//									break;
//								
//							}
//							break;
//						case 1:
//							switch(keynum)
//							{
//								case 0:
//									//SetSystemStatus(SYS_STATUS_TEST);
//								break;
//								case 1:
//									Jk516save .Set_Data.trip=1;
//									ButtonSound();
//									break;
//								case 2:
//									//Jk516save .Set_Data.speed=1;
//									break;
//								case 3:
//									Jk516save .Set_Data.Res_comp=1;
//									ButtonSound();
//									break;
//								case 4:
//									break;
//								case 5:
//									Jk516save .Set_Data.V_comp=1;
//									ButtonSound();
//									break;
//								case 6:
//									break;
//								case 7:
//									Jk516save.Set_Data.Range_Set=1;
//									ButtonSound();
//									break;
//								case 8:
//									Jk516save .Set_Data.beep=1;
//									ButtonSound();
//									break;
//								case 9:
//									break;
//								case 10:
//									break;
//								case 11:
//									break;
//								case 12:
//									break;
//								
//								default :
//									break;
//								
//							}
//							break;
//							
//						case 2:
//							switch(keynum)
//							{
//								case 0:
//									SetSystemStatus(SYS_STATUS_SYSSET);
//									ButtonSound();
//								break;
//								case 1:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 2:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 3:
//									break;
//								case 4:
//									break;
//								case 5:
//									break;
//								case 6:
//									break;
//								case 7:
//									Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
//									Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
//									Jk516save.Set_Data.Range=Range;
//									Range_Control(Range);
//									ButtonSound();
//									break;
//								case 8:
//									Jk516save .Set_Data.beep=2;
//									ButtonSound();
//									break;
//								case 9:
//									break;
//								case 10:
//									break;
//								case 11:
//									break;
//								case 12:
//									break;
//								
//								default :
//									break;
//								
//							}
//							break;
//							//SetSystemStatus();
//							
//						case 3:
//							switch(keynum)
//							{
//								case 0:
//									SetSystemStatus(SYS_STATUS_SYS);
//									ButtonSound();
//								break;
//								case 1:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 2:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 3:
//									break;
//								case 4:
//									break;
//								case 5:
//									break;
//								case 6:
//									break;
//								case 7:
//									Jk516save.Set_Data.Range_Set=1;
//									if(Jk516save.Set_Data.Range<RANGE_MAX)
//										Jk516save.Set_Data.Range++;
//									else
//										Jk516save.Set_Data.Range=0;
//									Range=Jk516save.Set_Data.Range;
//									ButtonSound();
//									break;
//								case 8:
//									break;
//								case 9:
//									break;
//								case 10:
//									break;
//								case 11:
//									break;
//								case 12:
//									break;
//								
//								default :
//									break;
//								
//							}
//							break;
//							//SetSystemStatus();
//							
//						case 4:
//							switch(keynum)
//							{
//								case 0:
//									//SetSystemStatus(SYS_STATUS_TEST);
//								break;
//								case 1:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 2:
//									//Jk516save .Set_Data.trip=0;
//									break;
//								case 3:
//									break;
//								case 4:
//									break;
//								case 5:
//									break;
//								case 6:
//									break;
//								case 7:
//									Jk516save.Set_Data.Range_Set=1;
//									if(Jk516save.Set_Data.Range<RANGE_MAX)
//										Jk516save.Set_Data.Range++;
//									else
//										Jk516save.Set_Data.Range=0;
//									Range=Jk516save.Set_Data.Range;
//									ButtonSound();
//									break;
//								case 8:
//									break;
//								case 9:
//									break;
//								case 10:
//									break;
//								case 11:
//									break;
//								case 12:
//									break;
//								
//								default :
//									break;
//								
//							}
//							break;
//						case 5:
//							keynum=1;
//							ButtonSound();
//							break;
//						case 6:
//							keynum=2;
//							ButtonSound();
//							break;
//						case 7:
//							//keynum=3;
//							break;
//						case 8:
//							keynum=3;
//							ButtonSound();
//							break;
//						case 9://弹出数字框
//							keynum=4;
//							ButtonSound();
//							break;
//						case 10:
//							//keynum=5;
//							break;
//						case 11:
//							keynum=5;
//							ButtonSound();
//							break;
//						case 12:
//							keynum=6;
//							ButtonSound();
//							break;
//						case 13:
//							keynum=7;
//							ButtonSound();
//							break;
//						
//						case 14:
//							keynum=8;
//							ButtonSound();
//							break;
//						case 15:
//							break;
//						case 16:
//							keynum=9;
//							ButtonSound();
//							break;
//						case 17:
//							keynum=10;
//							ButtonSound();
//							break;
//						case 18:
//							break;
//						case 19:
//							keynum=11;
//							ButtonSound();
//							break;
//						case 20:
//							keynum=12;
//							ButtonSound();
//							break;
//						default:
//							break;
//					
//					
//					}
//			}
//		
//		}
        key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        if(key!=KEY_NONE)
		{	Disp_Flag=1;
			ButtonSound();
			switch(key)
			{
				case Key_F1:

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_TEST);//
//							else
//								SetSystemStatus(SYS_STATUS_FILE);
								
							break;
						case 1:
							Jk516save.Set_Data.trip=0;
							
							break;
						case 2:
							Jk516save.Set_Data.speed=0;//慢速
							
								
							
							
							
							break;
						case 3:
							Jk516save.Set_Data.Res_comp=0;
							//SaveData.Main_Func.Level=0;
							break;
						case 4:
                            Coordinates.xpos=DISPX1;
                            Coordinates.ypos=FIRSTLINE+HIGH1*4-6;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Res_low=Disp_Set_Num(&Coordinates);
                            
                    
							
							break;
						case 5:
							Jk516save.Set_Data.V_comp=0;
							break;
						case 6:
                            Coordinates.xpos=DISPX1;
                            Coordinates.ypos=FIRSTLINE+HIGH1*7-6;
                            Coordinates.lenth = RECT;
                            Jk516save.Set_Data.V_low = Disp_Set_CompNum(&Coordinates);
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=0;
							Jk516save.Set_Data.Range=0;//自动
							break;
						case 8:
							Jk516save.Set_Data.beep=0;
							break;
						case 9:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*3-8;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Nominal_Res=Disp_Set_Num(&Coordinates);

							break;
						case 10:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*4-6;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.High_Res=Disp_Set_Num(&Coordinates);

							break;
						case 11:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*6-6;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.Nominal_V=Disp_Set_CompNum(&Coordinates);

							break;
						case 12:
                            Coordinates.xpos=DISPX2;
                            Coordinates.ypos=FIRSTLINE+HIGH1*7-6;
                            Coordinates.lenth=RECT;
                            Jk516save.Set_Data.V_high = Disp_Set_CompNum(&Coordinates);

							break;
						
							
						default:
							break;
					
					
					}

				break;
				case Key_F2:
					

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUP);
//							else
//								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 1:
							
							Jk516save.Set_Data.trip=1;//中速
							
							
							break;
						case 2:
							Jk516save.Set_Data.speed=1;
							
							
							
							break;
						case 3:
							Jk516save.Set_Data.Res_comp=1;
							break;
						case 4:
//							if(Save_Res.Set_Data.Res_low.Num%1000<900)
//								Save_Res.Set_Data.Res_low.Num+=100;
//							else
//							{
//								//Save_Res.Set_Data.Res_low.Num%=100;
//								Save_Res.Set_Data.Res_low.Num-=900;
//								
//							}
							break;
						case 5:
							Jk516save.Set_Data.V_comp=1;
							break;
						case 6:
//							if(Save_Res.Set_Data.V_low.Num%1000<900)
//								Save_Res.Set_Data.V_low.Num+=100;
//							else
//								Save_Res.Set_Data.V_low.Num-=900;
//							Uart_Send_Flag=2;
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=1;
							//Jk516save.Set_Data.Range=10;//手动
							break;
						case 8:
							Jk516save.Set_Data.beep=1;
							break;

						default:
							break;
					
					
					}				
				

				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 1:
                           // Jk516save.Set_Data.trip=2;//快速
							break;
						case 2:
                            Jk516save.Set_Data.speed=2;
							break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:

							break;
						case 7:
//                            Jk516save.Set_Data.Range_Set=0;
//                            Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                            
                            Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
                            Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                            Jk516save.Set_Data.Range=Range;
                            Range_Control(Range);
							//Jk516save.Set_Data.Range=11;//当前量程计算量程位置
							break;
						case 8:
							Jk516save.Set_Data.beep=2;
							break;

						
						default:
							break;
					
					
					}	
					
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SDFILE);
							break;
						case 1:
                            //Jk516save.Set_Data.trip=3;//极速
							
									
							break;
						case 2:
                            Jk516save.Set_Data.speed=3;
							
							break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:
							if(Jk516save.Set_Data.V_low.Num%10<9)
								Jk516save.Set_Data.V_low.Num+=1;
							else
								Jk516save.Set_Data.V_low.Num-=9;
							break;
						case 7:
                            Jk516save.Set_Data.Range_Set=1;
                            if(Jk516save.Set_Data.Range<RANGE_MAX)
                                Jk516save.Set_Data.Range++;
                            else
                                Jk516save.Set_Data.Range=0;
                            Range=Jk516save.Set_Data.Range;
							break;

						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(keynum)
					{
						case 0:
							JumpBoot(55);
						break;
						case 4:
						case 9:
							break;
						case 7:	
							Jk516save.Set_Data.Range_Set=1;
                            if(Jk516save.Set_Data.Range)
							Jk516save.Set_Data.Range--;
                            else
                                Jk516save.Set_Data.Range=RANGE_MAX;
                                
                            Range=Jk516save.Set_Data.Range;
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;
				case Key_Disp:
                        SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                        //SetSystemStatus(SYS_STATUS_SETUPTEST);
				break;
				
				case Key_LEFT:
					if(keynum==0)
						keynum=12;
					else
					if(keynum>6)
						keynum-=6;
					else
						keynum+=5;
					
						
				break;
				case Key_RIGHT:
					if(keynum==0)
						keynum=1;
					else
					if(keynum<=6)
						keynum+=6;
					else
						keynum-=5;
					
						
				break;
				case Key_DOWN:
					if(keynum>11)
						keynum=0;
					else
						keynum++;
					
					
				break;
				case Key_UP:
					if(keynum<1)
						keynum=11;
					else
						keynum--;
					
				break;
				case Key_DOT:

					break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				break;
                case Key_FAST:
					keynum=0;
					break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	Store_set_flash();
	
	}
	
}

//==========================================================
//函数名称：Test_Process
//函数功能：测试主程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 14:18
//备注说明：x.xx ms(12MHz/12T)
//==========================================================
void Test_Process(void)
{
	vu8 key;
    vu16 USB_Count = 0;
    UINT fnum;
    vu8 test_Vsorting,test_Rsorting;
    u32 color;
    u8 keynum = 0;
    u8 test_over = 0;
    u8 OpenRangflag = 0;
    vu8 Trip_Over_usb = 0;
		vu16 i;
    u8 ry_flag=0;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
		u8 Disp_Flag = 1 ;//显示标志
    vu8 range_flag = 0;
	  ffit_data1 = 0;
    Ad_over = 0;
    Set_flag = 0;
    test_start = 0;
    V_Range = 0;
	  F_100ms = FALSE;//100ms定时	
    debug_flag = 0;
	Init_CH376();
    LCD_Clear( LCD_COLOR_TEST_BACK );
    pt=Send_To_U;
    Disp_Test_Item();  
    open_flag = 0;    
    V_Range = Jisuan_V_Range(Jk516save.Set_Data.Nominal_V);
    Range_Control(Range);	
    open_flag = 1; 
    Select_V_I(1);
    delay_us(120);
    i=0;
    range_over=0;
	V_Range=1;
	
		while(GetSystemStatus()==SYS_STATUS_TEST)
		{
			
			if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
			{
				delay_ms(100);
				if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
				{
				  GPIO_ResetBits(GPIOH,GPIO_Pin_4);
					delay_ms(1000);
					delay_ms(1000);
					delay_ms(1000);
				}
				
			}
			powerstat = PowerOffDetect();
			if(powerstat == 0)
			{
				PowerOffHandle();
			}	
			DispBattery();
      Colour.Fword = White;
      if(Disp_RTCflag)
      {
				Disp_RTCflag = 0;
				Disp_dateandtime();
				Int_Pe3flag = 0;
            
      }
      if( Disp_Flag == 1 )  //显示设置的值
		  {
				Disp_Test_value(keynum);
				Colour.Fword = White;
				Colour.black = LCD_COLOR_TEST_BACK;
				Disp_R_V();
			}
	  if(holdflag == 1)
	  {
		  Colour.black = LCD_COLOR_TEST_BACK;
		  Colour.Fword = LCD_COLOR_RED;
		  WriteString_16(500-48-80-30,8,"H",0);
	  }else{
		  Colour.black = LCD_COLOR_TEST_BACK;
		  Colour.Fword = LCD_COLOR_TEST_BACK;
		  WriteString_16(500-48-80-30,8," ",0);
	  }
		  
      if( Disp_usbflag || Disp_Flag )
      {
        Disp_Usbflag(usbstatus);
        Disp_Flag=0; 
      }
		  DCD_EP_PrepareRx( &USB_OTG_dev, HID_OUT_EP, usbbuf, 64);//接收PC数据
		if( UsbHidReceiveComplete )                             //接收到数据
		{
				MODS_03H();
				UsbHidReceiveComplete=0;
		}
      OpenRangflag=read_cpld();//判断是否开路 
	  OpenRangflag = 1;
	  open_flag = Read_Openflag();
      if(OpenRangflag)
      {
            ry_flag++;
            if(ry_flag>3)
                ry_flag=3;
            OpenRangflag=0;
        
        }else
        ry_flag=0;
            
        if(Jk516save.Set_Data.trip==0)
            test_start=1;
        if(open_flag==0 && holdflag == 0)//不开路
        {
            if(test_start==1)
            {
                range_flag=1;
//				Jk516save.Set_Data.speed=3;
                switch(Jk516save.Set_Data.speed)
                {
                    case 0://慢速
					Select_V_I(1);
                        if(Range==0)
							V_I_amp(1);
						else
							V_I_amp(0);
						delay_ms(15);
						read_adI_one();
						if(I_ad>0x7fffff)
								I_ad=0;
						I_ad/=720;
                        Range_value=I_ad;	
                        Range_Changecomp();	//换挡 比较
                        while(range_over)
                        {
							read_adI_one();
							if(I_ad>0x7fffff)
								I_ad=0;
							
							I_ad/=720;
							Range_value=I_ad;
                            Range_Changecomp();	//换挡 比较
							
                        }
                        read_adI_1();    
                        Select_V_I(0);
                        if(V_Range==0)
                            V_I_amp(1);
						else
							V_I_amp(0);
                        delay_ms(15);;
                        read_adV_one();
						if(V_ad>0x7fffff)
								V_ad=0xffffff-V_ad;
						V_ad/=8;
                        Range_Value_V=V_ad;
                        VRange_Changecomp();	//换挡 比较
                        while(range_over)
                        {
                            //VRange_Changecomp();	//换挡 比较
                            
                            //read_adV_4();//
                           //Range_Value_V=V_ad/100;
							
							read_adV_one();
							if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
							V_ad/=8;
							Range_Value_V=V_ad;
							VRange_Changecomp();	//换挡 比较
                        
                        
                        }
						read_adV_1();
                    break;
                    case 1://中速
					Select_V_I(1);
                        if(Range==0)
							V_I_amp(1);
						else
							V_I_amp(0);
						delay_ms(15);
						read_adI_one();
						if(I_ad>0x7fffff)
								I_ad=0;
						I_ad/=720;
                        Range_value=I_ad;	
                        Range_Changecomp();	//换挡 比较
                        while(range_over)
                        {
							read_adI_one();
							if(I_ad>0x7fffff)
								I_ad=0;
							
							I_ad/=720;
							Range_value=I_ad;
                            Range_Changecomp();	//换挡 比较
							
                        }
                        read_adI_2();    
                        Select_V_I(0);
                        if(V_Range==0)
                            V_I_amp(1);
						else
							V_I_amp(0);
                        delay_ms(15);;
                        read_adV_one();
						if(V_ad>0x7fffff)
								V_ad=0xffffff-V_ad;
						V_ad/=8;
                        Range_Value_V=V_ad;
                        VRange_Changecomp();	//换挡 比较
                        while(range_over)
                        {
                            //VRange_Changecomp();	//换挡 比较
                            
                            //read_adV_4();//
                           //Range_Value_V=V_ad/100;
							
							read_adV_one();
							if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
							V_ad/=8;
							Range_Value_V=V_ad;
							VRange_Changecomp();	//换挡 比较
                        
                        
                        }
						read_adV_1();
                    break;
                    case 2://快速
					Select_V_I(1);
                        if(Range==0)
							V_I_amp(1);
						else
							V_I_amp(0);
						delay_ms(15);
						read_adI_one();
						if(I_ad>0x7fffff)
								I_ad=0;
						I_ad/=720;
                        Range_value=I_ad;	
                        Range_Changecomp();	//换挡 比较
                        while(range_over)
                        {
							read_adI_one();
							if(I_ad>0x7fffff)
								I_ad=0;
							
							I_ad/=720;
							Range_value=I_ad;
                            Range_Changecomp();	//换挡 比较
							
                        }
                        read_adI_3();    
                        Select_V_I(0);
                        if(V_Range==0)
                            V_I_amp(1);
						else
							V_I_amp(0);
                        delay_ms(15);;
                        read_adV_one();
						if(V_ad>0x7fffff)
								V_ad=0xffffff-V_ad;
						V_ad/=8;
                        Range_Value_V=V_ad;
                        VRange_Changecomp();	//换挡 比较
                        while(range_over)
                        {
                            //VRange_Changecomp();	//换挡 比较
                            
                            //read_adV_4();//
                           //Range_Value_V=V_ad/100;
							
							read_adV_one();
							if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
							V_ad/=8;
							Range_Value_V=V_ad;
							VRange_Changecomp();	//换挡 比较
                        
                        
                        }
						read_adV_1();
                    break;
                    case 3://极速
                        Select_V_I(1);
                        if(Range==0)
							V_I_amp(1);
						else
							V_I_amp(0);
						delay_ms(15);
						read_adI_one();
						if(I_ad>0x7fffff)
								I_ad=0;
						I_ad/=720;
                        Range_value=I_ad;	
                        Range_Changecomp();	//换挡 比较
                        while(range_over)
                        {
							read_adI_one();
							if(I_ad>0x7fffff)
								I_ad=0;
							
							I_ad/=720;
							Range_value=I_ad;
                            Range_Changecomp();	//换挡 比较
							
                        }
                        read_adI_4();    
                        Select_V_I(0);
                        if(V_Range==0)
                            V_I_amp(1);
						else
							V_I_amp(0);
                        delay_ms(15);;
                        read_adV_one();
						if(V_ad>0x7fffff)
								V_ad=0xffffff-V_ad;
						V_ad/=8;
                        Range_Value_V=V_ad;
                        VRange_Changecomp();	//换挡 比较
                        while(range_over)
                        {
                            //VRange_Changecomp();	//换挡 比较
                            
                            //read_adV_4();//
                           //Range_Value_V=V_ad/100;
							
							read_adV_one();
							if(V_ad>0x7fffff)
							V_ad=0xffffff-V_ad;
							V_ad/=8;
							Range_Value_V=V_ad;
							VRange_Changecomp();	//换挡 比较
                        
                        
                        }
						read_adV_1();
						//Select_V_I(1);
                        break;
                    default:

                        break;
                
                
                
                }
                
                test_over=1;
            
               
            }
           
                      
        }
        else
        {
			if(holdflag == 0)
			{
				if(test_start==1)
				{
					V_Range=1;
					VRange_Changecomp();
					
	//			   if(range_flag) //开路的时候的档位控制
				   {
					   if(Jk516save.Set_Data.Range_Set==0)
					   {
						   Range=3;
					   }
					   else
					   {
						   
					   }
					   if(Jk516save.Set_Data.Range_Set!=1)
					   Range_Control(Range);
						   i=0;
						   range_flag=0;
						   RangeChange_Flag=1;
					   
					   
					   
				   }
					Disp_Open();
			   }
		   }
        }
            
       
	 	if(test_start&&test_over==1 && holdflag == 0)//显示测试值
		{   

            test_over=0;
            if(Jk516save.Set_Data.trip)
            Trip_Over_usb=1;
            
            Ad_over=0; 

              disp_I=disp_I-Jk516save.Clear[Range];//再判断极性
            
               if(disp_I<0)
               {
                   disp_I=-disp_I;
                   polarity_r=0;
               }
               else
               {
                polarity_r=1;
               
               }
                
               if(disp_I>3800)
               {
                   i=0;
                   open_flag=1;
           
                    Disp_Open();
         
                   
               }
                if(polarity_v == 0 && disp_V <= abs(Jk516save.Clear_V[V_Range])+10)
				{
					polarity_v = 1;
					disp_V=abs(Jk516save.Clear_V[V_Range]) - disp_V;
				}else{
//					if(Jk516save.Clear_V[V_Range] > 0)
//					{
						disp_V=disp_V - Jk516save.Clear_V[V_Range];
//					}else{
//						disp_V=disp_V + Jk516save.Clear_V[V_Range];
//					}
				}
                
                if((disp_V>0 && disp_V < 100) || (disp_V<0 && disp_V > -100))
                {
                    polarity_v=1;
                    disp_V=0;
                
                }
//				else
//                {
//                    polarity_v=1;
//                
//                }

//                Res_count.r=I_ad;
                Test_Debug();//校正
//                I_ad=Res_count.r;
                if(open_flag==0)
                {
                    
                    if(RangeChange_Flag)
                    {
                        Disp_Range(Jk516save.Set_Data.Range_Set,Range);
                        Send_ComBuff.Range=Range;
                        RangeChange_Flag=0;
                        i=0;
                    }

                    if(range_over==0)
                    {
                        test_start=0;
                        Test_Value_V=V_Datacov(disp_V ,V_Range);//把数据的小数点和单位 和极性都加上
						if(V_Range == 1 && Test_Value_V.res > 60000)
						{
							open_flag=1;
							Disp_Open();
						}else{
							
							if(Range == 0)
							{
								Test_Value=Datacov(disp_I,Range);
							}else{
								Test_Value=Datacov(disp_I*10,Range);
							}
							Disp_Testvalue(Test_Value,Test_Value_V,0);//显示电阻和电压
							if(Jk516save.Set_Data.V_comp)
							{
								test_Vsorting= V_Comp();
								if(polarity_v==0)
									test_Vsorting=2;
								
							}
							if(Jk516save.Set_Data.Res_comp)
								test_Rsorting=R_Comp();
							color=Colour.black;
							//下面是分选
						   
							if(Jk516save.Set_Data.Res_comp==1&&Jk516save.Set_Data.V_comp==1)
							{
								 
								Colour.black=LCD_COLOR_RED;
								if(test_Vsorting==0&&test_Rsorting==0)
								{
									Colour.black=LCD_COLOR_GREEN;
									memcpy(DispBuf,"RV GD",5);  
									
									Send_ComBuff.comp=0;
									Beep_Out(0);
									Led_Pass_On();
									
								}
								else 
								{
									Beep_Out(1);
									Led_Fail_On();
									if(test_Vsorting==0)
									{
										memcpy(DispBuf,"R  FL",5);
										Send_ComBuff.comp=1;
									}
									else if(test_Rsorting==0)
									{
										memcpy(DispBuf,"V  FL",5);
										Send_ComBuff.comp=2;
										
									}
									else
									{
										memcpy(DispBuf,"RV FL",5);
										Send_ComBuff.comp=3;

									}                                    
								}
								memcpy((void *)Send_To_U.comp,DispBuf,5);
								Send_To_U.comp[5] = ' ';
								DispBuf[5]=0;
								LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
								WriteString_16(SORTING_XDISP, SORTING_Y_DISP, DispBuf,  0);
							 Colour.black=color;
							}
							else
								if(Jk516save.Set_Data.Res_comp==1)
								{
									if(test_Rsorting)//不合格
									{
										Colour.black=LCD_COLOR_RED;
										memcpy(DispBuf,"R  FL",5);
										Send_ComBuff.comp=1;                                    
										Beep_Out(1);
										Led_Fail_On();
									}
									else//合格
									{
										Colour.black=LCD_COLOR_GREEN;
										memcpy(DispBuf,"R  GD",5);
										Send_ComBuff.comp=0;                                    
										Beep_Out(0);
										Led_Pass_On();
									
									}
								
								 memcpy((void *)Send_To_U.comp,DispBuf,5);
								 Send_To_U.comp[5] = ' ';
								 DispBuf[5]=0;
								 LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
								 WriteString_16(SORTING_XDISP, SORTING_Y_DISP, DispBuf,  0);
								
								}else if(Jk516save.Set_Data.V_comp==1)
								{
									if(test_Vsorting)//不合格
									{
										Colour.black=LCD_COLOR_RED;
										memcpy(DispBuf,"V  FL",5);
										Send_ComBuff.comp=3;
										Beep_Out(1);
										Led_Fail_On();
									}
									else//合格
									{
										Beep_Out(0);
										Led_Pass_On();
										Colour.black=LCD_COLOR_GREEN;
										memcpy(DispBuf,"V  GD",5);
										Send_ComBuff.comp=0;
									
									}
								 memcpy((void *)Send_To_U.comp,DispBuf,5);
								 Send_To_U.comp[5] = ' ';
								 DispBuf[5]=0; 
								 LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
								 WriteString_16(SORTING_XDISP, SORTING_Y_DISP, DispBuf,  0);
								}
								else
								{
									memcpy((void *)Send_To_U.comp,"----- ",6);
								}
						  
							if(Jk516save.Sys_Setvalue.u_flag)
							{
					//			i=1001;
					//            
					//			while(i--)
								udisk_scan();

								if(usbstatus == CONNECTED)
								{
									vu8 copybuff[100];
									memset((void *)copybuff,0,100);

									if(fileflag == 0)
									{
										memcpy ((void *)copybuff,"/",1);
										strcat((char *)copybuff,(char *)Jk516save.Sys_Setvalue.textname);
										strcat((char *)copybuff,(char *)".XLS");
										result=CH376FileOpenPath(copybuff);
										if(result == ERR_MISS_FILE)
										{
											result = CH376FileCreatePath(copybuff);
											if(result == USB_INT_SUCCESS)
											{
												CH376ByteWrite( "序号\t电阻\t电压\t分选\r\n", strlen("序号\t电阻\t电压\t分选\r\n"), NULL );
												if(result == USB_INT_SUCCESS)
												{
													result = CH376FileClose(TRUE);
												}
											}
										}
										fileflag = 1;
									}else{
										memcpy ((void *)copybuff,"/",1);
										strcat((char *)copybuff,(char *)Jk516save.Sys_Setvalue.textname);
										strcat((char *)copybuff,(char *)".XLS");
										result=CH376FileOpenPath(copybuff);
										result = CH376ByteLocate(0xFFFFFFFF);
										if(result == USB_INT_SUCCESS)
										{
											pt=Send_To_U;
											CH376ByteWrite((u8 *)&pt, sizeof(Send_To_U), NULL );
											if(result == USB_INT_SUCCESS)
											{
												result = CH376FileClose(TRUE);
											}
										}
									}
								}else{
									fileflag = 0;
									Disp_usbflag=0;
								}
							}
							if(sdstatus == 1)
							{
								if(recordflag == 1)
								{
									if(sdcount == 7)
									{
										SDSAVE.SAVEBUFF[sdcount] = Send_To_U;
										Write_His_Data();
										sdcount = 0;
									}else{
										SDSAVE.SAVEBUFF[sdcount] = Send_To_U;
										sdcount++;
									}
								}
							}
							
						}
					}
                    
                }
            
		}
//		if(Jk516save.Sys_Setvalue.u_flag)
//		{
//	//			i=1001;
//	//            
//	//			while(i--)
//			udisk_scan();

//			if(usbstatus == CONNECTED)
//			{
//				vu8 copybuff[100];
//				memset((void *)copybuff,0,100);

//				if(fileflag == 0)
//				{
//					memcpy ((void *)copybuff,"/",1);
//					strcat((char *)copybuff,(char *)Jk516save.Sys_Setvalue.textname);
//					strcat((char *)copybuff,(char *)".XLS");
//					result=CH376FileOpenPath(copybuff);
//					if(result == ERR_MISS_FILE)
//					{
//						result = CH376FileCreatePath(copybuff);
//						if(result == USB_INT_SUCCESS)
//						{
//							CH376ByteWrite( "序号\t电阻\t电压\t分选\r\n", strlen("序号\t电阻\t电压\t分选\r\n"), NULL );
//							if(result == USB_INT_SUCCESS)
//							{
//								result = CH376FileClose(TRUE);
//							}
//						}
//					}
//					fileflag = 1;
//				}else{
//					memcpy ((void *)copybuff,"/",1);
//					strcat((char *)copybuff,(char *)Jk516save.Sys_Setvalue.textname);
//					strcat((char *)copybuff,(char *)".XLS");
//					result=CH376FileOpenPath(copybuff);
//					result = CH376ByteLocate(0xFFFFFFFF);
//					if(result == USB_INT_SUCCESS)
//					{
//						pt=Send_To_U;
//						CH376ByteWrite((u8 *)&pt, sizeof(Send_To_U), NULL );
//						if(result == USB_INT_SUCCESS)
//						{
//							result = CH376FileClose(TRUE);
//						}
//					}
//				}
//			}else{
//				fileflag = 0;
//				Disp_usbflag=0;
//			}
//		}
             
		Uart_Process();//串口处理
//		tp_dev.scan(0); 		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
//		{
//			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
//			{	
//				
//				for(i=0;i<5;i++)
//				{
//					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
//						touch_value=i;
//					else
//						touch_value=0xff;
//					
//					switch(touch_value)
//					{
//						case 0:
//							break;
//						case 1:
//							SetSystemStatus(SYS_STATUS_SETUP);
//							ButtonSound();
//							break;
//						case 2:
//							SetSystemStatus(SYS_STATUS_SYSSET);
//							ButtonSound();
//							break;
//						case 3:
//							SetSystemStatus(SYS_STATUS_SYS);
//							ButtonSound();
//							break;
//						case 4:
//                            SetSystemStatus(SYS_STATUS_CLEAR);
//							ButtonSound();
//							break;
//						default:
//							break;
//					
//					
//					}
//				}
//				
//			}
//		
//		}
        if(Keyboard.state == TRUE)
        {
            Disp_Flag=1;
			ButtonSound();
			
            key=Key_Read();
			
            if(key!=KEY_NONE)
				switch(key)
				{
                    case Key_F1:
                        
                        switch(keynum)
                        {
                            case 0:
                                
								keynum=0;
									
                            break;
                            case 1:
                                Jk516save.Set_Data.trip=0;
                            
                                break;
                            case 2:
                                
                                Coordinates.xpos = DISPX1;
                                Coordinates.ypos = FIRSTLINE+HIGH1+10+5;
                                Coordinates.lenth = RECT;
							
                                Jk516save.Set_Data.High_Res = Disp_Set_Num(&Coordinates);
							
                            break;
                            case 3:
                                Coordinates.xpos=DISPX1;
                                Coordinates.ypos=FIRSTLINE+HIGH1*2+20+5;
                                Coordinates.lenth=RECT;
                                Jk516save.Set_Data.Res_low = Disp_Set_Num(&Coordinates);
                                
                            break;
    						case 4:
                                Jk516save.Set_Data.Range=RANGE_MAX;    //0  自动  10   最佳量程
                                Jk516save.Set_Data.Range_Set=0;
   
                                break;
                            case 5:
                                Coordinates.xpos = DISPX2-16;
                                Coordinates.ypos = FIRSTLINE+HIGH1*1+10+5;
                                Coordinates.lenth = RECT+8;
                                Jk516save.Set_Data.V_high=Disp_Set_CompNum(&Coordinates);
                                
                            break;
                            
                            
                            case 6:
                                Coordinates.xpos = DISPX2-16;
                                Coordinates.ypos = FIRSTLINE+HIGH1*2+20+5;
                                Coordinates.lenth = RECT+8;
                                Jk516save.Set_Data.V_low=Disp_Set_CompNum(&Coordinates);
                                
                            break;
						
                            default:
                                break;
                        
                        
                        }

                    break;
                    case Key_F2:
                        switch(keynum)
                        {
                            case 0:
                                
    							SetSystemStatus(SYS_STATUS_SETUP);
                                break;
                            case 1:
    							Jk516save.Set_Data.trip=1;
								test_start = 0;
                                
                                break;
  					
    						case 4:
    							
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                          

    							break;
 
                            
                            default:
                                break;
                        
                        
                        }
 
                    break;
                    case Key_F3:
                        switch(keynum)
                        {
                            case 0:
								if(recordflag == 0)
								{
									recordflag = 1;
//									Drawhomemenu();
//									count = 0;
									indexflag = 1;
									Disp_Button_value1(0);
								}else if(recordflag == 1){
									Write_His_Data_Man();
									recordflag = 0;
//									Drawhomemenu();
									sdcount = 0;
									Write_Block_Rec();
									Disp_Button_value1(0);								
								}
                                break;
					
    						case 4://MAX_R_RANGE
                                Jk516save.Set_Data.Range_Set=2;//增加档位选择的计算
                                Range=Jisuan_Range(Jk516save.Set_Data.Nominal_Res);
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                

    							break;

                            
                            default:
                                break;
                        
                        
                        }
                    break;
                    case Key_F4:
                        switch(keynum)//
                        {
                           case 0:
//    								SetSystemStatus(SYS_STATUS_SYS);
								if(holdflag == 0)
								{
									holdflag = 1;
								}else{
									holdflag = 0;
								}
                                break;
                            case 4:
                                if(Range>=RANGE_MAX)
                                    Range=0;
                                else
                                    Range++;
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                
                            break;
  
                            default:
                                break;
                        
                        
                        }	
                    break;
                    case Key_F5:
                        switch(keynum)
                        {
                            case 0:
								
                                 SetSystemStatus(SYS_STATUS_CLEAR);
                                break;
							case 4:
                                if(Range<=0)
                                    Range=RANGE_MAX;
                                else
                                    Range--;
                                Jk516save.Set_Data.Range_Set=1;
                                Jk516save.Set_Data.Range=Range;
                                Range_Control(Range);
                                
                            break;
                            default:
                                break;
                        }
  
                    break;
                    case Key_Disp:
                    break;
                    case Key_SETUP://ENTER
						test_start=1;
                    break;
                    case Key_FAST:
						keynum=0;
                    break;
                    case Key_LEFT:
                        if(keynum<1)
                            keynum=6;
                        else
                            keynum--;
                      
                    break;
                    case Key_RIGHT:
                        if(keynum>5)
                            keynum=0;
                        else
                            keynum++;
                       
                 
                            
                    break;
                    case  Key_UP:
                        if(keynum<1)
                            keynum=6;
                        else
                            keynum--;
                        
                    break;
                    case Key_DOWN:
                        if(keynum>5)
                            keynum=0;
                        else
                            keynum++;
                    break;
                    
                    case Key_NUM1:
                    //break;
                    case Key_NUM2:
                    //break;
                    case Key_NUM3:
                    //break;
                    case Key_NUM4:
                    //break;
                    case Key_NUM5:
                    //break;
                    case Key_NUM6:
                    //break;
                    case Key_NUM7:
                    //break;
                    case Key_NUM8:
                    //break;
                    case Key_NUM9:
                    //break;
                    case Key_NUM0:
                    break;
                    case Key_DOT:

                    break;
                    case Key_BACK:
                        
                    break;
                    case Key_LOCK:


                    break;
                    case Key_BIAS:
                        
                    break;
                    case Key_REST:
                    break;
                    case Key_TRIG:
                        
                        
                    break;
                    default:
                        SetSystemStatus(SYS_STATUS_TEST);
                    break;
                        
                }
			Store_set_flash();
         }
     }
    
   // f_mount(NULL, "0:", 0);
}
//==========================================================
//函数名称：TestAbort_Process
//函数功能：测试中止处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.29 13:35
//备注说明：无
//==========================================================
//void TestAbort_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Plc_Start_Off();//关远控启动

// 	//讯响控制
////	if(SaveData.Parameter.Beep)
////		Beep_One();//响一声

//	//显示信息
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	
//	//DAC_SetChannel1Data(DAC_Align_12b_R,0);
////	Led_HV_Off();//关测试灯
////	delay_ms(30);
////	Electro_Test(0);
////	delay_ms(30);
////	
////	Electro_discharge(0);
////	delay_ms(30);
////	BeepOff();
////	
//	while(GetSystemStatus()==SYS_STATUS_TEST_ABORT)
//	{
////		Uart_Process();//串口处理
//		
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/20);//等待按键(100*10ms/20=50ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;

//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试开始
//				break;
//	
//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
////				break;
//			
//			default:
//				break;
//		}
//	}
//	Test_value.Test_Time=0;
//}
//==========================================================
//函数名称：TestFinish_Process
//函数功能：测试结束处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 13:38
//备注说明：无
//注意事项：无
//==========================================================
//void TestFinish_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Led_HV_Off();//关闭高压灯
////	Plc_Start_Off();//关远控启动
//	//显示信息
//	//SetSystemMessage(MSG_PASS);
//	
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	//分选讯响控制

//	
//	Electro_discharge(0);
//	while(GetSystemStatus()==SYS_STATUS_TEST_FINISH)
//	{
////		Uart_Process();//串口处理
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/50);//等待按键(100*10ms/50=20ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				SetSystemStatus(SYS_STATUS_SETUP);//设置状态
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试状态
//				break;
//	
//			case KEY_ENTER:		//确认键
////			case L_KEY_ENTER:	//长按确认键
////				SetSystemStatus(SYS_STATUS_IDLE);//待测状态
////				break;
////	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//待机状态
////				break;
//			
//			default:
//				break;
//		}
//	}
//}	
void Electro_discharge(u8 data)//data=1  测试  0  放电
{
	u8 value;
	value=U16_4094;
	if(!data)
		value|=HW_GYBFD_ON;
	else
		value&=HW_GYBFD_OFF;
	U16_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(30);

}
void Electro_Test(u8 data)
{
	u8 value;
	value=U15_4094;
	if(data)
		value|=HW_FD_TEST;
	else
		value&=HW_FD_NOTEST;
	U15_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(30);

}

void Feedback_RY(u8 dat)//反馈电阻
{
	
	//U16_4094
	u8 value;
	value=U15_4094;
	//value&=HW_GYB_1000VFEEDMASK;
	switch(dat)
	{
		case 0:
			value|=HW_GYB_1000VFEED;
			break;
		case 1:
			value&=HW_GYB_100VFEED;
			break;
		
		default:
			//value&=HW_GYB_1000VFEED;
			break;
	}
	U15_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);

}
//电压采样电阻切换
//输入 ： 0 1 2 3
// 0  1000V  1  100V  2  10V
//
//
//
void V_Samping(u8 dat)
{
	vu8 value;
	value=U16_4094;
	switch(dat)
	{
		case 0:
			value&=HW_GYB_100V;
			break;
		case 1:
			value|=HW_GYB_1000V;
			break;
		
			
		default:
			//value&=HW_GYB_1000V;
			break;
	}
	U16_4094=value;
	_4094_data( );
	Out_4094(_4094_databuff);
	delay_ms(20);

}

//用户校正
void Use_DebugProcess(void)
{
	
	vu32 keynum=0;
    float a,b;
	vu8 key;
//    u8 Disp_Testflag=0;
    vu8 range_flag=0;
//    float i_mid;
//	float ddd,eee;
	Disp_Coordinates_Typedef Coordinates;
    Disp_Coordinates_Typedef Debug_Cood;
    vu8 i;
	vu8 Disp_flag=1;
    vu8 list=0;
 //   uint32_t  scan_I[200],scan_V[200];
    test_start=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_UserCheck_Item();
//    Debug_stanedcomp();//校正值比较
    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
    NVIC_EnableIRQ(EXTI3_IRQn);
    Range_Control(0);
 	while(GetSystemStatus()==SYS_STATUS_DEBUG)
	{

         
//        read_adI_1();//
//        Range_value=I_ad;

//        Select_V_I(0);
//        
//        read_adV_1();
//        Range_Value_V=V_ad;
        
        Select_V_I(1);
        delay_ms(15);
        read_adI_4();//
        Range_value=disp_I;
        Select_V_I(0);
         delay_ms(15);;
		read_adV_1();
        Range_Value_V=disp_V;


		if(test_start)
		{
            {
               // test_start=0;
                Ad_over=0; 
                //先判断极性
                //根据极性 来确定换挡值
                //再计算电阻
                
                if(disp_I<0)
                {
                    polarity_r=0;//﹣
                    disp_I=-disp_I;
                }
                else
                {
                    polarity_r=1;
                    
                                        
                }
                
                disp_I=disp_I-Jk516save.Clear[list-1];//再判断极性

                if(disp_V<0)
                {
                    disp_V=-disp_V;
                    
                }
                if(list>4)
//				disp_V=disp_V;
				if(polarity_v == 0)
				{
					disp_V=disp_V+Jk516save.Clear_V[list-5];
				}else{
					disp_V=disp_V-Jk516save.Clear_V[list-5];
				}
                //V_ad=V_ad/45;
                {
                    Test_Value_V=V_Datacov(disp_V ,V_Range);//把数据的小数点和单位 和极性都加上
                    Test_Value=Datacov(disp_I,list-1);
                    Disp_Debug_Reference(list,Test_Value_V,Test_Value);
                    
                }

               
            }

            
		}else
		if(Disp_flag==1)//不启动的时候
		{
			Disp_Debug_value(list);
			Disp_flag=0;	
		}
    
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		
		if(key!=KEY_NONE)
		{
			Disp_flag=1;
			//Key_Beep();
			switch(key)
			{
				case Key_F1:
                    test_start=1;

				break;
				case Key_F2:

				break;
				case Key_F3:

				break;
				case Key_F4:
                    if(list==0)
                    {
                        Store_cal_flash();
                        SetSystemStatus(SYS_STATUS_TEST);
                    }

				break;
				case Key_F5:
					  test_start=0;
                    if(list==0)
                    {
                        LCD_Clear(LCD_COLOR_TEST_BACK);
                        
                        Debug_Cood.xpos=70;
                        Debug_Cood.ypos =272-70;
                        Debug_Cood.lenth=120;
                        input_num(&Debug_Cood);
                         LCD_Clear(LCD_COLOR_TEST_BACK);
                        Disp_UserCheck_Item();
                    
						
                    }
				break;
				case Key_Disp:
                    if(list)
                        test_start=1;
                    
					//Savetoeeprom();
                    //SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
//                        test_start=0;
//                    if(list==0)
//                    {
//                        LCD_Clear(LCD_COLOR_TEST_BACK);
//                        
//                        Debug_Cood.xpos=70;
//                        Debug_Cood.ypos =272-70;
//                        Debug_Cood.lenth=120;
//                        input_num(&Debug_Cood);
//                         LCD_Clear(LCD_COLOR_TEST_BACK);
//                        Disp_UserCheck_Item();
//                    
//                    }

				break;
				
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_UP:
                    if(test_start==0)
                        {
                    open_flag=1;
                    
                    if(test_start==0)
                    {
                        if(list<1)
                            list=DEBUG_RANGE;
                        else
                            list--;
                        if(list)
                        {
                            if(list<5)
                            {
                                Range_Control(list-1);
                                Range=list-1;
                                
                            }
                            else
                            {
//                                V_Range=DEBUG_RANGE-list-1;
                                V_Range_Control(list-5);
                                
                            }
                        }
                    
                    }
                }

							
				break;
				case Key_DOWN:
                    if(test_start==0)
                    {
                        open_flag=1;
                        if(list<DEBUG_RANGE)
                            list++;
                        else
                            list=0;
                        if(list)
                        {
                            if(list<5)
                            {
                                Range_Control(list-1);
                                Range=list-1;
                                
                            }
                            else
                                
                                {
                                   V_Range_Control(list-5);
                                }
                        }
                    }
					
				break;
					case Key_DOT:
						break;
				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
				break;
				case Key_FAST:
                    
                
                                
                                
                                //Jk516save.Set_Data.High_Res=Disp_Set_Num(&Coordinates);
                
				Coordinates.xpos=LIST1+160;
				Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list);
				Coordinates.lenth=70;
                if(test_start)
                {
                    if(list<5)
                    {
						if(list == 3)
						{
							Jk516cal.Debug_Value[list-1].standard=Debug_Set_Res(&Coordinates);//电阻
						}else{
							Jk516cal.Debug_Value[list-1].standard=Debug_Set_Res(&Coordinates)/10;//电阻
						}
                        Jk516cal.Debug_Value[list-1].ad_value=(float)Test_Value.res/Jk516cal.Debug_Value[list-1].standard;
                    }
                    else
                    {
                      Jk516cal.Debug_Value[list-1].standard=Debug_Set_Num(&Coordinates);//电压
                        Jk516cal.Debug_Value[list-1].ad_value=(float)Test_Value_V.res/Jk516cal.Debug_Value[list-1].standard;
                        
                    }
                    
                }
                test_start=0;

				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
                    
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}
void Clear_Process(void)
{
//    u32 i=0;
    u8 range_v,range;;
    u8 list=0;
    u8 Clear_num=0;
   // float v_ad,i_ad;
    //u32 scan_I[50],scan_V[50];
    range=Jk516save.Set_Data.Range;
    Range_Control(0);
	delay_ms(80);
    Disp_Range(0,list);
    while(GetSystemStatus()==SYS_STATUS_CLEAR)
    {
		delay_ms(80);
         Select_V_I(1);
        delay_ms(80);
         read_adI_4();//
                           
		delay_ms(80);				
         Select_V_I(0);
                        
        if(list==0)
            V_Range_Control(0);
        else
            V_Range_Control(1);
        delay_ms(80);
        read_adV_1();

            Jk516save.Clear[list]=disp_I;
            if(list<2)
            {
				if(polarity_v == 0)
				{
					Jk516save.Clear_V[list]=-disp_V;
				}else{
					Jk516save.Clear_V[list]=disp_V;
				}
                range_v=list;
                
            }
            Range_Control(list+1);
            Disp_Range(0,list);
        
            if(list>=RANGE_MAX)
            {
                SetSystemStatus(SYS_STATUS_TEST);
                //SaveData.Parameter.Range=range;
                Jk516save.Set_Data.Range=range;
                Store_set_flash();
                
            }
            list++;
        
    
    }

}
//系统设置
void Use_SysSetProcess(void)
{	

	vu8 key,i;

    char key_count=0;
	vu8 Disp_flag=1;

    u8 keynum=0;
    RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
    LCD_Clear(LCD_COLOR_TEST_BACK);

	Disp_Sys_Item();

 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  	powerstat = PowerOffDetect();
		if(powerstat == 0)
		{
			PowerOffHandle();
		}
		if(Disp_flag==1)
		{
           
            
            Jk516save.Sys_Setvalue.textname[7]=0;
			Disp_Sys_value(keynum);
			Disp_flag=0;
            
		}
//		tp_dev.scan(0); 		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
//		{
//			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
//			{	
//				
//				for(i=0;i<5;i++)
//				{
//					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
//						touch_value=i;
//					else
//						touch_value=0xff;
//				
//					switch(touch_value)
//					{
//						case 0:
//							SetSystemStatus(SYS_STATUS_TEST);
//							ButtonSound();
//							break;
//						case 1:
//							SetSystemStatus(SYS_STATUS_SETUP);
//							ButtonSound();
//							break;
//						case 2:
//							//SetSystemStatus(SYS_STATUS_SYSSET);
//							break;
//						case 3:
//							SetSystemStatus(SYS_STATUS_SYS);
//							ButtonSound();
//							break;
//						case 4:
//							break;
//						default:
//							break;
//					
//					
//					}
//				}
//				
//			}
//		
//		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		if(key!=KEY_NONE)
		{
			ButtonSound();
			Disp_flag=1;
           //Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						case 1:
							Jk516save.Sys_Setvalue.uart=0;
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=0;
							break;
						case 3:
                            Jk516save.Sys_Setvalue.u_flag=0;
							
							break;
						case 4:
							Jk516save.Sys_Setvalue.plc=0;
							break;
						case 5:
							Jk516save.Sys_Setvalue.lanage=0;
                            LCD_Clear(LCD_COLOR_TEST_BACK);
                            Disp_Sys_Item();
							break;
						case 6:
							if(RTC_DateStructure.RTC_Year<1)
								RTC_DateStructure.RTC_Year=RTC_YEAR_MAX;
							else
                                if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
                                    RTC_DateStructure.RTC_Year=0;
                                else
								RTC_DateStructure.RTC_Year--;
                           // RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
                            //RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
                            //RTC_CalibCounterCmd(LPC_RTC, ENABLE);
                                RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                                RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month<=1)
								RTC_DateStructure.RTC_Month=RTC_MONTH_MAX;
							else
								RTC_DateStructure.RTC_Month--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date<=1)
								RTC_DateStructure.RTC_Date=RTC_DAYOFMONTH_MAX;
							else
								RTC_DateStructure.RTC_Date--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
                           
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours<1)
								RTC_TimeStructure.RTC_Hours=RTC_HOUR_MAX;
							else
								RTC_TimeStructure.RTC_Hours--;
//                            
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes<1)
								RTC_TimeStructure.RTC_Minutes=RTC_MINUTE_MAX;
							else
								RTC_TimeStructure.RTC_Minutes--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds<1)
								RTC_TimeStructure.RTC_Seconds=RTC_SECOND_MAX;
							else
								RTC_TimeStructure.RTC_Seconds--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 12://
							//SaveData.Sys_Setup.Bias=0;
							break;
						case 18:
							//SaveData.Sys_Setup.Bus_Mode=0;
							break;
						case 13:
//							if(SaveData.Sys_Setup.GP_Addr<1)
//								SaveData.Sys_Setup.GP_Addr=99;
//							else
//								SaveData.Sys_Setup.GP_Addr--;
							break;
//						case 14:
//							SaveData.Sys_Setup.Talk_Only=1;
//							break;
//						case 15:
//							if(SaveData.Sys_Setup.Timer_Value.Hour<1)
//								SaveData.Sys_Setup.Timer_Value.Hour=12;
//							else
//								SaveData.Sys_Setup.Timer_Value.Hour--;
//							break;
//						case 16:
//							if(SaveData.Sys_Setup.Timer_Value.Min<1)
//								SaveData.Sys_Setup.Timer_Value.Min=59;
//							else
//								SaveData.Sys_Setup.Timer_Value.Min--;
//							break;
//						case 17:
//							if(SaveData.Sys_Setup.Timer_Value.Sec<1)
//								SaveData.Sys_Setup.Timer_Value.Sec=59;
//							else
//								SaveData.Sys_Setup.Timer_Value.Sec--;
//							break;
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						case 1:
							Jk516save.Sys_Setvalue.uart=1;
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=1;
							break;
						case 3:
							Jk516save.Sys_Setvalue.u_flag=1;
							break;
						case 4:
							Jk516save.Sys_Setvalue.plc=1;
							break;
						case 5:
							Jk516save.Sys_Setvalue.lanage=1;
                            LCD_Clear(LCD_COLOR_TEST_BACK);
                            Disp_Sys_Item();
							break;
						case 6:
							if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
								RTC_DateStructure.RTC_Year=0;
							else
								RTC_DateStructure.RTC_Year++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month>=RTC_MONTH_MAX)
								RTC_DateStructure.RTC_Month=1;
							else
								RTC_DateStructure.RTC_Month++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date>=RTC_DAYOFMONTH_MAX)
								RTC_DateStructure.RTC_Date=1;
							else
								RTC_DateStructure.RTC_Date++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours>=RTC_HOUR_MAX)
								RTC_TimeStructure.RTC_Hours=0;
							else
								RTC_TimeStructure.RTC_Hours++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes>=RTC_MINUTE_MAX)
								RTC_TimeStructure.RTC_Minutes=0;
							else
								RTC_TimeStructure.RTC_Minutes++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds>=RTC_SECOND_MAX)
								RTC_TimeStructure.RTC_Seconds=0;
							else
								RTC_TimeStructure.RTC_Seconds++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;

						default:
							break;
					
					}
                    
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=2;
							
						break;
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 2:
							Jk516save.Sys_Setvalue.buard=3;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_F5:
					switch(keynum)
					{
						case 0:
							break;//恢复系统复位
						case 2:
							Jk516save.Sys_Setvalue.buard=4;
							
						break;
						default:
						break;
					
					}
				break;
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUP);
				break;
				case Key_FAST:
//                    if(keynum==12)
//                    {
                        //dispflag=0;
//                        for(i=0;i<8;i++)
//                        {
//                            Save_Res.Sys_Setvalue.textname[i]=Disp_buff[i];
//                        
//                        }
                        key_count=0;
                        keynum=0;
                        //Savetoeeprom();
//                    }
				break;
				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;
				case Key_DOWN:
					if(keynum>SYS_MAX-1)
						keynum=0;
					else
						keynum++;
						
				break;
				case Key_UP:
					if(keynum<1)
						keynum=SYS_MAX;
					else
						keynum--;
				break;
				case Key_NUM1:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='1';
                            key_count++;
                                
                        }
                    
                    }
                        //Save_Res.Sys_Setvalue
				break;
				case Key_NUM2:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='2';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM3:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='3';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM4:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='4';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM5:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='5';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM6:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='6';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM7:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='7';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM8:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='8';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM9:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='9';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_NUM0:
                    if(keynum==12)
                    {
                        if(key_count<PASSWORD_LENTH-1)
                        {
                            Jk516save.Sys_Setvalue.textname[key_count]='0';
                            key_count++;
                                
                        }
                    
                    }
				break;
				case Key_DOT:
				break;
				case Key_BACK:
				break;
				case Key_LOCK:
				break;
				case Key_BIAS:
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		Store_set_flash();
		}
	
	
	}
    
}

void Sys_Process(void)
{
	vu32 keynum=0;
	vu8 key,i;
	vu8 Disp_flag=1;
    LCD_Clear(LCD_COLOR_TEST_BACK);

	while(GetSystemStatus()==SYS_STATUS_SYS)
	{
		powerstat = PowerOffDetect();
		if(powerstat == 0)
		{
			PowerOffHandle();
		}
		if(Disp_flag==1)
		{
			Disp_Sys();
			Disp_flag=0;	
		}
//		tp_dev.scan(0); 		 
//		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
//		{
//			
//			if(tp_dev.x[0]<640&&tp_dev.y[0]<480)
//			{	

//				for(i=0;i<5;i++)
//				{
//					if((tp_dev.x[0]>(24+BUTTON_W*i))&&(tp_dev.x[0]<(24+BUTTON_W*(i+1)))&&(tp_dev.y[0]>BUTTON_1))
//						touch_value=i;
//					else
//						touch_value=0xff;
//				
//					switch(touch_value)
//					{
//						case 0:
//							SetSystemStatus(SYS_STATUS_TEST);
//							ButtonSound();
//							break;
//						case 1:
//							SetSystemStatus(SYS_STATUS_SETUP);
//							ButtonSound();
//							break;
//						case 2:
//							SetSystemStatus(SYS_STATUS_SYSSET);
//							ButtonSound();
//							break;
//						case 3:
//							
//							break;
//						case 4:
//							break;
//						default:
//							break;
//					
//					
//					}
//				}
//				
//			}
//		
//		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		if(key!=KEY_NONE)
		{
			ButtonSound();
			Disp_flag=1;
//            Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						
					
					}
                    
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						
							
						
						default:
						break;
					
					}
				break;
				
				
				case Key_Disp:
                    SetSystemStatus(SYS_STATUS_TEST);
				break;
				case Key_SETUP:
                    SetSystemStatus(SYS_STATUS_SETUP);
				break;
				
				
				default:
				break;
					
			}
		
		
		}
	
	}

}

static void MODS_03H(void){
	
  uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[64];
  uint8_t TxCount = 0;
	uint16_t CRC_num;

    
  uint8_t Rsp_Code = RSP_OK;

	reg = BEBufToUint16(&usbbuf[2]); 				
	num = BEBufToUint16(&usbbuf[4])*2;					
	if (num > sizeof(reg_value) / 2)
	{
		Rsp_Code = RSP_ERR_VALUE;					
		goto err_ret;
	}

	for (i = 0; i < num; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	
		{
			Rsp_Code = RSP_ERR_REG_ADDR;				
			break;
		}
		reg++;
	}

err_ret:
	if (Rsp_Code == RSP_OK)							/* ???? */
	{
		TxCount = 0;
		send_usbbuf[TxCount++] = usbbuf[0];
		send_usbbuf[TxCount++] = usbbuf[1];
		send_usbbuf[TxCount++] = num * 2;			/* ????? */

		for (i = 0; i < num; i++)
		{
			send_usbbuf[TxCount++] = reg_value[2*i];
			send_usbbuf[TxCount++] = reg_value[2*i+1];
		}
		
		CRC_num = CRC16( send_usbbuf, 15);
		
		send_usbbuf[15] = CRC_num>>8;
		send_usbbuf[16] = CRC_num;
		
		USBD_HID_SendReport( &USB_OTG_dev, send_usbbuf, 64); 
		
    
	}
}






uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}


uint16_t CRC16(uint8_t *_pBuf, uint16_t _usLen)
{    
    uint8_t ucCRCHi = 0xFF; /* 高CRC字节初始化 */
	  uint8_t ucCRCLo = 0xFF; /* 低CRC 字节初始化 */
	  uint16_t usIndex;  /* CRC循环中的索引 */

    while (_usLen--)
    {
		usIndex = ucCRCHi ^ *_pBuf++; /* 计算CRC */
		ucCRCHi = ucCRCLo ^ s_CRCHi[usIndex];
		ucCRCLo = s_CRCLo[usIndex];
    }
    return ((uint16_t)ucCRCHi << 8 | ucCRCLo);
}

static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
    uint16_t value;
	static u32 sendunit;
	static u32 sendrvalue;
	static u32 sendvvalue;
	
	switch(Range)
	{
		case 0:
		{
			sendrvalue = Test_Value.res;
			sendunit = 0;
		}break;
		case 1:
		{
			sendrvalue = Test_Value.res;
			sendunit = 0;
		}break;
		case 2:
		{
			sendrvalue = Test_Value.res*10;
			sendunit = 0;
		}break;
		case 3:
		{
			sendrvalue = Test_Value.res/10;
			sendunit = 1;
		}break;
		case 4:
		{
			sendrvalue = Test_Value.res;
			sendunit = 1;
		}break;
		case 5:
		{
			sendrvalue = Test_Value.res;
			sendunit = 1;
		}break;
		case 6:
		{
			sendrvalue = Test_Value.res/10;
			sendunit = 2;
		}break;
		
	}
	sendvvalue = Test_Value_V.res*10;
	if(open_flag == 1)
	{
		sendrvalue = 0xffffffff;	
		sendvvalue = 0xffffffff;
	}
//	lock = 1;
	switch (reg_addr)									/* ??????? */
	{
        case SLAVE_REG_P00:
			value = (u16)(sendrvalue >> 16);	
			break;
		case SLAVE_REG_P01:
            value = (u16)(sendrvalue);	
			break;

		case SLAVE_REG_P02:
			value = (u16)(sendvvalue >> 16);
			break;
		case SLAVE_REG_P03: 
			value = (u16)(sendvvalue);
			break;

		case SLAVE_REG_P04:
			value = Test_Value_V.polar;		
			break;
		case SLAVE_REG_P05:
			value = sendunit;
			break;
		case SLAVE_REG_P06:
			value = 0;						
			break;
		case SLAVE_REG_P07:
			value =	0;							
			break;
		case SLAVE_REG_P08:
			value =	0;	
			break;

		case SLAVE_REG_P09:
			value =	0;							
			break;
		case SLAVE_REG_P10:
			value =	0;	
			break;

		case SLAVE_REG_P11:
			value =	0;
			break;
		case SLAVE_REG_P12:
			value =	0;	
			break;

		case SLAVE_REG_P13:
			value =	0;						
			break;
		case SLAVE_REG_P14:
			value =	0;
			break;

		case SLAVE_REG_P15:
			value =	0;
			break;
		case SLAVE_REG_P16:
			value =	0;	
			break;
		case SLAVE_REG_P17:
			value =	0;							
			break;
		case SLAVE_REG_P18:
			value =	0;							
			break;
		default:
			return 0;
    }
	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											
}
