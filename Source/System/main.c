/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
int main(void)
{
	SysTick_Init();

	Comm_Init();

	Motor_Init();

	Return_Zero_Position();

	while(1)
	{
		Comm_Process();
	}
}
