/******************************************************************************/
#include "Movement.h"

/******************************************************************************/
PROBE_POSITION currPos = {0};

/******************************************************************************/
volatile uint8 Movement_X_intNotResp = 0;
volatile uint8 Movement_X_originWaiting = 0;

volatile uint8 Movement_Z_intNotResp = 0;
volatile uint8 Movement_Z_originWaiting = 0;

float zPos = 0;
float z_distancePerStep = (3.1415926 * 19.1) / (4 * 200);
float z_washPos = 0;
float z_extractPos = 102.25;
float z_inject1stPos = 102.25 + 81.0;
float z_interval = 15.5;
uint32 z_runningSteps = 0;

float xPos = 0;
float x_distancePerStep = 8.0 / (2 * 200);
float x_injectPosition = 20;
float x_fusionPosition = 78;
uint32 x_runningSteps = 0;

/******************************************************************************/
void Movement_X_GotoTarget(MOTOR_DIR dir, uint32 Movement_X_Step)
{
	Movement_X_pulseNumber = Movement_X_Step;
	Movement_X_pulseCount = 0;
	Movement_X_start = TRUE;

	switch(dir)
	{
		case 0:
			Direction_Motor = DIR_CW;
			Movement_X_MotorDriver_DIR(DIR_CW);

			break;
		case 1:
			Direction_Motor = DIR_CCW;
			Movement_X_MotorDriver_DIR(DIR_CCW);

			break;
		default:
		break;
	}
	Movement_X_Start();
	while(Movement_X_start);
}

/******************************************************************************/
void Movement_Z_GotoInitialPosition(void)
{
	Movement_Z_MotorDriver_DIR(DIR_CCW);
	Movement_Z_Start();
}

/******************************************************************************/
void Movement_X_GotoInitialPosition(void)
{
	Movement_X_MotorDriver_DIR(DIR_CW);
	Movement_X_Start();
}

/******************************************************************************/
void Movement_Z_GotoTarget(MOTOR_DIR dir,uint32 Movement_Z_Step)
{
	Movement_Z_pulseNumber = Movement_Z_Step;
	Movement_Z_pulseCount = 0;
	Movement_Z_start = TRUE;

	switch(dir)
	{
	case 0:
		Direction_Motor = DIR_CCW;
		Movement_Z_MotorDriver_DIR(DIR_CCW);
		break;
	case 1:
		Direction_Motor = DIR_CW;
		Movement_Z_MotorDriver_DIR(DIR_CW);
		break;
	}

	Movement_Z_Start();
	while(Movement_Z_start);
}
/******************************************************************************/
void Movement_Z_Movement(float targetPos)
{
	if (targetPos == zPos)
	{
		zPos = targetPos;
		return;
	}

	if (targetPos > zPos)
	{
		z_runningSteps = (uint32)((targetPos - zPos) / z_distancePerStep);
		Movement_Z_GotoTarget(DIR_CCW, z_runningSteps);
	}
	else
	{
		z_runningSteps = (uint32)((zPos - targetPos) / z_distancePerStep);
		Movement_Z_GotoTarget(DIR_CW, z_runningSteps);
	}
	Delay_ms_SW(50);
	zPos = targetPos;
}

/******************************************************************************/
void Movement_X_Movement(float targetPos)
{
	if (targetPos == xPos)
	{
		xPos = targetPos;
		return;
	}

	if (targetPos > xPos)
	{
		x_runningSteps = (uint32)((targetPos - xPos) / x_distancePerStep);
		Movement_X_GotoTarget(DIR_CW, x_runningSteps);
	}
	else
	{
		x_runningSteps = (uint32)((xPos - targetPos) / x_distancePerStep);
		Movement_X_GotoTarget(DIR_CCW, x_runningSteps);
	}
	Delay_ms_SW(50);
	xPos = targetPos;
}

/******************************************************************************/
void ProcessCMD_Inject(uint8 Data)
{
	uint8 Buffer[2] = {0x00};
	uint8 Inject_Conut = 0;

	if(!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW, 10000);
	}
	Delay_ms_SW(50);

	/*  */
	Movement_Z_Movement(z_inject1stPos);
	Delay_ms_SW(50);
	Movement_X_Movement(x_injectPosition);
	Delay_ms_SW(50);

	/* 通道2注液  */
	Comm_CanDirectSend(STDID_SEND_INJECT_CH2,Buffer,2);
	Delay_ms_SW(2500);

	Movement_X_Movement(0);
	Delay_ms_SW(50);

	for(Inject_Conut = 0;Inject_Conut < Inject_Times;Inject_Conut++)
	{
		Movement_Z_Movement(z_inject1stPos  + ((Inject_Conut + 1) * z_interval));
		Movement_X_Movement(x_injectPosition);

		if(Inject_Conut < 3)
		{
			/* 通道1注液  */
			Buffer[0] = 0;
			Comm_CanDirectSend(STDID_RX_INJECT,Buffer,2);
			Delay_ms_SW(2500);
		}

		if(3 == Inject_Conut)
		{
			/* 通道1注液  */
			Buffer[0] = 1;
			Comm_CanDirectSend(STDID_RX_INJECT,Buffer,2);
			Delay_ms_SW(2500);
		}

		if(4 == Inject_Conut)
		{
			/* 通道1注液  */
			Buffer[0] = 1;
			Comm_CanDirectSend(STDID_SEND_INJECT_CH1,Buffer,2);
			Delay_ms_SW(2500);
		}

		Movement_X_Movement(0);
		Delay_ms_SW(50);
	}

	/* 回初始位置  */
	if(!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW, 20000);
	}
	Delay_ms_SW(50);

	if (!Movement_Z_ReadPosSensor())
	{
		Movement_Z_GotoTarget(DIR_CW, 10000);
	}

	Comm_CanDirectSend(STDID_FILLING_ACHIEVE,Buffer,2);
}

/******************************************************************************/
void ProcessCMD_Infusion(uint8 Data)
{
	uint8 Buffer[2] = {0x7F,0x7F};

	if(!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW, 20000);
	}
	Delay_ms_SW(50);

	Movement_Z_Movement(z_extractPos);
	Delay_ms_SW(50);

	Movement_X_Movement(x_fusionPosition);
	Delay_ms_SW(50);

	Comm_CanDirectSend(STDID_SEND_INFUSION,Buffer,2);
}

/******************************************************************************/
void Return_Zero_Position(void)
{
	Delay_ms_SW(700);
	if(!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW, 20000);
	}
	Delay_ms_SW(100);

	if (Movement_Z_ReadPosSensor())
	{
		Movement_Z_GotoTarget(DIR_CCW, 800);
	}
	Delay_ms_SW(100);

	Movement_Z_GotoInitialPosition();
	Delay_ms_SW(100);

	Movement_X_ResetPosition();
	Movement_Z_ResetPosition();
}

/******************************************************************************/
void Back_Zero_XZ(void)
{
	uint8 Buffer[2] = {0x00};
	if (!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW, 20000);
	}
	Delay_ms_SW(50);

	if (!Movement_Z_ReadPosSensor())
	{
		Movement_Z_GotoTarget(DIR_CW, 20000);
	}
	Delay_ms_SW(50);
	Movement_Z_ResetPosition();

	Comm_CanDirectSend(STDID_SEND_BACK_ZERO_ACHIEVE, Buffer, 1);
}

/******************************************************************************/
void Movement_X_ResetPosition(void)
{
	/* Reset position */
	currPos.zPos = 0;
}

/******************************************************************************/
void Movement_Z_ResetPosition(void)
{
	xPos = 0;
}
