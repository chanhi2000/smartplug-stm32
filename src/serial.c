/*-----------------------------------------------------------------------------
 *      SmartPlug-ARM
 *-----------------------------------------------------------------------------
 *      Name:    serial.c
 *      Purpose: Serial I/O for STmicroelectronics STM32F4xx
 *-----------------------------------------------------------------------------
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Includes
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "serial.h"

UART_HandleTypeDef UartHandle;							// UART Handler

/**
 *  @brief    Retargets the C library printf function to the USART.
 *  @param    None
 *  @retval   None
 */

struct __FILE 
{
	int dummy;																// FILE struct is used from __FILE
};
 
FILE __stdout;															// Struct FILE is implemented in stdio.h


int fputc(int ch, FILE *f)
{
  /**
   * Place your implementation of fputc here
   * e.g. write a character to the EVAL_COM1 and Loop until the end of transmission
   */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}



/**
 *	@brief  	This function puts the USART peripheral in the Asynchronous 
 *	         	mode (UART Mode)
 *
 * 						UART1 configured as follow:
 *     				 - Word Length = 8 Bits
 *     				 - Stop Bit = One Stop bit
 *     				 - Parity = None
 *     				 - BaudRate = 115200 baud
 *     				 - Hardware flow control disabled (RTS and CTS signals)
 *	@param  	None
 *	@retval 	None
 */
void UART1_Config(void)
{
	// Configure the UART peripheral
	UartHandle.Instance						= USART1;
	UartHandle.Init.BaudRate			= 115200;
	UartHandle.Init.WordLength		= UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits			= UART_STOPBITS_1;
	UartHandle.Init.Parity				= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode					= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling	= UART_OVERSAMPLING_16;
	
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		//Error_Handler();
	}	
}



/**
 * @brief		UART MSP Initialization 
 *	        This function configures the hardware resources used in this example: 
 *	         - Peripheral's clock enable
 *	         - Peripheral's GPIO Configuration  
 * @param		huart: UART handle pointer
 * @retval	None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
	GPIO_InitTypeDef  GPIO_InitStruct;

	/**
	 *	-1- Enable peripherals and GPIO Clocks
	 */
	// Enable GPIO TX/RX clock
	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	// Enable USART2 clock
	__USART1_CLK_ENABLE(); 
	
	/**
	 *	-2- Configure peripheral GPIO
   */
	// UART TX GPIO pin configuration
	GPIO_InitStruct.Pin				= GPIO_PIN_9;
	GPIO_InitStruct.Mode			= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull			= GPIO_PULLUP;
	GPIO_InitStruct.Speed			= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate	= GPIO_AF7_USART1;
	
	HAL_GPIO_Init(GPIOA , &GPIO_InitStruct);
	
	// UART RX GPIO pin configuration
	GPIO_InitStruct.Pin 			= GPIO_PIN_7;
	GPIO_InitStruct.Alternate	= GPIO_AF7_USART1;
	
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);

	
	/**
   * -3- Configure the NVIC for UART
   */
	// NVIC for USART2
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/**
 * @brief		Serial Interface Initialization  
 *	        This function puts the USART peripheral in the Asynchronous 
 *	        mode (UART Mode)
 *
 * 					UART1 configured as follow:
 *	     		 - Word Length = 8 Bits
 *	         - Stop Bit = One Stop bit
 *	         - Parity = None
 *	         - BaudRate = 115200 baud
 *	         - Hardware flow control disabled (RTS and CTS signals)
 * @param		None
 * @retval	None
 */
void SER_Init(void)
{
  /**
   * -1- Configure the UART peripheral
   */
	UartHandle.Instance						= USART1;
	UartHandle.Init.BaudRate			= 115200;
	UartHandle.Init.WordLength		= UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits			= UART_STOPBITS_1;
	UartHandle.Init.Parity				= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl			= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode					= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling	= UART_OVERSAMPLING_16;
	
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		//Error_Handler();
	}	
}

/**
 * @brief		This function writes a character to Serial Port
 * @param		ch: character (in int32_t)
 * @retval	ch: character (in int32_t)
 */
int32_t SER_PutChar (int32_t ch) 
{	
	UART_HandleTypeDef *huart;
	huart = &UartHandle;
	
	while(!(__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE))) {}
	huart->Instance->DR  = (ch & 0xFF);
	
	while( __HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET) {}

	return (ch);
}


/**
 * @brief		This function reads a character from Serial Port
 * @param		ch: character (in int32_t)
 * @retval	ch: character (in int32_t)
 */
int32_t SER_GetChar (void) 
{
	UART_HandleTypeDef *huart;
	huart = &UartHandle;
	
	while(!(__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))) {}
  
  return (huart->Instance->DR   & 0x1FF);
}


/**
 * @brief		This function writes characters squentially to Serial Port
 * @param		pcBuf: 		pointer to the character buffer
 * @param		ui32Len:	size of the buffer
 * @retval	uIdx:			total number of characters written
 */
int UARTwrite(const char *pcBuf, uint32_t ui32Len)
{
	unsigned int uIdx;
	
	// Send the characters
	for(uIdx = 0; uIdx < ui32Len; uIdx++)
	{
		if(pcBuf[uIdx] == '\n')
		{
			/**
			 *	If the character to the UART is `\n`, then add a `\r` before it 
			 *	so that `\n` is translated to `\n\r` in the output.
			 */
			SER_PutChar('\r');
		}
		SER_PutChar(pcBuf[uIdx]);					// Send the character to the UART output.				
	}
	return(uIdx);													// Return the number of characters written.
}

/**
 * @brief		This function reads characters squentially from Serial Port
 * @param		pcBuf: 			pointer to the character buffer
 * @param		ui32Len:		size of the buffer
 * @retval	ui32Count:	total number of characters read
 */
int UARTgets(char *pcBuf, uint32_t ui32Len)
{
	uint32_t ui32Count = 0;
	int8_t cChar;
	static int8_t bLastWasCR = 0;

	/**
	 *	Adjust the length back by 1 to leave space for the 
	 *	trailing null terminator.
	 */
	ui32Len--;

	// Process characters until a newline is received.
	while(1)
	{
		// Read the next character from the console.
		cChar = SER_GetChar();
			
		// See if the backspace key was pressed.
		if(cChar == '\b')
		{
			/**
       *	If there are any characters already in the buffer, 
       *	then delete the last.
       */
			if(ui32Count)
			{
				UARTwrite("\b \b", 3);								// Rub out the previous character.

				ui32Count--;													// Decrement the number of characters in the buffer.
			}
			continue;																// Skip ahead to read the next character.
		}
		/** 
		 *	If this character is LF and last was CR, then just gobble up the
		 *	character because the EOL processing was taken care of with the CR.
		 */
		if((cChar == '\n') && bLastWasCR)
		{
			bLastWasCR = 0;
			continue;
		}
				
		// See if a newline or escape character was received.
		if((cChar == '\r') || (cChar == '\n') || (cChar == 0x1b))
		{
			/**
			 *	If the character is a CR, then it may be followed by a LF which
			 *	should be paired with the CR.  So remember that a CR was
			 *	received.
			 */
			if(cChar == '\r') {  bLastWasCR = 1;  }
			break;																		// Stop processing the input and end the line.
		}

			/**
			 *	Process the received character as long as we are not at the end of
			 *	the buffer.  If the end of the buffer has been reached then all
			 *	additional characters are ignored until a newline is received.
			 */
			if(ui32Count < ui32Len)
			{
				pcBuf[ui32Count] = cChar;									// Store the character in the caller supplied buffer.

				ui32Count++;															// Increment the count of characters received.

				SER_PutChar(cChar);												// Reflect the character back to the user.
			}
		}
		pcBuf[ui32Count] = 0;													// Add a null termination to the string.
		UARTwrite("\r\n", 2);													// Send a CRLF pair to the terminal to end the line.
		return(ui32Count);														// Return the count of int8_ts in the buffer, not counting the trailing 0.
}
