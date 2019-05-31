/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 

/******************************************************************************/
void SysTick_Handler(void)
{
	/* Decrease TimingDelay */
}
 
/******************************************************************************/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/******************************************************************************/
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

/******************************************************************************/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/******************************************************************************/
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{

	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

/******************************************************************************/
void EXTI5_9_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	else if (EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	else if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

/******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	else if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{ 
		Movement_X_pulseCount = Movement_X_pulseNumber;
		Movement_X_Stop();
		xPos = 0;
		EXTI_ClearITPendingBit(EXTI_Line11);
	} 
	else if (EXTI_GetITStatus(EXTI_Line12) != RESET) 
	{
		Movement_Z_pulseCount = Movement_Z_pulseNumber;
		Movement_Z_Stop();
		zPos = 0;
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	else if (EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
