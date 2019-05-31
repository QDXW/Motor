/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
int main(void)
{
	SysTick_Init();

	Comm_Init();

	Motor_Init();
#if MOVEMENT_MOTOR
	Return_Zero_Position();
#endif
//	Movement_Z_Forever();
//	Movement_X_Forever();

	while(1)
	{
		Comm_Process();
	}
}
