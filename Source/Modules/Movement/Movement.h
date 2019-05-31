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

extern float zPos;
extern float z_distancePerStep;
extern float z_washPos;
extern float z_extractPos;
extern float z_inject1stPos;
extern float z_interval;
extern uint32 z_runningSteps;

extern float xPos;
extern float x_distancePerStep;
extern float x_injectPosition;
extern uint32 x_runningSteps;

/******************************************************************************/
extern void Back_Zero_XZ(void);
extern void Movement_X_Forever(void);
extern void Movement_Z_Forever(void);
extern void Return_Zero_Position(void);
extern void Movement_Z_GotoOrigin(void);
extern void Movement_X_ResetPosition(void);
extern void Movement_Z_ResetPosition(void);
extern void Movement_Z_Movement(float targetPos);
extern void Movement_X_Movement(float targetPos);
extern void Movement_Z_GotoInitialPosition(void);
extern void Movement_X_GotoInitialPosition(void);
extern void Movement_X_GotoTarget(MOTOR_DIR dir, uint32 Movement_X_Step);
extern void Movement_Z_GotoTarget(MOTOR_DIR dir, uint32 Movement_Z_Step);

#endif

