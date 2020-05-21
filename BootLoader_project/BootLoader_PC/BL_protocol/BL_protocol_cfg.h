/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_CFG_H_
#define COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_CFG_H_

#define FRAME_TYPE_NUM		5

#define FRAME_OVERHEAD		5
typedef struct
{
	u8 sof;
	u8 ID;
	u8 eof;
	u8 frame_length;
}BL_protocol_cfg;


extern const BL_protocol_cfg protocol_cfg[];

#endif /* COM_PROTOCOLS_BL_PROTOCOL_BL_PROTOCOL_CFG_H_ */
