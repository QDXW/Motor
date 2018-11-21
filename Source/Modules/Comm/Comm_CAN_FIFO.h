/*
 * Comm.h
 *
 *  Created on: 2016��8��17��
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef SOURCE_MODULE_COMM_COMM_FIFO_H_
#define SOURCE_MODULE_COMM_COMM_FIFO_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define CAN_FIFO_LenMax      (14)
#define CAN_FIFO_StdId_Len   (4)
#define CAN_FIFO_DLC_Len     (1)
#define CAN_FIFO_Data_Len    (8)
#define CAN_FIFO_FMI_Len     (1)

#define CAN_FIFO_StdId_Pos  (RESET)                                    		//0
#define CAN_FIFO_DLC_Pos	 (CAN_FIFO_StdId_Pos + CAN_FIFO_StdId_Len)		//4
#define CAN_FIFO_Data_Pos   (CAN_FIFO_DLC_Pos + CAN_FIFO_DLC_Len)			   //5
#define CAN_FIFO_FMI_Pos    (CAN_FIFO_Data_Pos + CAN_FIFO_Data_Len)			//13

/* ���漶�� */
#define CAN_FIFO_Index    (8) 

/* ��֡�����֧���ֽ��� */
#define MultiFrameData_LenMax      (50)

#define MultiFrameOnePackage_LenMax (7)

/********************************************************************************/
/* ��֡FIFO���� */
typedef struct
{
	/* ���ݻ�����:   ���漶��        ����������   */
	uint8 Buffer[CAN_FIFO_Index][CAN_FIFO_LenMax];
	
	/* ����λ�õ�ַ ����λ�õ�ַ */
	uint8 PutPos, GetPos;
	
	/* ����������  ���������໺������ �����־λ */
	uint8  Size, Free, Error;
}SingleFrameFIFOTypedef;

/******************************************************************************/
/* ��֡FIFO���� */
typedef struct
{
	/* �ܵ�����֡�� */
	uint8 TotleLenth;
	
	/* ��ǰ����֡�� */
	uint8 CurrentLenth;
	
	/* ��֡���յ�״̬ 0-δ��ɽ��� 1-��ɽ��� */
	uint8 Status;
	
	/* ������� */
	uint8 ErrorCnt;
	
	uint8 DLC;
	
	/* ��֡����Buffer */
	uint8 MultiDataBuffer[MultiFrameData_LenMax];
}MultiFrameBufferTypedef;

/******************************************************************************/
typedef struct
{
	/* ���յ���StdId */
	uint32 StdId;
	
	/* ���յ���Data���� */
	uint8 DLC;
	
	/* ���յ���Data */
	uint8 Data[8];
	
	/* StdIdͨ����Filter���,һһ��Ӧ,����ͨ��FMI�ж�StdId */
	uint8 FMI;
}CAN_ReceiveDataTypedef;

/******************************************************************************/
#define StructRecvDataLenth (14)

/******************************************************************************/
typedef struct
{
	/* ���������ݵ�StdId */
	uint32 StdId;
	
	/* �����͵�Data���� */
	uint8 DLC;
	
	/* �����͵�Data */
	uint8 Data[8];
}CAN_SendDataTypedef;

/******************************************************************************/
#define StructSendDataLenth (13)

/******************************************************************************/
extern SingleFrameFIFOTypedef RxDataFIFO,TxDataFIFO;
extern MultiFrameBufferTypedef RxMultiDataBuffer,TxMultiDataBuffer;

/******************************************************************************/
void Comm_CAN_FIFO_Init(SingleFrameFIFOTypedef *FIFO);
ErrorStatus Comm_CAN_FIFO_RxDataPut(CanRxMsg *SourceData,
							  SingleFrameFIFOTypedef *DestinationData);
ErrorStatus Comm_CAN_FIFO_RxDataGet(SingleFrameFIFOTypedef *SourceData,
							  CAN_ReceiveDataTypedef *DestinationData);
ErrorStatus Comm_CAN_FIFO_TxDataPut(CAN_SendDataTypedef *SourceData,
							  SingleFrameFIFOTypedef *DestinationData);
ErrorStatus Comm_CAN_FIFO_TxDataGet(SingleFrameFIFOTypedef *SourceData,
							  CanTxMsg *DestinationData);
void Comm_CAN_SendPackage(SingleFrameFIFOTypedef *SourceData);
void Comm_CAN_FIFO_Copy(SingleFrameFIFOTypedef *SourceData,
						SingleFrameFIFOTypedef *DestinationData);

void Comm_CanMultiFIFOInit(MultiFrameBufferTypedef* MultiDataStatus);
void Comm_CanRxMulitDataPut(CAN_ReceiveDataTypedef* SourceData,
								   MultiFrameBufferTypedef* DestinationData);
#endif /* SOURCE_MODULE_COMM_COMM_FIFO_H_ */
