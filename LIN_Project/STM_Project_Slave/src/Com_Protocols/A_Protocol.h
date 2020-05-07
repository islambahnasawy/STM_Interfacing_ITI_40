#ifndef __A_PROTOCOL_H_
#define __A_PROTOCOL_H_

#include "A_Protocol_cfg.h"


typedef struct
{
	u8 SOF;
	u8 ID;
	u8 data_Len;
	u8 res;
}AProtocol_Header_t;


typedef struct
{
	AProtocol_Header_t header;
	u8 data[8];
	u8 eof;
}AProtocol_t;


u8 AProtocol_ConstructFrame(u8 ID , u8** buffer);
u8 AProtocol_getFrameLength(u8 ID);
u8 AProtocol_extractData(u8 ID, u8 ** buffer);

#endif

