/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 1 April 2020									*/
/*Version : V01											*/
/********************************************************/
#include "../LIB/STD_TYPES.h"
#include "A_Protocol.h"
#include "A_Protocol_cfg.h"


#define APROTOCOL_SOF 0x44
#define APROTOCOL_EOF 0x77
#define APROTOCOL_HEADER_LENGTH 4U
#define APROTOCOL_EOF_LENGTH 1U


u8 AProtocol_ConstructFrame(u8 ID , u8** buffer)
{
	AProtocol_t * frame = (AProtocol_t *)(*buffer);

	frame->header.ID = ID;
	frame->header.SOF = APROTOCOL_SOF;
	frame->header.data_Len = data_size[ID];

	u8 EOF_idx = APROTOCOL_HEADER_LENGTH + frame->header.data_Len;

	(*buffer)[EOF_idx] = APROTOCOL_EOF;

	(*buffer) += APROTOCOL_HEADER_LENGTH;

	return (EOF_idx+APROTOCOL_EOF_LENGTH);
}


u8 AProtocol_getFrameLength(u8 ID)
{
	return (APROTOCOL_HEADER_LENGTH + data_size[ID] + APROTOCOL_EOF_LENGTH);
}


u8 AProtocol_extractData(u8 ID, u8 ** buffer)
{
	u8 local_Status = OK;
	AProtocol_t * frame = (AProtocol_t *)(*buffer);

	if(frame->header.ID == ID)
	{
		(*buffer) += APROTOCOL_HEADER_LENGTH;
	}
	else
	{
		local_Status=NOT_OK;
	}

	return local_Status;


}
