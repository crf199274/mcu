#include<reg51.h>

sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;
sbit LED5 = P0^4;
sbit LED6 = P0^5;
sbit LED7 = P0^6;
sbit LED8 = P0^7;

int num =0;
int num2 =0;
int key =0;
int P1_status = 0xFF;

void T0_Timer();
void LED_Run2();
void key_Scan();
void delay_10us(unsigned int i);
void delay_ms(unsigned int i);
void delay_s(unsigned int i);
void LED_Run();

void main()
{
   P0 = 0x00;
   P1 = 0xFF;
   T0_Timer();
   while(1)
   {
      //LED_Run();
	  LED_Run2();
	  key_Scan();
   }
}

void key_Scan()
{
  if(P1!=P1_status)
  {
  	TR0 = 0;
    P1_status = P1;			
	TR0 = 1;		
  }
}

int getkey(unsigned int num)
{
   return (int)((P1_status>>num)&0x01);
}

void T0_Timer()
{
 TMOD |= 0x01;
 TH0 = 0xFC;
 TL0 =0x66;
 TR0 =1;
 ET0 =1;
 EA = 1;
}

void T0_interrupt() interrupt 1
{
 static int i=0;
 TH0 = 0xFC;
 TL0 = 0x66;
 i++;
 if(i==1000)
 {
  i=0;
  if(num==7)
  num=0;
  else
  num++;
 }

}

void LED_Run2()
{
 switch(num)
 {
  case 0:
	  LED8 = 0;
	  if(getkey(num))
	  	break;
	  LED1 = 1;break;
  case 1:
	  LED1 = 0;
	  if(getkey(num))
	 	break;
	  LED2 = 1;break;
  case 2:
	  LED2 = 0;
	  if(getkey(num))
	 	break;
	  LED3 = 1;break;
  case 3:
	  LED3 = 0;
	  if(getkey(num))
	 	break;
	  LED4 = 1;break;
  case 4:
	  LED4 = 0;
	  if(getkey(num))
	 	break;
	  LED5 = 1;break;
  case 5:
	  LED5 = 0;
	  if(getkey(num))
	 	break;
	  LED6 = 1;break;
  case 6:
	  LED6 = 0;
	  if(getkey(num))
	 	break;
	  LED7 = 1;break;
  case 7:
	  LED7 = 0;
	  if(getkey(num))
	 	break;
	  LED8 = 1;break;
 }
}

void LED_Run()
{
   LED1 = 1;
   delay_s(1);
   LED1 = 0;
   
   LED2= 1;
   delay_s(1);
   LED2 = 0;
   
   LED3 = 1;
   delay_s(1);
   LED3 = 0;
   
   LED4 = 1;
   delay_s(1);
   LED4 = 0;
   
   LED5 = 1;
   delay_s(1);
   LED5 = 0;
   
   LED6 = 1;
   delay_s(1);
   LED6 = 0;
   
   LED7 = 1;
   delay_s(1);
   LED7 = 0;
   
   LED8 = 1;
   delay_s(1);
   LED8 = 0;
   
}

void delay_10us(unsigned int i)
{
   while(i--);
}

void delay_ms(unsigned int i)
{
   while(i--)
      delay_10us(100);
}

void delay_s(unsigned int i)
{
   while(i--)
      delay_ms(1000);
}