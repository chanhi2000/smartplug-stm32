/*-----------------------------------------------------------------------------
 *      SmartPlug-ARM
 *-----------------------------------------------------------------------------
 *      Name:    main.c
 *      Purpose: main function
 *---------------------------------------------1--------------------------------
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Includes
 *----------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Board_Buttons.h"
#include "Board_LED.h"
#include "system.h"
#include "serial.h"
#include "cmdline.h"
#include "cmd_func.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern UART_HandleTypeDef UartHandle;

void TIM3_Config(void);
void TIM4_Config(void);
void USART1_IRQHandler(void);

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
		printf("cmd> ");										// Print a prompt to the console.  Show the CWD.

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