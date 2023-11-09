#include "config.h"

void systemInit(void);

/***********************************************
�� �� ����
��    �ܣ�
˵    ����
��ڲ�����
���ó���:
�� �� ֵ��
************************************************/
void systemInit(void)
{
    delay_ms(1);
    GPIO_config();
    Uart_Device_Init();
    Timer0_Init();
    EA = 1; // �������ж�
}
u8 Timer[20] = {0};
#define RCV_GB_SIZE 8 // ���չ��485 ���ݵĳ���(N�ֽ�N<50)
// ��дЭ����룬����Ϊ=Serial_Port_Device.Rxd_Size
// ����Ҫ����0XFF���,Ҳ���԰Ѳ�Ҫ��0xffɾ��
u8 code GB_Protocol_Data[][RCV_GB_SIZE] = {
    {0x01, 0x04, 0x03, 0x01, 0x01, 0x01, 0x61, 0xDE},
    {0x01, 0x04, 0x03, 0x02, 0x01, 0x01, 0x91, 0xDE}};
#define RCV_LF_SIZE 11
u8 code LF_Protocol_Data[][RCV_LF_SIZE] = {
    {0x00, 0xFE, 0x61, 0x07, 0xFE, 0x55, 0x05, 0xC0, 0x04, 0x94},
    {0x00, 0xFE, 0x61, 0x06, 0xFE, 0x55, 0x04, 0x55, 0x01, 0x05}};
#define command_num 8
void Protocol_Translation() // ����Э��
{
    u8 i, index = 0;
    u8 Count_Rec_Arr1 = 0;
    u8 Count_Rec_Arr2 = 0;
    MODBUS_CRC16_2_Check(&Serial_Port_Device.Serial_Rxd_buf, sizeof(Serial_Port_Device.Serial_Rxd_buf));
    Serial_Port_Device.Serial_Rxd_buf[Serial_Port_Device.Rxd_Cnt + 1] = Decompose_8Bit.high_bit;
    Serial_Port_Device.Serial_Rxd_buf[Serial_Port_Device.Rxd_Cnt + 2] = Decompose_8Bit.low_bit;
    // �����ڽ��յ����������CRC-16У�飬�������һλд�뵽�������顣
    for (index = 0; index < command_num; index++)
    {
        for (i = 0; i < Serial_Port_Device.Rxd_Cnt + 1; i++)
        {
            if (Serial_Port_Device.Serial_Rxd_buf[i] == GB_Protocol_Data[index][i])
            {
                Count_Rec_Arr1++;
            }
            if (Serial_Port_Device.Serial_Rxd_buf[i] == LF_Protocol_Data[index][i])
            {
                Count_Rec_Arr2++;
            }
        }
        if (Count_Rec_Arr1 == command_num)
        {
            memset(&Serial_Port_Device.Serial_Txd_buf2, 0, sizeof(Serial_Port_Device.Serial_Txd_buf2));
            for (i = 0; i < (RCV_GB_SIZE + 2); i++)
                Serial_Port_Device.Serial_Txd_buf2[i] = GB_Protocol_Data[index][i];
            break;
        }
        else if (Count_Rec_Arr2 == command_num)
        {
            memset(&Serial_Port_Device.Serial_Txd_buf2, 0, sizeof(Serial_Port_Device.Serial_Txd_buf2));
            for (i = 0; i < (RCV_LF_SIZE + 2); i++)
                Serial_Port_Device.Serial_Txd_buf2[i] = LF_Protocol_Data[index][i];
            break;
        }
        else
        {
            Count_Rec_Arr1 = 0;
            Count_Rec_Arr2 = 0;
        }
    }
}
void Send_Protocol_Data()
{
    Protocol_Translation();
    Serial_Port_Device.Bus_Busy_LED_FLG = 1;
    Uart_Send2(7);
    //    Timer[index] = 10; // ��ʱ10S,���͵ڶ�������
}
void main(void)
{
    systemInit();
    while (1)
    {
        ClrWtd(); // watch dog clear
        TimeControl();
        // ������д����
        // �������²�����д�ض�����,�����ȡ���ݲ��Կɵ���ʱ��
        Serial_Port_Device.Timer    = 5;  // �Ͼ�ʱ��n ms
        Serial_Port_Device.Rxd_Size = 11; // 8
        Send_Protocol_Data();
    }
}
