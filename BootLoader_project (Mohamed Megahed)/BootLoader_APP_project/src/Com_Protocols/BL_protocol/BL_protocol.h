/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_H_
#define COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_H_
/*Frame sent to initiate the flashing sequence*/
#define WRITE_REQ_FRAME_ID		0
/*Frame that carry the start address, entry point and size*/
#define START_FRAME_ID			1
/*Frame that carry the data*/
#define DATA_FRAME_ID			2
/*Frame to respond (positive or negative) to the last frame received*/
#define RESPONSE_FRAME_ID		3
/*Frame that indicates if there are still headers to be sent or not*/
#define LAST_HEADER_FRAME_ID	4

/*Responses sent in the response frame*/
#define POSITIVE_RESPONSE		0x11
#define NEGATIVE_RESPONSE		0x22

/*indication whether there will be next header or not*/
#define HEADER_FRAME_INCOMMING	0x44
#define HEADER_FRAME_END		0x55

/*Keys that indicate the bootloader action when found*/
#define FLASH_NEW_PROGRAM		0x66666666
#define NEW_PROGRAM_FLASHED		0x77777777
#define NO_PORGRAM_EXISTS		0x88888888

typedef struct
{
	u32 marker;
	u32 startAdress;
	u32 entryPoint;
}keyData_t;

typedef struct
{
	u8 Frame_ID;
	u8 response;
	u16 data_length;
	u8* buffer;
	u8* data;
	u32 startAdress;
	u32	entryPoint;
	u32 size;
	u32 key;
	u8 header_indicator;
}packet_t;


typedef struct
{
	u8 SOF;
	u8 ID;
	u8 reqNo;
	u8 checksum;
}BL_Protocol_Header_t;


typedef struct
{
	BL_Protocol_Header_t header;
	union
	{
		struct
		{
			u32 startAdress;
			u32 entryPoint;
			u32 size;
		}start_frame;
		u32 key;
		u8 header_indicator;
		u8 response;
		u8 data[1024];
	};

	u8 eof;
}BL_Protocol_Frame;


u8 BLProtocol_constructFrame(packet_t* packet,u16* frame_length);
u8 BLProtocol_extractData(packet_t* packet);
void BLProtocol_getFrameLength(u8 ID,u16* length);

#endif /* COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_H_ */
