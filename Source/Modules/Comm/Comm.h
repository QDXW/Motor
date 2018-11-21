/*
 * Comm.h
 *
 *  Created on: 2016Äê5ÔÂ23ÈÕ
 *      Author: Administrator
 */
/******************************************************************************/
#ifndef SOURCE_MODULE_COMM_COMM_H_
#define SOURCE_MODULE_COMM_COMM_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
extern void Comm_Init(void);
void Comm_CanTxSingleDataPut(uint32 stdid, uint8* dataPtr, uint8 len);
void Comm_CanTxMultiDataPut(uint32 stdid, uint8* dataPtr, uint8 len);
void Comm_CanTxDataPut(uint32 stdid, uint8* dataPtr, uint8 len);
void Comm_CAN_SendOnePackage(uint16 stdId, uint8 *dataPtr, uint8 len);
extern void Comm_CanDirectSend(uint16 stdId, uint8 *dataPtr, uint16 len);
extern ErrorStatus Comm_CanTxDataSend(void);
extern ErrorStatus Comm_CanRxData(CanRxMsg *RxMsg);
extern void Comm_CanRxDataGet(void);
extern void Comm_Process(void);
extern void ProcessCMD_Inject(uint8 Data);
extern void ProcessCMD_Infusion(uint8 Data);

#endif /* SOURCE_MODULE_COMM_COMM_H_ */
