/*
 * LIN_cfg.c
 *
 *  Created on: May 5, 2020
 *      Author: anwar
 */
#include "../../LIB/STD_TYPES.h"

#include "LIN_cfg.h"


const LIN_cfg_t LIN_cfg_Arr[MESSAGES_NUM] =
{
		{.ID=0x35 , .execTime_ms=30, .dataLength = 1, .response = SEND},
		//{.ID=0x03 , .execTime_ms=25, .dataLength = 1, .response = REC},
		//{.ID=0x13 , .execTime_ms=20, .dataLength = 2, .response = REC}
};

/*
 * Put the Index of the  ID in the LIN_cfg_Arr in the sutiable slot of the PID_map
 * (according to the commented message ID next to it)
 */
const u8 PID_map[MAX_MESSAGE_NUM]=
{

INVALID_ID,		/*0x00*/
INVALID_ID,		/*0x01*/
INVALID_ID,		/*0x02*/
INVALID_ID,		/*0x03*/
INVALID_ID,		/*0x04*/
INVALID_ID,		/*0x05*/
INVALID_ID,		/*0x06*/
INVALID_ID,		/*0x07*/
INVALID_ID,		/*0x08*/
INVALID_ID,		/*0x09*/
INVALID_ID,		/*0x0A*/
INVALID_ID,		/*0x0B*/
INVALID_ID,		/*0x0C*/
INVALID_ID,		/*0x0D*/
INVALID_ID,		/*0x0E*/
INVALID_ID,		/*0x0F*/
INVALID_ID,		/*0x10*/
INVALID_ID,		/*0x11*/
INVALID_ID,		/*0x12*/
INVALID_ID,		/*0x13*/
INVALID_ID,		/*0x14*/
INVALID_ID,		/*0x15*/
INVALID_ID,		/*0x16*/
INVALID_ID,		/*0x17*/
INVALID_ID,		/*0x18*/
INVALID_ID,		/*0x19*/
INVALID_ID,		/*0x1A*/
INVALID_ID,		/*0x1B*/
INVALID_ID,		/*0x1C*/
INVALID_ID,		/*0x1D*/
INVALID_ID,		/*0x1E*/
INVALID_ID,		/*0x1F*/
INVALID_ID,		/*0x20*/
INVALID_ID,		/*0x21*/
INVALID_ID,		/*0x22*/
INVALID_ID,		/*0x23*/
INVALID_ID,		/*0x24*/
INVALID_ID,		/*0x25*/
INVALID_ID,		/*0x26*/
INVALID_ID,		/*0x27*/
INVALID_ID,		/*0x28*/
INVALID_ID,		/*0x29*/
INVALID_ID,		/*0x2A*/
INVALID_ID,		/*0x2B*/
INVALID_ID,		/*0x2C*/
INVALID_ID,		/*0x2D*/
INVALID_ID,		/*0x2E*/
INVALID_ID,		/*0x2F*/
INVALID_ID,		/*0x30*/
INVALID_ID,		/*0x31*/
INVALID_ID,		/*0x32*/
INVALID_ID,		/*0x33*/
INVALID_ID,		/*0x34*/
0,				/*0x35*/
INVALID_ID,		/*0x36*/
INVALID_ID,		/*0x37*/
INVALID_ID,		/*0x38*/
INVALID_ID,		/*0x39*/
INVALID_ID,		/*0x3A*/
INVALID_ID		/*0x3B*/

};


