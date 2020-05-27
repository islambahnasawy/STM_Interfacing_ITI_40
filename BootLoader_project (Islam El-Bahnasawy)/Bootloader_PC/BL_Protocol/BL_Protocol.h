#ifndef __A_PROTOCOL_H_
#define __A_PROTOCOL_H_

#define BL_PROTOCOL_SOF 0x55
#define BL_PROTOCOL_EOF 0xAA
#define BL_PROTOCOL_HEADER_LENGTH 4U
#define BL_PROTOCOL_EOF_LENGTH 1U
#define BL_STARTFRAME_DATA_SIZE	16U
#define BL_STARTFRAME_ReqNum	0U
#define BL_STARTFRAME_SIZE	21U

#define KEY_IDX 5
#define CMD_FLASH_Header1	1U
#define CMD_FLASH_Header2	2U
#define CMD_FLASH_Header3	3U
#define CMD_FLASH_Header4	4U

#define ACK			0x15
#define NOT_ACK		0x81

typedef struct
{
	u8 SOF;
	u8 REQ_No;
	u16 Data_Length;
}BL_Protocol_Header_t;


typedef struct
{
	BL_Protocol_Header_t header;
	u32 data[4];
	u8 eof;
}BL_Protocol_Start_t;


typedef struct
{
	BL_Protocol_Header_t header;
	u8 data[1024];
	u8 eof;
}BL_Protocol_Data_t;

typedef struct
{
	BL_Protocol_Header_t header;
	u8 ack;
	u8 eof;
}BL_Protocol_ACK_t;

u8 BL_Protocol_ConstructStartFrame(u32 key, u32 address, u32 entryPoint, u32 size, u8** buffer);
u16 BL_Protocol_ConstructDataFrame(u16 data_len, u8** buffer);
u16 BL_Protocol_ConstructACKFrame(u8 ack, u8 reqNo, u8* buffer);
u8 BL_Protocol_extractACK(u8 reqNo, u8* buffer);
u8 BL_Protocol_extractData(u8 req_No, u8 ** buffer, u16* dataLength);

u8 BL_Protocol_getReqNo(u8* buffer);

#endif

