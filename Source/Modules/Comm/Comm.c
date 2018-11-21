/*
 * Comm.c
 *
 *  Created on: 2016��5��23��
 *      Author: Administrator
 */
/******************************************************************************/
#include "Comm.h"

/******************************************************************************/
void Comm_Init(void)
{
	/* Initialize CAN bus */
	Comm_CAN_Init();
	
	/* ���ͺͽ���FIFO��ʼ�� */
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
/* �����͵ĵ�֡�������к��� */
void Comm_CanTxSingleDataPut(uint32 stdid, uint8 *dataPtr, uint8 len)
{
	CAN_SendDataTypedef TxMsg;

	TxMsg.StdId = stdid;
	TxMsg.DLC = len;

	memcpy(&TxMsg.Data, dataPtr, len);
	
	/* �����͵����ݴ���FIFO */
	Comm_CAN_FIFO_TxDataPut( &TxMsg, &TxDataFIFO );
}

/******************************************************************************/
/* �����͵Ķ�֡�������к��� */
void Comm_CanTxMultiDataPut(uint32 stdid, uint8* dataPtr, uint8 len)
{
	MultiFrameBufferTypedef SourceData;
	CAN_SendDataTypedef TxMsg;

	/* ��֡����ӵ����ͬ��ID�� */
	TxMsg.StdId = stdid;
	
	/* ��ȡ��֡�������� */
	SourceData.TotleLenth = (len / MultiFrameOnePackage_LenMax - 1);
	
	/* ��:�ܳ�Ϊ6֡������,ÿ֡��һ���ֽ�Ϊ֡�ź�
			��һ֡����0x0500
			�ڶ�֡����0x0501
			...
			���һ֡����0x0505 --->�������
	*/
	
	/* ���һ֡�ж� */
	if(len % MultiFrameOnePackage_LenMax != RESET)
	{
		SourceData.TotleLenth++;
	}
	
	/* ��ȡlen���ȵ����� */
	memcpy(&SourceData.MultiDataBuffer, dataPtr, len);
	
	/* ��ʼ����ǰ���Ⱥ�״̬ */
	SourceData.CurrentLenth = RESET;
	SourceData.Status = RESET;
	
	/* �����ݷָ�ɶ�֡���е�FIFO�� */
	for(SourceData.CurrentLenth = 0; 
		SourceData.CurrentLenth <= SourceData.TotleLenth; 
		SourceData.CurrentLenth++)
	{
		/* �������һ֡ */
		if(SourceData.CurrentLenth == SourceData.TotleLenth)
		{
			/* ��ȡ���һ֡�����ݳ��� */
			TxMsg.DLC = len % MultiFrameOnePackage_LenMax + 1;
		}
		else
		{
			/* �������һ֡,�򳤶ȹ̶�Ϊ8 */
			TxMsg.DLC = MultiFrameOnePackage_LenMax + 1;
		}
		
		/* ÿһ֡�ĵ�һ���ֽ���Ϊ֡��Ϣ,��4λ--��֡��,��4λ--��ǰ֡�� */
		TxMsg.Data[0] = (((SourceData.TotleLenth & 0x0f) << 4) 
						| (SourceData.CurrentLenth & 0x0f));
		
		/* ��Դ������Copy��Ӧ���ȵ�����, */
		memcpy(&TxMsg.Data[1], 
			   &SourceData.MultiDataBuffer
					[SourceData.CurrentLenth * MultiFrameOnePackage_LenMax],
			   (TxMsg.DLC - 1));
		
		/* Copy�õ��������е�FIFO�� */
		Comm_CAN_FIFO_TxDataPut( &TxMsg, &TxDataFIFO );
	}
	
	/* ����������ǵ���� ^-^ */
	Comm_CanMultiFIFOInit(&SourceData);
}

/******************************************************************************/
void Comm_CanTxDataPut(uint32 stdid, uint8* dataPtr, uint8 len)
{
	if(len > 8)
	{
		/* ����һ����֡ */
		Comm_CanTxMultiDataPut(stdid, dataPtr, len);
	}
	else
	{
		/* ����һ����֡ */
		Comm_CanTxSingleDataPut(stdid, dataPtr, len);
	}
}
/******************************************************************************/
/* �����͵����ݳ��в�����CAN���� */
ErrorStatus Comm_CanTxDataSend(void)
{
	CanTxMsg TxMsg;
	uint8 TMailBox, Status;
	uint8 i;
	
	Status = Comm_CAN_FIFO_TxDataGet(&TxDataFIFO, &TxMsg);
	
	/* ��ȡ�ɹ� */
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
	/* Time Out ���ش��� */
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
/* ��FIFO�л�ȡ���ݲ����� */
void Comm_CanRxDataGet(void)            
{
	uint8 Status;
	uint8 buf[2] = {0x09,0x00};
	CAN_ReceiveDataTypedef RxMsg;
	Status = Comm_CAN_FIFO_RxDataGet(&RxDataFIFO, &RxMsg);

	/* ��ȡ�ɹ� */
	if(Status == SUCCESS)
	{
		switch(RxMsg.StdId)
		{
			case STDID_START:
				Movement_start = 1;
				Comm_CanDirectSend(STDID_START_ACHIEVE,buf,1);
//				Comm_CanDirectSend(STDID_FILL_END,buf,1);
				break;

			case STDID_INFUSION_PREPARE:
				ProcessCMD_Infusion(0);
				ProcessCMD_Inject(0);
				break;

			case STDID_EXHAUST_AIR:
				Exhaust_Air = RxMsg.Data[0];
				break;		

			case STDID_RECYCLE_BEAD:
				Recycle_Bead = RxMsg.Data[0];
				break;

			case STDID_SEND_INJUCET_TIME:
				memcpy(&Injucet_time,RxMsg.Data,2);
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