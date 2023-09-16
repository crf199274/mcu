#include "time_effect_roll.h"
#include "font_roll.h"

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
void matrix_led_draw_char_roll_(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc)
{
  int16_t i,j;
  uint16_t x_conv,y_conv;
  uint8_t ch;

  if((x<0||x>64)||(y<0||y>64))
    return;

  y_conv = y;
  
  for(i=15;i>=0;i--)
  {
    ch=roll_1608[num][i];
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

/*
 * @brief     绘制一个字符
 * 
 * @param[x]     横坐标起始位置
 * @param[y]     纵坐标起始位置
 * 
 * @param[ch]      需要绘制字符的ascll
 * @param[cnt]     滚动序号
 * 
 * @param[fc]     字体颜色
 * @param[bc]     背景颜色
 */
static void matrix_led_draw_char_roll(uint16_t x,uint16_t y,uint8_t num,uint8_t cnt,CRGB fc,CRGB bc)
{
  int16_t i,j,x_conv,y_conv;
  uint8_t ch_ascll;
  
  y_conv = y;
  
  for(i=0;i<16;i++)
  {
    switch(num)
    {
      case 0:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i]; //当前时刻
        else
          ch_ascll=roll_1608[num+9][15-i]; //前一时刻
        break;
      case 1:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 2:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 3:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 4:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 5:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 6:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 7:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 8:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
      case 9:
        if(i<=cnt)
          ch_ascll=roll_1608[num][cnt-i];
        else
          ch_ascll=roll_1608[num-1][15-i];
        break;
    }
    for(j=0;j<8;j++)
    {
      x_conv = x + j;
      if(x_conv!=0)
        x_conv = PANEL_RES_X-x_conv;
      if(ch_ascll&(0x01<<j))
        dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
      else
        dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
    }
    y_conv++;
  }
}

/*
 * @brief     绘制一个字符
 * 
 * @param[x]     横坐标起始位置
 * @param[y]     纵坐标起始位置
 * 
 * @param[ch]      需要绘制的时间的字符串的存储地址
 * 
 * @param[fc]     字体颜色
 * @param[bc]     背景颜色
 */
void matrix_led_draw_time_roll(uint16_t x,uint16_t y,CRGB fc,CRGB bc)
{
  uint16_t i;
  
  for(i=0;i<16;i++)
  { 
    if(cmo.clock6)
    {
      matrix_led_draw_char_roll(x+44,y,(ds1339_time.second%10),i,fc,bc);
    }                          
    if(cmo.clock5)             
    {                          
      matrix_led_draw_char_roll(x+36,y,(ds1339_time.second/10),i,fc,bc);
    }                          
    if(cmo.clock4)             
    {                          
      matrix_led_draw_char_roll(x+26,y,(ds1339_time.minute%10),i,fc,bc);
    }                          
    if(cmo.clock3)             
    {                          
      matrix_led_draw_char_roll(x+18,y,(ds1339_time.minute/10),i,fc,bc);
    }                          
    if(cmo.clock2)             
    {                          
      matrix_led_draw_char_roll(x+8,y,(ds1339_time.hour%10),i,fc,bc);
    }                          
    if(cmo.clock1)             
    {                          
      matrix_led_draw_char_roll(x,y,(ds1339_time.hour/10),i,fc,bc);
    }
    delay(25);
  }
}
