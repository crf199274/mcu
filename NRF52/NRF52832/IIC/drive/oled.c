#include "oled.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"
#include "ascll.h"
#include "string.h"

//TWI��������ʵ��ID,ID�������Ŷ�Ӧ��0:TWI0  1:TWI1
#define TWI_INSTANCE_ID     0

//TWI������ɱ�־
static volatile bool m_xfer_done = false;
//����TWI��������ʵ��������Ϊm_twi
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);

//TWI�¼�������
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    //�ж�TWI�¼�����
	  switch (p_event->type)
    {
        //��������¼�
			  case NRF_DRV_TWI_EVT_DONE:
            m_xfer_done = true;//��λ������ɱ�־
            break;
				case NRF_DRV_TWI_EVT_ADDRESS_NACK:
//						printf("NRF_DRV_TWI_EVT_ADDRESS_NACK");
						break;
				case NRF_DRV_TWI_EVT_DATA_NACK:
//						printf("NRF_DRV_TWI_EVT_DATA_NACK");
						break;
        default:
            break;
    }
}

//TWI��ʼ��
void twi_master_init(void)
{
    ret_code_t err_code;
    //���岢��ʼ��TWI���ýṹ��
    const nrf_drv_twi_config_t twi_config = {
       .scl                = TWI_SCL_M,  //����TWI SCL����
       .sda                = TWI_SDA_M,  //����TWI SDA����
       .frequency          = NRF_DRV_TWI_FREQ_100K, //TWI����
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH, //TWI���ȼ�
       .clear_bus_init     = false//��ʼ���ڼ䲻����9��SCLʱ��
    };
    //��ʼ��TWI
    err_code = nrf_drv_twi_init(&m_twi, &twi_config, twi_handler, NULL);
	//��鷵�صĴ������
    APP_ERROR_CHECK(err_code);
    //ʹ��TWI
    nrf_drv_twi_enable(&m_twi);
}

/*************************************************************************
 * ��  �� : дSSD1315�Ĵ���
 * ��  �� : register_address[in]���Ĵ�����ַ
 *        : value[in]��д�������
 * ����ֵ : true:д���ݳɹ�,false��д��ʧ��
 *************************************************************************/ 
bool ssd1315_register_write(uint8_t register_address, uint8_t value)
{
	  ret_code_t err_code;
	  uint8_t tx_buf[2];
	
	  //׼��д�������
		tx_buf[0] = register_address;
    tx_buf[1] = value;
	  //TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
		//д������
    err_code = nrf_drv_twi_tx(&m_twi, OLED_ADDRESS, tx_buf, 2, false);
	  //�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
	  if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;	
}

/*************************************************************************
 * ��  �� : ��SSD1315�Ĵ���
 * ��  �� : register_address[in]���Ĵ�����ַ
 *        : * destination[out]  ��ָ�򱣴��ȡ���ݵĻ���
 *        : number_of_bytes[in] ����ȡ�����ݳ���
 * ����ֵ : true:�����ɹ�,false������ʧ��
 *************************************************************************/ 
bool ssd1315_register_read(uint8_t register_address, uint8_t * destination, uint8_t number_of_bytes)
{
	  ret_code_t err_code;
	  //TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
	  err_code = nrf_drv_twi_tx(&m_twi, OLED_ADDRESS, &register_address, 1, true);
	  //�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
    if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		//TWI������ɱ�־����Ϊfalse
		m_xfer_done = false;
	  err_code = nrf_drv_twi_rx(&m_twi, OLED_ADDRESS, destination, number_of_bytes);
		//�ȴ�TWI���ߴ������
		while (m_xfer_done == false){}
		if (NRF_SUCCESS != err_code)
    {
        return false;
    }
		return true;
}

void oled_write_cmd(unsigned char cmd)
{
	ssd1315_register_write(0x00,cmd);
}

void oled_write_data(unsigned char mdata)
{
	ssd1315_register_write(0x40,mdata);
}

void oled_screen_fill(unsigned char cmd)
{
	unsigned int i,j;
	for(i=0;i<8;i++)
	{
		oled_write_cmd(0xb0+i);
		oled_write_cmd(0x00);
		oled_write_cmd(0x10);
		for(j=0;j<128;j++)
		{
			oled_write_data(cmd);
		}
	}
}

void oled_Set_pos(unsigned char x,unsigned char y)
{
	oled_write_cmd(0xb0+y);
	oled_write_cmd(((x&0xf0)>>4)|0x10);
	oled_write_cmd((x&0x0f)|0x01);
}

void OLED_P6x8Str(unsigned char x, unsigned char y,const char *const ch)
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		oled_Set_pos(x,y);
		for(i=0;i<6;i++)
		oled_write_data(F6x8[c][i]);
		x+=6;
		j++;
	}
}

void OLED_P8x16Str(unsigned char x, unsigned char y,const char *const ch)
{
	unsigned char i=0,j=0;
	unsigned int addr=0;
	while (ch[j]!='\0')
	{
		addr =(ch[j]-32)*16;
		if(x>128){x=0;y+=2;}
		oled_Set_pos(x,y);
		for(i=0;i<8;i++)
		{
			oled_write_data(F8X16[addr]);
			addr += 1;
		}
		oled_Set_pos(x,y+1);
		for(i=0;i<8;i++)
		{
			oled_write_data(F8X16[addr]);
			addr += 1;
		}
		x+=8;
		j++;
	}
}



/*************************************************************************
 * ��  �� : ��ʼ��SSD1315
 * ��  �� : ��
 * ����ֵ : true:��ʼ���ɹ���false����ʼ��ʧ��
 *************************************************************************/ 
void oled_init(void)
{   
  nrf_delay_ms(500);
	
	oled_write_cmd(0xAE); //display off
	oled_write_cmd(0x20);	//Set Memory Addressing Mode	
	oled_write_cmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	oled_write_cmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	oled_write_cmd(0xc8);	//Set COM Output Scan Direction
	oled_write_cmd(0x00); //---set low column address
	oled_write_cmd(0x10); //---set high column address
	oled_write_cmd(0x40); //--set start line address
	oled_write_cmd(0x81); //--set contrast control register
	oled_write_cmd(0xff); //���ȵ��� 0x00~0xff
	oled_write_cmd(0xa1); //--set segment re-map 0 to 127
	oled_write_cmd(0xa6); //--set normal display
	oled_write_cmd(0xa8); //--set multiplex ratio(1 to 64)
	oled_write_cmd(0x3F); //
	oled_write_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	oled_write_cmd(0xd3); //-set display offset
	oled_write_cmd(0x00); //-not offset
	oled_write_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	oled_write_cmd(0xf0); //--set divide ratio
	oled_write_cmd(0xd9); //--set pre-charge period
	oled_write_cmd(0x22); //
	oled_write_cmd(0xda); //--set com pins hardware configuration
	oled_write_cmd(0x12);
	oled_write_cmd(0xdb); //--set vcomh
	oled_write_cmd(0x20); //0x20,0.77xVcc
	oled_write_cmd(0x8d); //--set DC-DC enable
	oled_write_cmd(0x14); //
	oled_write_cmd(0xaf); //--turn on oled panel
	
	oled_screen_fill(0x00);
	
}

void oled_show_line(const char *const buf,uint8_t line,uint8_t size)
{
	
	if(size==8)
	{
		switch(line)
		{
			case 1:
				OLED_P6x8Str(0,0,buf);
				break;
			case 2:
				OLED_P6x8Str(0,1,buf);
				break;
			case 3:
				OLED_P6x8Str(0,2,buf);
				break;
			case 4:
				OLED_P6x8Str(0,3,buf);
				break;
			case 5:
				OLED_P6x8Str(0,4,buf);
				break;
			case 6:
				OLED_P6x8Str(0,5,buf);
				break;
			case 7:
				OLED_P6x8Str(0,6,buf);
				break;
			case 8:
				OLED_P6x8Str(0,7,buf);
			default:
				break;
		}
	}
	else if(size==16)
	{
		switch(line)
		{
			case 1:
				OLED_P8x16Str(0,0,buf);
				break;
			case 2:
				OLED_P8x16Str(0,2,buf);
				break;
			case 3:
				OLED_P8x16Str(0,4,buf);
				break;
			case 4:
				OLED_P8x16Str(0,6,buf);
				break;
			default:
				break;
		}
	}
	/*else{
		//...
	}
	...
	*/
}
