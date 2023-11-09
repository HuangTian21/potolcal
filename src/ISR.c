/**********************************************************************
 *				�ļ�����			ISR.C
 *				�����ˣ�			½����
 *				�������ڣ�		2017.6.1
 *				�޸��ˣ�
 *				�޸����ڣ�
 *				�汾�ţ�			V1.0
 *				��ע��
 *				��˾��
 ********************************************************************/

#include "ISR.h"

static u8 Timer0_Interrupt_Mode;
static void Timer0_Config_T1MS_Mode(void);

/***********************************************************************
 * ����:
 * ����: 	��������9600
 * ����:
 * ����: ��
 * ��ע��
 ************************************************************************/
void Uart_Init(void)
{
	S485 = S485_R_STATE; // 0���գ�1����
	P_SW1 &= ~0xc0;		 // UART1 ʹ��P30 P31��	Ĭ��
	PCON &= 0x3f;
	AUXR &= ~0x01; // S1 BRT Use Timer1;
	AUXR |= 0x40;  // Timer1 set as 1T mode
	TH1 = (u8)(Timer2_Reload >> 8);
	TL1 = (u8)Timer2_Reload;
	TR1 = 1;
	SCON = 0x50; // 8λ����,�ɱ䲨����
	ES = 1;		 // �����ж�
	PS = 1;
	EA = 1;
}
/***********************************************************************
 * ����:
 * ����: 	��������9600
 * ����:
 * ����: ��
 * ��ע��
 ************************************************************************/
void Uart_ZHQ_Init(void)
{
	S485_ZHQ = S485_R_STATE; // 0���գ�1����
	S2CON = 0x50;			 // 8λ����,�ɱ䲨����
	T2L = (u8)Timer2_Reload;
	T2H = (u8)(Timer2_Reload >> 8);
	AUXR |= 0x04; // ��ʱ��2ʱ��ΪFosc,��1T
	AUXR |= 0x10; // ������ʱ��2
	IE2 |= 0x01;  // ����uart2�ж�
}

/***********************************************************************
 * ����:
 * ����: 	��2�ڷ���
 * ����:
 * ����: ��
 * ��ע��
 ************************************************************************/
void Uart_Send2(u8 frame_size)
{
	u8 i = 0;
	while (Serial_Port_Device.Txd_Cnt2 != 0 && i < 100)
	{
		i++;
		Delayus(500);
	}
	S485_ZHQ = S485_T_STATE; // 485����ʹ��
	Delayus(100);
	Serial_Port_Device.Txd_Cnt2 = 1;
	Serial_Port_Device.Txd_Size2 = frame_size;
	S2BUF = Serial_Port_Device.Serial_Txd_buf2[0]; // ��ʼ�ֽ�
}

/********************* UART1�жϺ���************************/
void UART1_int(void) interrupt UART1_VECTOR // ���ڽ���
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
					Serial_Port_Device.Rcv_Status = 1; // �������
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
/********************* UART2�жϺ���************************/
void UART2_int(void) interrupt UART2_VECTOR // ���ڷ���
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
 * ����:
 * ����:	  ��ʱ��0���ú���
 * ����:
 * ����: ��
 * ��ע��
 ************************************************************************/
void Timer0_Init(void)
{
	Timer0_Config_T1MS_Mode();
}
/***********************************************************************
 * ����:
 * ����:	  ��ʱ��0���ú���
 * ����:
 * ����: ��
 * ��ע��	ʹ�ö�ʱ��0��Ϊ1ms����
 ************************************************************************/
static void Timer0_Config_T1MS_Mode(void)
{
	AUXR &= ~0x80;
	TMOD &= 0xf0; // ��ʱ��0 ���������ʽ
	TMOD |= 0X01; // ��ʱ��0����Ϊ16λ��ʱ��
	TH0 = 0XF8;
	TL0 = 0Xcc; // 1MS
	ET0 = 1;	// ��ʱ��T0�ж�����
	TR0 = 1;	// ������ʱ��0
	PT0 = 0;
	Timer0_Interrupt_Mode = T1MS;
}

/***********************************************************************
 * ����:
 * ����:	 ��ʱ��0�жϺ���
 * ����:
 * ����: ��
 * ��ע��
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
				Serial_Port_Device.Rcv_Status = 1; // �������
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
