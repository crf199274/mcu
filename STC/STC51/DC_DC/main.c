#include "reg52.h"
#include "ET2046.h"
#include "NixieTube.h"

void main()
{
	double dat = 0,value = 0.0011475,mult = 5.63;
	unsigned int i = 0;
	
	dat = (ET2046_ConvertAndRead(AIN3)*value)*mult;
	dat = (ET2046_ConvertAndRead(AIN3)*value)*mult;
	dat = (ET2046_ConvertAndRead(AIN3)*value)*mult;
	
	while(1)
	{
		display_NixieTube(dat);
		i++;
		if(i==33)
		{
			dat = (ET2046_ConvertAndRead(AIN3)*value)*mult;
			i=0;
		}
	}
}