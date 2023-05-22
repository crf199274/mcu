#include "nrf_gpio.h"
#include "led.h"

_Bool ledState = LOW;

void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 

void LED_OPEN_CLOSE(int ledx,_Bool state)
{
	if(state == true)
	{
		nrf_gpio_pin_set(ledx);
	}else{
		nrf_gpio_pin_clear(ledx);
	}
}

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	switch(pin)
	{
		case KEY_0:
			nrf_gpio_pin_toggle(LED_1);
			break;
		case KEY_1:
			nrf_gpio_pin_toggle(LED_2);
			break;
		default:break;
 }
}

void LED_Init(void)
{
	ret_code_t err_code;
	
	//配置IO为输出模式
	nrf_gpio_cfg_output(LED_0);
  nrf_gpio_cfg_output(LED_1);
  nrf_gpio_cfg_output(LED_2);
	nrf_gpio_cfg_output(LED_3);
	
	//配置IO为输入模式
//	nrf_gpio_cfg_input(KEY_0,NRF_GPIO_PIN_PULLUP);
	
	//初始化GPIOTE
	err_code = nrf_drv_gpiote_init();
	APP_ERROR_CHECK(err_code);

	
	//KEY_0
	
	nrfx_gpiote_out_config_t out_config1 = GPIOTE_CONFIG_OUT_TASK_TOGGLE(NRF_GPIOTE_INITIAL_VALUE_HIGH);
	//设置GPIOTE输入，极性，模式
	err_code = nrf_drv_gpiote_out_init(LED_3, &out_config1);
	APP_ERROR_CHECK(err_code);
	
	//使能GPIOTE
	nrf_drv_gpiote_out_task_enable(LED_3);
	
	//KEY_1
	
	nrfx_gpiote_out_config_t out_config2 = GPIOTE_CONFIG_OUT_TASK_LOW;
	//设置GPIOTE输入，极性，模式
	err_code = nrf_drv_gpiote_out_init(LED_2, &out_config2);
	APP_ERROR_CHECK(err_code);
	
	//使能GPIOTE
	nrf_drv_gpiote_out_task_enable(LED_2);
	
	LED_OPEN_CLOSE(LED_0,LED_OFF);
	LED_OPEN_CLOSE(LED_1,LED_OFF);
	
}
