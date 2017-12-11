/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

volatile uint32_t msTicks;					// counts 1ms timeTicks

void SysTick_Handler(void) 
{
	msTicks++;
}

void Delay (uint32_t dlyTicks) 
{
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) 
	{ 
		__NOP();
	}
}

