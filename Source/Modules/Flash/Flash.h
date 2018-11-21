/*
 * Flash.h
 *
 *  Created on: 2016Äê8ÔÂ2ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef SOURCE_MODULES_CALIBRATION_FLASH_H_
#define SOURCE_MODULES_CALIBRATION_FLASH_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define STM32_FLASH_SIZE (512)
#define STM32_FLASH_WREN (1)
#define STM32_FLASH_BASE (0x08000000)
#define VET6_SIZE (0x80000)
#define USER_DATA_START_ADDR (STM32_FLASH_BASE + VET6_SIZE - 256)

#define DATA_RINGINITSTEPS_ADDR      (USER_DATA_START_ADDR)
#define TEST 	DATA_RINGINITSTEPS_ADDR + 2

/******************************************************************************/
typedef struct
{
	/* 
	 *	The position sensor detects the initial position and 
	 *	continues to walk a certain number of steps.
	*/
	uint16 initPosStep;
	
} RINGROTATIONPOS;

/******************************************************************************/
extern uint16 CalibrationFlag;
extern RINGROTATIONPOS RingRotationInitPosStep;

/******************************************************************************/
extern void Flash_Read(uint32 ReadAddr, uint16 *pBuffer, uint16 NumToRead);
extern void Flash_Write(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite);
extern void Parameter_Init(void);

#endif /* SOURCE_MODULES_FLASH_FLASH_H_ */
