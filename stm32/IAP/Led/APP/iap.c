#include "iap.h"

pFunction       Jump_To_Bootloader; /* ����һ��App��ת���� */

/**
	* @funtion: ����ջ����ַ
	* @addr: ջ����ַ
	*/
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**
	* @funtion: ��ת��bootloader
	* @addr: bootloader��ַ
	*/
void iap_load_bootloader(uint32_t appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		Jump_To_Bootloader=(pFunction)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		Jump_To_Bootloader();								//��ת��APP.
	}
}
