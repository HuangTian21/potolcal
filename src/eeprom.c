//	本程序是STC系列的内置EEPROM读写程序。
#include "eeprom.h"
#define  WT_30M  0x80
#define  WT_24M  0x81
#define  WT_20M  0x82
#define  WT_12M  0x83
#define  WT_6M  0x84
#define  WT_3M  0x85
#define  WT_2M  0x86
#define  WT_1M  0x87

//========================================================================
// 函数: void	ISP_Disable(void)
// 描述: 禁止访问ISP/IAP.
// 参数: non.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void	DisableEEPROM(void)
{
	ISP_CONTR = 0;			//禁止ISP/IAP操作
	ISP_CMD   = 0;			//去除ISP/IAP命令
	ISP_TRIG  = 0;			//防止ISP/IAP命令误触发
	ISP_ADDRH = 0x80; // 将地址设置到非 IAP 区域
	ISP_ADDRL = 0;
}

//========================================================================
// 函数: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 从指定EEPROM首地址读出n个字节放指定的缓冲.
// 参数: EE_address:  读出EEPROM的首地址.
//       DataAddress: 读出数据放缓冲的首地址.
//       number:      读出的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================

void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	//EA = 0;		//禁止中断
	ISP_CONTR = WT_24M;
	ISP_CMD = 1;		//ISP读出命令
	do
	{
		ISP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		//送地址低字节

		IAP_TRIG = 0x5a; // 写触发命令 (0x5a)
		IAP_TRIG = 0xa5; // 写触发命令 (0xa5)


											//送完A5H后，ISP/IAP命令立即被触发启动
											//CPU等待IAP完成后，才会继续执行程序。
		_nop_();
		*DataAddress = ISP_DATA;			//读出的数据送往
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	//EA = 1;		//重新允许中断
}


/******************** 扇区擦除函数 *****************/
//========================================================================
// 函数: void EEPROM_SectorErase(u16 EE_address)
// 描述: 把指定地址的EEPROM扇区擦除.
// 参数: EE_address:  要擦除的扇区EEPROM的地址.
// 返回: non.
// 版本: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(u16 EE_address)
{
	//EA = 0;		//禁止中断
											//只有扇区擦除，没有字节擦除，512字节/扇区。
											//扇区中任意一个字节地址都是扇区地址。
	ISP_CONTR = WT_24M; // 使能 IAP
	IAP_CMD = 3; // 设置 IAP 写命令	

	ISP_ADDRH = EE_address / 256;			//送扇区地址高字节（地址需要改变时才需重新送地址）
	ISP_ADDRL = EE_address % 256;			//送扇区地址低字节
	IAP_TRIG = 0x5a; // 写触发命令 (0x5a)
	IAP_TRIG = 0xa5; // 写触发命令 (0xa5)
	_nop_();
	DisableEEPROM();
	//EA = 1;		//重新允许中断
}

//========================================================================
// 函数: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 把缓冲的n个字节写入指定首地址的EEPROM.
// 参数: EE_address:  写入EEPROM的首地址.
//       DataAddress: 写入源数据的缓冲的首地址.
//       number:      写入的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	//EA = 0;		//禁止中断

	ISP_CONTR = WT_24M; // 使能 IAP
	IAP_CMD = 2; // 设置 IAP 写命令	
	do
	{
		ISP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		//送地址低字节
		ISP_DATA  = *DataAddress;			//送数据到ISP_DATA，只有数据改变时才需重新送
		IAP_TRIG = 0x5a; // 写触发命令 (0x5a)
		IAP_TRIG = 0xa5; // 写触发命令 (0xa5)
		_nop_();
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	//EA = 1;		//重新允许中断
}

/***********************************************************************
* 函数:  
* 描述: 	在某一个扇区内写入一个或多个字节，保存其他字节
* 参数:     不大于48*6
* 返回: 无
* 备注：
************************************************************************/
void EEPROM_write_Save_Other(u16 EE_address,u8 *DataAddress,u16 number)
{
	u8 xdata temp[512];
	u16  i;
	u16  Save_Addr_Offset = EE_address%512;	//偏移地址
	if(number>512)									//数据长度不合法
	{
		return ;
	}
	EE_address=EE_address - Save_Addr_Offset;//得到一个扇区的起始地址
    EEPROM_read_n(EE_address,temp,512);
	EEPROM_SectorErase(EE_address);
	for(i=0;i<number;i++)
	{
		temp[Save_Addr_Offset+i] = DataAddress[i];  //改变数据的内容
	}
	EEPROM_write_n(EE_address,temp,512);
}
//写多个扇区函数
void EEPROM_write_n_Sector(u16 EE_address,u8 *DataAddress,u16 number)
{

	u16  Sector_Offset;
	u16  Save_Addr_Offset;
	u16  writer_n_byte;
	Save_Addr_Offset = EE_address%512;	//偏移地址
	Sector_Offset = EE_address%512+number;	
	if (Sector_Offset > 512)//跨越扇区,分多次
	{
		//看第一次能写入几个字节
		writer_n_byte = 512-Save_Addr_Offset;
		EEPROM_write_Save_Other(EE_address,DataAddress,writer_n_byte);
		//然后再写一次
		EEPROM_write_Save_Other(EE_address+writer_n_byte,&DataAddress[writer_n_byte],number-writer_n_byte);
	}
	else
	{
		EEPROM_write_Save_Other(EE_address,DataAddress,number);
	}
}
