/*
 * Comm.h
 *
 *  Created on: 2016年8月17日
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

/* 缓存级数 */
#define CAN_FIFO_Index    (8) 

/* 多帧数据最长支持字节数 */
#define MultiFrameData_LenMax      (50)

#define MultiFrameOnePackage_LenMax (7)

/********************************************************************************/
/* 单帧FIFO定义 */
typedef struct
{
	/* 数据缓存区:   缓存级数        缓存数据区   */
	uint8 Buffer[CAN_FIFO_Index][CAN_FIFO_LenMax];
	
	/* 入列位置地址 出列位置地址 */
	uint8 PutPos, GetPos;
	
	/* 缓存区容量  缓存区空余缓存数量 错误标志位 */
	uint8  Size, Free, Error;
}SingleFrameFIFOTypedef;

/******************************************************************************/
/* 多帧FIFO定义 */
typedef struct
{
	/* 总的数据帧数 */
	uint8 TotleLenth;
	
	/* 当前数据帧数 */
	uint8 CurrentLenth;
	
	/* 多帧接收的状态 0-未完成接收 1-完成接收 */
	uint8 Status;
	
	/* 错误诊断 */
	uint8 ErrorCnt;
	
	uint8 DLC;
	
	/* 多帧接收Buffer */
	uint8 MultiDataBuffer[MultiFrameData_LenMax];
}MultiFrameBufferTypedef;

/******************************************************************************/
typedef struct
{
	/* 接收到的StdId */
	uint32 StdId;
	
	/* 接收到的Data长度 */
	uint8 DLC;
	
	/* 接收到的Data */
	uint8 Data[8];
	
	/* StdId通过的Filter编号,一一对应,可以通过FMI判断StdId */
	uint8 FMI;
}CAN_ReceiveDataTypedef;

/******************************************************************************/
#define StructRecvDataLenth (14)

/******************************************************************************/
typedef struct
{
	/* 待发送数据的StdId */
	uint32 StdId;
	
	/* 待发送的Data长度 */
	uint8 DLC;
	
	/* 待发送的Data */
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
