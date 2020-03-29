#include "STD_TYPES.h"
#include "USART_MYTYPES.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "STK_interface.h"
#include "C_LCD_interface.h"
#include "RTOS_interface.h"
#include "LED_interface.h"
#include "Switch_interface.h"
#include "USRTH_interface.h"
#include <stdio.h>
u8 RxData[4];
u8 TxData[4];


u8 Display_Flag;
u8 count_flag;
void ReceivingTask(void);
void SendingTask(void);
void RxCallBack(void);


task_t tasks[4] =
{
		{.handler=ReceivingTask,.preodicityMS=30},
		{.handler=SendingTask,.preodicityMS=30},
		{.handler=LCD_OS_runnable,.preodicityMS=5},
		{.handler=Switch_updateValue,.preodicityMS=5}
		//{.handler=led3,.preodicityMS=30}
};

int
main(int argc, char* argv[])
{
	RCC_Peripheral_Config(RCC_IOPC,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPB,PERIPHERAL_ON);
	RCC_u8SwitchSystemClock(HSE);
	USRTH_voidInit(USART1);
	switch_init();
	CLCD_init();
	RTOS_init();
	RTOS_start();
	while(1);

}

void ReceivingTask(void)
{
	u32 RecValue;
	static u8 local_arr[11];
	if(!Display_Flag)
	{
		USRTH_u8ReceivingReq(USART1,RxData,4,RxCallBack);
	}
	else
	{

		RecValue = RxData[0]+(RxData[1]<<8)+(RxData[2]<<16)+(RxData[3]<<24);
		LCD_OS_move_cursor(0,5);
		sprintf(local_arr,"%lu",RecValue);
		LCD_OS_write(local_arr);
		Display_Flag = 0;
	}

}


void SendingTask(void)
{
	static u32 TransValue;
	static u8 sw_flag=0;
	if(getSwitchState(SWITCH1)==SWITCH_PRESSED )
	{
		if(sw_flag == 0)
		{

			sw_flag =1 ;
			TransValue ++;
			TxData[0] = TransValue ;
			TxData[1] = TransValue>>8 ;
			TxData[2] = TransValue>>16 ;
			TxData[3] = TransValue>>24 ;

			USRTH_u8SendingReq(USART1,TxData,4,0);
		}
	}
	else
	{
		sw_flag = 0 ;
	}
}


void RxCallBack(void)
{
	Display_Flag = 1;
}

