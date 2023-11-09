#ifndef		__CONFIG_H
#define		__CONFIG_H


#define		ENABLE		1
#define		DISABLE		0




/*********************************************************/
#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
//#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��
//#define MAIN_Fosc		24000000L	//������ʱ��
/**************************************************************************/
#include <stdio.h>
#include <string.h>

#include	"STC8AXX.H"
#include	"delay.h"
#include	"GPIO.h"
#include 	"eeprom.h"
#include	"io_cfg.h"
#include	"ISR.h"
#include	"usart_Process.h"



//����ֵ
#define RESET		0
#define SET			!RESET


//////////////////////EEPROM////////////////////////////
#define EEPROM_BASIC_ADDR	0X0000	//����ַ
#define EEPROM_SECTION_SIZE	512		//������С

//���밴����Ϣ
////////////////////////////ʹ�õ�һ����//////////////////////////////////////////////////////////////
#define EE_DEVICE_ADDR 			EEPROM_BASIC_ADDR				//�豸�ĵ�ַ 1bytes	

#endif
