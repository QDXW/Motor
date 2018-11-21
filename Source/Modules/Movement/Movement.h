#ifndef SOURCE_MODULES_MOVEMENT_MOVEMENT_H
#define SOURCE_MODULES_MOVEMENT_MOVEMENT_H

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
extern volatile uint8 Movement_X_intNotResp;
extern volatile uint8 Movement_X_originWaiting;

extern volatile uint8 Movement_Z_intNotResp;
extern volatile uint8 Movement_Z_originWaiting;

/******************************************************************************/
typedef struct {
	int32 xPos;
	int32 zPos;
} PROBE_POSITION;

/******************************************************************************/
extern PROBE_POSITION currPos;

/******************************************************************************/
extern void Movement_X_GotoTarget(MOTOR_DIR dir, uint16 Movement_X_Step);
extern void Movement_Z_GotoTarget(MOTOR_DIR dir, uint16 Movement_Z_Step);
extern void Return_Zero_Position(void);
extern void Movement_Z_GotoOrigin(void);
extern void Movement_X_ResetPosition(void);
extern void Movement_Z_ResetPosition(void);


#endif

