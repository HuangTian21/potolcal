#ifndef		__CONFIG_H
#define		__CONFIG_H


#define		ENABLE		1
#define		DISABLE		0




/*********************************************************/
#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟
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



//配置值
#define RESET		0
#define SET			!RESET


//////////////////////EEPROM////////////////////////////
#define EEPROM_BASIC_ADDR	0X0000	//基地址
#define EEPROM_SECTION_SIZE	512		//扇区大小

//输入按键信息
////////////////////////////使用第一扇区//////////////////////////////////////////////////////////////
#define EE_DEVICE_ADDR 			EEPROM_BASIC_ADDR				//设备的地址 1bytes	

#endif
