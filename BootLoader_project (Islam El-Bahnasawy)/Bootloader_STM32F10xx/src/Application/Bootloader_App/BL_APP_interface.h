#ifndef _WATCH_APP_H_
#define _WATCH_APP_H_




typedef struct
{
	u32 key;
	u32 address;
	u32 entryPoint;
	u32 size;
}BL_Frame_info_t;



void sendFrame_Runnable(void);
void receiveStartFrame(void);
void receiveDataFrame();


#endif
