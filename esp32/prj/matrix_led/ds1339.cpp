#include "ds1339.h"

static uint8_t ConvertDataToSet(const T_DS1339TIME *time, uint8_t regType);
static void ConvertDataToGet(uint8_t rData, T_DS1339TIME *time, uint8_t regType);

/*
 * @brief     初始化ds1339
 */
void ds1339_init(void)
{
  Wire.setPins(2,0);
  Wire.begin();
}

/*
 * @brief     ds1339写寄存器
 * 
 * @param[address]     寄存器地址
 * 
 * @param[txdata]      需要写入的数据
 */
static uint8_t ds1339_write_byte(uint8_t address,uint8_t txdata)
{
  Wire.beginTransmission(0x68); /* 启动传输 */

  Wire.write(address);
  
  Wire.write(txdata);

  return Wire.endTransmission(); /* 结束传输 */
}

/*
 * @brief     ds1339读寄存器
 * 
 * @param[address]     寄存器地址
 * 
 * @param[rxdata]      读取数据的存储地址
 */
static uint8_t ds1339_read_byte(uint8_t address,uint8_t *rxdata)
{
  uint8_t ret=0;
  uint8_t t=200;
  
  Wire.beginTransmission(0x68); /* 启动传输 */

  Wire.write(address);

  ret = Wire.endTransmission(false); /* 结束传输 */
  
  Wire.requestFrom(0x68,1); /* 请求数据 */

  while(!Wire.available())
  {
    t--;
    delay(1);
    if(t==0)
    {
      return 1;
    }
  }
  
  *rxdata = Wire.read();

  return ret;
}

/*
 * @brief     把时间写到ds1339
 * 
 * @param[time]     存储时间信息的地址
 */
uint8_t ds1339_write_time(const T_DS1339TIME *time)
{
  uint8_t ret=0,i;
  for(i=0;i<7;i++)
  {
    if(ds1339_write_byte(REGADDR_SECONDS+i,ConvertDataToSet(time,REGADDR_SECONDS+i)))
    {
      ret = 1;
      return ret;
    }
  }

  return ret;
}

/*
 * @brief     从ds1339读取时间
 * 
 * @param[time]     存储时间信息的地址
 */
uint8_t ds1339_read_time(T_DS1339TIME *time)
{
  uint8_t ret=0,i,rx_data=0;
  for(i=0;i<7;i++)
  {
    if(ds1339_read_byte(REGADDR_SECONDS+i,&rx_data))
    {
      ret = 1;
      return ret;
    }else{
      ConvertDataToGet(rx_data,time,REGADDR_SECONDS+i);
    }
  }

  return ret;
}

/*
 * @brief     格式转换 用户->寄存器
 * 
 * @param[time]     存储时间信息的地址
 * 
 * @param[regType]  寄存器宏
 */
static uint8_t ConvertDataToSet(const T_DS1339TIME *time, uint8_t regType)
{
    uint8_t sendData = 0;

    switch(regType)
    {
        case REGADDR_SECONDS:
            if(time->second > 10)
            {
                sendData = ((time->second / 10) << 4) & 0x7F;
                sendData |= time->second % 10;
            }
            else
            {
                sendData = time->second;
            }
            break;

        case REGADDR_MINUTES:
            if(time->minute > 10)
            {
                sendData = ((time->minute / 10) << 4) & 0x7F;
                sendData |= time->minute % 10;
            }
            else
            {
                sendData = time->minute;
            }
            break;

        case REGADDR_HOURS:
            if(time->hour > 10)
            {
//                sendData = ((time->hour / 10) << 4) & 0x1F;
                sendData = ((time->hour / 10) << 4) & 0x30;
                sendData |= time->hour % 10;
            }
            else
            {
                sendData = time->hour;
            }
            break;

        case REGADDR_DAY:
            sendData = time->weekday & 0x0E;
            break;

        case REGADDR_DATA:
            if(time->day > 10)
            {
                sendData = ((time->day / 10) << 4) & 0x3F;
                sendData |= time->day % 10;
            }
            else
            {
                sendData = time->day;
            }
            break;

        case REGADDR_MONTH_CENTURY:
            if(time->month > 10)
            {
                sendData = ((time->month / 10) << 4) & 0x1F;
                sendData |= time->month % 10;
            }
            else
            {
                sendData = time->month;
            }
            break;
        case REGADDR_YEAR:
            if(time->year > 10)
            {
                sendData = ((time->year / 10) << 4) & 0xF0;
                sendData |= time->year % 10;
            }
            else
            {
                sendData = time->year;
            }
            break;
        default:
            ;
    }
    return sendData;
}

/*
 * @brief     格式转换 寄存器->用户
 * 
 * @param[rData]    寄存器读到的数据
 * 
 * @param[time]     存储时间信息的地址
 * 
 * @param[regType]  寄存器宏
 */
static void ConvertDataToGet(uint8_t rData, T_DS1339TIME *time, uint8_t regType)
{
    switch(regType)
    {
        case REGADDR_SECONDS:
            time->second = (rData >> 4 & 0x07) * 10 + (rData & 0x0F);
            break;

        case REGADDR_MINUTES:
            time->minute = (rData >> 4 & 0x07) * 10 + (rData & 0x0F);
            break;

        case REGADDR_HOURS:
            time->hour = (rData >> 4 & 0x03) * 10 + (rData & 0x0F);
            break;

        case REGADDR_DAY:
            time->weekday = rData & 0x07;
            break;

        case REGADDR_DATA:
            time->day = (rData >> 4 & 0x03) * 10 + (rData & 0x0F);
            break;

        case REGADDR_MONTH_CENTURY:
            time->month = (rData >> 4 & 0x01) * 10 + (rData & 0x0F);
            break;

        case REGADDR_YEAR:
            time->year = (rData >> 4 & 0x0F) * 10 + (rData & 0x0F);
            break;

        default:
            ;
    }
}

void ds1339_set_time_by_serial(void)
{
  T_DS1339TIME time;
  
  if(Serial.available()==2)
  {
    time.year = 23;
    time.month = 9;
    time.day = 10;
    time.weekday = 7;
    time.hour = 9;
    time.minute = Serial.read();
    time.second = Serial.read();
    ds1339_write_time(&time);
    Serial.println("OK\r\n");
  }
}
