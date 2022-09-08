#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define SPI_HARDWARE_ENABLE											1

#ifdef SPI_HARDWARE_ENABLE
	#if SPI_HARDWARE_ENABLE == 1
	
	#define SD_SPI																		SPI2
	
	#define SPI_IO_CLOCK													RCC_APB2Periph_GPIOB
	#define SPI_CLOCK															RCC_APB1Periph_SPI2
	
	#define SPI_CS_PORT														GPIOB
	#define SPI_CS																GPIO_Pin_12
	
	#define SPI_CSK_PORT													GPIOB
	#define SPI_CSK																GPIO_Pin_13
	
	#define SPI_MISO_PORT													GPIOB
	#define SPI_MISO															GPIO_Pin_14
	
	#define SPI_MOSI_PORT													GPIOB
	#define SPI_MOSI															GPIO_Pin_15
	
	#endif
#endif


void SPIX_Init(void);			 //初始化SPI口
void SPI_SetSpeed(uint8_t SpeedSet); //设置SPI速度   
uint8_t SPI2_ReadWriteByte(uint8_t TxData);
void SPI_Deinit(void);
		 
#endif
