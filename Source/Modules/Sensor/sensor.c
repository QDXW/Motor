/******************************************************************************/
#include "sensor.h"

/******************************************************************************/
void MOTOR_PosSensor_Init(void)
{
	MOTOR_X_PosSensor_Init();
	MOTOR_Z_PosSensor_Init();
}

/******************************************************************************/
void MOTOR_X_PosSensor_Init(void)
{
	Common_EXTI_Init(PORT_MOTOR_X_POSSENSOR, PIN_MOTOR_X_POSSENSOR,
			GPIO_PortSourceGPIOC, GPIO_PinSource11, EXTI_Line11,
			EXTI_Trigger_Rising, ENABLE, EXTI15_10_IRQn, 0X02, 0X01);
}

/******************************************************************************/
void MOTOR_Z_PosSensor_Init(void)
{
	Common_EXTI_Init(PORT_MOTOR_Z_POSSENSOR, PIN_MOTOR_Z_POSSENSOR,
			GPIO_PortSourceGPIOC, GPIO_PinSource12, EXTI_Line12,
			EXTI_Trigger_Rising, ENABLE, EXTI15_10_IRQn, 0X02, 0X02);
}

/******************************************************************************/
uint8 Movement_X_ReadPosSensor(void)
{
	uint8 state = 0;
	state = (GPIO_ReadInputDataBit(PORT_MOTOR_X_POSSENSOR, PIN_MOTOR_X_POSSENSOR))?1:0;
	return state;
}

/******************************************************************************/
uint8 Movement_Z_ReadPosSensor(void)
{
	uint8 state = 0;
	state = (GPIO_ReadInputDataBit(PORT_MOTOR_Z_POSSENSOR, PIN_MOTOR_Z_POSSENSOR))?1:0;
	return state;
}

