/**********************************************************************
 *				�ļ�����			USART_PROCESS.h
 *				�����ˣ�			½����
 *				�������ڣ�			2017.6.1
 *				�޸��ˣ�
 *				�޸����ڣ�
 *				�汾�ţ�			V1.0
 *				��ע��
 *				��˾��
 ********************************************************************/


#ifndef __USART_PROCESS_H__
#define __USART_PROCESS_H__

#include "config.h"


#define BaudRate1     9600UL                                  // ѡ������					//  ���ڲ�����
#define Timer2_Reload (65536UL - (MAIN_Fosc / 4 / BaudRate1)) // Timer 2 ��װֵ�� ��Ӧ300KHZ
#define Timer1_Reload (65536UL - (MAIN_Fosc / 4 / BaudRate1)) // Timer 1 ��װֵ�� ��Ӧ300KHZ
#define S485_R_STATE  0                                       // 0����
#define S485_T_STATE  1                                       // 1����
sbit S485     = P3 ^ 2;                                       // ͨѶ�ӿ�1���ͣ�0����
sbit S485_ZHQ = P1 ^ 3;                                       // ͨѶ�ӿ�1���ͣ�0����
// �봮����صı���
#define SERIAL_TXD_MAX 55
#define SERIAL_RXD_MAX 55
#define FRAME_BUFFER   3
typedef struct serial_port
{
    u8 Serial_Rxd_buf[SERIAL_RXD_MAX];
    u8 Rcv_Status;
    u8 Rxd_Cnt;
    u8 Rxd_Size;
    u8 RevTimer;
    u8 Timer;

    u8 Txd_Cnt2;
    u8 Serial_Txd_buf2[SERIAL_TXD_MAX];
    u8 Txd_Size2;


    u8  Ms_10_flg;
    u16 MS_1_S;
    u8  Bus_Busy_LED_FLG;
} serial_port_device;
typedef struct Decompose_16bit_8Bit
{
    u8 high_bit;
    u8 low_bit;
} decompose_bit;
extern decompose_bit xdata      Decompose_8Bit;
extern serial_port_device xdata Serial_Port_Device;

void Uart_Device_Init(void);
void TimeControl(void);
void MODBUS_CRC16_2_Check(u8 *addr, u8 num_length);
#endif
