#include "stm32f10x.h"
#include "led.h"
#include "stdelay.h"

int main()
{
	SCB->VTOR = FLASH_BASE | 0x1400; /* Vector Table Relocation in Internal FLASH. */
	
	stdelay_init(72);
	
	LED_Init();
	
	while(1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);						 //PB.5 输出高
		stdelay_ms(500);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PB.5 输出高
		stdelay_ms(500);
	}
	
}
