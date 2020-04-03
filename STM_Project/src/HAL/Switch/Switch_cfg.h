#ifndef _SWITCH_CFG_H_
#define _SWITCH_CFG_H_

#define NUM_OF_SWITCH 5
#define SWITCH1 0
#define SWITCH2 1
#define SWITCH3 2
#define SWITCH4	3
#define SIWTCH5	4

typedef struct
{
	u32 GPIO_pinNum;
	u32 GPIO_pinPort;
	u32 state;
}switch_t;

extern const switch_t switchMap[];

#endif
