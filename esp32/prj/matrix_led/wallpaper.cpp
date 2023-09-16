#include "wallpaper.h"
#include "spaceman.h"
#include "tv.h"
#include "caishen.h"

void matrix_led_startup_cartoon(void)
{
  int16_t i,j,m,n,x_conv;
  uint32_t k;

  for(m=0;m<6;m++)
  {
    for(n=0;n<12;n++)
    {
      k=0;
      for(i=63;i>=0;i--)
      {
        for(j=0;j<64;j++)
        {
          x_conv = j;
          if(x_conv!=0)
            x_conv = PANEL_RES_X - x_conv;
          dma_display->drawPixelRGB888(x_conv,i,gImage_spaceman[n][k*3+2],gImage_spaceman[n][k*3+1],gImage_spaceman[n][k*3]);
          k++;
        }
      }
      delay(20);
    }
  }
}

void matrix_led_clear_wallpaper(void)
{
  int16_t i,j,x_conv;
  uint32_t k=0;
  
  for(i=63;i>=0;i--)
  {
    for(j=0;j<64;j++)
    {
      x_conv = j;
      if(x_conv!=0)
        x_conv = PANEL_RES_X - x_conv;
      if((j>=6&&j<=57)&&(i>=46&&i<=61))
      {
        k++;
        continue;
      }else {
        dma_display->drawPixelRGB888(x_conv,i,0,0,0);
        k++;
      }
    }
  }
}

void matrix_led_draw_wallpaper(uint8_t wp)
{
  int16_t i,j,x_conv;
  uint32_t k=0;
  
  switch(wp)
  {
    case TV:
      for(i=63;i>=0;i--)
      {
        for(j=0;j<64;j++)
        {
          x_conv = j;
          if(x_conv!=0)
            x_conv = PANEL_RES_X - x_conv;
          if((j>=6&&j<=57)&&(i>=46&&i<=61))
          {
            k++;
            continue;
          }else {
            dma_display->drawPixelRGB888(x_conv,i,gImage_tv[k*3+2],gImage_tv[k*3+1],gImage_tv[k*3]);
            k++;
          }
        }
      }
      break;
    case CAISHEN:
      for(i=63;i>=0;i--)
      {
        for(j=0;j<64;j++)
        {
          x_conv = j;
          if(x_conv!=0)
            x_conv = PANEL_RES_X - x_conv;
          if((j>=6&&j<=57)&&(i>=46&&i<=61))
          {
            k++;
            continue;
          }else {
            dma_display->drawPixelRGB888(x_conv,i,gImage_caishen[k*3+2],gImage_caishen[k*3+1],gImage_caishen[k*3]);
            k++;
          }
        }
      }
      break;
    default :break;
  }
}
