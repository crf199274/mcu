#include "sccb.h"

/* 起始信号 */
void SCCB_Start(void)
{
  SCCB_SDA_OUT();   //数据线为输出模式
  SCCB_SDA_SET();     //数据线高电平
  delayMicroseconds(50);
  
  SCCB_SCL_SET();     //在时钟高的时候数据线由高至低
  delayMicroseconds(50);
  
  SCCB_SDA_CLR();
  delayMicroseconds(50);
  
  SCCB_SCL_CLR();     //数据线恢复低电平，单操作函数必要
  delayMicroseconds(50);
}

/* 终止信号 */
void SCCB_Stop(void)
{
  SCCB_SDA_OUT();   //数据线为输出模式
  SCCB_SDA_CLR();     //数据线低电平
  delayMicroseconds(50);
  
  SCCB_SCL_SET();
  delayMicroseconds(50);
  
  SCCB_SDA_SET();
  delayMicroseconds(50);
}

/* NA信号 */
/* SCCB_D先拉高，再把SCCB_C拉高，后把SCCB_C拉低，最后把SCCB_D拉低 */
void SCCB_NA(void)
{
  SCCB_SDA_OUT();
  SCCB_SDA_SET();
  delayMicroseconds(50);
  
  SCCB_SCL_SET();
  delayMicroseconds(50);
  
  SCCB_SCL_CLR();
  delayMicroseconds(50);
  
  SCCB_SDA_CLR();
  delayMicroseconds(50);
}

/* 读一个字节 */
u8 SCCB_RD_Byte(void)
{
  u8 read,j;
  read=0x00;
  
  SCCB_SDA_IN();      //设置SDA为输入
  delayMicroseconds(50);
  for(j=8;j>0;j--)
  {
    delayMicroseconds(50);
    SCCB_SCL_SET();
    
    read=read<<1;     
    if(SCCB_SDA_READ()) read++;   //左移一位，若读取到高电平 read相应位写1
    
    delayMicroseconds(50);
    SCCB_SCL_CLR();
  }
  
  SCCB_SDA_OUT();
  return (read);
}

/* 写一个字节 */
u8 SCCB_WR_Byte(u8 dat)
{
  u8 temp=0,j;
  
  for(j=0;j<8;j++)
  {
    if((dat<<j)&0x80) //高位先发送
    {
      SCCB_SDA_SET();
    }
    else  
    {
      SCCB_SDA_CLR();
    }
    delayMicroseconds(50);
    SCCB_SCL_SET();     //高电平时写入
    delayMicroseconds(50);
    SCCB_SCL_CLR();     //低电平时 改变SDA
  }
  
  SCCB_SDA_IN();      //设置SDA为输入
  delayMicroseconds(50);
  SCCB_SCL_SET();       //高电平时读取第九位，以判断是否发送成功
  delayMicroseconds(50);
  if(SCCB_READ_SDA)   //SDA=1 发送失败
  {
    temp=1;
  }
  else
  {
    temp=0;           
  }
  SCCB_SCL_CLR();       //写完字节后 SCL拉低 方便继续操作
  SCCB_SDA_OUT();     //设置SDA为输出
  return (temp);
}
