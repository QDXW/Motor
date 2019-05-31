/******************************************************************************/
#include "motor.h"

/******************************************************************************/
void Motor_Init(void)
{
	Motor_X_Init();
	Motor_Z_Init();
	MOTOR_PosSensor_Init();
}

/******************************************************************************/
void Motor_X_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);
	/* Initialize pins */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	/* EN */ 
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_X_EN;
	GPIO_Init(PORT_MOTOR_X_EN, &GPIO_InitStructure);
	
	/* CW */
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_X_DIR;
	GPIO_Init(PORT_MOTOR_X_DIR, &GPIO_InitStructure);
	Movement_X_MotorDriver_EN(LEVEL_HIGH);
	Movement_X_MotorDriver_DIR(DIR_CCW);
	
	/* TIM3_CH4 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* STP */
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_X_STP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(PORT_MOTOR_X_STP, &GPIO_InitStructure);
	
	  /* Time base structure */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Enable the TIM3 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	Movement_X_MotorDriver_PWM(DISABLE);
}

/******************************************************************************/
void Motor_Z_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	/* Initialize pins */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/* EN */ 
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_Z_EN;
	GPIO_Init(PORT_MOTOR_Z_EN, &GPIO_InitStructure);
	/* CW */
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_Z_DIR;
	GPIO_Init(PORT_MOTOR_Z_DIR, &GPIO_InitStructure);

	Movement_Z_MotorDriver_EN(LEVEL_HIGH);
	Movement_Z_MotorDriver_DIR(DIR_CW);

	/* TIM8_CH1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	/* CLK */
	GPIO_InitStructure.GPIO_Pin = PIN_MOTOR_Z_STP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(PORT_MOTOR_Z_STP, &GPIO_InitStructure);

    /* Time base structure */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	/* Enable the TIM8 Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	Movement_Z_MotorDriver_PWM(DISABLE);
}

/******************************************************************************/
void Movement_X_MotorDriver_EN(POWER_LEVEL status)
{
	if(LEVEL_HIGH == status)
		GPIO_SetBits(PORT_MOTOR_X_EN, PIN_MOTOR_X_EN);
	else
		GPIO_ResetBits(PORT_MOTOR_X_EN, PIN_MOTOR_X_EN);
}

/******************************************************************************/
void Movement_X_MotorDriver_DIR(MOTOR_DIR dir)
{
	Direction_Motor = dir;

	if (DIR_CCW == dir)
		GPIO_SetBits(PORT_MOTOR_X_DIR, PIN_MOTOR_X_DIR);
	else
		GPIO_ResetBits(PORT_MOTOR_X_DIR, PIN_MOTOR_X_DIR);
}

/******************************************************************************/
void Movement_X_MotorDriver_PWM(FunctionalState status)
{
	TIM_Cmd(TIM3, status);
	TIM_CtrlPWMOutputs(TIM3, status);
}

/******************************************************************************/
void Movement_Z_MotorDriver_EN(POWER_LEVEL status)
{
	if(LEVEL_HIGH == status)
		GPIO_SetBits(PORT_MOTOR_Z_EN, PIN_MOTOR_Z_EN);
	else
		GPIO_ResetBits(PORT_MOTOR_Z_EN, PIN_MOTOR_Z_EN);
}

/******************************************************************************/
void Movement_Z_MotorDriver_DIR(MOTOR_DIR dir)
{
	if (DIR_CCW == dir)
		GPIO_SetBits(PORT_MOTOR_Z_DIR, PIN_MOTOR_Z_DIR);
	else
		GPIO_ResetBits(PORT_MOTOR_Z_DIR, PIN_MOTOR_Z_DIR);
}

/******************************************************************************/
void Movement_Z_MotorDriver_PWM(FunctionalState status)
{
	TIM_Cmd(TIM8, status);
	TIM_CtrlPWMOutputs(TIM8, status);
}

/******************************************************************************/
void Movement_X_Start(void)
{
	/* 50% */
#if PERISTALTIC_PUMP
	TIM3->ARR = 1600;
#endif
#if MOVEMENT_MOTOR
	TIM3->ARR = 500;
#endif
	TIM3->CCR4 = TIM3->ARR / 2;
	Movement_X_MotorDriver_EN(LEVEL_HIGH);
	Movement_X_MotorDriver_PWM(ENABLE);
}

/******************************************************************************/
void Movement_X_Stop(void)
{
	/* 50% */
	TIM3->CCR4 = 0;
	Movement_X_start = FALSE;
	Movement_X_MotorDriver_PWM(DISABLE);
}

/******************************************************************************/
void Movement_Z_Start(void)
{
	/* 50% */
#if PERISTALTIC_PUMP
	TIM8->ARR = 1600;
#endif
#if MOVEMENT_MOTOR
	TIM8->ARR = 600;
#endif
	TIM8->CCR1 = TIM8->ARR / 2;
	Movement_Z_MotorDriver_EN(LEVEL_HIGH);
	Movement_Z_MotorDriver_PWM(ENABLE);
}

/******************************************************************************/
void Movement_Z_Stop(void)
{
	/* 50% */
	TIM8->CCR1 = 0;
	Movement_Z_start = FALSE;
	Movement_Z_MotorDriver_PWM(DISABLE);
}

/******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{		
		/* Clear the interrupt pending flag */
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		if(Movement_Z_start)
		{
			Movement_Z_pulseCount++;

			/* Move specified steps */
			if (Movement_Z_pulseCount >= Movement_Z_pulseNumber)
			{
				Movement_Z_Stop();

				/* Clear flags */
				Movement_Z_pulseCount = 0;
				Movement_Z_start = FALSE;
			}
		}
	}
}

/******************************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		/* Clear the interrupt pending flag */
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		if(Movement_X_start)
		{
			Movement_X_pulseCount++;

			/* Move specified steps */
			if ((Movement_X_pulseCount >= Movement_X_pulseNumber))
			{
				Movement_X_Stop();

				/* Clear flags */
				Movement_X_pulseCount = 0;
				Movement_X_start = FALSE;
			}
		}
	}
}
