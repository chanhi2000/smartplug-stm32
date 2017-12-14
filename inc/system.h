/*-----------------------------------------------------------------------------
 *      SmartPlug-ARM
 *-----------------------------------------------------------------------------
 *      Name:    system.h
 *      Purpose: System Overall Configurations
 *---------------------------------------------1--------------------------------
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

extern uint16_t uwPrescalerValue;

void            Error_Handler                 (void);
extern void     SysTick_Handler               (void);
void            Delay                         (uint32_t dlyTicks);
extern void     SystemClock_Config            (void);

/**
extern void 		HAL_TIM_Base_MspInit					(TIM_HandleTypeDef *htim);
extern void 		HAL_TIM_PeriodElapsedCallback	(TIM_HandleTypeDef *htim);

extern void 		TIM3_IRQHandler								(void);
extern void 		TIM4_IRQHandler								(void);

void 						TIM_Config										(TIM_HandleTypeDef *htim, 
																							 TIM_TypeDef 			 *timer, 
																							 uint32_t 				 period);
extern void 		TIM3_Config										(void);
extern void 		TIM4_Config										(void);
*/
#if 0
void 						USART1_IRQHandler(void);
#endif