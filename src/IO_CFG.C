/**********************************************************************
*				�ļ�����			IO_CFG.C
*				�����ˣ�			½����
*				�������ڣ�		2018.4.1
*				�޸��ˣ�
*				�޸����ڣ�	
*				�汾�ţ�			V1.0
*				��ע��
*				��˾��
********************************************************************/

#include	"io_cfg.h"



/***********************************************************************
* ����:  
* ����: 	IO���ú���
* ����:
* ����: ��
* ��ע��
************************************************************************/

void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
		//ͨѶ�ӿ�485
   	GPIO_InitStructure.Pin  = GPIO_Pin_2;		//ͨѶ�ӿ�//P3.2
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//�������
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	//��ʼ��

		//ͨѶ�ӿ�485������
   	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//ͨѶ�ӿ�//P3.2
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//�������
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	//��ʼ��

	//LED
   	GPIO_InitStructure.Pin  = GPIO_Pin_7;		//LEDP2.7
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//�������
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	//��ʼ��


	AUXR &= ~2;//ʹ���ڲ�2k SRAM
	
}


/***********************************************************************
* ����:  
* ����: 	���Ź����ú���
* ����:
* ����: ��
* ��ע��
************************************************************************/
void ClrWtd(void)
{
	WDT_CONTR=0X3E;//0X3D;		//�������־������wtd�����������wtd�ڿ���ģʽ�¼�����wtd���ʱ��2s
}
