/*-----------------------------------------------------------------------------
 *      SmartPlug-ARM
 *-----------------------------------------------------------------------------
 *      Name:    system.c
 *      Purpose: System Overall Configurations
 *---------------------------------------------1--------------------------------
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Includes
 *----------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Board_LED.h"
#include "system.h"


volatile 						uint32_t msTicks;							// counts 1ms timeTicks
uint16_t						uwPrescalerValue = 0;
TIM_HandleTypeDef		TimHandle3;
TIM_HandleTypeDef		TimHandle4;


/**
 *	@brief		This function is executed in case of error occurrence.
 *	@param		None
 *	@retval 	None
 */
void Error_Handler(void)
{
  LED_On(1);														// Turn LED4 on  
  while(1) {}
}

/**
 *	@brief  	This function counts time (in ms)
 *	@param  	None
 *	@retval 	None
 */
void SysTick_Handler(void) 
{
	msTicks++;
}

/**
 *	@brief  	This function delays number of tick Systicks (happens every 10 ms)
 *	@param  	None
 *	@retval 	None
 */
void Delay (uint32_t dlyTicks) 
{
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) {  __NOP();  }
}

/**
 *	@brief		System clock is configured as follow : 
 *	        	 - System Clock source            = PLL (HSE)
 *             - SYSCLK(Hz)                     = 180000000
 *             - HCLK(Hz)                       = 180000000
 *             - AHB Prescaler                  = 1
 *             - APB1 Prescaler                 = 4
 *             - APB2 Prescaler                 = 2
 *             - HSE Frequency(Hz)              = 8000000
 *             - PLL_M                          = 8
 *             - PLL_N                          = 360
 *             - PLL_P                          = 2
 *             - PLL_Q                          = 7
 *             - VDD(V)                         = 3.3
 *             - Main regulator output voltage  = Scale1 mode
 *             - Flash Latency(WS)              = 5
 *	@param		None
 *	@retval 	None
 */
void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
    
	__HAL_RCC_PWR_CLK_ENABLE();							// Enable Power Control clock
  
	/**  
	 *	The voltage scaling allows optimizing the power consumption when the device is 
	 *  clocked below the maximum system frequency, to update the voltage scaling value 
	 *  regarding system frequency refer to product datasheet.	
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
	// Enable HSE Oscillator and activate PLL with HSE as source
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 				= RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM 				= 8;
	RCC_OscInitStruct.PLL.PLLN 				= 360;
	RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ 				= 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_PWREx_EnableOverDrive();						// Activate the Over-Drive mode
 
	/**
   *  Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
   */
	RCC_ClkInitStruct.ClockType 			= (RCC_CLOCKTYPE_SYSCLK | 
																			 RCC_CLOCKTYPE_HCLK 	| 
																			 RCC_CLOCKTYPE_PCLK1 	| 
																			 RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;  
  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief  This function handles TIM interrupt request.  
  * @param  htim: 	pointer to a TIM_HandleTypeDef structure that contains
  *                	the configuration information for TIM module.
	*	@param  timer:	pointer to a TIM_TypeDef structure that contains 
	*									the information for TIMx (e.g. TIM3, TIM4)
	* @param	period:	period value to 
  * @retval None
  * @Note   This function needs prescaler value defined in the global scope so it can be applied 
	*					to other Timer interrupt requests
  */
void TIM_Config(TIM_HandleTypeDef *htim, TIM_TypeDef *timer, uint32_t period)
{
	htim->Instance 							= timer;
	htim->Init.Period 					= period - 1;
	htim->Init.Prescaler 				= uwPrescalerValue;
	htim->Init.ClockDivision 		= 0;
	htim->Init.CounterMode 			= TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(htim) != HAL_OK) 
	{
		Error_Handler();
	}
	if(HAL_TIM_Base_Start_IT(htim) != HAL_OK)
  {
    Error_Handler();														// Starting Error
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
   if(htim->Instance == TIM3)
   {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIMx Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /*##-2- Configure the NVIC for TIMx ########################################*/
    /* Set the TIMx priority */
    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
  
    /* Enable the TIMx global Interrupt */
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
   }
   else if(htim->Instance == TIM4)
   {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIMx Peripheral clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    /*##-2- Configure the NVIC for TIMx ########################################*/
    /* Set the TIMx priority */
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 2);
  
    /* Enable the TIMx global Interrupt */
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
   }   
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM3)				GPIOG->ODR ^= (0x01 << 13);
	else if(htim->Instance == TIM4)		GPIOG->ODR ^= (0x01 << 14);
}


void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle3);
}

void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle4);
}