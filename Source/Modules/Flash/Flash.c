/*
 * Flash.c
 *
 *  Created on: 2016年8月2日
 *      Author: Administrator
 */
/******************************************************************************/
#include "Flash.h" 

/******************************************************************************/
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024
#else
#define STM_SECTOR_SIZE	2048
#endif

/******************************************************************************/
uint16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];

/******************************************************************************/
uint16 CalibrationFlag;

RINGROTATIONPOS RingRotationInitPosStep;

/******************************************************************************/
uint16 Flash_ReadHalfWord(uint32 faddr);
void Flash_Write_NoCheck(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite);

/******************************************************************************/
void Flash_Write(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite)
{
	uint32 secpos;
	uint16 secoff;
	uint16 secremain;
 	uint16 i;
	uint32 offaddr;

	if((WriteAddr < STM32_FLASH_BASE) ||
			(WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
		return;

	FLASH_Unlock();

	offaddr = WriteAddr - STM32_FLASH_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	secoff = (offaddr % STM_SECTOR_SIZE) / 2;
	secremain = STM_SECTOR_SIZE / 2 - secoff;

	if(NumToWrite <= secremain)
		secremain = NumToWrite;

	while(1)
	{
		Flash_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE,
				STMFLASH_BUF, STM_SECTOR_SIZE / 2);

		for(i = 0; i < secremain; i++)
		{
			if(STMFLASH_BUF[secoff + i] != 0XFFFF)
				break;
		}

		if(i < secremain)
		{
			FLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE);
			for(i = 0; i < secremain; i++)
			{
				STMFLASH_BUF[i + secoff] = pBuffer[i];
			}
			Flash_Write_NoCheck(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE,
					STMFLASH_BUF, STM_SECTOR_SIZE / 2);
		}
		else
			Flash_Write_NoCheck(WriteAddr, pBuffer, secremain);

		if(NumToWrite == secremain)
			break;
		else
		{
			secpos++;
			secoff = 0;
		   	pBuffer += secremain;
			WriteAddr += secremain;
		   	NumToWrite -= secremain;
			if(NumToWrite > (STM_SECTOR_SIZE / 2))
				secremain = STM_SECTOR_SIZE / 2;
			else
				secremain = NumToWrite;
		}
	};
	FLASH_Lock();
}

/******************************************************************************/
void Flash_Read(uint32 ReadAddr, uint16 *pBuffer, uint16 NumToRead)
{
	uint16 i;
	for(i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = Flash_ReadHalfWord(ReadAddr);
		ReadAddr += 2;
	}
}

/******************************************************************************/
uint16 Flash_ReadHalfWord(uint32 faddr)
{
	return *(volatile uint16*)faddr;
}

/******************************************************************************/
void Flash_Write_NoCheck(uint32 WriteAddr, uint16 *pBuffer, uint16 NumToWrite)
{
	uint16 i;
	for(i = 0; i < NumToWrite; i++)
	{
		FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
	    WriteAddr += 2;
	}
}
void Parameter_Init(void)
{
	/*
			维持温度
			加热时间
			
	*/
}
