#include "../STD_TYPES.h"

#include "DELAY_interface.h"

#define T1 (0.00539875f)
#define TM (0.634125f)

/* for loop time          t1 = 5.39875 us */
/* calculation time       t2 = 627.875 us */
/* context switching into t3 = 3.995 us */
/* context switching out  t4 = 2.255 us */


void delay_mSec(u32 m_Sec)
{
	u32 i;
	u32 count = (u32)((float)m_Sec-TM)/(T1);

	for(i=0 ; i<count ; i++)
	{
		asm("NOP");
	}

}



/*
 * context switching --->(function calling)<----
 * t3 = context switching 1 --->
 * t4 = context switching 1 <---
 * t2 = calculation time
 * Loop iteration = t1
 * count = (value-t2-t3-t4)/t1
 *
 *TM = t2+t3+t4
 * */
