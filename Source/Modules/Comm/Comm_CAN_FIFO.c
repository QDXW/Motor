/*
 * Comm.c
 *
 *  Created on: 2016年8月17日
 *      Author: Administrator
 */
/******************************************************************************/
#include "Comm_CAN_FIFO.h"

/******************************************************************************/
/* 接收/发送CAN数据FIFO */
SingleFrameFIFOTypedef RxDataFIFO,TxDataFIFO;

/* 多帧发送/接收FIFO,有多少个多帧信号就应该定义多少个变量 */
MultiFrameBufferTypedef RxMultiDataBuffer;

/******************************************************************************/
void Comm_CAN_FIFO_Init(SingleFrameFIFOTypedef *FIFO)
{
	uint8 i;
	
	FIFO->Error = RESET;
	FIFO->Free = CAN_FIFO_Index;
	FIFO->GetPos = RESET;
	FIFO->PutPos = RESET;
	FIFO->Size = CAN_FIFO_Index;
	
	/* FIFO->Buffer清零 */
	for(i = 0; i < CAN_FIFO_Index; i++)
	{
		memset(&(FIFO->Buffer[i]), 0, CAN_FIFO_LenMax);
	}
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_RxDataPut(CanRxMsg *SourceData, 
							        SingleFrameFIFOTypedef *DestinationData)
{	
	/* 没有空余缓存,无法入列,返回错误 */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* Copy Stdid到FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_StdId_Pos]),
		&SourceData->StdId, CAN_FIFO_StdId_Len);
	
	/* Copy DLC数据长度到FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_DLC_Pos]),
		&SourceData->DLC, CAN_FIFO_DLC_Len);
	
	/* Copy Data到FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_Data_Pos]),
		&SourceData->Data, SourceData->DLC);
	
	/* Copy FMI (Filter编号)到FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_FMI_Pos]),
		&SourceData->FMI, CAN_FIFO_FMI_Len);
	
	/* 入列地址增加,空白缓存减少 */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* 数据入列成功 */
	return SUCCESS; 
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_RxDataGet(SingleFrameFIFOTypedef *SourceData, 
							        CAN_ReceiveDataTypedef *DestinationData)
{
	uint8* pData;
	uint8* pDest;
	uint8 i;
	
	/* 没有数据可读,返回错误 */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* 获取源数据和目标数据首地址 */
	pData = (uint8*)&(SourceData->Buffer[SourceData->GetPos]);
	pDest = (uint8*)&(DestinationData->StdId);
	
	/* 源数据和目标数据一一对应,直接Copy */
	for(i = 0; i < StructRecvDataLenth; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;
	}

	/* 出列地址增加,空白缓存释放 */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* 数据出列成功 */
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_TxDataPut(CAN_SendDataTypedef *SourceData,
							        SingleFrameFIFOTypedef *DestinationData)
{	
	uint8* pDest;
	uint8* pData;
	uint8 i;
	
	/* 没有空余缓存,无法入列,返回错误 */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* 获取源数据和目标数据首地址 */
	pData = (uint8*)&(SourceData->StdId);
	pDest = (uint8*)&(DestinationData->Buffer[DestinationData->PutPos]);
	
	/* 源数据和目标数据一一对应,直接Copy */
	/* 结构体的sizeof用法不成功,待改进 */
	//for(i = 0; i < sizeof(CAN_SendDataTypedef); i++)
	for(i = 0; i < StructSendDataLenth; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;	
	}
	
	/* 入列地址增加,空白缓存减少 */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* 数据入列成功 */	
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_TxDataGet(SingleFrameFIFOTypedef *SourceData, 
							        CanTxMsg *DestinationData)
{
	/* 没有数据可读,返回错误 */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* 从FIFO Copy Stdid到发送区 */
	memcpy(&DestinationData->StdId, 
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_StdId_Pos],
		   CAN_FIFO_StdId_Len);
	
	/* 从FIFO Copy DLC字节长度到发送区 */
	memcpy(&DestinationData->DLC,
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_DLC_Pos],
		   CAN_FIFO_DLC_Len);
	
	/* 从FIFO Copy Data到发送区 */
	memcpy(&DestinationData->Data,
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_Data_Pos],
		   DestinationData->DLC);
	
	/* 出列地址增加,空白缓存释放 */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* 地址到缓存区末尾,从头再开始 */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* 数据出列成功 */
	return SUCCESS;
}

/******************************************************************************/
void Comm_CAN_FIFO_Copy(SingleFrameFIFOTypedef *SourceData, 
						SingleFrameFIFOTypedef *DestinationData)
{
	uint8 *pData;
	uint8 *pDest;
	uint8 i;
	
	/* 获取源数据和目标数据首地址 */
	pData = (uint8*)&(SourceData->Buffer);
	pDest = (uint8*)&(DestinationData->Buffer);
	
	/* 关闭全局中断 */
	__disable_irq();
	
	/* 执行复制 */
	for(i = 0; i < sizeof(SingleFrameFIFOTypedef); i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;
	}
	
	/* 开启全局中断 */
	__enable_irq();
	
	/* 初始化源FIFO */
	Comm_CAN_FIFO_Init(SourceData);
}

/******************************************************************************/
/* 多帧缓存区清空 */
void Comm_CanMultiFIFOInit(MultiFrameBufferTypedef* MultiDataStatus)
{
	MultiDataStatus->TotleLenth = RESET;
	MultiDataStatus->CurrentLenth = RESET;
	MultiDataStatus->Status = RESET;
	MultiDataStatus->DLC = RESET;
	
	memset(&MultiDataStatus->MultiDataBuffer, 0, MultiFrameData_LenMax);
}


/******************************************************************************/
/* 多帧缓存区接收入列 */
void Comm_CanRxMulitDataPut(CAN_ReceiveDataTypedef* SourceData,
						    MultiFrameBufferTypedef* DestinationData)
{
	/* 这是一个多帧 && 还未接收完全 */
 	if((SourceData->Data[0] != RESET) && (DestinationData->Status == RESET))
	{
		/* 第一帧 只有检测到第一帧才开启整个多帧接收队列 */
		if((SourceData->Data[0] & 0x0f) == 0)
		{
			/* 获取总的帧数 */
			DestinationData->TotleLenth = ((SourceData->Data[0] >> 4) & 0x0f);
			
			/* 获取当前帧数 */
			DestinationData->CurrentLenth = SourceData->Data[0] & 0x0f;
			
			/* 获取该帧的数据 */
			memcpy(&DestinationData->MultiDataBuffer
				   [DestinationData->CurrentLenth * MultiFrameOnePackage_LenMax], 
				   &SourceData->Data[1], (SourceData->DLC - 1));
			
			DestinationData->DLC += SourceData->DLC - 1;
		}
		
		/* 与上一帧相比,判断是不是下一帧 */
		else if(((SourceData->Data[0] & 0x0f) - 1) 
				  == DestinationData->CurrentLenth)
		{
			/* 帧计数 */
			DestinationData->CurrentLenth++;
		
			/* 获取该帧的数据 */
			memcpy(&DestinationData->MultiDataBuffer
				   [DestinationData->CurrentLenth * MultiFrameOnePackage_LenMax], 
				   &SourceData->Data[1], (SourceData->DLC - 1));
			
			DestinationData->DLC += SourceData->DLC - 1;
			
			/* 如果是最后一帧,接收完成 */
			if(DestinationData->CurrentLenth == DestinationData->TotleLenth)
			{
				DestinationData->Status = SET;
			}	
		}
		
		/* 错误诊断,不是第一帧,也不是顺序的下一帧,错误计数 */
		else
		{
			DestinationData->ErrorCnt++;
			if(DestinationData->ErrorCnt > 3)
			{
				/* 非顺序接收,表明该列数据异常 */
				Comm_CanMultiFIFOInit(DestinationData);
			}
		}
		/* 错误改正机制----(待改进) */
		/* 当前接收的帧数 > 总的帧数 发生错误 该FIFO无效 */
		if((DestinationData->CurrentLenth) > (DestinationData->TotleLenth))
		{
			Comm_CanMultiFIFOInit(DestinationData);
		}			
	}
}

