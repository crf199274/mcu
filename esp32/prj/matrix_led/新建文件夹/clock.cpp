#include "clock.h"
#include "font.h"
#include "wallpaper.h"
 
const unsigned char colon[4]={0x30,0x0c,0x30,0x0c}; /* 逗号编码 */
matrix_led_clock clock_info={((PANEL_RES_X-52)/2),(PANEL_RES_Y-16)-2,{0,0,255},{0,0,0}}; /* 时钟显示数据结构 w=8*6+4=52,h=16*/
uint8_t matrix_led_time[6]={'0','0','0','0','0','0'}; /* 时间存储 */
CMO cmo={0}; 

/*
 * @brief     时间计数
 * 
 * @param[tim]     时间计数结果存储地址
 */
static void matrix_led_count(uint8_t *tim)
{
  static uint8_t s=0,m=0,h=0;

  s++;
  if(s==60)
  {
    m++;
    s=0;
    if(m==60)
    {
      h++;
      m=0;
      if(h==24)
      {
        h=0;
      }
    }
  }
  
  cmo.clock6=1;
  tim[5]=s%10+48;
  if(((s/10)%10)!=(tim[4]-48))
    cmo.clock5=1;
  tim[4]=(s/10)%10+48;

  if((m%10)!=(tim[3]-48))
    cmo.clock4=1;
  tim[3]=m%10+48;
  if(((m/10)%10)!=(tim[2]-48))
    cmo.clock3=1;
  tim[2]=(m/10)%10+48;
  
  if((h%10)!=(tim[1]-48))
    cmo.clock2=1;
  tim[1]=h%10+48;
  if(((h/10)%10)!=(tim[0]-48))
    cmo.clock1=1;
  tim[0]=(h/10)%10+48;
}

/*
 * @brief     绘制冒号
 * 
 * @param[x]     横坐标起始位置
 * @param[y]     纵坐标起始位置
 * 
 * @param[fc]     字体颜色
 * @param[bc]     背景颜色
 */
static void matrix_led_draw_colon(uint16_t x,uint16_t y,CRGB fc,CRGB bc)
{
  uint16_t i,j,x_conv,y_conv,ch;
  
  if((x<0||x>64)||(y<0||y>64)) /* 检验是否超出范围 */
    return;
  
  for(i=0;i<2;i++)
  {
    y_conv = y;
    x_conv = x+i;
    if(x_conv!=0)
      x_conv = PANEL_RES_X - x_conv;
    ch = colon[i*2];
    for(j=0;j<8;j++)
    {
      if(ch&(0x01<<j))
        dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
      else
        dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
      y_conv++;
    }
    ch = colon[i*2+1];
    for(j=0;j<8;j++)
    {
      if(ch&(0x01<<j))
        dma_display->drawPixelRGB888(x_conv,y_conv,fc.red,fc.green,fc.blue);
      else
        dma_display->drawPixelRGB888(x_conv,y_conv,bc.red,bc.green,bc.blue);
      y_conv++;
    }
  }
}

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
static void matrix_led_draw_char(uint16_t x,uint16_t y,uint8_t ch_ascll,CRGB fc,CRGB bc)
{
  int16_t i,j;
  uint16_t x_conv,y_conv;
  uint8_t ch;
#if CLOCK_TYPE==CLOCK_ROLL || CLOCK_TYPE==CLOCK_DEFAULT /* 滚动效果 / 默认效果 */
  ch_ascll = ch_ascll-' ';
#else
  ch_ascll = ch_ascll-48;
#endif

  if((x<0||x>64)||(y<0||y>64))
    return;

  y_conv = y;
  
  for(i=15;i>=0;i--)
  {
#if CLOCK_TYPE==CLOCK_ROLL || CLOCK_TYPE==CLOCK_DEFAULT /* 滚动效果 / 默认效果 */
    ch=ascii_1608[ch_ascll][i];
#else
    ch=seven_segment[ch_ascll][i];
#endif
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

#if CLOCK_TYPE==CLOCK_ROLL /* 滚动效果 */

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
static void matrix_led_draw_char_roll(uint16_t x,uint16_t y,uint8_t ch,uint8_t cnt,CRGB fc,CRGB bc)
{
  int16_t i,j,x_conv,y_conv;
  uint8_t ascll=ch-' ',ch_ascll;
  
  y_conv = y;
  
  for(i=0;i<16;i++)
  {
    switch(ch)
    {
      case '0':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i]; //当前时刻
        else
          ch_ascll=ascii_1608[ascll+9][15-i]; //前一时刻
        break;
      case '1':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '2':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '3':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '4':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '5':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '6':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '7':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '8':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
        break;
      case '9':
        if(i<=cnt)
          ch_ascll=ascii_1608[ascll][cnt-i];
        else
          ch_ascll=ascii_1608[ascll-1][15-i];
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
static void matrix_led_draw_time_roll(uint16_t x,uint16_t y,uint8_t *ch,CRGB fc,CRGB bc)
{
  uint16_t i;
  
  for(i=0;i<16;i++)
  { 
    if(cmo.clock6)
    {
      matrix_led_draw_char_roll(x+44,y,ch[5],i,fc,bc);
    }
    if(cmo.clock5)
    {
      matrix_led_draw_char_roll(x+36,y,ch[4],i,fc,bc);
    }
    if(cmo.clock4)
    {
      matrix_led_draw_char_roll(x+26,y,ch[3],i,fc,bc);
    }
    if(cmo.clock3)
    {
      matrix_led_draw_char_roll(x+18,y,ch[2],i,fc,bc);
    }
    if(cmo.clock2)
    {
      matrix_led_draw_char_roll(x+8,y,ch[1],i,fc,bc);
    }
    if(cmo.clock1)
    {
      matrix_led_draw_char_roll(x,y,ch[0],i,fc,bc);
    }
    delay(25);
  }
}

#elif CLOCK_TYPE==CLOCK_DEFAULT /* 默认效果 */

static void matrix_led_draw_time(uint16_t x,uint16_t y,uint8_t *ch,CRGB fc,CRGB bc)
{
  if(cmo.clock6)
  {
    matrix_led_draw_char(x+44,y,ch[5],fc,bc);
  }
  if(cmo.clock5)
  {
    matrix_led_draw_char(x+36,y,ch[4],fc,bc);
  }
  if(cmo.clock4)
  {
    matrix_led_draw_char(x+26,y,ch[3],fc,bc);
  }
  if(cmo.clock3)
  {
    matrix_led_draw_char(x+18,y,ch[2],fc,bc);
  }
  if(cmo.clock2)
  {
    matrix_led_draw_char(x+8,y,ch[1],fc,bc);
  }
  if(cmo.clock1)
  {
    matrix_led_draw_char(x,y,ch[0],fc,bc);
  }
}

#else /* 数码管变换效果 */

static void matrix_led_draw_char_seven_segment(uint16_t x,uint16_t y,uint8_t ch,uint8_t scan_mode,uint8_t cnt,CRGB fc,CRGB bc)
{
  int16_t i,j,x_conv,y_conv;
  uint8_t ch_ascll;
  
  y_conv = y;
  
  switch(scan_mode)
  {
    case 0:
      for(i=15;i>=0;i--)
      {
        ch_ascll = seven_segment[ch-48][i];
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
        ch_ascll = seven_segment[ch-48][i];
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

static void matrix_led_draw_time_seven_segment(uint16_t x,uint16_t y,uint8_t *ch,CRGB fc,CRGB bc)
{
  uint16_t i;
  uint8_t scan_mode;

  for(i=0;i<8;i++)
  { 
    if(cmo.clock6)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x+44,y,ch[5],scan_mode,i,fc,bc);
    }
    if(cmo.clock5)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x+36,y,ch[4],scan_mode,i,fc,bc);
    }
    if(cmo.clock4)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x+26,y,ch[3],scan_mode,i,fc,bc);
    }
    if(cmo.clock3)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x+18,y,ch[2],scan_mode,i,fc,bc);
    }
    if(cmo.clock2)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x+8,y,ch[1],scan_mode,i,fc,bc);
    }
    if(cmo.clock1)
    {
      scan_mode = matrix_led_draw_time_seven_segment_get_sacn_mode(ch[5]);
      matrix_led_draw_char_seven_segment(x,y,ch[0],scan_mode,i,fc,bc);
    }
    delay(25);
  }
}

#endif

/*
 * @brief     时间计数的线程
 * 
 * @param[parameter]     任务创建时传入的参数地址
 */
void matrix_led_count_task(void * parameter)
{
  uint16_t cnt=0;
//  UBaseType_t uxHighWaterMark;
  
  while(1)
  {
    delay(100);
    cnt++;
    if(cnt==10)
    {
      cnt = 0;
      matrix_led_count(matrix_led_time);
    }
//    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); 
//    Serial.printf("matrix_led_count_task-Stack:%d\r\n",uxHighWaterMark);
  }
  vTaskDelete( NULL );
}

/*
 * @brief     时间字符串绘制的线程
 * 
 * @param[parameter]     任务创建时传入的参数地址
 */
void matrix_led_clock_task(void * parameter)
{
  matrix_led_clock *clock_info=(matrix_led_clock*)parameter;
//  UBaseType_t uxHighWaterMark;
  
  while(1)
  {
    delay(100);
    if(cmo.clock1||cmo.clock2||cmo.clock3||cmo.clock4||cmo.clock5||cmo.clock6)
    {
#if CLOCK_TYPE==CLOCK_ROLL /* 滚动效果 */
      matrix_led_draw_time_roll(clock_info->x,clock_info->y,matrix_led_time,clock_info->fc,clock_info->bc);
#elif CLOCK_TYPE==CLOCK_DEFAULT /* 默认效果 */
      matrix_led_draw_time(clock_info->x,clock_info->y,matrix_led_time,clock_info->fc,clock_info->bc);
#else /* 数码管变换效果 */
      matrix_led_draw_time_seven_segment(clock_info->x,clock_info->y,matrix_led_time,clock_info->fc,clock_info->bc);
#endif
      cmo.clock1 = 0;
      cmo.clock2 = 0;
      cmo.clock3 = 0;
      cmo.clock4 = 0;
      cmo.clock5 = 0;
      cmo.clock6 = 0;
    }
//    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); 
//    Serial.printf("matrix_led_clock_task-Stack:%d\r\n",uxHighWaterMark);
  }
  vTaskDelete( NULL );
}

/*
 * @brief     时间显示初始化
 */
void matrix_led_clock_init(void)
{
  uint8_t t=3;
  T_DS1339TIME time={0};

  matrix_led_draw_wallpaper(TV);
  
  ds1339_init();
  
  /* 读取ds1330 */
  while(ds1339_read_time(&time))
  {
    t--;
    if(t==0)
      break;
  }

  matrix_led_time[0]=(time.hour/10)%10+48;
  matrix_led_time[1]=time.hour%10+48;
  matrix_led_time[2]=(time.minute/10)%10+48;
  matrix_led_time[3]=time.minute%10+48;
  matrix_led_time[4]=(time.second/10)%10+48;
  matrix_led_time[5]=time.second%10+48;
  
  /* 初始化显示 */
  matrix_led_draw_char(clock_info.x,clock_info.y,matrix_led_time[0],clock_info.fc,clock_info.bc);
  matrix_led_draw_char((clock_info.x+8),clock_info.y,matrix_led_time[1],clock_info.fc,clock_info.bc);
  matrix_led_draw_colon((clock_info.x+16),clock_info.y,clock_info.fc,clock_info.bc);
  matrix_led_draw_char((clock_info.x+18),clock_info.y,matrix_led_time[2],clock_info.fc,clock_info.bc);
  matrix_led_draw_char((clock_info.x+26),clock_info.y,matrix_led_time[3],clock_info.fc,clock_info.bc);
  matrix_led_draw_colon((clock_info.x+34),clock_info.y,clock_info.fc,clock_info.bc);
  matrix_led_draw_char((clock_info.x+36),clock_info.y,matrix_led_time[4],clock_info.fc,clock_info.bc);
  matrix_led_draw_char((clock_info.x+44),clock_info.y,matrix_led_time[5],clock_info.fc,clock_info.bc);

  /* 创建线程 */
  // 字符1
  xTaskCreatePinnedToCore(
              matrix_led_clock_task,      /*任务函数*/
              "matrix_led_clock_task",    /*带任务名称的字符串*/
              2000,                       /*堆栈大小，单位为字节*/
              &clock_info,                /*作为任务输入传递的参数*/
              1,                          /*任务的优先级*/
              NULL,                       /*任务句柄*/
              1);                         /*核心*/
}
