#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "ReceivingUSART_APP.h"
#include "../../HAL/USART_Handler/USRTH_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include <stdio.h>

static void RxCallBack(void);
void ReceivingTask(void);

task_t RecTask = {.handler=ReceivingTask,.preodicityMS=40};
u8 Display_Flag;

void ReceivingTask(void)
{
	u32 RecValue;
	static u8 RxData[4];
	static u8 local_arr[11];
	if(!Display_Flag)
	{
		USRTH_u8ReceivingReq(USART1,RxData,4,RxCallBack);
	}
	else
	{
		/*Parsing the received frame */
		RecValue = RxData[0]+(RxData[1]<<8)+(RxData[2]<<16)+(RxData[3]<<24);
		LCD_OS_move_cursor(0,5);
		sprintf(local_arr,"%lu",RecValue);
		LCD_OS_write(local_arr);
		Display_Flag = 0;
	}

}

static void RxCallBack(void)
{
	Display_Flag = 1;
}
