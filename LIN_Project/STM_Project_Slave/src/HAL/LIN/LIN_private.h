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

#define MASTER_IDLE					0
#define MASTER_SENT_BREAK			2


#define SLAVE_IDLE					0
#define SLAVE_BREAK_DETECTED		1
#define SLAVE_WAITING_HEADER		2
#define SLAVE_HEADER_RECEIVED		3
#define SLAVE_WAITING_RESPONSE		4
#define SLAVE_RESPONSE_RECEIVED		5
#define SLAVE_RECEIVING_RESPONSE  	6
#define SLAVE_CHECKING_CHECKSUM		7

#define SLAVE_SENDING_RESPONSE		8



typedef struct
{
	u8 msg_ID;
	u8 TicksToExec;
	u8 TicksInitValue;

}LIN_Msg_t;

static void LIN_Break_detected(void);
static void LIN_REC_HEADER(void);
static void Header_received(void);
static void Response_received(void);
static void Send_Checksum(void);

static void LIN_MasterTask(void);
static void LIN_SlaveTask(void);

#endif /* MCAL_LIN_LIN_PRIVATE_H_ */
