#include "ws2812b.h"
#include "color.h"
#include "stdalay.h"

int main()
{
	uint8_t i = 0;
	
	stdalay_init(72);
	
	ws2812bInit();
	
	for(i=0;i<12;i++)
	{
		ws2812bWriteAColorData(Type6[i*5+4]);
	}
	
	ws2812bReset();
	
	while(1)
	{
		
	}
}
