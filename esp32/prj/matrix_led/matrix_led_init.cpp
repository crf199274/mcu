#include "matrix_led_init.h"

MatrixPanel_I2S_DMA *dma_display = nullptr; /* 实例化一个显示类 */

// Module configuration
HUB75_I2S_CFG mxconfig(
  PANEL_RES_X,   // module width
  PANEL_RES_Y,   // module height
  PANEL_CHAIN    // Chain length
);

/*
 * @brief     频谱显示初始化
 */
void matrix_led_init(void)
{
  mxconfig.gpio.e = 32; /* 设置E_PIN为GPIO32 */

  dma_display = new MatrixPanel_I2S_DMA(mxconfig); /* 实例化一个显示类 */

  dma_display->begin(); /* 初始化显示 */

  dma_display->setBrightness8(128); /* 0-255 设置亮度为128 */
}
