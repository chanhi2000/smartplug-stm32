
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Board_Buttons.h"
#include "Board_LED.h"


static void SystemClock_Config(void);
static void Error_Handler(void);

volatile uint32_t msTicks;					// counts 1ms timeTicks

/*----------------------------------------------------------------------------
 *	SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) 
{
	msTicks++;
}

/*----------------------------------------------------------------------------
 *	delays number of tick Systicks (happens every 10 ms)
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) 
{
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < dlyTicks) 
	{ 
		__NOP(); 
	}
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    /*  This sample code shows how to use STM32F4xx GPIO HAL API to toggle PG13 
        IOs (connected to LED3 on STM32F429i-Discovery board) 
        in an infinite loop.
        To proceed, 3 steps are required: 	`*/

    /*	STM32F4xx HAL library initialization:
        - Configure the Flash prefetch, instruction and Data caches
        - Configure the Systick to generate an interrupt each 1 msec
        - Set NVIC Group Priority to 4
        - Global MSP (MCU Support Package) initialization	*/

	HAL_Init();
    
	LED_Initialize();                           // Configure LED3 and LED4
    
	SystemClock_Config();                       // Configure the system clock to 180 MHz
  
	SysTick_Config(SystemCoreClock / 100);      // SysTick 10 msec interrupts
	  
    
	while (1)
	{
		/* Infinite loop */
		//LED_On(1);		LED_Off(0);		Delay(25);
		//LED_On(0);		LED_Off(1);		Delay(25);
		
		// LED_On(1);	LED_Off(1);
		// GPIOG->ODR ^= 0x00004000;

		// GPIOG->ODR |= (0x1 << 14);
		// GPIOG->ODR &= ~(0x1 << 14);
	
		GPIOG->BSRR = 0x00004000;
		Delay(1);
		GPIOG->BSRR = 0x40000000;
		Delay(1);
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */

static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
    
	__HAL_RCC_PWR_CLK_ENABLE();				// Enable Power Control clock
  
	/*  The voltage scaling allows optimizing the power consumption when the device is 
        clocked below the maximum system frequency, to update the voltage scaling value 
        regarding system frequency refer to product datasheet.	*/
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_PWREx_EnableOverDrive();						// Activate the Over-Drive mode
 
	/*  Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
		clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/