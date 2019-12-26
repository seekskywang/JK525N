#ifndef _gpio_H
#define _gpio_H


#define BATCAP_PIN4							GPIO_Pin_2
#define BATCAP_PORT4          				GPIOG
#define BATCAP_CLK4              			RCC_AHB1Periph_GPIOG
#define BATCAP_PINSOURCE4            		GPIO_PinSource2

#define BATCAP_PIN3							GPIO_Pin_3
#define BATCAP_PORT3          				GPIOG
#define BATCAP_CLK3              			RCC_AHB1Periph_GPIOG
#define BATCAP_PINSOURCE3            		GPIO_PinSource3

#define BATCAP_PIN2							GPIO_Pin_6
#define BATCAP_PORT2          				GPIOC
#define BATCAP_CLK2              			RCC_AHB1Periph_GPIOC
#define BATCAP_PINSOURCE2            		GPIO_PinSource6

#define BATCAP_PIN1							GPIO_Pin_6
#define BATCAP_PORT1          				GPIOG
#define BATCAP_CLK1              			RCC_AHB1Periph_GPIOG
#define BATCAP_PINSOURCE1            		GPIO_PinSource6


void GPIO_Configuration(void);
void Key_beep(void);
void PGA_20mR(void);
void PGA_200mR(void);
void PGA_2R(void);
void PGA_20R(void);
void PGA_200R(void);
void PGA_2kR(void);
void PGA_20kR(void);
void PGA_200kR(void);//200K
void Led_HV_On(void);//开高压灯
void Led_HV_Off(void);//关高压灯

void Beep_On(void);
void Beep_Off(void);

void Trip_On(void);
void Trip_Off(void);

void Lock_On(void);
void Lock_Off(void);

void Turnon_Led(void);
void Turnoff_Led(void);

void Led_Pass_On(void);//合格灯

void Led_Fail_On(void);//不合格

void EXTI_Ads1251_Config(void);

void Select_V_I(u8 ch);//1  电流  0  电压
u8 Read_Openflag(void);
u8 read_cpld(void);
void Power_On_led(void);
void Power_Off_led(void);
void All_LedOff(void );
void V_I_amp(u8 amp);

void Disable_Extiint(void);
void Enable_Extiint(void);
void Close_Compled(void);
static void NVIC_Configuration_Gpioint(void);
void ReadBatGPIOInit(void);
#endif 
