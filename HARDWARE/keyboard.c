#include "pbdata.h"
//#include "touchscreen.h"

//==========================================================
//全局变量
Key_TypeDef Keyboard;//键码结构体

static u8 LastKeyVal;//上次键值
static u8 LastKeyTicks;//按键节拍
static u8 ContinueKeyTicks;//持续按键节拍
const u8 Scan_Value[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

void Key_Delay(u32 num)
{
    while(num)
    {
        num--;
    }
    

}
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(P1_GPIO_CLK|P2_GPIO_CLK|P3_GPIO_CLK|P4_GPIO_CLK|P5_GPIO_CLK|P6_GPIO_CLK,ENABLE);	/*开启按键GPIO口的时钟*/
	
//	 /* 配置 NVIC */
//	NVIC_Configuration();
	  
	GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/


  
	GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/       
	GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/       
	GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	

	GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/       
	GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/       
	GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P6_PIN;/*选择按键的引脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
	GPIO_Init(P6_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
}

void Keyboard_Init(void)
{
	
    
	Key_GPIO_Config();	
	Keyboard.value=KEY_NONE;//键值为空
	Keyboard.state=FALSE;	//按键无效
	Keyboard.continuance=FALSE;//持续按键无效
	
	LastKeyVal=KEY_NONE;//上次键值
	LastKeyTicks=0;//按键节拍
	ContinueKeyTicks=0;//持续按键节拍
	Key_Up_flag=0;
}

void _164Delaay(void)
{
	u16 i;
	for(i=0;i<10;i--)
	;

};
void HW_Sendvalueto164(u8 value)
{
	uint8_t i;
	_Hc164CpL;
	for(i=0;i<8;i++)
	{
		if((value&0x80)==0x80)
			_Hc164DH;
		else
			_Hc164DL;
		_Hc164CpH;
		_164Delaay();
		_Hc164CpL;
		value<<=1;
        //_164Delaay();
	}

}



//==========================================================
//函数名称：Key_Read
//函数功能：读取按键
//入口参数：无
//出口参数：无
//创建日期：2014.09.29
//修改日期：2014.09.29 10:50
//备注说明：无
//==========================================================
u8 Key_Read(void)
{
	if (Keyboard.state)	//有键按下
	{
		Keyboard.state=FALSE;
		//Beep_One();	//蜂鸣器响一声
        Key_beep();
        
		return (Keyboard.value);
	}
	//可直接处理电源等通用按键
	return (KEY_NONE);	
}

//==========================================================
//函数名称：Key_Read_WithTimeOut
//函数功能：阻塞式读取按键
//入口参数：ticks:等待节拍数(50ms周期)
//出口参数：按键值
//创建日期：2014.09.29
//修改日期：2014.09.29 10:56
//备注说明：直到按键响应或限时到
//==========================================================
u8 Key_Read_WithTimeOut(u8 ticks)
{
   // u8 i=1;
//	if (ticks)//定时等待按键
//		{
			SetSoftTimer(KEYBOARD_SOFTTIMER,ticks);//设置按键延时周期(即软定时器设置,50ms周期)
				
//			while ((!GetSoftTimerOut(KEYBOARD_SOFTTIMER))&&(!Keyboard.state)) //计时未到并且无按键按下
//				{
////					RunOnKeyFree();//在按键等待过程中处理其他业务
//                    //i=GetSoftTimerOut(KEYBOARD_SOFTTIMER);
//				}
//		}
//	else//无限等待按键
//		{
//			while (!Keyboard.state) 
//				{
//					RunOnKeyFree();//在按键等待过程中处理其他业务
//				}			
//		}
	return Key_Read();//按键更新
}

//读触摸键值
u8 Touch_Identiry(u8 page,u8 item)//page是设置项  item是二级界面
{
//	u8 i;
	u8 touch_key=KEY_NONE;
//	if(page==SYS_STATUS_TEST)//测试界面FIRSTLINE+i*HIGH1
//	{
//		for(i=0;i<6;i++)
//		{
//			if(i<3)
//			{
//				if((XCOOR>DISPX1&&XCOOR<LIST2)&&((YCOOR>(FIRSTLINE+i*HIGH1))&&(YCOOR<(FIRSTLINE+(i+1)*HIGH1))))
//					return 0x70+i;
//			}
//			else
//			{
//				if((XCOOR>DISPX1&&XCOOR<LIST2)&&((YCOOR>(FIRSTLINE+(i-3)*HIGH1))&&(YCOOR<(FIRSTLINE+(i-3+1)*HIGH1))))
//					return 0x70+i;
//			
//			
//			}
//		
//		}
//	
//	
//	}else if(page==SYS_STATUS_SYSSET)//系统设置
//	{
//	
//	}else if(page==SYS_STATUS_SYS)//系统信息
//	{
//	
//	}else if(page==SYS_STATUS_SETUP)//设置界面
//	{
//	
//	
//	}
	return touch_key;


}

//扫描键值
u8  Key_Identiry(void)
{
	u8 key_value;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/


  
	GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/       
	GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/
	GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	

	GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/       
	GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/       
	GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/

	
	GPIO_InitStructure.GPIO_Pin = P6_PIN;/*选择按键的引脚*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
	GPIO_Init(P6_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化          按键*/
	
	GPIO_SetBits(P1_GPIO_PORT,P1_PIN);/*P1脚置高*/
	GPIO_SetBits(P2_GPIO_PORT,P2_PIN);/*P2脚置高*/
	GPIO_SetBits(P3_GPIO_PORT,P3_PIN);/*P3脚置高*/
	GPIO_SetBits(P4_GPIO_PORT,P4_PIN);/*P4脚置高*/
	GPIO_SetBits(P5_GPIO_PORT,P5_PIN);/*P5脚置高*/
//	Key_Delay(0XFFFF);
	if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
	{
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			
//			Key_Delay(0x9FFFF);
			if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = 22;//ESC
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 12;//right////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = 17;//enter///////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = 23;//left////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 13;//FUNC_5
					return key_value;
				}
			}
		}
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*设置引脚速率为2MHz */    
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_SetBits(P2_GPIO_PORT,P2_PIN);/*P2脚置高*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
				
		Key_Delay(0xFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 20;//f4//////////
					return key_value;
				}
			}			
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 25;//down
					return key_value;
				}
				}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '0';//0/////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = 14;//dot
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 10;//BACKSPACE
					return key_value;
				}
			}
		}
				
			
			
			
			
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*设置引脚速率为2MHz */    
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_SetBits(P3_GPIO_PORT,P3_PIN);/*P2脚置高*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 11;//FUNC_3
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '2';//2///////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '5';//5///////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value ='8';//8///////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = 15;//up
					return key_value;
				}
			}
		}
			
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*设置引脚速率为2MHz */    
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_SetBits(P4_GPIO_PORT,P4_PIN);/*P2脚置高*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		Key_Delay(0XFF);
			
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 16;//FUNC_2
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '3';//3/////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 18;//ok////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN))
				{
					key_value = '9';//9///////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = '6';//6
					return key_value;
				}
			}
		}
			
		GPIO_InitStructure.GPIO_Pin = P5_PIN;/*选择按键的引脚*/     
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;/*设置引脚速率为2MHz */    
		GPIO_Init(P5_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_SetBits(P5_GPIO_PORT,P5_PIN);/*P2脚置高*/
		GPIO_InitStructure.GPIO_Pin = P1_PIN;/*选择按键的引脚*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;/*设置引脚为输入模式*/   
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;/*设置引脚下拉*/
		GPIO_Init(P1_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P2_PIN;/*选择按键的引脚*/
		GPIO_Init(P2_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P3_PIN;/*选择按键的引脚*/
		GPIO_Init(P3_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		GPIO_InitStructure.GPIO_Pin = P4_PIN;/*选择按键的引脚*/
		GPIO_Init(P4_GPIO_PORT, &GPIO_InitStructure);/*使用上面的结构体初始化按键*/
		Key_Delay(0XFF);
		if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
		{
			if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN))
				{
					key_value = 21;//FUNC_1
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN))
				{
					key_value = '1';//1//////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN))
				{
					key_value = 23;//left
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN))
				{
					key_value = '4';//////////////////////
					return key_value;
				}
			}
			else if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
			{
//				Key_Delay(0x9FFFF);
				if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
				{
					key_value = '7';//7////////
					return key_value;
				}
			}
		}

	}
	key_value=Touch_Identiry(GetSystemStatus(),Touch_Item);//触摸按键扫描
	if(key_value!=KEY_NONE)
		return key_value;
	/// 加一个键值转换
	return KEY_NONE;
	
}
//==========================================================
//函数名称：Key_Scan
//函数功能：按键扫描
//入口参数：无
//出口参数：无
//创建日期：2014.09.28 
//修改日期：2014.09.28 11:01
//备注说明：由10mS中断调用
//==========================================================
// u8 Key_Identiry(void)
//{
////	u8 key;
////    key=Key_Scan();
//    return  Key_Scan()
//    
//	
//    
//}
void Key_Scan(void)
{
	u8 keyval;
	
	keyval = Key_Identiry();//按键识别，返回键值Key_Up_flag,Touch_Up_flag
	//Touch_Scan();
	
	//按键长短识别
	if (keyval==KEY_NONE)//无键
	{
		if (LastKeyVal!=KEY_NONE)	//上次有键，短按键判别
		{
			if ( (LastKeyTicks>SHORTKEYCOUNT) && (LastKeyTicks<LONGKEYCOUNT) )
			{
				Keyboard.value=LastKeyVal;	//键值
				Keyboard.state=TRUE;		//有按键
			}
		}
		LastKeyVal=KEY_NONE;
		LastKeyTicks=0;
		Keyboard.continuance=FALSE; //持续按键	
	}
	else	//有键
	{
		if (LastKeyVal==keyval)//上次按键和本次按键相同
		{
			if (LastKeyTicks<LONGKEYCOUNT+1)//长按键节拍数100x10mS
				LastKeyTicks++;
			
			if (LastKeyTicks==LONGKEYCOUNT)//等于长按键节拍数
			{
				ContinueKeyTicks=0;//持续按键节拍
				//keyval|=LONG_PRESS;	//长按键
				Keyboard.value=keyval;	//键值
				Keyboard.state=TRUE;	//有按键
//				Keyboard.continuance=FALSE; //持续按键
				Keyboard.continuance=TRUE; //持续按键
			}
			else if (LastKeyTicks>LONGKEYCOUNT)//大于长按键节拍数
			{
				if(HW_KEYBOARD_CONTINUE_SUPPORT)//持续按键有效判别
				{
					//keyval|=LONG_PRESS;	//长按键
					Keyboard.value=keyval;//键值
//					Keyboard.state=TRUE;//有按键
					Keyboard.continuance=TRUE; //持续按键
					ContinueKeyTicks++;
					if(ContinueKeyTicks>CONTINUEKEYCOUNT)//持续按键节拍数
					{
						ContinueKeyTicks=0;//持续按键节拍
						if(Keyboard.state==FALSE)//按键已读取
							Keyboard.state=TRUE;//有按键
					}
				}
			}					
		}
		else//上次按键和本次按键不同
		{
			LastKeyVal=keyval;
			LastKeyTicks=0;
		}
	}
}

