#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/USART_Handler/USRTH_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "App1_interface.h"
#include <stdio.h>

static void RxCallBack(void);
static void Sending(void);
void Receiving(void);
static void updatingCounter(void);
static void APP1_runnable(void);

const task_t App1_task = {.handler=APP1_runnable,.preodicityMS=30};

u32 TransValue;
u8 Display_Flag;

/*************************Receving function*********************************/
void Receiving(void)
{
	u32 RecValue;
	static u8 RxData[4];
	static u8 local_arr[11];
	if(!Display_Flag)
	{
		//USRTH_u8ReceivingReq(USART1,RxData,4,RxCallBack);
		USRTH_DMARecieve(USART1,RxData,4,RxCallBack);

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
/*************************Sending function*********************************/

static void Sending(void)
{
	static u8 TxData[4];
	/*Forming the frame that will be transmitted*/
	TxData[0] = TransValue ;
	TxData[1] = TransValue>>8 ;
	TxData[2] = TransValue>>16 ;
	TxData[3] = TransValue>>24 ;

	/*transmitting the frame*/
	//USRTH_u8SendingReq(USART1,TxData,4,0);
	USRTH_DMASend(USART1,TxData,4,0);
}

/*************************Switch Increment function*********************************/

static void updatingCounter(void)
{

	static u8 sw_flag=0;

	if(getSwitchState(SWITCH1)==SWITCH_PRESSED )
	{
		/*Incerement once for long press*/
		if(sw_flag == 0)
		{
			sw_flag =1 ;
			TransValue ++;
		}
	}
	else
	{
		sw_flag = 0 ;
	}
}


/****************************APP 1 Runnable******************************************/

static void APP1_runnable(void)
{
	static u8 counter;
	updatingCounter();
	Receiving();
	if(counter == 2)
	{
		counter=0;
		Sending();
	}
	counter++;
}


