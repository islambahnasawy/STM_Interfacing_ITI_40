#include "../LIB/STD_TYPES.h"
#include "BL_Protocol.h"



u8 BL_Protocol_ConstructStartFrame(u32 key, u32 address, u32 entryPoint, u32 size, u8** buffer)
{
	BL_Protocol_Start_t * frame = (BL_Protocol_Start_t *)(*buffer);

	frame->header.SOF = BL_PROTOCOL_SOF;
	frame->header.REQ_No = BL_STARTFRAME_ReqNum;
	frame->header.Data_Length = BL_STARTFRAME_DATA_SIZE;

	//(u32)*(*buffer + KEY_IDX) = key;
	frame->data[0] = key;
	frame->data[1] = address;
	frame->data[2] = entryPoint;
	frame->data[3] = size;

	u8 EOF_idx = BL_PROTOCOL_HEADER_LENGTH + frame->header.Data_Length;

	(*buffer)[EOF_idx] = BL_PROTOCOL_EOF;

	//(*buffer) += BL_PROTOCOL_HEADER_LENGTH;

	return (EOF_idx+BL_PROTOCOL_EOF_LENGTH);

}

u16 BL_Protocol_ConstructDataFrame(u16 data_len, u8** buffer)
{

	static u8 req_No=1;
	BL_Protocol_Data_t * frame = (BL_Protocol_Data_t *)(*buffer);

	frame->header.SOF = BL_PROTOCOL_SOF;
	frame->header.REQ_No = req_No;
	frame->header.Data_Length = data_len;

	//(u32)*(*buffer + KEY_IDX) = key;


	u16 EOF_idx = BL_PROTOCOL_HEADER_LENGTH + frame->header.Data_Length;

	(*buffer)[EOF_idx] = BL_PROTOCOL_EOF;

	(*buffer) += BL_PROTOCOL_HEADER_LENGTH;

	req_No++;

	return (EOF_idx+BL_PROTOCOL_EOF_LENGTH);

}

u8 BL_Protocol_extractACK(u8 reqNo, u8* buffer)
{
	u8 ack=0;
	BL_Protocol_ACK_t * frame = (BL_Protocol_ACK_t *)(buffer);

	if((frame->header.REQ_No == reqNo) &&(frame->header.Data_Length == 1))
	{
		ack = frame->ack;
	}

	return ack;
}



u16 BL_Protocol_ConstructACKFrame(u8 ack, u8 reqNo, u8* buffer)
{
	u16 frameLength=0;
	BL_Protocol_ACK_t * frame = (BL_Protocol_ACK_t *)(buffer);

	frame->header.SOF = BL_PROTOCOL_SOF;
	frame->header.REQ_No = reqNo;
	frame->header.Data_Length = 1;
	frame->ack = ack;
	frame->eof = BL_PROTOCOL_EOF;

	//(u32)*(*buffer + KEY_IDX) = key;


	frameLength = BL_PROTOCOL_HEADER_LENGTH + frame->header.Data_Length + BL_PROTOCOL_EOF_LENGTH;

	return frameLength;
}


u8 BL_Protocol_getReqNo(u8* buffer)
{
	BL_Protocol_ACK_t * frame = (BL_Protocol_ACK_t *)(buffer);

	return (frame->header.REQ_No);
}
u8 BL_Protocol_extractData(u8 req_No, u8 ** buffer, u16* dataLength)
{
	u8 local_Status = OK;
	static u8 local_req_No=0;

	if(req_No == local_req_No)
	{
		BL_Protocol_Data_t * frame = (BL_Protocol_Data_t *)(*buffer);

		if(frame->header.REQ_No == req_No)
		{
			*dataLength = frame->header.Data_Length;
			(*buffer) += BL_PROTOCOL_HEADER_LENGTH;
			local_req_No++;
		}
		else
		{
			local_Status=NOT_OK;
		}
	}
	else
	{
		local_Status=NOT_OK;
	}


	return local_Status;
}

















