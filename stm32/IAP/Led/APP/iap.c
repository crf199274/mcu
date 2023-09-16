#include "iap.h"

pFunction       Jump_To_Bootloader; /* 声明一个App跳转函数 */

/**
	* @funtion: 设置栈顶地址
	* @addr: 栈顶地址
	*/
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/**
	* @funtion: 跳转到bootloader
	* @addr: bootloader地址
	*/
void iap_load_bootloader(uint32_t appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		Jump_To_Bootloader=(pFunction)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		Jump_To_Bootloader();								//跳转到APP.
	}
}
