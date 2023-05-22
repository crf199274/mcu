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

void LED_Init(void)
{
	//����IOΪ���ģʽ
	nrf_gpio_cfg_output(LED_0);
  nrf_gpio_cfg_output(LED_1);
  nrf_gpio_cfg_output(LED_2);
	nrf_gpio_cfg_output(LED_3);
	
	//����IOΪ����ģʽ
	nrf_gpio_cfg_input(KEY_0,NRF_GPIO_PIN_PULLUP);
	
	//
	NVIC_EnableIRQ(GPIOTE_IRQn);
	
	//
	NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos)//���ô��������½���
                           | (KEY_0 << GPIOTE_CONFIG_PSEL_Pos) //�󶨰���0 
                           | (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos);//����GPIOTEģʽ
	
	NRF_GPIOTE->INTENSET  = GPIOTE_INTENSET_IN0_Set << GPIOTE_INTENSET_IN0_Pos;// ʹ���ж�����:
	
	
	LED_OPEN_CLOSE(LED_0,LED_OFF);
	LED_OPEN_CLOSE(LED_1,LED_OFF);
	LED_OPEN_CLOSE(LED_2,LED_OFF);
	LED_OPEN_CLOSE(LED_3,LED_OFF);
	
}

void GPIOTE_IRQHandler(void)
{

    if((NRF_GPIOTE->EVENTS_IN[0] == 1) && 
        (NRF_GPIOTE->INTENSET & GPIOTE_INTENSET_IN0_Msk))
    {
			NRF_GPIOTE->EVENTS_IN[0] = 0; //�ж��¼�����.
			Delay(10000);	
			 
			ledState = nrf_gpio_pin_out_read(LED_2);
			
			LED_OPEN_CLOSE(LED_2,!ledState);
			
    }

 
}
