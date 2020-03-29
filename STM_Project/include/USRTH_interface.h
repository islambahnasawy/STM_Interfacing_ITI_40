/********************************************************/
/*Author : Mohamed Megahed                              */
/*Date   : 14 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _URTM_INTERFACE_H_
#define _URTM_INTERFACE_H_



typedef void (*Handler)(void);

void USRTH_voidInit(void);
u8 USRTH_u8SendingReq(u8* Copy_Data , u32 Copy_Length , Handler handler);
u8 USRTH_u8ReceivingReq(u8* Copy_Data , u32 Copy_Length , Handler handler);

#endif
