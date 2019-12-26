#ifndef _KEYBOARD_H
#define _KEYBOARD_H
//==========================================================
//调试配置
#define _HW_CONFIG_FALSE	(0)
#define _HW_CONFIG_TRUE		(1)

#define TICKS_PER_SEC 100	//系统时钟每秒节拍数，100x10mS(T0)=1S
#define TICKS_PER_SEC_SOFTTIMER 100	//软定时器时钟每秒节拍数，100x10mS=1S
//Lcd是否支持持续按键
#define HW_KEYBOARD_CONTINUE_SUPPORT	(_HW_CONFIG_TRUE)

//==========================================================
//按键常数定义

#define 	Key_F1  	21
#define		Key_F2  	11
#define 	Key_F3  	16
#define 	Key_F4  	20
#define		Key_F5 		13
#define		Key_Disp 	 22//9ESC
#define		Key_SETUP	 17//ENT
#define		Key_FAST	18
#define		Key_LEFT	23//
#define		Key_RIGHT  	12//
#define		Key_UP		15
#define		Key_DOWN	25
#define		Key_NUM7	'7'
#define		Key_NUM8	'8'
#define		Key_NUM9	'9'
#define		Key_NUM4	'4'
#define		Key_NUM5	'5'
#define		Key_NUM6	'6'
#define		Key_NUM1	'1'
#define		Key_NUM2	'2'
#define		Key_NUM3	'3'
#define		Key_NUM0	'0'
#define		Key_DOT		14
#define		Key_BACK	10
#define		Key_LOCK	(60)//
#define		Key_BIAS	(61)
#define		Key_REST	(62)
#define		Key_TRIG	(63)
#define		Key_POWER	(64)//



#define _Hc164CpL GPIO_ResetBits(GPIOC,GPIO_Pin_8)//GPIO_ClearValue(1, (1<<1))
#define _Hc164CpH GPIO_SetBits(GPIOC,GPIO_Pin_8)//GPIO_SetValue(1, (1<<1))
#define _Hc164DL GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define _Hc164DH GPIO_SetBits(GPIOC,GPIO_Pin_12)


#define LONGKEYCOUNT 	(TICKS_PER_SEC)	//长按键节拍数(1s)
//#define SHORTKEYCOUNT 	(TICKS_PER_SEC/20)	//短按键节拍数(50ms)
#define SHORTKEYCOUNT 	(TICKS_PER_SEC/30)	//短按键节拍数(33ms)
#define CONTINUEKEYCOUNT (TICKS_PER_SEC/7)	//持续按键节拍数(100ms)
//键值定义
#define HW_KEY_SET			((u16)(ClrD5))
#define HW_KEY_UP			((u16)(ClrD3))
#define HW_KEY_DOWN			((u16)(ClrD4))
#define HW_KEY_LEFT			((u16)(ClrD7))
#define HW_KEY_RIGHT		((u16)(ClrD8))
#define HW_KEY_ENTER		((u16)(ClrD6))
#define HW_KEY_RESET		((u16)(ClrD2))//pc4
#define HW_KEY_START		((u16)(ClrD1))
#define HW_KEY_RESET1		((u16)(ClrD0))//pc4
#define HW_KEY_START1		((u16)(ClrD15))

#define P1_PIN                  GPIO_Pin_9                 
#define P1_GPIO_PORT            GPIOH                      
#define P1_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P2_PIN                  GPIO_Pin_10                 
#define P2_GPIO_PORT            GPIOH                      
#define P2_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P3_PIN                  GPIO_Pin_5                
#define P3_GPIO_PORT            GPIOC                      
#define P3_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P4_PIN                  GPIO_Pin_11                 
#define P4_GPIO_PORT            GPIOH                      
#define P4_GPIO_CLK             RCC_AHB1Periph_GPIOH

#define P5_PIN                  GPIO_Pin_1                 
#define P5_GPIO_PORT            GPIOI                      
#define P5_GPIO_CLK             RCC_AHB1Periph_GPIOI

#define P6_PIN                  GPIO_Pin_3                 
#define P6_GPIO_PORT            GPIOI                      
#define P6_GPIO_CLK             RCC_AHB1Periph_GPIOI
//#define P6_EXTI_PORTSOURCE      EXTI_PortSourceGPIOI
//#define P6_EXTI_PINSOURCE       EXTI_PinSource3
//#define P6_EXTI_LINE            EXTI_Line3
//#define P6_EXTI_IRQ             EXTI0_IRQn

//#define P6_IRQHandler           EXTI0_IRQHandler

/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0
/*******************************************************/

 /** 键值
	* 
	*/



//按键编码
#define LONG_PRESS		((u8)0x80)
#define KEY_NONE		((u8)0xff)
#define KEY_SET			((u8)('a'))
//#define KEY_UP			((u8)('b'))
//#define KEY_DOWN		((u8)('c'))
//#define KEY_LEFT		((u8)('d'))
//#define KEY_RIGHT		((u8)('e'))
//#define KEY_ENTER		((u8)('f'))
#define KEY_RESET		((u8)('g'))
#define KEY_START		((u8)('h'))

#define L_KEY_SET		(LONG_PRESS | KEY_SET)		//设置
#define L_KEY_UP		(LONG_PRESS | KEY_UP)		//上
#define L_KEY_DOWN		(LONG_PRESS | KEY_DOWN)		//下
#define L_KEY_LEFT		(LONG_PRESS | KEY_LEFT)		//左
#define L_KEY_RIGHT		(LONG_PRESS | KEY_RIGHT)	//右
#define L_KEY_ENTER		(LONG_PRESS | KEY_ENTER)	//确认
#define L_KEY_RESET		(LONG_PRESS | KEY_RESET)	//复位
#define L_KEY_START		(LONG_PRESS | KEY_START)	//启动
//==========================================================
//按键结构体声明
typedef struct 
{
	u8 value;	//键值
	u8 state;	//键有按下=TRUE
	u8 continuance;//持续按键有效=TRUE
}Key_TypeDef;

extern  uint32_t CurrentTextColor   ;
extern  uint32_t CurrentBackColor   ;
//==========================================================
//全局变量
extern Key_TypeDef Keyboard;//键码结构体

//==========================================================
//函数声明
extern u8 Key_Identiry(void);//按键识别，返回键值
extern void Keyboard_Init(void);//按键结构体初始化
extern void Key_Scan(void);//按键扫描
extern u8 Key_Read(void);//读取按键，返回键值
extern u8 Key_Read_WithTimeOut(u8 timeticks);//阻塞式读取按键，返回键值


#endif 
