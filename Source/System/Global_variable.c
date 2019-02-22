/*
 * Global_variable.c
 *
 *  Created on: 2018Äê9ÔÂ12ÈÕ
 *      Author: Administrator
 */

/******************************************************************************/
#include "comDef.h"
#include "stm32f10x.h"
#include "Global_variable.h"

/******************************************************************************/
uint8 Movement_X_start = 0,Movement_Z_start = 0;
uint8 Action_Count = 0,Exhaust_Air = 0,Bump_Wash = 0;
uint8 Movement_start = 0,Direction_Motor = 0,Infusion_Count = 0;

uint16 Injucet_time = 900,Extract_Air_Count = 0;
uint32 Movement_X_pulseCount = 0,Movement_X_pulseNumber = 0;
uint32 Movement_Z_pulseCount = 0,Movement_Z_pulseNumber = 0;

uint8 Inject_Times = 0;

