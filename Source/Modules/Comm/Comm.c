/*
 * Comm.c
 *
 *  Created on: 2016年5月23日
 *      Author: Administrator
 */
/******************************************************************************/
#include "Comm.h"

float x_washPosition = 75;

/******************************************************************************/
void Comm_Init(void)
{
	/* Initialize CAN bus */
	Comm_CAN_Init();
	
	/* 发送和接收FIFO初始化 */
	Comm_CAN_FIFO_Init(&RxDataFIFO);
	Comm_CAN_FIFO_Init(&TxDataFIFO);
}

/******************************************************************************/
void Comm_Process(void)
{
	/* Send Queue Management */
	Comm_CanTxDataSend();

	/* Receive Queue Management */
	Comm_CanRxDataGet();
}

/******************************************************************************/
/* 待发送的单帧数据入列函数 */
void Comm_CanTxSingleDataPut(uint32 stdid, uint8 *dataPtr, uint8 len)
{
	CAN_SendDataTypedef TxMsg;

	TxMsg.StdId = stdid;
	TxMsg.DLC = len;

	memcpy(&TxMsg.Data, dataPtr, len);
	
	/* 待发送的数据存入FIFO */
	Comm_CAN_FIFO_TxDataPut( &TxMsg, &TxDataFIFO );
}

/******************************************************************************/
/* 待发送的多帧数据入列函数 */
void Comm_CanTxMultiDataPut(uint32 stdid, uint8* dataPtr, uint8 len)
{
	MultiFrameBufferTypedef SourceData;
	CAN_SendDataTypedef TxMsg;

	/* 多帧数据拥有相同的ID号 */
	TxMsg.StdId = stdid;
	
	/* 获取总帧数并保存 */
	SourceData.TotleLenth = (len / MultiFrameOnePackage_LenMax - 1);
	
	/* 例:总长为6帧的数据,每帧第一个字节为帧信号
			第一帧发送0x0500
			第二帧发送0x0501
			...
			最后一帧发送0x0505 --->发送完成
	*/
	
	/* 最后一帧判断 */
	if(len % MultiFrameOnePackage_LenMax != RESET)
	{
		SourceData.TotleLenth++;
	}
	
	/* 获取len长度的数据 */
	memcpy(&SourceData.MultiDataBuffer, dataPtr, len);
	
	/* 初始化当前长度和状态 */
	SourceData.CurrentLenth = RESET;
	SourceData.Status = RESET;
	
	/* 将数据分割成多帧入列到FIFO中 */
	for(SourceData.CurrentLenth = 0; 
		SourceData.CurrentLenth <= SourceData.TotleLenth; 
		SourceData.CurrentLenth++)
	{
		/* 这是最后一帧 */
		if(SourceData.CurrentLenth == SourceData.TotleLenth)
		{
			/* 获取最后一帧的数据长度 */
			TxMsg.DLC = len % MultiFrameOnePackage_LenMax + 1;
		}
		else
		{
			/* 不是最后一帧,则长度固定为8 */
			TxMsg.DLC = MultiFrameOnePackage_LenMax + 1;
		}
		
		/* 每一帧的第一个字节作为帧信息,高4位--总帧数,低4位--当前帧数 */
		TxMsg.Data[0] = (((SourceData.TotleLenth & 0x0f) << 4) 
						| (SourceData.CurrentLenth & 0x0f));
		
		/* 从源数据中Copy相应长度的数据, */
		memcpy(&TxMsg.Data[1], 
			   &SourceData.MultiDataBuffer
					[SourceData.CurrentLenth * MultiFrameOnePackage_LenMax],
			   (TxMsg.DLC - 1));
		
		/* Copy好的数据入列到FIFO中 */
		Comm_CAN_FIFO_TxDataPut( &TxMsg, &TxDataFIFO );
	}
	
	/* 缓存区用完记得清空 ^-^ */
	Comm_CanMultiFIFOInit(&SourceData);
}

/******************************************************************************/
void Comm_CanTxDataPut(uint32 stdid, uint8* dataPtr, uint8 len)
{
	if(len > 8)
	{
		/* 这是一个多帧 */
		Comm_CanTxMultiDataPut(stdid, dataPtr, len);
	}
	else
	{
		/* 这是一个单帧 */
		Comm_CanTxSingleDataPut(stdid, dataPtr, len);
	}
}
/******************************************************************************/
/* 待发送的数据出列并触发CAN发送 */
ErrorStatus Comm_CanTxDataSend(void)
{
	CanTxMsg TxMsg;
	uint8 TMailBox, Status;
	uint8 i;
	
	Status = Comm_CAN_FIFO_TxDataGet(&TxDataFIFO, &TxMsg);
	
	/* 获取成功 */
	if(Status == SUCCESS)
	{
		TxMsg.IDE = CAN_Id_Standard;
		TxMsg.RTR = CAN_RTR_Data;
		
		/* Transmit */
		TMailBox = CAN_Transmit(CAN1, &TxMsg);

		/* Wait for completion */
		i=0;
		while((CAN_TransmitStatus(CAN1, TMailBox) != CAN_TxStatus_Ok) 
			   && (i < 0XFF))
		{
			i++;
		}
	}
	/* Time Out 返回错误 */
	if(i > 0xff)
	{
		return ERROR;
	}
	return SUCCESS;
}

/******************************************************************************/
void Comm_CanDirectSend(uint16 stdId, uint8 *dataPtr, uint16 len)
{
	uint8 packageNumSend = 0;
	uint8 packageCountSend = 0;
	uint8 lastPackageSize = 0;

	if(len <= CAN_PACKAGE_MAX)
	{
	   Comm_CAN_SendOnePackage(stdId, 
				&dataPtr[packageCountSend * CAN_PACKAGE_MAX], len);
	}
	else
	{
		/* Calculate package number */
		packageNumSend = len / CAN_PACKAGE_MAX;
		lastPackageSize = len % CAN_PACKAGE_MAX;
		
		if(lastPackageSize != 0)
			packageNumSend ++;
		
		/* Send one by one */
		for (packageCountSend = 0; packageCountSend < packageNumSend;
			packageCountSend++)
		{
			if (packageCountSend == (packageNumSend - 1))
				/* Last package */
				Comm_CAN_SendOnePackage(stdId, 
					&dataPtr[packageCountSend * CAN_PACKAGE_MAX], lastPackageSize);
			else
				/* Others */
				Comm_CAN_SendOnePackage(stdId, 
					&dataPtr[packageCountSend * CAN_PACKAGE_MAX], CAN_PACKAGE_MAX);
		}
	}
}

/******************************************************************************/
void Comm_CAN_SendOnePackage(uint16 stdId, uint8 *dataPtr, uint8 len)
{
	CanTxMsg TxMessage;
	uint8 i = 0;
	uint8 TMailbox;

	/* Fill header */
	TxMessage.StdId = stdId;
	TxMessage.IDE = CAN_Id_Standard;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = len;
	/* Move data */
	for(i = 0; i < len; i++)
		TxMessage.Data[i]= dataPtr[i];

	/* Transmit */
	TMailbox = CAN_Transmit(CAN1, &TxMessage);
	
	/* Wait for completion */
	i=0;
	while((CAN_TransmitStatus(CAN1, TMailbox) == CAN_TxStatus_Failed) &&
			(i < 0XFF))
		i++;
}

/******************************************************************************/
/* 从FIFO中获取数据并处理 */
void Comm_CanRxDataGet(void)            
{
	uint8 Status;
	uint8 buf[2] = {0x09,0x00};
	CAN_ReceiveDataTypedef RxMsg;
	Status = Comm_CAN_FIFO_RxDataGet(&RxDataFIFO, &RxMsg);

	/* 获取成功 */
	if(Status == SUCCESS)
	{
		switch(RxMsg.StdId)
		{
		/* WASH - 1 */
		case STDID_PUMP_WASH_PREARE:
			/* TIME */
			Bump_Wash = RxMsg.Data[0];
			if(Bump_Wash)
			{
				/* 开始清洗 - 2 */
				if (!Movement_X_ReadPosSensor())
				{
					Movement_X_GotoTarget(DIR_CCW, 20000);
				}
				Delay_ms_SW(200);

				if (!Movement_Z_ReadPosSensor())
				{
					Movement_Z_GotoTarget(DIR_CW, 10000);
				}
				Delay_ms_SW(100);
				Movement_X_Movement(x_washPosition);
				Delay_ms_SW(200);
				Movement_Z_ResetPosition();

				/* 告知泵 - 3  */
				buf[0] = 0X01;
				Comm_CanDirectSend(STDID_BUMP_WASH_START,buf,1);
				Delay_ms_SW(2);
			}
			else
			{
				/* 清洗  */
				buf[0] = 0X01;
				Comm_CanDirectSend(STDID_PUMP_WASH_ACHIEVE,buf,1);
				Delay_ms_SW(600);

				/* 主板 - 停止 */
				if (!Movement_X_ReadPosSensor())
				{
					Movement_X_GotoTarget(DIR_CCW, 10000);
				}
				Delay_ms_SW(200);

				if (!Movement_Z_ReadPosSensor())
				{
					Movement_Z_GotoTarget(DIR_CW, 10000);
				}
				Delay_ms_SW(200);
				Movement_Z_ResetPosition();
				while(!Movement_X_ReadPosSensor());
			}
			break;

			/* 抽液准备  */
			case STDID_INFUSION_PREPARE:
				ProcessCMD_Infusion(0);
				break;

			/* 抽液 - 抽液完成 */
			case STDID_INFUSION_ACHIEVE:
				Delay_ms_SW(1000);
				if (!Movement_X_ReadPosSensor())
				{
					Movement_X_GotoTarget(DIR_CCW, 20000);
				}
				Delay_ms_SW(200);
				ProcessCMD_Inject(0);
				break;

			/* 注液 */
			case STDID_INJECT_PREPARE:
				ProcessCMD_Inject(0);
				break;

			/* 初始化 */
			case STDID_SEND_BACK_ZERO:
				Back_Zero_XZ();
				break;

			/* 泵初始化 */
			case STDID_BUMP_INT_PREPARE:
				if (!Movement_X_ReadPosSensor())
				{
					Movement_X_GotoTarget(DIR_CCW, 20000);
				}
				Delay_ms_SW(200);

				if (!Movement_Z_ReadPosSensor())
				{
					Movement_Z_GotoTarget(DIR_CW, 20000);
				}
				Delay_ms_SW(200);

				Movement_Z_ResetPosition();
				buf[0] = 0;
				Comm_CanDirectSend(STDID_FILLING_ACHIEVE, buf, 1);
				break;

			default:
				break;
		}
	}
}

/******************************************************************************/
ErrorStatus Comm_CanRxData(CanRxMsg *RxMsg)            
{	
	/* Deal directly with the data from the CAN interrupt */
	switch(RxMsg->StdId)
	{
		case 1:
			break;
		default:
			return SUCCESS;
	}
	return ERROR;
}
