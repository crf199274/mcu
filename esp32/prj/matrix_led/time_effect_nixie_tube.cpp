#include "time_effect_nixie_tube.h"
#include "font_nixie_tube.h"

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
void matrix_led_draw_char_nixie_tube(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc)
{
  int16_t i,j;
  uint16_t x_conv,y_conv;
  uint8_t ch;

  if((x<0||x>64)||(y<0||y>64))
    return;

  y_conv = y;
  
  for(i=15;i>=0;i--)
  {
    ch=seven_segment[num][i];
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

static void matrix_led_draw_char_seven_segment(uint16_t x,uint16_t y,uint8_t num,uint8_t scan_mode,uint8_t cnt,CRGB fc,CRGB bc)
{
  int16_t i,j,x_conv,y_conv;
  uint8_t ch_ascll;
  
  y_conv = y;
  
  switch(scan_mode)
  {
    case 0:
      for(i=15;i>=0;i--)
      {
        ch_ascll = seven_segment[num][i];
        x_conv = x + cnt;
        if(x_conv!=0)
          x_conv = PANEL_RES_X - x_conv;
        if(ch_ascll&(0x01<<cnt))
          dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
        else
          dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
        y_conv++;
      }
      break;
    case 1:
      for(i=15;i>=0;i--)
      {
        ch_ascll = seven_segment[num][i];
        x_conv = x + 7 - cnt;
        if(x_conv!=0)
          x_conv = PANEL_RES_X - x_conv;
        if(ch_ascll&(0x80>>cnt))
          dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
        else
          dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
        y_conv++;
      }
      break;
    default:break;
  }
}

static uint8_t matrix_led_draw_time_seven_segment_get_sacn_mode(uint8_t ch)
{
  switch(ch)
  {
    case '0':
      return 1;
      break;
    case '1':
      return 1;
      break;
    case '2':
      return 0;
      break;
    case '3':
      return 0;
      break;
    case '4':
      return 1;
      break;
    case '5':
      return 1;
      break;
    case '6':
      return 1;
      break;
    case '7':
      return 0;
      break;
    case '8':
      return 1;
      break;
    case '9':
      return 1;
      break;
    default : 
      return 0;
      break;
  }
}

void matrix_led_draw_time_seven_segment(uint16_t x,uint16_t y,CRGB fc,CRGB bc)
{
  uint16_t i;
  uint8_t scan_mode;

  for(i=0;i<8;i++)
  { 
    if(cmo.clock6)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.second%10));
      matrix_led_draw_char_seven_segment(x+44,y,(ds1339_time.second%10),scan_mode,i,fc,bc);
    }
    if(cmo.clock5)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.second/10));
      matrix_led_draw_char_seven_segment(x+36,y,(ds1339_time.second/10),scan_mode,i,fc,bc);
    }
    if(cmo.clock4)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.minute%10));
      matrix_led_draw_char_seven_segment(x+26,y,(ds1339_time.minute%10),scan_mode,i,fc,bc);
    }
    if(cmo.clock3)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.minute/10));
      matrix_led_draw_char_seven_segment(x+18,y,(ds1339_time.minute/10),scan_mode,i,fc,bc);
    }
    if(cmo.clock2)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.hour%10));
      matrix_led_draw_char_seven_segment(x+8,y,(ds1339_time.hour%10),scan_mode,i,fc,bc);
    }
    if(cmo.clock1)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode((ds1339_time.hour/10));
      matrix_led_draw_char_seven_segment(x,y,(ds1339_time.hour/10),scan_mode,i,fc,bc);
    }
    delay(25);
  }
}
