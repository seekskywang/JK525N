#ifndef _pbdata_H
#define _pbdata_H

#define _4094_STBL GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define _4094_STBH GPIO_SetBits(GPIOE,GPIO_Pin_6)

#define _5618_CSL GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define _5618_CSH GPIO_SetBits(GPIOC,GPIO_Pin_3)

//#define _DS1302_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_4)
//#define _DS1302_CSH GPIO_SetBits(GPIOB,GPIO_Pin_4)

//#define _CPLD_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_8)
//#define _CPLD_CSH GPIO_SetBits(GPIOB,GPIO_Pin_8)

//hc595�ӿڶ���
#define _SPI_595_CSL    GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define _SPI_595_CSH    GPIO_SetBits(GPIOA,GPIO_Pin_11)

#define _SPI_595_DSL    GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define _SPI_595_DSH    GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define _SPI_595_SHCPL  GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define _SPI_595_SHCPH  GPIO_SetBits(GPIOA,GPIO_Pin_9)

#define _SPI_595_STCPL  GPIO_ResetBits(GPIOA,GPIO_Pin_10)
#define _SPI_595_STCPH  GPIO_SetBits(GPIOA,GPIO_Pin_10)


#define _7706_CSL GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define _7706_CSH GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define _ADS1216_CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define _ADS1216_CS_H GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define _SPI_4094_SCLKL GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define _SPI_4094_SCLKH GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define _SPI_4094_DATAL GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define _SPI_4094_DATAH GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define _SPI_4094_STROL GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define _SPI_4094_STROH GPIO_SetBits(GPIOC,GPIO_Pin_1)


//#define _SPI_SCLKL GPIO_ResetBits(GPIOB,GPIO_Pin_11)
//#define _SPI_SCLKH GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define _SPI_SCLKL GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define _SPI_SCLKH GPIO_SetBits(GPIOB,GPIO_Pin_6)


#define _SPI_DINL GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define _SPI_DINH GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define _SPI_DOUT GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
//#define _SPI_DOUTH GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define _FMQ_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define _FMQ_CSH GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define _TESTLED_CSL GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define _TESTLED_CSH GPIO_SetBits(GPIOE,GPIO_Pin_2)


#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./flash/bsp_spi_flash.h"
#include "./tim/bsp_basic_tim.h"
#include "./usart/bsp_debug_usart.h"
#include "./tim_pwm/bsp_general_tim.h"
#include "./systick/bsp_SysTick.h"
#include "./adc/bsp_adc.h"
#include "./ch376/ch376.h"

//#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "stdio.h"
//#include "Lcd2002.h"
#include "gpio.h"
//#include "cpld.h"
#include "4094.h"
//#include "24c256.h"
//#include "dac.h"
//#include "adc.h"
//#include "ds1302.h"
#include "ad7706.h"
//#include "tlc5618.h"
//#include "time.h"



#include "disp.h"
#include "keyboard.h"
#include "user.h"
#include  "test.h"
#include  <string.h>
#include "flash_eerom.h"
#include "uart.h" 
#include "flash.h" 
#include "spi.h"
#include "bsp_exti.h"






//���庯��

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
//void delay_us(u32 nus);
//void delay_ms(u16 nms);
void SetSoftTimer(u8 id, u8 ticks);
u8 GetSoftTimerOut(u8 id);

//16bit���ݵ�λ��������
#define	SetD0	(0x0001)
#define	SetD1	(0x0002)
#define	SetD2	(0x0004)
#define	SetD3	(0x0008)
#define	SetD4	(0x0010)
#define	SetD5	(0x0020)
#define	SetD6	(0x0040)
#define	SetD7	(0x0080)
#define	SetD8	(0x0100)
#define	SetD9	(0x0200)
#define	SetD10	(0x0400)
#define	SetD11	(0x0800)
#define	SetD12	(0x1000)
#define	SetD13	(0x2000)
#define	SetD14	(0x4000)
#define	SetD15	(0x8000)

#define	ClrD0	(0xfffe)
#define	ClrD1	(0xfffd)
#define	ClrD2	(0xfffb)
#define	ClrD3	(0xfff7)
#define	ClrD4	(0xffef)
#define	ClrD5	(0xffdf)
#define	ClrD6	(0xffbf)
#define	ClrD7	(0xff7f)
#define	ClrD8	(0xfeff)
#define	ClrD9	(0xfdff)
#define	ClrD10	(0xfbff)
#define	ClrD11	(0xf7ff)
#define	ClrD12	(0xefff)
#define	ClrD13	(0xdfff)
#define	ClrD14	(0xbfff)
#define	ClrD15	(0x7fff)
#define PASSWORD  "20185260"

#define HW_HW_PGA_MASK		((u8)((ClrD1&ClrD2&ClrD3&ClrD4&ClrD6&ClrD7)))
#define HW_PGA_1K 			((u8)(SetD4))
#define HW_PGA_10K 			((u8)(SetD7))
#define HW_PGA_100K 		((u8)(SetD3))
#define HW_PGA_103K			((u8)(SetD6))
#define HW_PGA_104K			((u8)(SetD2))
#define HW_PGA_105K			((u8)(SetD1))

#define HW_PRO_TEST			((u8)(SetD1))//�������ź�
#define HW_PRO_UNTEST		((u8)(ClrD1))

#define HW_FD_TEST			((u8)(SetD5))//����
#define HW_FD_NOTEST		((u8)(~SetD5))//�ŵ�


#define HW_GYB_100V 		((u8)(ClrD3))
#define HW_GYB_1000V 		((u8)(SetD3))

//#define HW_GYB_1000VFEEDMASK ((u8)(ClrD2&ClrD1))
#define HW_GYB_1000VFEED	((u8)(SetD0))
#define HW_GYB_100VFEED		((u8)(ClrD0))


#define HW_GYBFD_ON			((u8)(SetD1))
#define HW_GYBFD_OFF		((u8)(ClrD1))
//#define HW_HW_GYBDZ_MASK	((u8)((ClrD5&ClrD4)))

//#define HW_GYB_0R			((u8)(SetD4|SetD5))//����
//#define HW_GYB_10KR			((u8)(ClrD4&ClrD5))//����
//#define HW_GYB_100KR		((u8)(SetD4))//����

//==========================================================

#define FIT_INUM    12  //�˲�����
#define STEP_MAX			(4)//�����(0-3)

#define IDLE_MENU_MAX  		(4)//�������˵�
#define IDLE_MENU_PAGE 		(4)//ÿҳ�����˵�

#define PARAMETER_MENU_MAX	(6)//�������˵�
#define PARAMETER_MENU_PAGE	(4)//ÿҳ�����˵�

#define PARAMETER_ACW_MAX	(8)//ACW������(0-7)
#define PARAMETER_DCW_MAX	(8)//DCW������(0-7)
#define PARAMETER_IR_MAX	(6)//IR������(0-5)
#define PARAMETER_GND_MAX	(7)//ACW������(0-6)

#define SYSTEM_MENU_MAX		(4)//���ϵͳ�˵�(0-5)
#define SYSTEM_MENU_PAGE	(4)//ÿҳϵͳ�˵�

#define CONFIG_MENU_MAX		(3)//���������(0-2)

#define PW_LENGTH 			(4)//�������λ��

#define AD_BUF_LENGTH		(8)//A/Dֵ����������


#define MAXTSOFTTIMER (2)		//��ʱ����Ŀ

#define UART_BUFF_LENTH         (REC_LEN_MAX)//���ڽ������ݳ���

#define FIT_NUM                 140

#define MAXFITNUM 0xff

//#define BASTARDLY  //�������Ժ� Ҫ�������������ֲű仯��
//==========================================================
//����汾��10
#define SOFTWARE_VERSION		(0x10)

//==========================================================
//Ĭ�ϳ������(0)
#define DEFAULT_MACHINE_ID 		(0)

//Ĭ�ϳ�������(0)
#define DEFAULT_PASSWORD 		(0)

//Ĭ�Ͽ�������(0)
#define SET_BOOT_DEFAULT		(0)

#define CHANNEL_NUM             (15)

//ADֵ�˲����ã����������ȶ���
#define AD_FILTER_SUPPORT		(_DEBUG_CONFIG_TRUE)

//==========================================================
#define SETUP_MENU_MAX  	(2)//������ò˵�

//#define PARAMETER_MENU_MAX	(5)//�������˵�

//#define SYSTEM_MENU_MAX		(2)//���ϵͳ�˵�

#define CONFIG_MENU_MAX		(3)//���������(0-2)

#define PW_LENGTH 			(4)//�������λ��

#define RANGE_MAX 			(3)//�������

#define RANGE_V_MAX         (1)

#define AD_BUF_LENGTH		(8)//A/Dֵ����������

#define KEY_NUM				(3)	//����������

#define SYS_MAX				(12)

#define PASSWORD_LENTH  8

#define NUM_LENTH 			(6)//��ʾ����
#define NUM_LENTH_V 			(6)//��ʾ����
#define MAX_DOTNUM          (5)//���С����

//==========================================================
#define OHM 				(0xF4)	//CGROM���Ц��ַ�������Ϊ0xF4

//==========================================================
//��ʾ������X����
#define MSG_ADDR_X 			(0)
#define AUTO_ADDR_X 		(7)
#define RANGE_ADDR_X 		(9)

//#define FALSE 0

//#define TRUE 0

#define RES_ADDR_X 			(12)
#define VOL_ADDR_X 			(1)
#define TIM_ADDR_X 			(12)
#define RAN_ADDR_X			(7)

//��������������
#define RANGR_LIMIT_HIGH (3100)
#define RANGR_LIMIT_LOW  (300)

// ��ѹ����������
#define RANGE_LIMIT_VHIGH (60000)
#define RANGE_LIMIT_VLOW (4500)

#define DEBUG_RANGE 6
//==========================================================
//��׼��ѹDA   50V   500V
//==========================================================
#define  CAL_50V		(3600)
#define  CAL_500V		(3600)

//��ʱ������
#define KEYBOARD_SOFTTIMER	(0) //������ʱ��ʱ��
#define DISP_SOFTTIMER 		(1) //��ʾ��ʱ��ʱ��

//==========================================================
//���ⳤ��
#define LIST_TITLE_LEN		(8)
#define TITLE_LEN_MAX		(8)
#define SetSystemStatus(status) (SystemStatus=status)
#define GetSystemStatus() (SystemStatus)
#define GetSystemMessage() (SystemMessage)
#define SetSystemMessage(msg) (SystemMessage=msg)


//USB�����Ӧ
#define NO_CONNECTION				0
#define UDISK_READY					1
#define UDISK_NOTREADY				2

/*USB״̬*/
#define UNKNOWN		     2
#define UNCONNECTED		 0
#define CONNECTED		 1

//�������
extern uint8_t softswitch;
extern u8 dt;
extern vu8 SystemStatus;//ϵͳ״̬
extern vu8 SystemMessage;//ϵͳ��Ϣ
extern vu8 MenuIndex;//�����˵���
extern vu8 Range;//��ǰ����
extern vu8 ffit_data1;
extern vu16	count_ffit;
extern u8 fft_flag;
extern u32  g_LcdDirection;
extern u8 polarity_v,polarity_r;
extern u8 Readfirst_cpld,Readsecond_cpld;
extern u8 RangeChange_Flag;
extern u32 fit_i_buff[MAXFITNUM],fit_v_buff[MAXFITNUM];
extern u32 scan_V[130],scan_I[500];
extern u8 range_over;
extern u8 Int_Pe3flag;
extern const u8 fit_allnum[4];
extern u32 last_R_disp,last_V_disp;
extern float disp_I,disp_V;
extern u32 POINT_COLOR;
//==========================================================
//ϵͳ״̬
enum SysStatusEnum
{	
	SYS_STATUS_ERROR ,			//�쳣
	SYS_STATUS_RESET ,			//��λ
	SYS_STATUS_POWER ,			//����
//	SYS_STATUS_POWER ,
	SYS_STATUS_DATASTORE ,		//���ݱ���
	SYS_STATUS_FILE ,			//�ļ�����
	SYS_STATUS_SETUPTEST ,			//��������
	SYS_STATUS_TEST ,			//����
	SYS_STATUS_RANGE ,		//������ʾ
	SYS_STATUS_RANGECOUNT,	//��������ʾ
	SYS_STATUS_ITEM ,			//�б�ɨ����ʾ
	SYS_STATUS_USERDEBUG,//�û�У�� 
	SYS_STATUS_FACRDEBUG, //����У��
	SYS_STATUS_SYSSET,     //ϵͳ����
    SYS_STATUS_LIMITSET,     //��������
    SYS_STATUS_ITEMSET,//�б�ɨ������
	SYS_STATUS_TOOL,//����
	SYS_STATUS_SYS,
    SYS_STATUS_CLEAR,
    SYS_STATUS_SETUP,
    SYS_STATUS_DEBUG,
};

//==========================================================
//ϵͳ��Ϣ
enum SysMessageEnum
{
	MSG_CLEAR , 
	MSG_IDLE , 
	MSG_TEST , 
	MSG_ABORT , 
	MSG_PASS ,
	MSG_FAIL ,
	MSG_HIGH , 
	MSG_LOW , 
};



//��ֵ�����Զ���
typedef struct
{
	vu8 *Title;//����
	vu16 Num;//��ֵ
	vu8  Dot;//С����(0-3: xxxx xxx.x xx.xx x.xxx)
	//vu8  Unit;//��λ(0-2: M��  G��  T��)
	vu16 Max;//���ֵ
	vu16 Min;//��Сֵ
}NumBox_TypeDef;
extern NumBox_TypeDef NumBox;//��ֵ��

//��Ŀֵ																					   
enum ItemEnum
{	
	ITEM_ACW ,
	ITEM_DCW ,
	ITEM_IR ,
	ITEM_GND ,
	ITEM_NONE ,
};
typedef struct
{
    vu8 xpos;
    vu8 ypos;

}Channel_Pos_Typedef;
typedef struct
{
    vu8 Range;//����  0 �Զ�
    vu8 Speed;//�ٶ� 0 ����  1 ����  2 ����
    vu8 Avg;//ƽ������  1-255
    vu8 Scan;//���λ���ɨ�� 0 ��·  1 ɨ��
    vu8 Signel;//0 ��������  1  ���β���
    vu8 Mode;//����ģʽ 0 pt100,1 pt500 ,2 0-2v  3 �������
    vu8 Temp_Channel;//ѡ���¶Ȳ��Ե�ͨ��

}UINT8_rec_Typedef;
  
typedef struct 
{
	vu16 value;	//��ֵ
	vu8 dot;	//С���� 0 1 2 3    xxxx   xxx.x  xx.xx x.xxx
	vu8 unit;	//��λ 0 1 2 3   M��    G��   T��  
	

}SystemRes_TypeDef;
typedef struct
{
    UINT8_rec_Typedef   UINT8_rec;
    Channel_Pos_Typedef Channel[CHANNEL_NUM];//�������������
    float Debug_vlaue[RANGE_MAX];//У��ֵ
    float Clear[CHANNEL_NUM];//����ֵ
}Savedata_2516_Typedef;
extern Savedata_2516_Typedef Savedata_2516;
//==========================================================
//������ת��  
typedef struct
{
	u8 Sign;//������
	u32 Digit;//β��
	s8 Exponent;//����

}Float_TypeDef;
extern Float_TypeDef Float;
//==========================================================
//У��ֵ
typedef struct 
{
	vu16 Num;  //���ֵ
	vu16 NumAd;//���A/Dֵ
}Cal_TypeDef;

//У����
typedef struct 
{
	Cal_TypeDef IRVol[2];//��Ե��ѹ��  0  50V  1  500V
	Cal_TypeDef IRCur[8];// 0 1K�������赵 1  10K  2  100K  3  1000K   4 10M   5  80M 6 64M   7  128M
	
}Calibrate_TypeDef;
extern Calibrate_TypeDef Calibrate;
//==========================================================
//������(3��)
typedef struct 
{
	vu16 BootNum;//��������(0-9999��Ĭ��0)
	vu16 ProductId;//�������(0-9999��Ĭ��0)
	vu16 Password;//��������(0-9999��Ĭ��0)
}Factory_TypeDef;
//==========================================================
//ϵͳ��(2��)

typedef struct
{
    vu8 comp;
    vu8 mode;//����ֵ  �ٷֱ�  ֱ��SEQ
    SystemRes_TypeDef Low_limit;//����
    
    vu8 beep;
    SystemRes_TypeDef criterion;//���ֵ
    SystemRes_TypeDef Hi_limit;//����        

}Compset_TypeDef;
typedef struct
{
    vu8 Temp_equalize;//�¶Ȳ���
    u32 temp_coefficient;//�¶�ϵ��
    u32 temp_reference;//�ο��¶�
    u32 Range_Delay;//������ʱ
    u8 compute;//����
    u32 pole_constant;//�缫����

}Setup_TypeDef;
typedef struct
{
    u8 Trip;
    u8 mode;
    u8 Range;
    u8 speed;
    

}Testset_TypeDef;
typedef struct
{
    u8 name;
    u32 password;

}accountnum_TypeDef;
typedef struct
{
    u8 language;
    u8 blacklight;
    u8 turnoff;
    u8 buard;
    accountnum_TypeDef accountnum;//����Ա
    

}Sysconfig_TypeDef;

typedef struct
{
	vu8 trip;//����
	vu8 speed;//�����ٶ�
	vu8 Res_comp;
	Sort_TypeDef Res_low;//��������
	vu8 V_comp;
	Sort_TypeDef V_low;
	vu8 Range;
    vu8 Range_Set;
	vu8 beep;
	Sort_TypeDef Nominal_Res;
	Sort_TypeDef High_Res;
	Sort_TypeDef Nominal_V;
	Sort_TypeDef	V_high;
	


}Set_Data_Typedef;
typedef struct
{
	vu32 standard;//��׼��ʾֵ
	float ad_value;

}Debug_Value_Typedef;
typedef struct
{
	vu8 Res_dot;
	vu8 V_dot;
	vu8 V_Neg;
}Test_UnitTypedef;
extern Test_UnitTypedef Test_Unit;
typedef struct
{
	vu8 Hour;
	vu8 Min;
	vu8 Sec;

}Timer_Value_Typedef;
typedef struct
{
	vu8 Year;
	vu8 Mon;
	vu8 data;

}Data_Value_Typedef;
typedef struct
{
	vu8 uart;
	vu8 buard;
    vu8 u_flag;
    vu8 plc;
	vu8 lanage;
    vu8 textname[20];
	//vu8 
	

}Sys_Setvalue_Typedef;

typedef struct
{
	u8 save_flag;
	u16 xoff;
	u16 yoff;
	float x_pos;
	float y_pos;
	
} Touch_save_Typedef;
typedef struct
{
//    Compset_TypeDef Compset;//�Ƚ�����
//    Setup_TypeDef   Setup;//����
//    Testset_TypeDef Testset;//����ʱ����������
//    Sysconfig_TypeDef Sysconfig;//ϵͳ����
    Set_Data_Typedef Set_Data;
	
	Sys_Setvalue_Typedef Sys_Setvalue;
    float Clear[RANGE_MAX+1];
     float Clear_V[2];
    vu8 fac_num[10];
	Touch_save_Typedef Touch_save;
    
    
    

}Jk516save_TypeDef;

typedef struct
{
    Debug_Value_Typedef Debug_Value[6];

}Jk516cal_TypeDef;

extern Jk516save_TypeDef    Jk516save;
extern Jk516cal_TypeDef     Jk516cal;
typedef struct//2516����Ĳ���������ݴ洢
{
    SystemRes_TypeDef newvalue[16];
    SystemRes_TypeDef oldvalue[16];


}Test_value_buff_Typedef;
//==========================================================
//ϵͳ��(4��)
typedef struct 
{
	vu16 Vol_Out;	//�����ѹ(10-1000 V)
	vu16 Set_Time;	//����ʱ��(0-999.9s)
	vu16 Res_Hi;	//����(xxxx M��)
	vu16 Res_Lo;	//����(xxxx M��)

}System_TypeDef;
typedef struct 
{
	vu32 Test_v;	//���Ե�ѹ(10-1000 V)
	vu8 dot;	//С����λ

}Test_VTypeDef;
typedef struct
{
	float v;//�¶�
	float i;
	float r;//����ֵ
	u32 rate;

}Res_countTypeDef;
extern Res_countTypeDef Res_count;
typedef struct 
{
	vu16 Test_Res;	//���Ե�ѹ(10-1000 V)
	vu8 dot;	//С����λ
	vu8 unit;

}Test_RESTypeDef;
//==========================================================
//ϵͳ��(2��)
typedef struct 
{
	Test_VTypeDef Test_v;	//���Ե�ѹ(10-1000 V)
	vu16 Test_Time;	//����ʱ��(0-999.9s)
	Test_RESTypeDef Test_Res;	//���Ե���(xxxx M��)
	vu16 Test_I;

}Test_TypeDef;
extern Test_TypeDef Test_value;
//==========================================================
//
//==========================================================
//������(3��)
typedef struct 
{
	vu16 Password;//��������(0-9999��Ĭ��0)
	vu16 BootNum;//��������(0-9999��Ĭ��0)
	vu16 ProductId;//�������(0-9999��Ĭ��0)
}Config_TypeDef;
//==========================================================
//������(5��)
typedef struct 
{
	vu8 Range;//����ģʽ(0-7��Ĭ��0�Զ�)
	vu8 Beep;//Ѷ��ģʽ(0-2��Ĭ�Ͽ�1-PassOn)
	vu8 ClrOn;//���㿪��(0-1��Ĭ�Ϲ�0)
	
	vu8 ExTrig;//�ⲿ��������(0-1��Ĭ�Ϲ�0)
	vu8 UartOn;//���ڿ���(0-1��Ĭ�Ϲ�0)
//	vu8 FastOn;//���ٲ���(0-1��Ĭ�Ϲ�0)
}Parameter_TypeDef;


typedef struct
{
    vu8 send_head;
    vu8 king;
    vu8 Range;
    vu8 send_res[7];
    vu8 send_V[8];
    vu8 comp;
    vu8 sendend;
    vu8 over;

}Send_ComBuff_Typedef;
typedef struct
{
    vu8 No[20];
    vu8 Send_res[9];
    vu8 back;
    vu8 Send_V[9];
    vu8 back1;
    vu8 comp[5];
    vu8 ret[2];

}Send_To_UTypedef;
extern Send_To_UTypedef Send_To_U;
typedef struct 
{
   // vu8 Rev_head;
    vu8 Trip;
    vu8 Speed;
    vu8 R_comp;
    
    vu8 Rev_LoRes[6];//4�ֽ�����  һ��С����  һ����λ
    vu8 Rev_V_comp;
    vu8 Rev_V_Lo[6];
    vu8 Range;
    vu8 Beep;
    vu8 Norm_Res[6];
    vu8 Rev_Hires[6];
    vu8 Norm_V[6];
    vu8 Rev_VHi[6];


} Rev_Combuff_Typedef;
extern Rev_Combuff_Typedef Rev_Combuff;
extern Send_ComBuff_Typedef Send_ComBuff;

//==========================================================
//������Ϣ
typedef struct 
{
	u8 group;
	Parameter_TypeDef Parameter;//������
	//System_TypeDef System;//ϵͳ��
	Config_TypeDef Config;//������
	Calibrate_TypeDef Calibrate;//У׼��
	vu16 Crc16;//У����
}Save_TypeDef;
extern System_TypeDef System;
extern u8 F_Fail;//����ʧ�ܱ�־
extern u8 F_100ms;//100ms��ʱ��־
extern u8 F_Password;//������Ч��־
extern Save_TypeDef SaveData;//����ֵ
extern vu32 Voltage;//��ѹ
extern vu32 Test_Time;//����
extern vu32 Resistance;//����
extern u8 SoftTimer[MAXTSOFTTIMER];
extern float i_buff[FIT_INUM];		   //�˲�ֵ
extern u32 V_ad,I_ad;
extern vu32 Ad_value;
extern vu8 Ad_over;
extern vu8 Relay_c;
extern vu8 debug_buff[UART_BUFF_LENTH];
extern vu8 test_start;
extern vu8 Set_flag;
extern vu8 debug_flag;
extern float Range_value;
extern float Range_Value_V;
extern  u8 cpld_buff[2];
extern u8 open_flag;//��·��־λ
extern vu8 V_Range;
extern u8 USB_Openflag,Disp_usbflag,Disp_RTCflag;
extern u8 Key_Up_flag,Touch_Up_flag;
extern u8 Touch_Item;
extern u8 touch_value;

#endif
