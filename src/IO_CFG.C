/**********************************************************************
*				文件名：			IO_CFG.C
*				创建人：			陆洋洋
*				创建日期：		2018.4.1
*				修改人：
*				修改日期：	
*				版本号：			V1.0
*				备注：
*				公司：
********************************************************************/

#include	"io_cfg.h"



/***********************************************************************
* 函数:  
* 描述: 	IO配置函数
* 参数:
* 返回: 无
* 备注：
************************************************************************/

void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
		//通讯接口485
   	GPIO_InitStructure.Pin  = GPIO_Pin_2;		//通讯接口//P3.2
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//推挽输出
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//初始化

		//通讯接口485第三方
   	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//通讯接口//P3.2
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//推挽输出
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//初始化

	//LED
   	GPIO_InitStructure.Pin  = GPIO_Pin_7;		//LEDP2.7
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//推挽输出
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//初始化


	AUXR &= ~2;//使用内部2k SRAM
	
}


/***********************************************************************
* 函数:  
* 描述: 	看门狗配置函数
* 参数:
* 返回: 无
* 备注：
************************************************************************/
void ClrWtd(void)
{
	WDT_CONTR=0X3E;//0X3D;		//清溢出标志，启动wtd，清计数器，wtd在空闲模式下计数，wtd溢出时间2s
}

