#include "spi.h"
#include "stdelay.h"

//以下是SD_SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SD_SPI口初始化
//这里针是对SPI2的初始化

#ifdef SPI_HARDWARE_ENABLE
	#if SPI_HARDWARE_ENABLE == 1
	
		void SPIX_Init(void)
		{
			GPIO_InitTypeDef GPIO_InitStructure;
			SPI_InitTypeDef  SPI_InitStructure;

			RCC_APB2PeriphClockCmd(	SPI_IO_CLOCK, ENABLE );//PORTB时钟使能 
			RCC_APB1PeriphClockCmd(	SPI_CLOCK,  ENABLE );//SPI2时钟使能 	
		 
			GPIO_InitStructure.GPIO_Pin = SPI_CS;  // PB12 推挽 
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);
			GPIO_SetBits(SPI_CS_PORT,SPI_CS);
			
			GPIO_InitStructure.GPIO_Pin = SPI_CSK;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_CSK_PORT, &GPIO_InitStructure);//初始化GPIOB
			GPIO_SetBits(SPI_CSK_PORT,SPI_CSK);  
			
			GPIO_InitStructure.GPIO_Pin = SPI_MISO;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);//初始化GPIOB
			GPIO_SetBits(SPI_MISO_PORT,SPI_MISO);
			
			GPIO_InitStructure.GPIO_Pin = SPI_MOSI;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);//初始化GPIOB
			GPIO_SetBits(SPI_MOSI_PORT,SPI_MOSI);

			SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SD_SPI单向或者双向的数据模式:SD_SPI设置为双线双向全双工
			SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SD_SPI工作模式:设置为主SD_SPI
			SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SD_SPI的数据大小:SD_SPI发送接收8位帧结构
			SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
			SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
			SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
			SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
			SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
			SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
			
			SPI_Init(SD_SPI, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
		 
			SPI_Cmd(SD_SPI, ENABLE); //使能SD_SPI外设	 
			
			SPI2_ReadWriteByte(0xff);//启动传输
		}   
	
	#endif
#endif		
								
//SD_SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI_SetSpeed(uint8_t SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SD_SPI->CR1&=0XFFC7;
	SD_SPI->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SD_SPI,ENABLE); 

}		

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI2_ReadWriteByte(uint8_t TxData)
{		 			 
 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//等待发送区空  
	
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个byte  数据
		
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET); //等待接收完一个byte  
 
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据	
 		    
}
