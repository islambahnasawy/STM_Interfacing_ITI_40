#ifndef _LED_INTERFACE_H_
#define _LED_INTERFACE_H_

#define LED_NUM 3

#define LED1 0
#define LED2 1
#define LED3 2

#define LED_STATE_ON 	1
#define LED_STATE_OFF 	0

u8 LED_Init(u8 ledNum);

u8 LED_setLedState(u8 ledNum , u8 state);

#endif
