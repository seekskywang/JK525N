#include "pbdata.h"
#include <CORE_CMINSTR.h>


/* ************************************ */
/*��������SpisendByte(INT8U dat)*/
/*���룺���͵�����*/
/*�������*/
/*����������SPI����һ���ֽ�*/
/* ************************************ */
u8 SpiRxByte(u8 rdry)
{	u8 i,temp;
//	temp = 0;
//	if(rdry)
		_SPI_DINH;
//	else
//		_SPI_DINL;
   //delay_1us(1);
	for(i=0; i<8; i++)
	{		
        _SPI_SCLKL;
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
		__NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
       // delay_1us(1);

		_SPI_SCLKH;
        temp <<= 1;
		if(_SPI_DOUT)  /* CPU_IN */
			temp++;
       // delay_1us(1);
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
		__NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
	}
    _SPI_SCLKL;
	return temp;
    
}



u32 Read_Convert_read(void)
{
    u32 temp=0;
	
    delay_1us(120);
    temp=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
    return temp;
    
}

void Ads1251_Int(void)
{
   _SPI_SCLKL; 

}

