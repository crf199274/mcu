#include "wait_enablement.h"

void wait_enablement_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_WakeUpPinCmd(ENABLE);
	PWR_ClearFlag(PWR_FLAG_WU);
	PWR_EnterSTANDBYMode();
}
