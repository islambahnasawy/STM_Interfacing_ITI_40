/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 1 APRIL 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef _DMA_REG_H_
#define _DMA_REG_H_

#define DMA1_NUM_OF_CHANNELS 7

typedef struct
{
	u32 CCR;
	u32 CNDTR;
	u32 CPAR;
	u32 CMAR;
	u32 Reserved;
}DMA_Channel;


typedef struct 
{
	u32 ISR;
	u32 IFCR ;
	DMA_Channel Channel[DMA1_NUM_OF_CHANNELS];
}DMA_Type;

#define DMA_BASEADDRESS	0x40020000
#define DMA 			((volatile DMA_Type*)(DMA_BASEADDRESS))

#endif