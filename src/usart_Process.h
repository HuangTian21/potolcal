/**********************************************************************
 *				文件名：			USART_PROCESS.h
 *				创建人：			陆洋洋
 *				创建日期：			2017.6.1
 *				修改人：
 *				修改日期：
 *				版本号：			V1.0
 *				备注：
 *				公司：
 ********************************************************************/


#ifndef __USART_PROCESS_H__
#define __USART_PROCESS_H__

#include "config.h"


#define BaudRate1     9600UL                                  // 选择波特率					//  串口波特率
#define Timer2_Reload (65536UL - (MAIN_Fosc / 4 / BaudRate1)) // Timer 2 重装值， 对应300KHZ
#define Timer1_Reload (65536UL - (MAIN_Fosc / 4 / BaudRate1)) // Timer 1 重装值， 对应300KHZ
#define S485_R_STATE  0                                       // 0接收
#define S485_T_STATE  1                                       // 1发送
sbit S485     = P3 ^ 2;                                       // 通讯接口1发送，0接收
sbit S485_ZHQ = P1 ^ 3;                                       // 通讯接口1发送，0接收
// 与串口相关的变量
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
