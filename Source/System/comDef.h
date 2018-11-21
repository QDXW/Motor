/*
 * comDef.h
 *
 *  Created on: 2016Äê5ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef SOURCE_SYSTEM_COMDEF_H_
#define SOURCE_SYSTEM_COMDEF_H_

/******************************************************************************/
/* exact-width signed integer types */
typedef   signed          char int8;
typedef   signed short     int int16;
typedef   signed           int int32;
typedef   signed       __int64 int64;

/* exact-width unsigned integer types */
typedef unsigned          char uint8;
typedef unsigned short     int uint16;
typedef unsigned           int uint32;
typedef unsigned       __int64 uint64;

typedef enum {LEVEL_LOW, LEVEL_HIGH} POWER_LEVEL;
typedef enum {CLOSED, OPEN} SWITCH_STATUS;
typedef enum {FALSE, TRUE} Judge;
typedef enum {MODE_NORMAL, MODE_DEBUG} MODE_STATUS;
typedef enum {DIR_CW, DIR_CCW} MOTOR_DIR;

/******************************************************************************/

#endif /* SOURCE_SYSTEM_COMDEF_H_ */
