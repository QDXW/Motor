/******************************************************************************/
#include "Movement.h"

/******************************************************************************/
PROBE_POSITION currPos = {0};

/******************************************************************************/
volatile uint8 Movement_X_intNotResp = 0;
volatile uint8 Movement_X_originWaiting = 0;

volatile uint8 Movement_Z_intNotResp = 0;
volatile uint8 Movement_Z_originWaiting = 0;

/******************************************************************************/
void Movement_X_GotoTarget(MOTOR_DIR dir, uint16 Movement_X_Step)
{
	Movement_X_pulseNumber = Movement_X_Step;
	Movement_X_pulseCount = 0;
	Movement_X_start = TRUE;

	switch(dir)
	{
		case 0:
			Movement_X_MotorDriver_DIR(DIR_CW);
			Direction_Motor = DIR_CW;
			break;
		case 1:
			Movement_X_MotorDriver_DIR(DIR_CCW);
			Direction_Motor = DIR_CCW;
			break;
		default:
		break;
	}
	Movement_X_Start();
	while(Movement_X_start);
}

/******************************************************************************/
void Movement_Z_GotoTarget(MOTOR_DIR dir,uint16 Movement_Z_Step)
{
	Movement_Z_pulseNumber = Movement_Z_Step;
	Movement_Z_pulseCount = 0;
	Movement_Z_start = TRUE;

	switch(dir)
	{
	case 0:
		Movement_Z_MotorDriver_DIR(DIR_CW);
		Direction_Motor = DIR_CW;
		break;
	case 1:
		Movement_Z_MotorDriver_DIR(DIR_CCW);
		Direction_Motor = DIR_CCW;
		break;
	}

	Movement_Z_Start();
	while(Movement_Z_start);
}

/******************************************************************************/
void ProcessCMD_Inject(uint8 Data)
{
	uint8 buf[2] = {0x09,0x00};
	while(Movement_start)
	{
		/* 发送注液命令  */
		Comm_CanDirectSend(STDID_INJECT,buf,2);
		Delay_ms_SW(Injucet_time);

		/* 注液完成Z轴抬起 */
		Movement_Z_GotoTarget(DIR_CW,1600);
		Delay_ms_SW(50);

		if((Action_Count++) > 7)
		{
			Action_Count = 0;
			Movement_start = 0;

			/* 抽液准备完成  */
			Comm_CanDirectSend(STDID_INFUSION,buf,2);
			Movement_X_GotoTarget(DIR_CCW,10000);
			Comm_CanDirectSend(STDID_FILL_END,buf,1);
			return;
		}

		/* x轴移动 */
		Movement_X_GotoTarget(DIR_CW,490);

		/* Z轴下放  */
		Movement_Z_GotoTarget(DIR_CCW,1600);
		Delay_ms_SW(50);
	}

	Comm_CanDirectSend(STDID_INJECT_PREPARE_ACHIEVE,buf,1);
}

/******************************************************************************/
void ProcessCMD_Infusion(uint8 Data)
{
	if(!Movement_X_ReadPosSensor())
	{
		Movement_X_GotoTarget(DIR_CCW,10000);
		Movement_X_ResetPosition();
	}

	if(!Movement_Z_ReadPosSensor())
	{
		Movement_Z_GotoTarget(DIR_CW,10000);
		Movement_Z_ResetPosition();
	}

	/* Z轴下放  */
	Movement_Z_GotoTarget(DIR_CCW,1600);
	Delay_ms_SW(200);
}

/******************************************************************************/
void Return_Zero_Position(void)
{
	uint8 buf[2] = {0x09,0x00};
	Delay_ms_SW(700);
	Movement_Z_GotoTarget(DIR_CW,10000);
	Movement_Z_ResetPosition();
	Movement_X_GotoTarget(DIR_CCW,10000);
	Movement_X_ResetPosition();
	Comm_CanDirectSend(STDID_INFUSION,buf,2);
	Delay_ms_SW(9000);
}

/******************************************************************************/
void Movement_X_ResetPosition(void)
{
	/* Reset position */
	currPos.xPos = 0;	
}

/******************************************************************************/
void Movement_Z_ResetPosition(void)
{
	/* Reset position */
	currPos.zPos = 0;	
}
