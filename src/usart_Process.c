
/**********************************************************************
 *				文件名：			USART_PROCESS.c
 *				创建人：			陆洋洋
 *				创建日期：			2017.6.1
 *				修改人：
 *				修改日期：
 *				版本号：			V1.0
 *				备注：
 *				公司：
 ********************************************************************/

#include "usart_Process.h"

serial_port_device xdata Serial_Port_Device;
decompose_bit xdata      Decompose_8Bit;
static void              Uart_Search_Slave(u8 *pBuf);
/***********************************************************************
 * 函数:
 * 描述: 	串口变量初始化
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/

void Uart_Device_Init(void)
{
    memset(&Serial_Port_Device, 0, sizeof(struct serial_port));
    Uart_Init();
    Uart_ZHQ_Init();
}

/***********************************************************************
 * 函数:
 * 描述: 	10ms定时
 * 参数:
 * 返回: 无
 * 备注：
 ************************************************************************/
void TimeControl(void)
{
    static u8 idata ms10cnt = 0;
    if (Serial_Port_Device.Ms_10_flg == RESET)
        return;
    Serial_Port_Device.Ms_10_flg = RESET;

    ms10cnt++;
    if (ms10cnt > 4)
    {
        static u8 cnt1s = 0;
        ms10cnt         = 0;
        cnt1s++;

        if (Serial_Port_Device.Bus_Busy_LED_FLG == 1)
        {
            static u8 tempcnt = 0;
            if (cnt1s > 1)
            {
                cnt1s = 0;
                LED   = !LED;
                tempcnt++;
                if (tempcnt > 5)
                {
                    tempcnt                             = 0;
                    Serial_Port_Device.Bus_Busy_LED_FLG = 0;
                }
            }
        }
        else
        {
            if (cnt1s > 6)
            {
                cnt1s = 0;
                LED   = !LED;
            }
        }
    }
}

void MODBUS_CRC16_2_Check(u8 *addr, u8 num_length)
{
    u16 i, j, temp;
    u16 crc = 0xFFFF;
    for (i = 0; i < num_length; i++)
    {
        crc = crc ^ (*addr);
        for (j = 0; j < 8; j++)
        {
            temp = crc & 0x0001;
            crc  = crc >> 1;
            if (temp)
            {
                crc = crc ^ 0xA001;
            }
        }
        addr++;
    }
    Decompose_8Bit.high_bit = (crc >> 8) & 0xFF;
    Decompose_8Bit.low_bit  = crc & 0xFF;
}