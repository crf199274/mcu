#include "time_effect_default.h"
#include "font_default.h"

/*
 * @brief     绘制一个字符
 * 
 * @param[x]     横坐标起始位置
 * @param[y]     纵坐标起始位置
 * 
 * @param[ch_ascll]     需要绘制字符的ascll
 * 
 * @param[fc]     字体颜色
 * @param[bc]     背景颜色
 */
void matrix_led_draw_char_default(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc)
{
  int16_t i,j;
  uint16_t x_conv,y_conv;
  uint8_t ch;

  if((x<0||x>64)||(y<0||y>64))
    return;

  y_conv = y;
  
  for(i=15;i>=0;i--)
  {
    ch=default_1608[num][i];
    for(j=0;j<8;j++)
    {
      x_conv = x + j;
      if(x_conv!=0)
        x_conv = PANEL_RES_X - x_conv;
      if(ch&(0x01<<j))
        dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
      else
        dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
    }
    y_conv++;
  }
}

void matrix_led_draw_time_default(uint16_t x,uint16_t y,CRGB fc,CRGB bc)
{
  if(cmo.clock6)
  {
    matrix_led_draw_char_default(x+44,y,(ds1339_time.second%10),fc,bc);
  }
  if(cmo.clock5)
  {
    matrix_led_draw_char_default(x+36,y,(ds1339_time.second/10),fc,bc);
  }
  if(cmo.clock4)
  {
    matrix_led_draw_char_default(x+26,y,(ds1339_time.minute%10),fc,bc);
  }
  if(cmo.clock3)
  {
    matrix_led_draw_char_default(x+18,y,(ds1339_time.minute/10),fc,bc);
  }
  if(cmo.clock2)
  {
    matrix_led_draw_char_default(x+8,y,(ds1339_time.hour%10),fc,bc);
  }
  if(cmo.clock1)
  {
    matrix_led_draw_char_default(x,y,(ds1339_time.hour/10),fc,bc);
  }
}
