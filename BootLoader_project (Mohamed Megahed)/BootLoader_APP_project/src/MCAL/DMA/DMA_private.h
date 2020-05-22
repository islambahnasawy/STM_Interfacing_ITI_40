/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 1 APRIL 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef _DMA_PRIVATE_H_
#define _DMA_PRIVATE_H_

#include "../../LIB/STD_TYPES.h"
/************************Channel1 default Config********************************/

#ifndef DMA_CHANNEL1_PRIOIRITY
#define DMA_CHANNEL1_PRIOIRITY					DMA_LOW_PL
#endif 
#ifndef DMA_CHANNEL1_ERROR_INT
#define DMA_CHANNEL1_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL1_HALF_TRANS_COMP_INT
#define DMA_CHANNEL1_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif
/************************Channel2 default Config********************************/

#ifndef DMA_CHANNEL2_PRIOIRITY
#define DMA_CHANNEL2_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef DMA_CHANNEL2_ERROR_INT
#define DMA_CHANNEL2_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL2_HALF_TRANS_COMP_INT
#define DMA_CHANNEL2_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif
/************************Channel3 default Config********************************/

#ifndef DMA_CHANNEL3_PRIOIRITY
#define DMA_CHANNEL3_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef	DMA_CHANNEL3_ERROR_INT
#define DMA_CHANNEL3_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef	DMA_CHANNEL3_HALF_TRANS_COMP_INT
#define DMA_CHANNEL3_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif

/************************Channel4 default Config********************************/

#ifndef DMA_CHANNEL4_PRIOIRITY
#define DMA_CHANNEL4_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef DMA_CHANNEL4_ERROR_INT
#define DMA_CHANNEL4_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL4_HALF_TRANS_COMP_INT
#define DMA_CHANNEL4_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif 

/************************Channel5 default Config********************************/

#ifndef DMA_CHANNEL5_PRIOIRITY
#define DMA_CHANNEL5_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef DMA_CHANNEL5_ERROR_INT
#define DMA_CHANNEL5_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL5_HALF_TRANS_COMP_INT
#define DMA_CHANNEL5_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif

/************************Channel6 default Config********************************/

#ifndef DMA_CHANNEL6_PRIOIRITY
#define DMA_CHANNEL6_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef DMA_CHANNEL6_ERROR_INT
#define DMA_CHANNEL6_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL6_HALF_TRANS_COMP_INT
#define DMA_CHANNEL6_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif

/************************Channel7 default Config********************************/

#ifndef DMA_CHANNEL7_PRIOIRITY
#define DMA_CHANNEL7_PRIOIRITY					DMA_LOW_PL
#endif
#ifndef DMA_CHANNEL7_ERROR_INT
#define DMA_CHANNEL7_ERROR_INT					DMA_ERROR_INT_DISABLE
#endif
#ifndef DMA_CHANNEL7_HALF_TRANS_COMP_INT
#define DMA_CHANNEL7_HALF_TRANS_COMP_INT		DMA_HALF_TRANS_COMP_INT_DISABLE
#endif 

/*Clear Masks*/
#define INIT_BITS_CLEAR_MASK	0xffffcff1
#define PRIORITY_CLEAR_MASK		0xffffcfff
#define INTERRUPTS_CLEAR_MASK	0xfffffff3
#define DMA_START_CLEAR_MASK	0xffffB00E

#define DMA_BUSY	0x22
#define DMA_IDLE	0x0

#define TRUE		0x23
#define FALSE		0x0


typedef struct
{
	CHN_Handler Channel_TCHandler[7];
	CHN_Handler Channel_HTCHandler[7];
	CHN_Handler Channel_ERRHandler[7];
}CB_t;


static u8 get_TCFlagStatus(u8 Channel);
static u8 get_HTCFlagStatus(u8 Channel);
static u8 get_ERRFlagStatus(u8 Channel);
static void clear_TCFlag(u8 Channel);
static void clear_HTCFlag(u8 Channel);
static void clear_ERRFlag(u8 Channel);



#endif
