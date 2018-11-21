#ifndef SOURCE_MODULES_MOTOR_MOTOR_CAN_H_
#define SOURCE_MODULES_MOTOR_MOTOR_CAN_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define PORT_MOTOR_X_EN		(GPIOA)
#define PIN_MOTOR_X_EN			(GPIO_Pin_8)

#define PORT_MOTOR_X_STP		(GPIOC)
#define PIN_MOTOR_X_STP		(GPIO_Pin_9)

#define PORT_MOTOR_X_DIR		(GPIOC)
#define PIN_MOTOR_X_DIR		(GPIO_Pin_8)

#define PORT_MOTOR_Z_EN		(GPIOB)
#define PIN_MOTOR_Z_EN			(GPIO_Pin_15)

#define PORT_MOTOR_Z_STP		(GPIOC)
#define PIN_MOTOR_Z_STP		(GPIO_Pin_6)

#define PORT_MOTOR_Z_DIR		(GPIOC)
#define PIN_MOTOR_Z_DIR		(GPIO_Pin_7)

/******************************************************************************/
extern void Motor_Init(void);
extern void Motor_X_Init(void);
extern void Motor_Z_Init(void);
extern void Movement_X_MotorDriver_EN(POWER_LEVEL status);
extern void Movement_X_MotorDriver_DIR(MOTOR_DIR dir);
extern void Movement_X_MotorDriver_PWM(FunctionalState status);
extern void Movement_Z_MotorDriver_EN(POWER_LEVEL status);
extern void Movement_Z_MotorDriver_DIR(MOTOR_DIR dir);
extern void Movement_Z_MotorDriver_PWM(FunctionalState status);
extern void Movement_X_Start(void);
extern void Movement_Z_Start(void);
extern void Movement_X_Stop(void);
extern void Movement_Z_Stop(void);

#endif

