/*----------------------------------------------------------------------------
 *  Includes
 *----------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Board_Buttons.h"
#include "Board_LED.h"
#include "serial.h"
#include "cmdline.h"
#include "cmd_func.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef    TimHandle3;
TIM_HandleTypeDef    TimHandle4;
uint16_t uwPrescalerValue = 0;

extern UART_HandleTypeDef UartHandle;

extern void SystemClock_Config(void);
extern void Error_Handler(void);

void TIM3_Config(void);
void TIM4_Config(void);


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    int nStatus;
 /* This sample code shows how to use STM32F4xx GPIO HAL API to toggle PG13 
     IOs (connected to LED3 on STM32F429i-Discovery board) 
    in an infinite loop.
    To proceed, 3 steps are required: */

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure LED3 and LED4 */
  LED_Initialize();
  
  /* Configure the system clock to 180 MHz */
	SystemClock_Config();
  
	SysTick_Config(SystemCoreClock / 100);        /* SysTick 10 msec interrupts */
  
	UART1_Config();
  
///////////////////////////////////////////////////////////////////////
//   TIM3
///////////////////////////////////////////////////////////////////////
  /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
	TIM3_Config();
	TIM4_Config();
   
  //__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
   
   
   
   printf("\n\rUART Printf Example: retarget the C library printf function to the UART\n\r");
   printf("SystemCoreClock[%d] \n\r" ,   SystemCoreClock);
   printf("Compile Data[%s] , Time[%s] \n\r" ,   __DATE__,__TIME__);
   
  /* Infinite loop */
  while (1)
  {
      
    
            //
        // Print a prompt to the console.  Show the CWD.
        //
        printf("cmd> ");

        //
        // Get a line of text from the user.
        //
        UARTgets(g_pcCmdBuf, sizeof(g_pcCmdBuf));

        //
        // Pass the line from the user to the command processor.  It will be
        // parsed and valid commands executed.
        //
        nStatus = CmdLineProcess(g_pcCmdBuf);

        //
        // Handle the case of bad command.
        //
        if(nStatus == CMDLINE_BAD_CMD)
        {
            printf("Bad command!\n\r");
        }

        //
        // Handle the case of too many arguments.
        //
        else if(nStatus == CMDLINE_TOO_MANY_ARGS)
        {
            printf("Too many arguments for command processor!\n\r");
        }

        //
        // Otherwise the command was executed.  Print the error code if one was
        // returned.
        //
        else if(nStatus != 0)
        {
            //printf("Command returned error code \n");
        }      
      
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
  * @brief  Period elapsed callback in non blocking mode
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
	htim->Instance = timer;
	
	htim->Init.Period = period - 1;
	htim->Init.Prescaler = uwPrescalerValue;
	htim->Init.ClockDivision = 0;
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(htim) != HAL_OK) 
	{
		Error_Handler();
	}
	
	if(HAL_TIM_Base_Start_IT(htim) != HAL_OK)
  {
    Error_Handler();														// Starting Error
  }
}


void TIM3_Config(void)
{
	TIM_Config(&TimHandle3, TIM3, 10000); 
}

void TIM4_Config(void)
{
	TIM_Config(&TimHandle4, TIM4, 5000); 
}

#if 0
/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
     uint8_t t;
   
     if( __HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE) )
     {
            
          __HAL_UART_CLEAR_FLAG(&UartHandle, UART_IT_RXNE);
            
         t = USART1->DR;
  

       if(HAL_UART_Transmit(&UartHandle, (uint8_t*) &t, 1,500) != HAL_OK)
       {
         Error_Handler();    
       }   
   
      LED_On (1);
     }

}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/