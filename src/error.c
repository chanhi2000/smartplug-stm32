#include "stm32f4xx_hal.h"
#include "Board_LED.h"

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  LED_On(1);		// Turn LED4 on  
  while(1) {}
}