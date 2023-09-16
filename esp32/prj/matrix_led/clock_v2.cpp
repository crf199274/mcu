#include "clock_v2.h"
#include "wallpaper.h"

static uint8_t time_effect=0;

const unsigned char colon[4]={0x30,0x0c,0x30,0x0c}; /* 逗号编码 */

hw_timer_t* timer= NULL;  /* 实例化一个定时器 */

T_DS1339TIME ds1339_time={0}; /* 时间结构体 */
matrix_led_clock clock_info={((PANEL_RES_X-52)/2),(PANEL_RES_Y-16)-2,{0,0,255},{0,0,0}}; /* 时钟显示数据结构 w=8*6+4=52,h=16*/
CMO cmo={0};

void IRAM_ATTR clock_cnt(void)
{
	ds1339_time.second++;
  cmo.clock6=1;
  if(ds1339_time.second%10==0)
    cmo.clock5=1;
	if(ds1339_time.second==60)
	{
		ds1339_time.minute++;
    cmo.clock4=1;
    if(ds1339_time.minute%10==0)
      cmo.clock3=1;
		ds1339_time.second=0;
		if(ds1339_time.minute==60)
		{
			ds1339_time.hour++;
      cmo.clock2=1;
      if(ds1339_time.hour%10==0)
        cmo.clock1=1;
			ds1339_time.minute=0;
			if(ds1339_time.hour==24)
			{
        cmo.clock1=1;
				ds1339_time.hour=0;
			}
		}
	}
}

void timer_init(void)
{
	timer = timerBegin(0, 80, true);  //设置定时器1，80分频，定时器是否上下计数
	timerAttachInterrupt(timer, clock_cnt, true);  //定时器地址指针，中断函数名称，中断边沿触发类型
	timerAlarmWrite(timer, 1000000ul , true);  //操作那个定时器，定时时长单位us，是否自动重装载
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
static void matrix_led_draw_char(uint16_t x,uint16_t y,uint8_t num,CRGB fc,CRGB bc)
{
  switch(time_effect)
  {
    case 0:
      matrix_led_draw_char_default(x,y,num,fc,bc);
      break;
    case 1:
      matrix_led_draw_char_roll_(x,y,num,fc,bc);
      break;
    case 2:
      matrix_led_draw_char_nixie_tube(x,y,num,fc,bc);
      break;
  }
}

/*
 * @brief     时间字符串绘制的线程
 * 
 * @param[parameter]     任务创建时传入的参数地址
 */
void matrix_led_clock_task(void * parameter)
{
	
//  UBaseType_t uxHighWaterMark;
  
  while(1)
  {
    delay(100);
    if(cmo.clock1||cmo.clock2||cmo.clock3||cmo.clock4||cmo.clock5||cmo.clock6)
    {
      switch(time_effect)
      {
        case 0:
          matrix_led_draw_time_default(clock_info.x,clock_info.y,clock_info.fc,clock_info.bc);
          break;
        case 1:
          matrix_led_draw_time_roll(clock_info.x,clock_info.y,clock_info.fc,clock_info.bc);
          break;
        case 2:
          matrix_led_draw_time_seven_segment(clock_info.x,clock_info.y,clock_info.fc,clock_info.bc);
          break;
      }
      cmo.clock1 = 0;
      cmo.clock2 = 0;
      cmo.clock3 = 0;
      cmo.clock4 = 0;
      cmo.clock5 = 0;
      cmo.clock6 = 0;
    }
//    Serial.print("APP_CPU正在运行：loop 核心：");
//    Serial.println(xPortGetCoreID());
//    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); 
//    Serial.printf("matrix_led_clock_task-Stack:%d\r\n",uxHighWaterMark);
  }
  vTaskDelete( NULL );
}

void matrix_led_clock_init(void)
{
  uint8_t t=3;

#if ENABLE_RANDOM_EFFECT
  time_effect = getARandom(0,3); /* 获取随机效果 */
#else
  time_effect = 0;
#endif

	timer_init(); /* 定时器初始化 */
	
	ds1339_init(); /* 初始化ds1399 */
	
	matrix_led_draw_wallpaper(CAISHEN); /* 显示背景图 */
  
	/* 读取ds1330 */
	while(ds1339_read_time(&ds1339_time))
	{
		t--;
		if(t==0)
			break;
	}

  timerAlarmEnable(timer);  /* 打开那个定时器 */
  
	/* 初始化显示 */
	matrix_led_draw_char(clock_info.x,clock_info.y,(ds1339_time.hour/10),clock_info.fc,clock_info.bc);
	matrix_led_draw_char((clock_info.x+8),clock_info.y,(ds1339_time.hour%10),clock_info.fc,clock_info.bc);
	matrix_led_draw_colon((clock_info.x+16),clock_info.y,clock_info.fc,clock_info.bc);
	matrix_led_draw_char((clock_info.x+18),clock_info.y,(ds1339_time.minute/10),clock_info.fc,clock_info.bc);
	matrix_led_draw_char((clock_info.x+26),clock_info.y,(ds1339_time.minute%10),clock_info.fc,clock_info.bc);
	matrix_led_draw_colon((clock_info.x+34),clock_info.y,clock_info.fc,clock_info.bc);
	matrix_led_draw_char((clock_info.x+36),clock_info.y,(ds1339_time.second/10),clock_info.fc,clock_info.bc);
	matrix_led_draw_char((clock_info.x+44),clock_info.y,(ds1339_time.second%10),clock_info.fc,clock_info.bc);
	
	/* 创建线程 */
	xTaskCreatePinnedToCore(
              matrix_led_clock_task,      /*任务函数*/
              "matrix_led_clock_task",    /*带任务名称的字符串*/
              2000,                       /*堆栈大小，单位为字节*/
              NULL,                /*作为任务输入传递的参数*/
              1,                          /*任务的优先级*/
              NULL,                       /*任务句柄*/
              1);                         /*核心*/
	
}
