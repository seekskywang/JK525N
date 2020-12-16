#include "pbdata.h"
#include "cpld.h"
#include "sdio/bsp_sdio_sd.h"
//#define BEEP GPIO_SetBits()
void PLC_GPIO_Configuration(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;	
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|
//    RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOG,ENABLE);
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;     //pro
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//    GPIO_Init(GPIOH,&GPIO_InitStructure); 
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;     //fail
//    GPIO_Init(GPIOA,&GPIO_InitStructure); 
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;     //pass
//    GPIO_Init(GPIOC,&GPIO_InitStructure); 
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;     //START
//    GPIO_Init(GPIOD,&GPIO_InitStructure); 
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;     //STOP
//    GPIO_Init(GPIOG,&GPIO_InitStructure); 

}
void Led_GPIO_Configuration(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|
//    RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOH,ENABLE);//
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;     //2  PASS
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//    GPIO_Init(GPIOE,&GPIO_InitStructure); 
//    
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14;//10 FAIL  11  LOCK  12 TRIP  14 ON
//    GPIO_Init(GPIOH,&GPIO_InitStructure); 
//   // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//OFF
//    GPIO_Init(GPIOD,&GPIO_InitStructure); 


}
void Channel_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    //GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	//GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    


}
void V_I_Select_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    //GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	//GPIO_Init(GPIOB,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    Channel_Configuration();
    


}


void V_I_amp(u8 amp)
{
    if(amp)//电流
        GPIO_SetBits(GPIOC,GPIO_Pin_7);
    else
        GPIO_ResetBits(GPIOC,GPIO_Pin_7);


}
void Open_flag_Configuration(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;	
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
    
    


}
void Select_V_I(u8 ch)//1  电流  0  电压
{

    if(ch==0)//电流
        GPIO_SetBits(GPIOD,GPIO_Pin_7);
    else
        GPIO_ResetBits(GPIOD,GPIO_Pin_7);
}

void GPIO_Configuration(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;	
	//串口 用于调试
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD/
    RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI,ENABLE);
    
    GPIO_SetBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_1);
    
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
    GPIO_Init(GPIOH,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOH,GPIO_Pin_5);
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
//    GPIO_Init(GPIOI,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//开机按键
	GPIO_Init(GPIOH,&GPIO_InitStructure); 
	GPIO_SetBits(GPIOH,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;

    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(GPIOE,&GPIO_InitStructure); 
 //   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   

	/*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
	/*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*设置引脚速率为2MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; 

	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	GPIO_Init(GPIOH, &GPIO_InitStructure);
	GPIO_SetBits(GPIOH,GPIO_Pin_4);
	
    
    Led_GPIO_Configuration();//LED
    CPLD_GPIO_Configuration( );//CPLD端口配置
    V_I_Select_Configuration();//电流电压选择信号
    Open_flag_Configuration();//开路信号
    PLC_GPIO_Configuration();//PLC控制信号
	CH376_GPIO_Init();//
    //V_I_Select_Configuration();
    SD_Init();

	
	
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   	
}

//电池电量读取脚位初始化
void ReadBatGPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=BATCAP_PIN4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BATCAP_PORT4,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=BATCAP_PIN3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BATCAP_PORT3,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=BATCAP_PIN2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BATCAP_PORT2,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=BATCAP_PIN1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BATCAP_PORT1,&GPIO_InitStructure);
}




u8 Read_Openflag(void)
{
    return GPIO_ReadInputDataBit( GPIOI,  GPIO_Pin_2);

}

void Beep_On(void)
{
//    GPIO_SetBits(GPIOH,GPIO_Pin_5);

}
void Beep_Off(void)
{
//    GPIO_ResetBits(GPIOH,GPIO_Pin_5);

}
void Key_beep(void)
{
    Beep_On();
    delay_ms(10);
    Beep_Off();
}
void Power_Relay_on(void)
{
//    GPIO_SetBits(GPIOC,GPIO_Pin_5);

}
void Power_Relay_OFF(void)
{
 //   GPIO_ResetBits(GPIOC,GPIO_Pin_5);

}
void Power_On_led(void)//开关红灯亮
{
//    Turnoff_Led();
//    Power_Relay_OFF();


}
void Power_Off_led(void)//开关绿灯亮
{
//    Turnon_Led();
//    Power_Relay_on();

}
void All_LedOff(void )//关所有的灯 除了开关按键灯
{
//    GPIO_ResetBits(GPIOE,GPIO_Pin_2);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//    GPIO_ResetBits(GPIOH,GPIO_Pin_10);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//    GPIO_ResetBits(GPIOD,GPIO_Pin_7);
//    Lock_Off();
//    Trip_Off();
    
    


}
void Led_Pass_On(void)//不合格
{
//	GPIO_SetBits(GPIOE,GPIO_Pin_2);
//    GPIO_SetBits(GPIOA,GPIO_Pin_0);
//    GPIO_ResetBits(GPIOH,GPIO_Pin_10);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_13);

}
void Led_Fail_On(void)//合格灯
{
//    GPIO_SetBits(GPIOH,GPIO_Pin_10);
//    GPIO_SetBits(GPIOC,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_0);

}
void Close_Compled(void)
{
//    GPIO_ResetBits(GPIOH,GPIO_Pin_10);
//    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_0);


}
void Turnon_Led(void)
{
//    GPIO_SetBits(GPIOH,GPIO_Pin_14);
//	GPIO_ResetBits(GPIOD,GPIO_Pin_3);

}
void Turnoff_Led(void)
{
//    GPIO_SetBits(GPIOD,GPIO_Pin_3);
//	GPIO_ResetBits(GPIOH,GPIO_Pin_14);

}
void Lock_On(void)
{
 //   GPIO_SetBits(GPIOH,GPIO_Pin_11);
}
void Lock_Off(void)
{
//    GPIO_ResetBits(GPIOH,GPIO_Pin_11);
}

void Trip_On(void)
{
//    GPIO_SetBits(GPIOH,GPIO_Pin_12);
}
void Trip_Off(void)
{
//    GPIO_ResetBits(GPIOH,GPIO_Pin_12);
}



void Enable_Extiint(void)
{
    
		EXTI_InitTypeDef EXTI_InitStructure;

		/* 选择 EXTI 中断源 */
		EXTI_InitStructure.EXTI_Line = EXTI_Line13;
		/* 中断模式 */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		/* 上升沿触发 */
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
		/* 使能中断/事件线 */
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

}
void Disable_Extiint(void)
{
    
		EXTI_InitTypeDef EXTI_InitStructure;
			
		/* 选择 EXTI 中断源 */
		EXTI_InitStructure.EXTI_Line = EXTI_Line13;
		/* 中断模式 */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		/* 上升沿触发 */
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
		/* 使能中断/事件线 */
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);

}


u8 read_cpld(void)
{
    u8 flag;
    if(Readfirst_cpld==1)//上次是开路
    {
        if(Read_Openflag()==0)//不再开路
        {
            Readfirst_cpld=0;
            //f_switch=TRUE;
            //Range=RANGE_MAX;
            open_flag=1;
            flag=0;
        
        }
        else
            open_flag=0;
            
    
    }
    else//上次不是开路
    {
        if(Read_Openflag()==1)//开路
        {
            Readfirst_cpld=1;//开路标志位
            //f_switch=TRUE;//继电器动作
            open_flag=0;
            flag=1;
            //Range=RANGE_MAX;
        
        }
        else
            open_flag=1;
        
    
    }
    return flag;

}
    
