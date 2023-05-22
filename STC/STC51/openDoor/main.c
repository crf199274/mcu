#include "main.h"
#include "string.h"

void init(void);
void Delay10ms();		//@11.0592MHz
void FstartMotor(void);
void RstartMotor(void);
void stopMotor(void);
unsigned char usart_WaitRecive(void);
void cleanUsartBuf(void);

void main()
{
	init();
	
	FstartMotor();
	
}

unsigned char usart_WaitRecive()
{
		if(usart_cnt == 0)
			return 0;
		
		if(usart_cnt == usart_cntPre)
		{
			usart_cnt = 0;
			return 1;	
		}
		
		usart_cntPre = usart_cnt;
		
		return 0;
}

void cleanUsartBuf()
{
	memset(usart_buf,0,sizeof(usart_buf));
	usart_cnt = 0;
}

void Usart() interrupt 4
{
	if(RI==1)
	{
		if(usart_cnt>=sizeof(usart_buf))
			usart_cnt = 0;
		usart_buf[usart_cnt++] = SBUF;
		RI = 0;
	}
}

void USARTinit(void)
{
	TMOD=0x20;
	TH1=0xFA;
	TL1=0xFA;
	TR1=1;
	PCON|=0x80;
	SCON|=0x50;
	ES=1;
	EA=1;

}

void init(void)
{
	IT0 = 1;                        //set INT0 int type (1:Falling 0:Low level)
  EX0 = 1;                        //enable INT0 interrupt
  EA = 1;                         //open global interrupt switch
	
	FEN_MOTOR1 = 1;
	FEN_MOTOR2 = 0;
	REN_MOTOR1 = 1;
	REN_MOTOR2 = 0;
	EN_MOTOR = 0;
	TDP1 = 1;
	TDP2 = 1;
	USARTinit();
}

void FstartMotor(void)
{
	REN_MOTOR1 = 1;
	REN_MOTOR2 = 0;
	Delay10ms();
	FEN_MOTOR1 = 0;
	FEN_MOTOR2 = 1;
}

void RstartMotor(void)
{
	FEN_MOTOR1 = 1;
	FEN_MOTOR2 = 0;
	Delay10ms();
	REN_MOTOR1 = 0;
	REN_MOTOR2 = 1;
}

void stopMotor(void)
{
	FEN_MOTOR1 = 1;
	FEN_MOTOR2 = 0;
	Delay10ms();
	REN_MOTOR1 = 1;
	REN_MOTOR2 = 0;
}

//External interrupt0 service routine
void exint0() interrupt 0           //(location at 0003H)
{
    P0++;
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 18;
	j = 235;
	do
	{
		while (--j);
	} while (--i);
}
