#ifndef __LCD_H
#define __LCD_H

#include "stc15.h"
#include "typedef.h"

extern float VIN; 
extern float VOUT;
extern float OCC;

//extern float DC_VOL_MAX;//��������ѹ
//extern float DC_VOL_MIN;//������С��ѹ
//extern float DC_VOL_Section;
	
void lcd_init(void);
void updata_LCD(void);


#endif
