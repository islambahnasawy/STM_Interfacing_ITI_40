#ifndef _SWITCH_CFG_H_
#define _SWITCH_CFG_H_




typedef enum
{
	SWITCH_UP,
	SWITCH_DOWN,
	SWITCH_LEFT,
	SWITCH_RIGHT,
	SWITCH_CFG,
	NUM_OF_SWITCH
}Switches;

typedef struct
{
	u32 GPIO_pinNum;
	u32 GPIO_pinPort;
	u32 state;
}switch_t;

extern const switch_t switchMap[];

#endif
