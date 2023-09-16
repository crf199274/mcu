#include "usart.h"	   	   

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (uint8_t) ch;      
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}

 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//uint16_t USART_RX_STA=0;       //����״̬���	  
uint16_t USART_RX_CNT_CURRENT = 0;
uint16_t USART_RX_CNT_LAST = 0;
uint16_t USART_RX_LENTH = 0;

/**
  * @brief  ����1��ʼ��
  * @param  bound: ����ͨ�ŵĲ�����
	*         ͨ����ֵΪ9600,115200 ...
  * @retval None
  */
void uart_init(uint32_t bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/**
  * @brief  �رմ���1
  * @param  None
  * @retval None
  */
void USART1_CLOSED(void)
{
	USART_Cmd(USART1,DISABLE);                    /* ʧ�ܴ���1 */
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);	/* �رս����ж� */
	USART_DeInit(USART1);													/* ��λ����1�ļĴ��� */
}

/**
  * @brief  ����1�жϷ�����
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res=0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		USART_RX_BUF[USART_RX_CNT_CURRENT++] = Res;
				 
   }
} 

/**
  * @brief  �õ�����1�Ľ���״̬
  * @param  None
  * @retval None
  */
USART_STATUS USART1_RXNE(void)
{
	if(USART_RX_CNT_CURRENT==0)
	{
		return NONE;
	}else if(USART_RX_CNT_CURRENT!=USART_RX_CNT_LAST)
	{
		USART_RX_CNT_LAST = USART_RX_CNT_CURRENT;
		return BUSY;
	}else {
		USART_RX_LENTH = USART_RX_CNT_CURRENT;
		USART_RX_CNT_LAST = 0;
		USART_RX_CNT_CURRENT = 0;
		return OK;
	}
}

#endif	

