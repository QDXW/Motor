#ifndef SOURCE_MODULES_SENSOR_SENSOR_H_
#define SOURCE_MODULES_SENSOR_SENSOR_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define PORT_MOTOR_X_POSSENSOR 			(GPIOC)
#define PIN_MOTOR_X_POSSENSOR 				(GPIO_Pin_11)

#define PORT_MOTOR_Z_POSSENSOR 			(GPIOC)
#define PIN_MOTOR_Z_POSSENSOR 				(GPIO_Pin_12)

/******************************************************************************/
extern void MOTOR_PosSensor_Init(void);
extern void MOTOR_X_PosSensor_Init(void);
extern void MOTOR_Z_PosSensor_Init(void);
extern uint8 Movement_X_ReadPosSensor(void);
extern uint8 Movement_Z_ReadPosSensor(void);


#endif


