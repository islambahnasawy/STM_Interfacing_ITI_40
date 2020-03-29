#ifndef _SWITCH_INTERFACE_H_
#define _SWITCH_INTERFACE_H_

#define NUM_OF_SWITCH 3
#define SWITCH1 0
#define SWITCH2 1
#define SWITCH3 2

#define SWITCH_PRESSED 	1
#define SWITCH_RELESED 	0

void switch_init(void);
void Switch_updateValue(void);
u8 getSwitchState(u8 switchNum);

#endif
