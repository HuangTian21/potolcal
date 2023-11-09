#include	"delay.h"

void Delay1ms()		//@22.1184MHz
{
	unsigned char i, j;

	i = 29;
	j = 183;
	do
	{
		while (--j);
	} while (--i);
}



void Delay1us()		//@22.1184MHz
{
	unsigned char i;

	_nop_();
	i = 5;
	while (--i);
}

void  delay_ms(u16 ms)
{
	do
	{
		Delay1ms();
	}while(--ms);
}

/***********************************************************************
* 函数:  
* 描述: 晶振22.1184Mhz
* 参数:
* 返回: 无
* 备注：
************************************************************************/
void Delayus(u16 Tus)
{
	do
	{
		Delay1us();
	}while(--Tus);

}
