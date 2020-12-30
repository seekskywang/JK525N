//==========================================================
//�ļ����ƣ�User.h
//�����������û���������ͷ�ļ�
//�ļ��汾��Ver1.0
//�������ڣ�2014.12.15
//�޸����ڣ�2015.08.20 11:03
//�ļ���ע����
//==========================================================
#ifndef __User_H__
#define __User_H__
#include "pbdata.h"
//==========================================================
#define LCD_WITH	640
#define LCD_HIGH	480
#define FIRSTLINE	40
#define	SPACE1		25
#define LIST1		0
#define DISPX1	LIST1+144
#define LIST2		LCD_WITH/2+16
#define DISPX2	LIST2+144
#define HIGH1		34
#define RECT		160
#define SELECT_1END 240
#define SELECT_2END (600)
#define BUTTON_W	128
#define BUTTON_1	LCD_HIGH-64
#define BUTTON_2	LCD_HIGH-32

//��������ʾλ�ö���
#define SETUP_ADDR_X		(6)
#define SETUP_ADDR_Y		(27)
#define HEIGHT_OF_SETUP		(20)
#define SORTING_Y_DISP  315
#define SORTING_XDISP   530


#define TESTVALUE_X 64
#define TESTVALUE_Y 160  //�������ݵ�Y����ֵ
#define TEST_VALUE_YMID 100

#define TESTVALUE_X_UINT TESTVALUE_X+72
#define TESTVALUE_Y_UINT TESTVALUE_Y  //��λ

//��˸���
#define CURSORCHAR (' ')

////�˵������ı���󳤶�(12���ַ�)
//#define MENU_MAX_TEXT_LEN (12)
//
////ÿҳ�˵������ֵ
////#define MENU_LIST_MAX_PAGE_INDEX (10)
//#define MENU_LIST_MAX_PAGE_INDEX (12)
////X��˵�����
//#define MENU_LIST_X_INDEX (2)
////Y��˵�����
////#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX-1)
//#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX)
//
////ϵͳ����,Y��˵�����
//#define SYSTEM_MENU_LIST_Y_INDEX (MENU_LIST_Y_INDEX-2)

//����λ��˸ʱ�䣬1000/3=333mS
#define BLINK_TICKS_SOFTTIMER (TICKS_PER_SEC_SOFTTIMER/3)

//==========================================================
//���ӡ�����
//extern const u16 POW_NUM[4];

////==========================================================
////��������ֵ�ϡ����ޡ�С����
//extern const u16 SetupLimit[][8][2];

////==========================================================
////��������ֵС����λ
//extern const u8 ParameterDotTab[][PARAMETER_MENU_MAX];

////==========================================================
////���
//extern const u8 NumTab[][3];

////==========================================================
////��Ŀ
//extern const u8 ItemTab[][9];
//extern const u8 ItemCharTab[][3];

//==========================================================

typedef struct
{
    u32 res;
    u8 dot;
    u8 uint;
    u8 polar;
    
} Test_ValueTypedef;

typedef struct 
{
	vu8 sign;
	vu32 Num;
	vu32 Dot;
	vu32 Unit;
	vu32 Updata_flag;
	vu32 Order;
	vu32 mid_data;
	
}Sort_TypeDef;

typedef struct
{
	vu16 xpos;
	vu16 ypos;
	vu16 lenth;
	
}Disp_Coordinates_Typedef;

//��ֵ�����Զ���
extern Test_ValueTypedef Test_Value;
extern Test_ValueTypedef Test_Value_V;

//�����б�
void Hex_Format(u32 dat , u8 dot , u8 len, u8 dispzero);//��ʽ��Hex��Bcd�ַ�����DispBuf
void Disp_SysMessage(void);
void Disp_PowerOn_Menu(void);
void Check_Parameter_Limit(void);
void Range_Control(u8 dat);
void Parameter_Setup(void);
void Disp_Setup_Menu(u8 index);
void System_Setup(void);
void Read_Ad(void);
void Range_Changecomp(void);
void Get_FFT(void);
void Test_Debug(void);
void _4094_data(void);
void InitGlobalValue(void);
void Parameter_valuecomp(void);
u16 Da_out(u16 v_value,u8 V_range);
void DispF(float f);
void Get_Res(void);
void Comp_test(u16 comp_data);
Test_ValueTypedef Datacov(s32 value,u8 range);
Test_ValueTypedef V_Datacov(s32 value ,u8 range);
void Uart_Process(void);
void Disp_Testvalue(Test_ValueTypedef value,Test_ValueTypedef value_v,u8 speed);
u8 Comp_choice(void);
void Beep_Out(u8 flag);
void lvbo_jisu(void);
void Soft_Turnon(void);
void SD_Process(void);
void VRange_Changecomp(void);
u8 read_adI_4(void);
u8 read_adV_4(void);
u8 read_adI_3(void);
u8 read_adV_3(void);
u8 read_adI_2(void);
u8 read_adV_2(void);
u8 read_adI_1(void);
u8 read_adV_1(void);
//void read_adI_3(void);
//void read_adV_3(void);
//void read_adI_2(void);
//void read_adV_2(void);
//void read_adI_1(void);
//void read_adV_1(void);
void read_adV_one(void);
void read_adI_one(void);
void V_Range_Control(u8 dat);//0  ���Ŵ�
void Disp_Sys_Item(void);
void Disp_Sys_value(u8 keynum);
void Disp_Sys(void);
void DispBattery(void);
//u8 Resistance_Setup(NumBox_TypeDef * pt);
//u8 ListBox_Setup(NumBox_TypeDef * pt);
//u8 Number_Setup(NumBox_TypeDef * pt);//��ֵ������
//u8 ListBox_Setup(NumBox_TypeDef * pt);//�б������

////void Disp_Kjjm( void );//��ʾ��������
//void Disp_PowerOn_Menu(void);//��ʾ��������

//void Disp_Group(u8 highlight);//��ʾ���
//void Disp_Frame(void);//��ʾ������

//void Disp_Idle_List(void);//��ʾ�����˵��б�
void Disp_Idle_Menu(void);//��ʾ��������
//void Disp_Idle_Info(void);//��ʾ������Ϣ

//void Step_Setup(void);//�������ô���
//void Disp_Step_Menu(void);//��ʾ����˵�
//void Disp_Step_Info(void);//��ʾ������Ϣ

//void Parameter_Setup(void);//�������ô���
//void Disp_Parameter_Menu(void);//��ʾ��������
//void Disp_Parameter_Info(u8 index);//��ʾ������Ϣ
//void Disp_Parameter_Value(u8 step ,u8 index, u8 highlight);//��ʾ�ڼ����ڼ������ֵ
//void Check_Parameter_Limit(u8 step);//����ֵ���
//void Check_Setup_Limit(void);//4������Ĳ���ֵ���

//void System_Setup(void);//ϵͳ���ô���
//void Disp_System_Menu(void);//��ʾϵͳ����
//void Disp_System_Info(u8 index);//��ʾϵͳ��Ϣ
//void Disp_System_Value(u8 index);//��ʾ�ڼ���ϵͳֵ
//void Check_System_Limit(void);//ϵͳֵ���

//void FactoryInfo_Setup(void);//������Ϣ���ô���
//void Disp_Factory_Info(void);//��ʾ������Ϣ
//void Disp_Software_Version(void);//��ʾ����汾
//void Disp_Hardware_Version(void);//��ʾӲ���汾
//void Disp_Boot_Number(void);//��ʾ��������
//void Disp_Compile_Date(void);//��ʾ��������

//void Disp_Instrument_Info(void);//��ʾ������Ϣ
//void Disp_Chip_ID(void);//��ʾоƬID��
//void Disp_Product_ID(void);//��ʾ��Ʒ���	
//void Disp_Product_Model(void);//��ʾ��Ʒ�ͺ�
//void Disp_Company_Name(void);//��ʾ��������
//void Disp_Company_Telephone(void);//��ʾ���̵绰
//void Disp_Company_Http(void);//��ʾ������ҳ

//void Config_Setup(void);//�������ô���
//void Disp_Config_Menu(void);//��ʾ���ý���
//void Disp_Config_Value(u8 index);//��ʾ����ֵ
//void Check_Config_Limit(void);//����ֵ���

//void Password_Setup(void);//��������
//Bool Password_Check(void);//����У��,TRUE��Ч

////==========================================================
////void Disp_SysStatus(void);//��ʾϵͳ״̬
//void Disp_SysMessage(void);//��ʾϵͳ��Ϣ

void Disp_xxx(u16 data,u16 pos);
void Disp_Test_Item(void);
void Disp_Button_value1( uint32_t value );
void Disp_Test_value(u8 num);
void Disp_Fastbutton(void);
void Disp_Test_Set_Item(void);
void DispSet_value(u8 keynum);
void Disp_SD_VIEW_Item(void);
void Use_SysSetProcess(void);

void Sys_Process(void);
Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);

void DoubHex_Format(u32 dat , u8 Dot , u8 len , u8 dispzero);
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates);
void Disp_Range(u8 hand,u8 range);
void Disp_Open(void);
void BubbleSort(uint32_t A[], uint16_t n);//

uint8_t R_Comp(void);//����Ƚ�
uint8_t V_Comp(void);//��ѹ�Ƚ�
void Disp_dateandtime(void);
uint8_t Jisuan_Range(Sort_TypeDef date);
uint8_t Jisuan_V_Range(Sort_TypeDef date);
void Disp_UserCheck_Item(void);
void Disp_Debug_value(vu8 list);
vu32 Debug_Set_Num(Disp_Coordinates_Typedef *Coordinates);//Ƶ������
void Debug_stanedcomp(void);
void Disp_Debug_Reference( vu8 list,Test_ValueTypedef eee,Test_ValueTypedef ddd);
vu32 Debug_Set_Res(Disp_Coordinates_Typedef *Coordinates);
void input_num(Disp_Coordinates_Typedef *Coordinates );
void input_password(Disp_Coordinates_Typedef *Coordinates );
void AD_ads2516(uint16_t i);
void lvbo_pingjun(void);
//void V_Range_Control(u8 dat);

//Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);

//==========================================================


#endif

//==========================================================
//END
//==========================================================
