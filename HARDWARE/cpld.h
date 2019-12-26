#ifndef _CPLD_H
#define _CPLD_H

#include "stm32f4xx.h"
#define CPLD_SCLK_H GPIO_SetBits(GPIOG,GPIO_Pin_14)
#define CPLD_SCLK_L GPIO_ResetBits(GPIOG,GPIO_Pin_14)

#define CPLD_DIN_H  GPIO_SetBits(GPIOD,GPIO_Pin_5)
#define CPLD_DIN_L  GPIO_ResetBits(GPIOD,GPIO_Pin_5)

void Send_To_Cpld(u8 *buff,u8 num);

void CPLD_GPIO_Configuration(void);
#endif
