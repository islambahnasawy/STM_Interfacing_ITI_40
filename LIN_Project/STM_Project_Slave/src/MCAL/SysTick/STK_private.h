/****************************************************/
/* Author : ANWAR									*/
/* Date : 8 MARCH 2020								*/
/* Version : V01									*/
/****************************************************/

#ifndef STK_PRIVATE_H_
#define STK_PRIVATE_H_

/*Default configuration*/
#ifndef CLOCKSOURCE
#define CLOCKSOURCE STK_SOURCE_AHB_8
#endif

#ifndef INTERRUPT_STATE 
#define INTERRUPT_STATE	STK_INT_ENABLE
#endif

#define STK_INIT_MASK	0xfffffff9
#define STK_ENABLE		0x10000001
#define STK_MASK 		0x10000000


#endif
