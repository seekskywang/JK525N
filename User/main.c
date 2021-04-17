#include "pbdata.h"
#include "bsp_exti.h"


int main(void)
{  	

    g_LcdDirection=1;
    softswitch=0;
    Int_Pe3flag=0;
    SetSystemStatus(SYS_STATUS_POWER);
    GPIO_Configuration();//�˿ڳ�ʼ��
	
	while(1)
	{
		switch(GetSystemStatus())
		{
				case SYS_STATUS_POWER:
							Power_Process();//�����ϵ紦��
				break;
				case SYS_STATUS_SETUP:
							Setup_Process();//���ô���
				break;
				case SYS_STATUS_TEST:
							Test_Process();//���Դ���
				break;
				case SYS_STATUS_SYSSET : //ϵͳ����
							Use_SysSetProcess();
				break;
				case SYS_STATUS_DEBUG:	
							Use_DebugProcess();//���Դ���
				break;
				case SYS_STATUS_RESET:
					//Reset_Process();//�����λ����
				break;   
				case SYS_STATUS_SYS://ϵͳ��Ϣ��ʾ
							Sys_Process();
				break;
				case SYS_STATUS_CLEAR:
							Clear_Process();
				break;
				case SYS_STATUS_TOOL://����
							Soft_Turnon();
			    break;
				case SYS_STATUS_SDFILE:
					
					SD_Process();
				break;
				default:
//				Disp_Clr( );//����
				//Error_Process();//������
				break;
		}

	}	
}









