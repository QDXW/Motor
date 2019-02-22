/*
 * Global_variable.h
 *
 *  Created on: 2018Äê9ÔÂ12ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef SOURCE_SYSTEM_GLOBAL_VARIABLE_H_
#define SOURCE_SYSTEM_GLOBAL_VARIABLE_H_

/******************************************************************************/
#include "comDef.h"
#include "stm32f10x.h"

/******************************************************************************/
extern uint8 Movement_X_start,Movement_Z_start,Exhaust_Air,Bump_Wash;
extern uint8 Movement_start,Action_Count,Direction_Motor,Infusion_Count;
extern uint8 Inject_Times;

extern uint16 Injucet_time,Extract_Air_Count;
extern uint32 Movement_X_pulseCount,Movement_X_pulseNumber;
extern uint32 Movement_Z_pulseCount,Movement_Z_pulseNumber;

#endif /* SOURCE_SYSTEM_GLOBAL_VARIABLE_H_ */
