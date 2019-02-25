/*
 * Comm.c
 *
 *  Created on: 2016��5��23��
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
		/* WASH - 1 */
		case STDID_PUMP_WASH_PREARE:
			/* TIME */
			Bump_Wash = RxMsg.Data[0];
			if(Bump_Wash)
			{
				/* ��ʼ��ϴ - 2 */
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

				/* ��֪�� - 3  */
				buf[0] = 0X01;
				Comm_CanDirectSend(STDID_BUMP_WASH_START,buf,1);
				Delay_ms_SW(2);
			}
			else
			{
				/* ��ϴ  */
				buf[0] = 0X01;
				Comm_CanDirectSend(STDID_PUMP_WASH_ACHIEVE,buf,1);
				Delay_ms_SW(600);

				/* ���� - ֹͣ */
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

			/* ��Һ׼��  */
			case STDID_INFUSION_PREPARE:
				ProcessCMD_Infusion(0);
				break;

			/* ��Һ - ��Һ��� */
			case STDID_INFUSION_ACHIEVE:
				Delay_ms_SW(1000);
				if (!Movement_X_ReadPosSensor())
				{
					Movement_X_GotoTarget(DIR_CCW, 20000);
				}
				Delay_ms_SW(200);
				ProcessCMD_Inject(0);
				break;

			/* עҺ */
			case STDID_INJECT_PREPARE:
				ProcessCMD_Inject(0);
				break;

			/* ��ʼ�� */
			case STDID_SEND_BACK_ZERO:
				Back_Zero_XZ();
				break;

			/* �ó�ʼ�� */
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
