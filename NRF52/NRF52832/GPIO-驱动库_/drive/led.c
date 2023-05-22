#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "led.h"

void LED_Init(void)
{
	
	NVIC_EnableIRQ(GPIOTE_IRQn);//�ж�Ƕ������
	
	NRF_GPIOTE->CONFIG[0] =  (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos)//���ü��ԣ���ת
                           | (LED_1 << GPIOTE_CONFIG_PSEL_Pos)  //�󶨹ܽ�
                           | (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);//����ģʽ
	
	NRF_GPIOTE->CONFIG[1] =  (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos)//����͵�ƽ
                           | (LED_2<< GPIOTE_CONFIG_PSEL_Pos)  
                           | (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos);////�����������״̬����ͨ��������ģʽ����ϸ˵����ο����̳̣�
	
}
