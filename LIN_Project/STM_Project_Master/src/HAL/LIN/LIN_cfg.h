/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 5 May 2020									*/
/*Version : V01											*/
/********************************************************/
#ifndef MCAL_LIN_LIN_CFG_H_
#define MCAL_LIN_LIN_CFG_H_

#define MESSAGES_NUM	3
#define MINI_SLOT		5

#define INVALID_ID 		60
#define MAX_MESSAGE_NUM	60
#define SEND	12U
#define REC		17U


typedef struct
{
	u8 ID;
	u8 execTime_ms;
	u8 dataLength; /* 0 to 8 Byte*/
	u8 response; /* receive or send or ignore */
}LIN_cfg_t;


extern const LIN_cfg_t LIN_cfg_Arr[];
extern const u8 PID_map[];

#endif /* MCAL_LIN_LIN_CFG_H_ */
