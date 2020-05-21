/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef _PCCOM_H_
#define _PCCOM_H_

u8 serial_Init(void);
u8 serial_Send(u8* buffer , u16 length);
u8 serial_Receive(u8*buffer ,u16 length);

#endif