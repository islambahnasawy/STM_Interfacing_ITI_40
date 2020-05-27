/*
 * LIN_private.h
 *
 *  Created on: May 5, 2020
 *      Author: anwar
 */

#ifndef MCAL_LIN_LIN_PRIVATE_H_
#define MCAL_LIN_LIN_PRIVATE_H_

#define P0		6
#define P1		7
#define BIT_0	0
#define BIT_1	1
#define BIT_2	2
#define BIT_3	3
#define BIT_4	4
#define BIT_5	5
#define P1_CLEAR_MASK	0x7f

#define NUM_OF_HEADER_BYTES	2

#define SYNC	0
#define PID		1

#define PARITY_MASK			0xC0
#define PARITY_CLEAR_MASK	0x3F
#define SYNC_BYTE 0x55


typedef struct
{
	u8 msg_ID;
	u8 TicksToExec;
	u8 TicksInitValue;

}LIN_Msg_t;

static void LIN_Break_detected(void);
static void Header_received(void);
static void Response_received(void);
static void Send_Checksum(void);

static void LIN_MasterTask(void);
static void LIN_SlaveTask(void);

#endif /* MCAL_LIN_LIN_PRIVATE_H_ */
