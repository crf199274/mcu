#include "iap.h"

pFunction       Jump_To_Application; /* 声明一个App跳转函数 */
uint16_t iapbuf[1024]; /* 升级数据缓冲 */
/*
 * funtion: 设置栈顶地址
 * addr: 栈顶地址
*/
__asm void MSR_MSP(uint32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

/*
 * funtion: 把升级数据写入App存储空间
 * appxaddr: 应用程序的起始地址
 * appbuf: 应用程序CODE.
 * appsize: 应用程序大小(字节).
 * return: none
*/
void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint16_t t;
	uint16_t i=0;
	uint16_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=2)
	{						    
		temp=(uint16_t)dfu[1]<<8;
		temp+=(uint16_t)dfu[0];	  
		dfu+=2;//偏移2个字节
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			STMFLASH_Write(fwaddr,iapbuf,1024);	
			fwaddr+=2048;//偏移2048  16=2*8.所以要乘以2.
		}
	}
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

/*
 * funtion: 跳转到应用程序段
 * appxaddr:用户代码起始地址.
 * return: none
*/
void iap_load_app(uint32_t appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		Jump_To_Application=(pFunction)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		Jump_To_Application();									//跳转到APP.
	}
}	


