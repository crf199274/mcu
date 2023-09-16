#include "gd32f10x_it.h"
#include "systick.h"

int main(void)
{
	systick_config(); /* 系统时钟初始化 */
	
	rcu_periph_clock_enable(RCU_GPIOB); /* 打开GPIOB的时钟 */
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10); /* 初始化IO */
	
	gpio_bit_write(GPIOB,GPIO_PIN_10,SET);
	
	while(1)
	{
		gpio_bit_write(GPIOB,GPIO_PIN_10,RESET);
		delay_1ms(500);
		gpio_bit_write(GPIOB,GPIO_PIN_10,SET);
		delay_1ms(500);
	}
}
