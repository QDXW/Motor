/*
 * Common.h
 *
 *  Created on: 2016Äê6ÔÂ20ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef SOURCE_MODULES_COMMON_COMMON_H_
#define SOURCE_MODULES_COMMON_COMMON_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define SYSTICK_FREQ_72MHz  72000000
#define SYSTICK_FREQ_1MS    SYSTICK_FREQ_72MHz/1000

#define ABS_MINUS(X, Y) (X >= Y? X - Y: Y - X)
#define ABS(X) (X >= 0? X : -X)

/******************************************************************************/
typedef enum {
	SYSMODE_NORMAL = 0,
	SYSMODE_DEBUG = 1,
} SYSMODE;

/******************************************************************************/
extern SYSMODE SystemMode_Status;

/******************************************************************************/
extern void Common_EXTI_Init(GPIO_TypeDef* port, uint16 pin,
		uint8 portSrc, uint8 pinSrc, uint32 extiLine, EXTITrigger_TypeDef type,
		FunctionalState defaultCmd, uint8 irqCh, uint8 prePri, uint8 subPri);
uint8 Common_HiByte(uint16 value);
uint8 Common_LoByte(uint16 value);
extern void Delay_ms_SW(__IO uint32 nCount);
extern void Delay_us_SW(__IO uint32 nCount);
extern void SysTick_Init(void);
extern void TimingDelay_Decrement(void);
extern void Delay_ms(__IO uint32 nTime);
extern void SoftReset(void);


#endif /* SOURCE_MODULES_COMMON_COMMON_H_ */
