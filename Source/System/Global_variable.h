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
extern uint8 Movement_X_start,Movement_Z_start,Exhaust_Air,Recycle_Bead;
extern uint8 Movement_start,Action_Count,Direction_Motor;

extern uint16 Movement_X_pulseCount,Movement_X_pulseNumber;
extern uint16 Movement_Z_pulseCount,Movement_Z_pulseNumber,Injucet_time;

#endif /* SOURCE_SYSTEM_GLOBAL_VARIABLE_H_ */
