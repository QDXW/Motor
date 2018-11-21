/******************************************************************************/
#include "Project_File.h"
uint8 Start_Exhaust_Air = 1;

/******************************************************************************/
int main(void)
{
	uint8 buf[2] = {0x09,0x00};

	SysTick_Init();
	Comm_Init();
	Motor_Init();
	Return_Zero_Position();

	while(1)
	{
		Comm_Process();

		if(Exhaust_Air)
		{
			Comm_CanDirectSend(STDID_EXHAUST_AIR_ACTON,buf,2);
			Delay_ms_SW(10000);

			Comm_CanDirectSend(STDID_INFUSION,buf,2);
			Delay_ms_SW(10000);
			Recycle_Bead = 0;
		}

		if(Recycle_Bead)
		{
			buf[0] = 0x01;
			Comm_CanDirectSend(STDID_RECYCLE_BEAD_ACTON,buf,2);
			Delay_ms_SW(10000);

			buf[0] = 0x00;
			Comm_CanDirectSend(STDID_RECYCLE_BEAD_ACTON,buf,2);
			Delay_ms_SW(10000);
			Exhaust_Air = 0;
		}
	}
}
