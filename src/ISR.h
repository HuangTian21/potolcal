
/**********************************************************************
*				�ļ�����			ISR.h
*				�����ˣ�			½����
*				�������ڣ�			2017.6.1
*				�޸��ˣ�
*				�޸����ڣ�	
*				�汾�ţ�			V1.0
*				��ע��
*				��˾��
********************************************************************/


#ifndef __ISR_H__
#define __ISR_H__

#include	"config.h"

#define T1MS	 0

void Uart_Init(void);
void Timer0_Init(void);
void Uart_ZHQ_Init(void);
void Uart_Send2(u8 frame_size);

#endif//__ISR_H__
  

