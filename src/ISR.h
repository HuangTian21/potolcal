
/**********************************************************************
*				文件名：			ISR.h
*				创建人：			陆洋洋
*				创建日期：			2017.6.1
*				修改人：
*				修改日期：	
*				版本号：			V1.0
*				备注：
*				公司：
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
  

