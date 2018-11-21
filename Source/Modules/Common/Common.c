/*
 * Common.c
 *
 *  Created on: 2016Äê6ÔÂ20ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#include "Common.h"

/******************************************************************************/
static __IO uint32 TimingDelay;
SYSMODE SystemMode_Status = SYSMODE_NORMAL;

/******************************************************************************/
void Common_EXTI_Init(GPIO_TypeDef* port, uint16 pin,
		uint8 portSrc, uint8 pinSrc, uint32 extiLine, EXTITrigger_TypeDef type,
		FunctionalState defaultCmd, uint8 irqCh, uint8 prePri, uint8 subPri)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	if (GPIOA == port)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if (GPIOB == port)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if (GPIOC == port)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if (GPIOD == port)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if (GPIOE == port)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}

	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(port, &GPIO_InitStructure);

	GPIO_EXTILineConfig(portSrc, pinSrc);

	EXTI_InitStructure.EXTI_Line = extiLine;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = type;
	EXTI_InitStructure.EXTI_LineCmd = defaultCmd;
	EXTI_Init(&EXTI_InitStructure);

	EXTI_ClearITPendingBit(extiLine);

	NVIC_InitStructure.NVIC_IRQChannel = irqCh;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************/
uint8 Common_HiByte(uint16 value)
{
	return (uint8)((value & 0xFF00) >> 8);
}

/******************************************************************************/
uint8 Common_LoByte(uint16 value)
{
	return (uint8)(value & 0x00FF);
}

/******************************************************************************/
void Delay_ms_SW(__IO uint32 nCount)
{
#define SW_72MHZ_1MS_COUNT (0x27FF)
	uint32 subCount;
	for(; nCount != 0; nCount--)
	{
		subCount = SW_72MHZ_1MS_COUNT;
		for(; subCount != 0; subCount--);
	}
}

/******************************************************************************/
void Delay_us_SW(__IO uint32 nCount)
{
#define SW_72MHZ_1US_COUNT (10)
	uint32 subCount;
	for(; nCount != 0; nCount--)
	{
		subCount = SW_72MHZ_1US_COUNT;
		for(; subCount != 0; subCount--);
	}
}

/******************************************************************************/
void Delay_ms(__IO uint32 nTime)
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}

/******************************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

/******************************************************************************/
void SysTick_Init(void)
{
	/*sysTick interrupt 1ms*/
	if(SysTick_Config(SYSTICK_FREQ_1MS))
	{
		/* Capture error */
		while (1);
	}
}

/******************************************************************************/
void SoftReset(void)
{
	/* Disable all the interrupts */
	__set_FAULTMASK(1);
	/* Software reset */
	NVIC_SystemReset();
}
