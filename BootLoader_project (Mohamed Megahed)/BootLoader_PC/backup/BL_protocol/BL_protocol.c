/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#include "../STD_TYPES.h"
#include "BL_protocol.h"
#include "BL_protocol_cfg.h"


static u8 writeReqFrameNum;
static u8 startFrameNum;
static u8 dataFrameNum;
static u8 responseFrameNum;
static u8 headerIdicatiorNum;

u8 BLProtocol_constructFrame(packet_t* packet,u16* frame_length)
{
	BL_Protocol_Frame* BL_Frame = (BL_Protocol_Frame*)packet->buffer;

	u8 local_status = OK;
	/*Assigning the SOF and ID as configured in the cfg file*/
	BL_Frame->header.SOF = protocol_cfg[packet->Frame_ID].sof;
	BL_Frame->header.ID = protocol_cfg[packet->Frame_ID].ID;

	*frame_length = protocol_cfg[packet->Frame_ID].frame_length;

	switch(packet->Frame_ID)
	{
	case WRITE_REQ_FRAME_ID:
		/*Assigning a request number specific to each frame type*/
		BL_Frame->header.reqNo=writeReqFrameNum;
		writeReqFrameNum++;
		/*Assigning the checksum (checksum=0 as there is no data in that frame)*/
		//BL_Frame->checksum=0;
		/*Assigning the key value*/
		BL_Frame->key=packet->key;
		/*Putting the EOF as the last byte of this frame*/
		packet->buffer[*frame_length-1] =protocol_cfg[packet->Frame_ID].eof;
		break;
	case START_FRAME_ID:
		/*Assigning a request number specific to each frame type*/
		BL_Frame->header.reqNo=startFrameNum;
		startFrameNum++;

		//BL_Frame->checksum=0;

		/*Assigning the start address, entry point and size in their right place in the frame*/
		BL_Frame->start_frame.startAdress = packet->startAdress;
		BL_Frame->start_frame.entryPoint = packet->entryPoint;
		BL_Frame->start_frame.size = packet->size;

		/*Putting the EOF as the last byte of this frame*/
		packet->buffer[*frame_length-1] =protocol_cfg[packet->Frame_ID].eof;
		break;
	case DATA_FRAME_ID:
		/*Assigning a request number specific to each frame type*/
		BL_Frame->header.reqNo=dataFrameNum;
		dataFrameNum++;

		//BL_Frame->checksum=0;

		/*Length varies according to the number of data bytes sent by user*/
		*frame_length = packet->data_length+FRAME_OVERHEAD;
		/*Assigning the data sent by user in the right place of the frame*/
		for(u16 i=0;i<*frame_length;i++)
		{
			BL_Frame->data[i] = packet->data[i];
		}

		/*Putting the EOF as the last byte of this frame*/
		packet->buffer[*frame_length-1] =protocol_cfg[packet->Frame_ID].eof;
		break;
	case RESPONSE_FRAME_ID:
		/*Assigning a request number specific to each frame type*/
		BL_Frame->header.reqNo=responseFrameNum;
		responseFrameNum++;
		//BL_Frame->checksum=0;
		/*Assigning the response sent by user in the right place of the frame*/
		BL_Frame->response = packet->response;
		/*Putting the EOF as the last byte of this frame*/
		packet->buffer[*frame_length-1] =protocol_cfg[packet->Frame_ID].eof;
		break;
	case LAST_HEADER_FRAME_ID :
		/*Assigning a request number specific to each frame type*/
		BL_Frame->header.reqNo=headerIdicatiorNum;
		headerIdicatiorNum++;
		//BL_Frame->checksum=0;
		/*Assigning the response sent by user in the right place of the frame*/
		BL_Frame->header_indicator = packet->header_indicator;
		/*Putting the EOF as the last byte of this frame*/
		packet->buffer[*frame_length-1] =protocol_cfg[packet->Frame_ID].eof;
		break;
	default :
		/*In case of wrong frame ID*/
		local_status = NOT_OK;
		break;
	}
	return local_status;
}

u8 BLProtocol_extractData(packet_t* packet)
{
	u8 local_status = OK;
	BL_Protocol_Frame* BL_Frame = (BL_Protocol_Frame*)packet->buffer;

	switch(packet->Frame_ID)
	{
	case WRITE_REQ_FRAME_ID :
		/*Making sure the received ID is correct*/
		if(BL_Frame->header.ID != protocol_cfg[WRITE_REQ_FRAME_ID].ID)
		{
			local_status = NOT_OK;
		}
		else
		{
			packet->key=BL_Frame->key;
		}
		break;
	case START_FRAME_ID:
		/*Making sure the received ID is correct*/
		if(BL_Frame->header.ID != protocol_cfg[START_FRAME_ID].ID)
		{
			local_status = NOT_OK;
		}
		else
		{
			/*Extracting the start address , entry point and size*/
			packet->startAdress = BL_Frame->start_frame.startAdress;
			packet->entryPoint = BL_Frame->start_frame.entryPoint;
			packet->size = BL_Frame->start_frame.size;
		}
		break;
	case DATA_FRAME_ID :
		/*Making sure the received ID is correct*/
		if(BL_Frame->header.ID != protocol_cfg[DATA_FRAME_ID].ID)
		{
			local_status = NOT_OK;
		}
		else
		{
			/*Point to where the data is located*/
			packet->data = BL_Frame->data;
		}
		break;
	case RESPONSE_FRAME_ID:
		/*Making sure the received ID is correct*/
		if(BL_Frame->header.ID != protocol_cfg[RESPONSE_FRAME_ID].ID)
		{
			local_status = NOT_OK;
		}
		else
		{
			/*Extracting the response*/
			packet->response = BL_Frame->response;
		}
		break;
	case LAST_HEADER_FRAME_ID :
		/*Making sure the received ID is correct*/
		if(BL_Frame->header.ID != protocol_cfg[LAST_HEADER_FRAME_ID].ID)
		{
			local_status = NOT_OK;
		}
		else
		{
			/*Extracting the response*/
			packet->header_indicator = BL_Frame->header_indicator;
		}
		break;
	default :
		local_status =NOT_OK;
	}
	return local_status;
}


void BLProtocol_getFrameLength(u8 ID,u16* length)
{
	*length = protocol_cfg[ID].frame_length;
}
