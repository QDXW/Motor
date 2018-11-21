/*
 * Comm_CAN.h
 *
 *  Created on: 2016Äê6ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef SOURCE_MODULES_COMM_COMM_CAN_H_
#define SOURCE_MODULES_COMM_COMM_CAN_H_

/******************************************************************************/
#include "Project_File.h"

/******************************************************************************/
#define CAN_PB8_PB9 /*  */
//#define CAN_PD0_PD1 /*  */
//#define CAN_PA11_PA12 /*  */
/* CAN speed */
#define CAN_BAUDRATE (500)
/* Command buffer size */
#define SIZE_CMD_BUFFER              (1024)
/* Response buffer size */
#define SIZE_RESP_BUFFER             (1024)
/* Maximal bytes of one package */
#define CAN_PACKAGE_MAX (8)

/******************************************************************************
                      CAN Filter ID == Receive CMD Value
 ******************************************************************************/

/* Receive StdID */                             /* Sender Address   CMD Code */

#define STDID_RX_INVALID                    		(0XFF)
#define STDID_RX_FILLING							(0x10B)
#define STDID_RX_TEST								(0x202)
#define STDID_TX_STOP_FILLING						(0x109)

#define STDID_START									(0x110)
#define STDID_START_ACHIEVE							(0x111)
#define STDID_FILL_END								(0x112)
#define STDID_INFUSION_PREPARE						(0x113)
#define STDID_INFUSION_PREPARE_ACHIEVE				(0x114)
#define STDID_INJECT_PREPARE						(0x115)
#define STDID_INJECT_PREPARE_ACHIEVE				(0x116)
#define STDID_INJECT								(0x120)
#define STDID_INFUSION								(0x11F)
#define STDID_EXHAUST_AIR							(0x123)
#define STDID_EXHAUST_AIR_ACTON						(0x124)
#define STDID_RECYCLE_BEAD							(0x125)
#define STDID_RECYCLE_BEAD_ACTON					(0x126)

#define STDID_SEND_INJUCET_TIME						(0x302)


/******************************************************************************/
#ifdef  CAN_PA11_PA12
#define COMM_CAN_RX_PORT 					(GPIOA)
#define COMM_CAN_RX_PIN 					(GPIO_Pin_11)
#define COMM_CAN_TX_PORT 					(GPIOA)
#define COMM_CAN_TX_PIN 					(GPIO_Pin_12)
#elif defined CAN_PD0_PD1
#define COMM_CAN_RX_PORT 					(GPIOD)
#define COMM_CAN_RX_PIN 					(GPIO_Pin_0)
#define COMM_CAN_TX_PORT 					(GPIOD)
#define COMM_CAN_TX_PIN 					(GPIO_Pin_1)
#elif defined CAN_PB8_PB9
#define COMM_CAN_RX_PORT 					(GPIOB)
#define COMM_CAN_RX_PIN 					(GPIO_Pin_8)
#define COMM_CAN_TX_PORT 					(GPIOB)
#define COMM_CAN_TX_PIN 					(GPIO_Pin_9)
#endif

/******************************************************************************/
extern uint8 Comm_dataBuffer[SIZE_CMD_BUFFER];
extern uint8 Comm_eventBuffer[SIZE_CMD_BUFFER];
extern uint8 Comm_respBuffer[SIZE_RESP_BUFFER];

/******************************************************************************/
extern void Comm_CAN_Init(void);
extern void CAN_Filter_Init(void);

#endif /* SOURCE_MODULES_COMM_COMM_CAN_H_ */
