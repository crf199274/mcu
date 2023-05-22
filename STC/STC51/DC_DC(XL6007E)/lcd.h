#ifndef __LCD_H
#define __LCD_H

#include "stc15.h"
#include "typedef.h"

extern float VIN; 
extern float VOUT;
extern float OCC;

//extern float DC_VOL_MAX;//极限最大电压
//extern float DC_VOL_MIN;//极限最小电压
//extern float DC_VOL_Section;
	
void lcd_init(void);
void updata_LCD(void);


#endif
