/*
 * Comm.c
 *
 *  Created on: 2016��8��17��
 *      Author: Administrator
 */
/******************************************************************************/
#include "Comm_CAN_FIFO.h"

/******************************************************************************/
/* ����/����CAN����FIFO */
SingleFrameFIFOTypedef RxDataFIFO,TxDataFIFO;

/* ��֡����/����FIFO,�ж��ٸ���֡�źž�Ӧ�ö�����ٸ����� */
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
	
	/* FIFO->Buffer���� */
	for(i = 0; i < CAN_FIFO_Index; i++)
	{
		memset(&(FIFO->Buffer[i]), 0, CAN_FIFO_LenMax);
	}
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_RxDataPut(CanRxMsg *SourceData, 
							        SingleFrameFIFOTypedef *DestinationData)
{	
	/* û�п��໺��,�޷�����,���ش��� */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* Copy Stdid��FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_StdId_Pos]),
		&SourceData->StdId, CAN_FIFO_StdId_Len);
	
	/* Copy DLC���ݳ��ȵ�FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_DLC_Pos]),
		&SourceData->DLC, CAN_FIFO_DLC_Len);
	
	/* Copy Data��FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_Data_Pos]),
		&SourceData->Data, SourceData->DLC);
	
	/* Copy FMI (Filter���)��FIFO */
	memcpy(&(DestinationData->
		Buffer[DestinationData->PutPos][CAN_FIFO_FMI_Pos]),
		&SourceData->FMI, CAN_FIFO_FMI_Len);
	
	/* ���е�ַ����,�հ׻������ */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* ��ַ��������ĩβ,��ͷ�ٿ�ʼ */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* �������гɹ� */
	return SUCCESS; 
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_RxDataGet(SingleFrameFIFOTypedef *SourceData, 
							        CAN_ReceiveDataTypedef *DestinationData)
{
	uint8* pData;
	uint8* pDest;
	uint8 i;
	
	/* û�����ݿɶ�,���ش��� */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* ��ȡԴ���ݺ�Ŀ�������׵�ַ */
	pData = (uint8*)&(SourceData->Buffer[SourceData->GetPos]);
	pDest = (uint8*)&(DestinationData->StdId);
	
	/* Դ���ݺ�Ŀ������һһ��Ӧ,ֱ��Copy */
	for(i = 0; i < StructRecvDataLenth; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;
	}

	/* ���е�ַ����,�հ׻����ͷ� */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* ��ַ��������ĩβ,��ͷ�ٿ�ʼ */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* ���ݳ��гɹ� */
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_TxDataPut(CAN_SendDataTypedef *SourceData,
							        SingleFrameFIFOTypedef *DestinationData)
{	
	uint8* pDest;
	uint8* pData;
	uint8 i;
	
	/* û�п��໺��,�޷�����,���ش��� */
	if(DestinationData->Free == RESET)
	{
		return ERROR;
	}
	
	/* ��ȡԴ���ݺ�Ŀ�������׵�ַ */
	pData = (uint8*)&(SourceData->StdId);
	pDest = (uint8*)&(DestinationData->Buffer[DestinationData->PutPos]);
	
	/* Դ���ݺ�Ŀ������һһ��Ӧ,ֱ��Copy */
	/* �ṹ���sizeof�÷����ɹ�,���Ľ� */
	//for(i = 0; i < sizeof(CAN_SendDataTypedef); i++)
	for(i = 0; i < StructSendDataLenth; i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;	
	}
	
	/* ���е�ַ����,�հ׻������ */
	DestinationData->PutPos++;
	DestinationData->Free--;
	
	/* ��ַ��������ĩβ,��ͷ�ٿ�ʼ */
	if(DestinationData->PutPos >= DestinationData->Size)
	{
		DestinationData->PutPos = RESET;
	}
	
	/* �������гɹ� */	
	return SUCCESS;
}

/******************************************************************************/
ErrorStatus Comm_CAN_FIFO_TxDataGet(SingleFrameFIFOTypedef *SourceData, 
							        CanTxMsg *DestinationData)
{
	/* û�����ݿɶ�,���ش��� */
	if(SourceData->Free == SourceData->Size)
	{
		return ERROR;
	}
	
	/* ��FIFO Copy Stdid�������� */
	memcpy(&DestinationData->StdId, 
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_StdId_Pos],
		   CAN_FIFO_StdId_Len);
	
	/* ��FIFO Copy DLC�ֽڳ��ȵ������� */
	memcpy(&DestinationData->DLC,
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_DLC_Pos],
		   CAN_FIFO_DLC_Len);
	
	/* ��FIFO Copy Data�������� */
	memcpy(&DestinationData->Data,
		   &SourceData->Buffer[SourceData->GetPos][CAN_FIFO_Data_Pos],
		   DestinationData->DLC);
	
	/* ���е�ַ����,�հ׻����ͷ� */
	SourceData->GetPos++;
	SourceData->Free++;
	
	/* ��ַ��������ĩβ,��ͷ�ٿ�ʼ */
	if(SourceData->GetPos >= SourceData->Size)
	{
		SourceData->GetPos = RESET;
	}
	
	/* ���ݳ��гɹ� */
	return SUCCESS;
}

/******************************************************************************/
void Comm_CAN_FIFO_Copy(SingleFrameFIFOTypedef *SourceData, 
						SingleFrameFIFOTypedef *DestinationData)
{
	uint8 *pData;
	uint8 *pDest;
	uint8 i;
	
	/* ��ȡԴ���ݺ�Ŀ�������׵�ַ */
	pData = (uint8*)&(SourceData->Buffer);
	pDest = (uint8*)&(DestinationData->Buffer);
	
	/* �ر�ȫ���ж� */
	__disable_irq();
	
	/* ִ�и��� */
	for(i = 0; i < sizeof(SingleFrameFIFOTypedef); i++)
	{
		*pDest = *pData;
		pDest++;
		pData++;
	}
	
	/* ����ȫ���ж� */
	__enable_irq();
	
	/* ��ʼ��ԴFIFO */
	Comm_CAN_FIFO_Init(SourceData);
}

/******************************************************************************/
/* ��֡��������� */
void Comm_CanMultiFIFOInit(MultiFrameBufferTypedef* MultiDataStatus)
{
	MultiDataStatus->TotleLenth = RESET;
	MultiDataStatus->CurrentLenth = RESET;
	MultiDataStatus->Status = RESET;
	MultiDataStatus->DLC = RESET;
	
	memset(&MultiDataStatus->MultiDataBuffer, 0, MultiFrameData_LenMax);
}


/******************************************************************************/
/* ��֡�������������� */
void Comm_CanRxMulitDataPut(CAN_ReceiveDataTypedef* SourceData,
						    MultiFrameBufferTypedef* DestinationData)
{
	/* ����һ����֡ && ��δ������ȫ */
 	if((SourceData->Data[0] != RESET) && (DestinationData->Status == RESET))
	{
		/* ��һ֡ ֻ�м�⵽��һ֡�ſ���������֡���ն��� */
		if((SourceData->Data[0] & 0x0f) == 0)
		{
			/* ��ȡ�ܵ�֡�� */
			DestinationData->TotleLenth = ((SourceData->Data[0] >> 4) & 0x0f);
			
			/* ��ȡ��ǰ֡�� */
			DestinationData->CurrentLenth = SourceData->Data[0] & 0x0f;
			
			/* ��ȡ��֡������ */
			memcpy(&DestinationData->MultiDataBuffer
				   [DestinationData->CurrentLenth * MultiFrameOnePackage_LenMax], 
				   &SourceData->Data[1], (SourceData->DLC - 1));
			
			DestinationData->DLC += SourceData->DLC - 1;
		}
		
		/* ����һ֡���,�ж��ǲ�����һ֡ */
		else if(((SourceData->Data[0] & 0x0f) - 1) 
				  == DestinationData->CurrentLenth)
		{
			/* ֡���� */
			DestinationData->CurrentLenth++;
		
			/* ��ȡ��֡������ */
			memcpy(&DestinationData->MultiDataBuffer
				   [DestinationData->CurrentLenth * MultiFrameOnePackage_LenMax], 
				   &SourceData->Data[1], (SourceData->DLC - 1));
			
			DestinationData->DLC += SourceData->DLC - 1;
			
			/* ��������һ֡,������� */
			if(DestinationData->CurrentLenth == DestinationData->TotleLenth)
			{
				DestinationData->Status = SET;
			}	
		}
		
		/* �������,���ǵ�һ֡,Ҳ����˳�����һ֡,������� */
		else
		{
			DestinationData->ErrorCnt++;
			if(DestinationData->ErrorCnt > 3)
			{
				/* ��˳�����,�������������쳣 */
				Comm_CanMultiFIFOInit(DestinationData);
			}
		}
		/* �����������----(���Ľ�) */
		/* ��ǰ���յ�֡�� > �ܵ�֡�� �������� ��FIFO��Ч */
		if((DestinationData->CurrentLenth) > (DestinationData->TotleLenth))
		{
			Comm_CanMultiFIFOInit(DestinationData);
		}			
	}
}

