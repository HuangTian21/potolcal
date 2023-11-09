/**********************************************************************
 *				文件名：			ISR.C
 *				创建人：			陆洋洋
 *				创建日期：		2017.6.1
 *				修改人：
 *				修改日期：
 *				版本号：			V1.0
 *				备注：
 *				公司：
 ********************************************************************/

#include "ISR.h"

static u8 Timer0_Interrupt_Mode;
static void Timer0_Config_T1MS_Mode(void);

/***********************************************************************
 * 函数:
 * 描述: 	串口配置9600
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void Uart_Init(void)
{
	S485 = S485_R_STATE; // 0接收；1发送
	P_SW1 &= ~0xc0;		 // UART1 使用P30 P31口	默认
	PCON &= 0x3f;
	AUXR &= ~0x01; // S1 BRT Use Timer1;
	AUXR |= 0x40;  // Timer1 set as 1T mode
	TH1 = (u8)(Timer2_Reload >> 8);
	TL1 = (u8)Timer2_Reload;
	TR1 = 1;
	SCON = 0x50; // 8位数据,可变波特率
	ES = 1;		 // 允许中断
	PS = 1;
	EA = 1;
}
/***********************************************************************
 * 函数:
 * 描述: 	串口配置9600
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void Uart_ZHQ_Init(void)
{
	S485_ZHQ = S485_R_STATE; // 0接收；1发送
	S2CON = 0x50;			 // 8位数据,可变波特率
	T2L = (u8)Timer2_Reload;
	T2H = (u8)(Timer2_Reload >> 8);
	AUXR |= 0x04; // 定时器2时钟为Fosc,即1T
	AUXR |= 0x10; // 启动定时器2
	IE2 |= 0x01;  // 允许uart2中断
}

/***********************************************************************
 * 函数:
 * 描述: 	串2口发送
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void Uart_Send2(u8 frame_size)
{
	u8 i = 0;
	while (Serial_Port_Device.Txd_Cnt2 != 0 && i < 100)
	{
		i++;
		Delayus(500);
	}
	S485_ZHQ = S485_T_STATE; // 485发送使能
	Delayus(100);
	Serial_Port_Device.Txd_Cnt2 = 1;
	Serial_Port_Device.Txd_Size2 = frame_size;
	S2BUF = Serial_Port_Device.Serial_Txd_buf2[0]; // 起始字节
}

/********************* UART1中断函数************************/
void UART1_int(void) interrupt UART1_VECTOR // 串口接收
{
	u8 Res;
	if (RI)
	{
		Res = SBUF;
		if (Serial_Port_Device.Rcv_Status == 0)
		{
			Serial_Port_Device.RevTimer = 0;
			if (Serial_Port_Device.Rxd_Cnt < Serial_Port_Device.Rxd_Size)
			{
				Serial_Port_Device.Serial_Rxd_buf[Serial_Port_Device.Rxd_Cnt] = Res;
				Serial_Port_Device.Rxd_Cnt++;
				if (Serial_Port_Device.Rxd_Cnt == Serial_Port_Device.Rxd_Size)
				{
					Serial_Port_Device.Rcv_Status = 1; // 接收完成
					Serial_Port_Device.Rxd_Cnt = 0;
				}
			}
		}

		RI = 0;
	}
	else if (TI)
	{
		TI = 0;
	}
	else
	{
		RI = 0;
		TI = 0;
	}
}
/********************* UART2中断函数************************/
void UART2_int(void) interrupt UART2_VECTOR // 串口发送
{
	u8 Res;
	if (S2CON & S2RI)
	{
		Res = S2BUF;

		S2CON &= ~S2RI;
	}
	else if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;
		if (Serial_Port_Device.Txd_Cnt2 < Serial_Port_Device.Txd_Size2)
		{
			S2BUF = Serial_Port_Device.Serial_Txd_buf2[Serial_Port_Device.Txd_Cnt2];
			Serial_Port_Device.Txd_Cnt2++;
		}
		else
		{
			Serial_Port_Device.Txd_Cnt2 = 0;
			Delayus(5);
			S485_ZHQ = S485_R_STATE;
		}
	}
	else
	{
		S2CON &= ~S2RI;
		S2CON &= ~S2TI;
	}
}

/***********************************************************************
 * 函数:
 * 描述:	  定时器0配置函数
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void Timer0_Init(void)
{
	Timer0_Config_T1MS_Mode();
}
/***********************************************************************
 * 函数:
 * 描述:	  定时器0配置函数
 * 参数:
 * 返回: 无
 * 备注：	使用定时器0作为1ms计数
 ************************************************************************/
static void Timer0_Config_T1MS_Mode(void)
{
	AUXR &= ~0x80;
	TMOD &= 0xf0; // 定时器0 清除工作方式
	TMOD |= 0X01; // 定时器0，均为16位定时器
	TH0 = 0XF8;
	TL0 = 0Xcc; // 1MS
	ET0 = 1;	// 定时器T0中断允许
	TR0 = 1;	// 启动定时器0
	PT0 = 0;
	Timer0_Interrupt_Mode = T1MS;
}

/***********************************************************************
 * 函数:
 * 描述:	 定时器0中断函数
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void Timer_Hardware_Interrupt_Timer0(void) interrupt TIMER0_VECTOR
{
	static u8 Ms_Cnt = 0;
	static u8 Ms10_Cnt = 0;
	static u8 ms100cnt = 0;
	if (Timer0_Interrupt_Mode == T1MS)
	{
		TH0 = 0XF8;
		TL0 = 0Xcc; // 1MS
		Ms_Cnt++;
		if (Serial_Port_Device.RevTimer++ > Serial_Port_Device.Timer)
		{
			if (Serial_Port_Device.Rxd_Cnt == Serial_Port_Device.Rxd_Size)
			{
				Serial_Port_Device.Rcv_Status = 1; // 接收完成
			}
			Serial_Port_Device.Rxd_Cnt = 0;
			Serial_Port_Device.RevTimer = 0;
		}
		if (8 == Ms_Cnt)
		{
		}
		if (Ms_Cnt > 9)
		{
			Ms_Cnt = 0;
			ms100cnt++;
			if (ms100cnt > 99)
			{
				ms100cnt = 0;
				Serial_Port_Device.MS_1_S = 1;
			}
			ClrWtd(); // watch dog clear
			Serial_Port_Device.Ms_10_flg = SET;
			Ms10_Cnt++;
			if (Ms10_Cnt > 9)
			{
				Ms10_Cnt = 0;
			}
		}
	}
}
