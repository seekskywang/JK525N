#include "touch.h" 
//#include "lcd.h"
//#include "bsp_lcd.h"
#include "stdlib.h"
#include "math.h"
#include "pbdata.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_adc.h"
//#include "24cxx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A等） 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/7
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									   
//********************************************************************************
//修改说明
//V1.1 20140721
//修正MDK在-O2优化时,触摸屏数据无法读取的bug.在TP_Write_Byte函数添加一个延时,解决问题.
//V1.2 20141130 
//电容触摸屏增加FT5206的支持
//////////////////////////////////////////////////////////////////////////////////
u8 touch_flag;
__IO uint16_t ADC_Converted[RHEOSTAT_NOFCHANEL1]={0};
_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//默认为touchtype=0的数据.
u8 CMD_RDX=0;
u8 CMD_RDY=1;

		 
void Touch_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (TOUCH_XPLUS_GPIO_CLK|TOUCH_XMINUS_GPIO_CLK|TOUCH_YPLUS_GPIO_CLK|
	TOUCH_YMINUS_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = TOUCH_YPLUS_GPIO_PIN|TOUCH_YMINUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输入模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_YPLUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XMINUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_XMINUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XPLUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_XPLUS_GPIO_PORT, &GPIO_InitStructure);
	
//	Touch_Up_flag=0;
//	GPIO_ResetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN);
}
static void AD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================通道1======================*/	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK0,ENABLE);		
	// 配置 IO
    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN0|ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    //不上拉不下拉	
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(ADC_GPIO_PORT0, &GPIO_InitStructure);

}
static void Rheostat_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Converted;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL1;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);
  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 扫描模式，多通道采集需要	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //使用软件触发，外部触发不用配置，注释掉即可
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL1;                                    
  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序和采样时间周期
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL0, 1, 
	                         ADC_SampleTime_144Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL1, 2, 
	                         ADC_SampleTime_144Cycles); 

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// 使能ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
  //开始adc转换，软件触发
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}
u16 TP_Read_AD(u8 CMD)	  
{ 	 

	u16 Num=0; 
	if(CMD)
	{
		
		set_yy();
		delay_1us(40);
		Num = ADC_Converted[1];
		

	}else{
		
		set_xx();
		delay_1us(40);
		Num = ADC_Converted[0];
		
	
	}
	return(Num);   
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 3 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
		buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;	
	//set_mid();	
	xtemp=TP_Read_XOY(CMD_RDX);
	//set_mid();
	ytemp=TP_Read_XOY(CMD_RDY);	 
	//set_mid();	
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
		
        return 1;
    }else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(u16 x,u16 y,u32 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x+12,y,25,0);//横线
	LCD_DrawLine(x,y+13,25,1);//竖线
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x-2,y+1);
	LCD_DrawPoint(x,y-1);
	LCD_DrawPoint(x-2,y-1);
	LCD_Draw_Circle(x,y,6);//画中心圈
}	  
//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x,u16 y,u32 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//中心点 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////		  
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 TP_Scan(u8 tp)
{		
	u16 value;
	set_yz(); 
    delay_us(20);
value=ADC_Converted[0];	
	//if(PEN==0)//有按键按下
	if(value>100)
	touch_flag=1;
	if(touch_flag)//
	{
		touch_flag=0;
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//读取屏幕坐标
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x[4]=tp_dev.x[0];//记录第一次按下时的坐标
			tp_dev.y[4]=480-tp_dev.y[0];  	   			 
		}			   
	}
	else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开	
		}else//之前就没有被按下
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}	  
//////////////////////////////////////////////////////////////////////////	 
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE 40
//保存校准参数										    
void TP_Save_Adjdata(void)
{
	s32 temp;			 
	//保存校正结果!		   							  
	Jk516save.Touch_save.x_pos=tp_dev.xfac*100000000;//保存x校正因素      
      
	Jk516save.Touch_save.y_pos=tp_dev.yfac*100000000;//保存y校正因素    
    
	//保存x偏移量
	Jk516save.Touch_save.xoff=tp_dev.xoff;
    		    
	//保存y偏移量
	Jk516save.Touch_save.yoff=tp_dev.yoff;	
	//保存触屏类型
	Jk516save.Touch_save.save_flag=0x0a;
	Store_set_flash();
}
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
u8 TP_Get_Adjdata(void)
{					  
	s32 tempfac;
	tempfac=Jk516save.Touch_save.save_flag;//读取标记字,看是否校准过！ 		 
	if(tempfac==0X0A)//触摸屏已经校准过了			   
	{    												 
		tempfac=Jk516save.Touch_save.x_pos;		   
		tp_dev.xfac=(float)tempfac/100000000;//得到x校准参数
		tempfac=Jk516save.Touch_save.y_pos;			          
		tp_dev.yfac=(float)tempfac/100000000;//得到y校准参数
	    //得到x偏移量
		tp_dev.xoff=Jk516save.Touch_save.xoff;			   	  
 	    //得到y偏移量
		tp_dev.yoff=Jk516save.Touch_save.yoff;				 	  
 		//tp_dev.touchtype=AT24CXX_ReadOneByte(SAVE_ADDR_BASE+12);//读取触屏类型标记
//		if(tp_dev.touchtype)//X,Y方向与屏幕相反
//		{
//			CMD_RDX=0X90;
//			CMD_RDY=0XD0;	 
//		}else				   //X,Y方向与屏幕相同
//		{
//			CMD_RDX=0XD0;
//			CMD_RDY=0X90;	 
//		}		 
		return 1;	 
	}
	return 0;
}	 
//提示字符串
u8* const TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  
//提示校准结果(各个参数)
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
//	POINT_COLOR=RED;
//	LCD_ShowString(40,160,lcddev.width,lcddev.height,16,"x1:");
// 	LCD_ShowString(40+80,160,lcddev.width,lcddev.height,16,"y1:");
// 	LCD_ShowString(40,180,lcddev.width,lcddev.height,16,"x2:");
// 	LCD_ShowString(40+80,180,lcddev.width,lcddev.height,16,"y2:");
//	LCD_ShowString(40,200,lcddev.width,lcddev.height,16,"x3:");
// 	LCD_ShowString(40+80,200,lcddev.width,lcddev.height,16,"y3:");
//	LCD_ShowString(40,220,lcddev.width,lcddev.height,16,"x4:");
// 	LCD_ShowString(40+80,220,lcddev.width,lcddev.height,16,"y4:");  
// 	LCD_ShowString(40,240,lcddev.width,lcddev.height,16,"fac is:");     
//	LCD_ShowNum(40+24,160,x0,4,16);		//显示数值
//	LCD_ShowNum(40+24+80,160,y0,4,16);	//显示数值
//	LCD_ShowNum(40+24,180,x1,4,16);		//显示数值
//	LCD_ShowNum(40+24+80,180,y1,4,16);	//显示数值
//	LCD_ShowNum(40+24,200,x2,4,16);		//显示数值
//	LCD_ShowNum(40+24+80,200,y2,4,16);	//显示数值
//	LCD_ShowNum(40+24,220,x3,4,16);		//显示数值
//	LCD_ShowNum(40+24+80,220,y3,4,16);	//显示数值
// 	LCD_ShowNum(40+56,240,fac,3,16); 	//显示数值,该数值必须在95~105范围之内.

}
		 
//触摸屏校准代码
//得到四个校准参数
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//坐标缓存值
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
	u16 outtime=0;
 	cnt=0;				
//	POINT_COLOR=BLUE;
//	BACK_COLOR =WHITE;
	LCD_Clear(LCD_COLOR_WHITE);//清屏   
//	POINT_COLOR=RED;//红色 
//	LCD_Clear(WHITE);//清屏 	   
//	POINT_COLOR=BLACK;
//	LCD_ShowString(40,40,160,100,16,(u8*)TP_REMIND_MSG_TBL);//显示提示信息
	TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);//画点1 
	tp_dev.sta=0;//消除触发信号 
	tp_dev.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 
	while(1)//如果连续10秒钟没有按下,则自动退出
	{
		tp_dev.scan(1);//扫描物理坐标
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//按键按下了一次(此时按键松开了.)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);//标记按键已经被处理过了.
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,LCD_COLOR_WHITE );	 			//清除点1 
					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,20,LCD_COLOR_RED);	//画点2
					break;
				case 2:
 					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,20,LCD_COLOR_WHITE);	//清除点2
					TP_Drow_Touch_Point(20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_RED);	//画点3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);			//清除点3
 					TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_RED);	//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//画点1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT,LCD_COLOR_WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//画点1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;

						TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);	
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//显示数据   
						continue;
					}//正确了
					//计算结果
					tp_dev.xfac=(float)(LCD_PIXEL_WIDTH-40)/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					tp_dev.xoff=(LCD_PIXEL_WIDTH-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					tp_dev.yfac=(float)(LCD_PIXEL_HEIGHT-40)/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					tp_dev.yoff=(LCD_PIXEL_HEIGHT-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  
					if(fabs(tp_dev.xfac)>2||fabs(tp_dev.yfac)>2)//触屏和预设的相反了.
					{
						cnt=0;
 				    	TP_Drow_Touch_Point(LCD_PIXEL_WIDTH-20,LCD_PIXEL_HEIGHT-20,LCD_COLOR_WHITE);	//清除点4
   	 					TP_Drow_Touch_Point(20,20,LCD_COLOR_RED);								//画点1
						//LCD_ShowString(40,26,lcddev.width,lcddev.height,16,"TP Need readjust!");
//						tp_dev.touchtype=!tp_dev.touchtype;//修改触屏类型.
//						if(tp_dev.touchtype)//X,Y方向与屏幕相反
//						{
//							CMD_RDX=0;
//							CMD_RDY=1;	 
//						}else				   //X,Y方向与屏幕相同
//						{
//							CMD_RDX=1;
//							CMD_RDY=0;	 
//						}			    
						continue;
					}		
//					POINT_COLOR=BLUE;
//					LCD_Clear(WHITE);//清屏
//					LCD_ShowString(35,110,lcddev.width,lcddev.height,16,"Touch Screen Adjust OK!");//校正完成
//					delay_ms(1000);
					TP_Save_Adjdata();  
// 					LCD_Clear(WHITE);//清屏   
					return;//校正完成				 
			}
		}
		delay_ms(10);
		outtime++;
		if(outtime>1000)
		{
			TP_Get_Adjdata();
			break;
	 	} 
 	}
}	
//触摸屏初始化  		    
//返回值:0,没有进行校准
//       1,进行过校准
u8 TP_Init(void)
{
//  GPIO_InitTypeDef  GPIO_InitStructure;	
	AD_GPIO_Config();
	Rheostat_ADC_Mode_Config();
	

		
   
		TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//第一次读取初始化	 

		if(TP_Get_Adjdata())return 0;//已经校准
		else			   //未校准?
		{ 										    //
//			LCD_Clear(WHITE);//清屏
			TP_Adjust();  	//屏幕校准 
			TP_Save_Adjdata();	 
		}			
//		TP_Get_Adjdata();	
//	}
	return 1; 									 
}
