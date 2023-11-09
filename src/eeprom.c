//	��������STCϵ�е�����EEPROM��д����
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
// ����: void	ISP_Disable(void)
// ����: ��ֹ����ISP/IAP.
// ����: non.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void	DisableEEPROM(void)
{
	ISP_CONTR = 0;			//��ֹISP/IAP����
	ISP_CMD   = 0;			//ȥ��ISP/IAP����
	ISP_TRIG  = 0;			//��ֹISP/IAP�����󴥷�
	ISP_ADDRH = 0x80; // ����ַ���õ��� IAP ����
	ISP_ADDRL = 0;
}

//========================================================================
// ����: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: ��ָ��EEPROM�׵�ַ����n���ֽڷ�ָ���Ļ���.
// ����: EE_address:  ����EEPROM���׵�ַ.
//       DataAddress: �������ݷŻ�����׵�ַ.
//       number:      �������ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================

void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	//EA = 0;		//��ֹ�ж�
	ISP_CONTR = WT_24M;
	ISP_CMD = 1;		//ISP��������
	do
	{
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�

		IAP_TRIG = 0x5a; // д�������� (0x5a)
		IAP_TRIG = 0xa5; // д�������� (0xa5)


											//����A5H��ISP/IAP������������������
											//CPU�ȴ�IAP��ɺ󣬲Ż����ִ�г���
		_nop_();
		*DataAddress = ISP_DATA;			//��������������
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	//EA = 1;		//���������ж�
}


/******************** ������������ *****************/
//========================================================================
// ����: void EEPROM_SectorErase(u16 EE_address)
// ����: ��ָ����ַ��EEPROM��������.
// ����: EE_address:  Ҫ����������EEPROM�ĵ�ַ.
// ����: non.
// �汾: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(u16 EE_address)
{
	//EA = 0;		//��ֹ�ж�
											//ֻ������������û���ֽڲ�����512�ֽ�/������
											//����������һ���ֽڵ�ַ����������ַ��
	ISP_CONTR = WT_24M; // ʹ�� IAP
	IAP_CMD = 3; // ���� IAP д����	

	ISP_ADDRH = EE_address / 256;			//��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	ISP_ADDRL = EE_address % 256;			//��������ַ���ֽ�
	IAP_TRIG = 0x5a; // д�������� (0x5a)
	IAP_TRIG = 0xa5; // д�������� (0xa5)
	_nop_();
	DisableEEPROM();
	//EA = 1;		//���������ж�
}

//========================================================================
// ����: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// ����: �ѻ����n���ֽ�д��ָ���׵�ַ��EEPROM.
// ����: EE_address:  д��EEPROM���׵�ַ.
//       DataAddress: д��Դ���ݵĻ�����׵�ַ.
//       number:      д����ֽڳ���.
// ����: non.
// �汾: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
{
	//EA = 0;		//��ֹ�ж�

	ISP_CONTR = WT_24M; // ʹ�� IAP
	IAP_CMD = 2; // ���� IAP д����	
	do
	{
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_DATA  = *DataAddress;			//�����ݵ�ISP_DATA��ֻ�����ݸı�ʱ����������
		IAP_TRIG = 0x5a; // д�������� (0x5a)
		IAP_TRIG = 0xa5; // д�������� (0xa5)
		_nop_();
		EE_address++;
		DataAddress++;
	}while(--number);

	DisableEEPROM();
	//EA = 1;		//���������ж�
}

/***********************************************************************
* ����:  
* ����: 	��ĳһ��������д��һ�������ֽڣ����������ֽ�
* ����:     ������48*6
* ����: ��
* ��ע��
************************************************************************/
void EEPROM_write_Save_Other(u16 EE_address,u8 *DataAddress,u16 number)
{
	u8 xdata temp[512];
	u16  i;
	u16  Save_Addr_Offset = EE_address%512;	//ƫ�Ƶ�ַ
	if(number>512)									//���ݳ��Ȳ��Ϸ�
	{
		return ;
	}
	EE_address=EE_address - Save_Addr_Offset;//�õ�һ����������ʼ��ַ
    EEPROM_read_n(EE_address,temp,512);
	EEPROM_SectorErase(EE_address);
	for(i=0;i<number;i++)
	{
		temp[Save_Addr_Offset+i] = DataAddress[i];  //�ı����ݵ�����
	}
	EEPROM_write_n(EE_address,temp,512);
}
//д�����������
void EEPROM_write_n_Sector(u16 EE_address,u8 *DataAddress,u16 number)
{

	u16  Sector_Offset;
	u16  Save_Addr_Offset;
	u16  writer_n_byte;
	Save_Addr_Offset = EE_address%512;	//ƫ�Ƶ�ַ
	Sector_Offset = EE_address%512+number;	
	if (Sector_Offset > 512)//��Խ����,�ֶ��
	{
		//����һ����д�뼸���ֽ�
		writer_n_byte = 512-Save_Addr_Offset;
		EEPROM_write_Save_Other(EE_address,DataAddress,writer_n_byte);
		//Ȼ����дһ��
		EEPROM_write_Save_Other(EE_address+writer_n_byte,&DataAddress[writer_n_byte],number-writer_n_byte);
	}
	else
	{
		EEPROM_write_Save_Other(EE_address,DataAddress,number);
	}
}
