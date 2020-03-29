#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_interface.h"
#include "STK_interface.h"
#include "SCHED_interface.h"
#include "SCHED_config.h"
#include "USRTH_interface.h"
#include "USART_interface.h"
#include "LED_interface.h"
#include "LCD_interface.h"
#include "Seven_Segment_interface.h"
#include "Switch_interface.h"
#include "DELAY_interface.h"


void SWITCH_Runnable(void);
void UART_CTR(void);
void Led_CTR(void);
void func_led1(void);
void func_led2(void);
void led_off(void);

const task_t Tasks[TASKS_NUM] =
{{.Runnable = &LCD_voidRunnable, .peridicity_ms = 100},
		{.Runnable = &SWITCH_Runnable, .peridicity_ms = 5},
		{.Runnable = &UART_CTR, .peridicity_ms = 10}
};

u8 arr[6] = {'a','n','w','a','r','\n'};
u8 arr2[4] = {'a','n','w','a'};
u8 arr3[5] = {'a','n','w','a','r'};
u8 switch_State;
void main(void)
{

	RCC_u8ChangeClockStatus(HSE_CLK,CLK_STATUS_ON);
	RCC_u8SwitchSystemClock(HSE);
	//trace_printf("%d", state);
	RCC_Peripheral_Config(IOPA ,PERIPHERAL_ON);
	RCC_Peripheral_Config(IOPC ,PERIPHERAL_ON);

	DIO_SetPinMode('C', 13, 1);
	DIO_SetPinMode('A', 1, 1);
	GPIO_t gpio = {GPIO_PIN_0,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ,GPIO_PORTA};
	GPIO_Config(&gpio);
	USRTH_voidInit();
	switch_init();
	SCHED_voidInit();

	SCHED_Start();

	//LED_Init();

	//u8 arr[6] = {'a','n','w','a','r','\n'};
	//USART_u8SendingDataASync(arr , 6 , led_off);
	//USART_u8RecDataAsync(arr, 6, led_off);
	//USART_u8RecDataAsync(arr, 6, led_off);

	Switch_updateValue();
	switch_State = getSwitchState(SWITCH1);

	/*USRTH_u8ReceivingReq(arr, 6, led_off);
	USRTH_u8SendingReq(arr, 6, led_off);
	USRTH_u8SendingReq(arr2, 4, led_off);
	USRTH_u8SendingReq(arr3, 5, led_off);*/
	while(1)
	{
		//USART_u8SendingDataASync(arr , 6 , led_off);
		switch_State = getSwitchState(SWITCH1);
		if (switch_State)
		{
			//LED_setLedState(LED2,LED_STATE_ON);
			DIO_SetPinVal('A',0,1);
		}
		else
		{
			//LED_setLedState(LED2,LED_STATE_OFF);
			DIO_SetPinVal('A',0,0);
		}

	}

}



void SWITCH_Runnable(void)
{
	Switch_updateValue();
}
void led_off(void)
{
	DIO_SetPinVal('C',13,1);
	//USART_u8SendingDataASync(arr , 6 , 0);
}

void func_led1(void)
{
	static u8 flag1 = 0;
	if(flag1 == 0)
	{
		DIO_SetPinVal('C',13,0);
		flag1 =1;
	}
	else if (flag1 == 1)
	{
		DIO_SetPinVal('C',13,1);
		flag1 =0;
	}
}

void func_led2(void)
{
	static u8 flag2 = 0;
	if(flag2 == 0)
	{
		DIO_SetPinVal('A',0,0);
		flag2 =1;
	}
	else if (flag2 == 1)
	{
		DIO_SetPinVal('A',0,1);
		flag2 =0;
	}
}

void Led_CTR(void)
{
	u8 switch_State;
	static u8 flag3=0;
	switch_State = getSwitchState(SWITCH1);
	trace_printf("%d", switch_State);
	//switch_state = DIO_GetPinVal(SWITCH_1_PORT, SWITCH_1_PIN);
	/*if(flag3 == 0)
	{
		DIO_SetPinVal('A',1,0);
		LCD_MOVE_CRS(1,7);
		flag3 =1;
	}
	else if (flag3 == 1)
	{
		DIO_SetPinVal('A',1,1);

		flag3 =0;
	}*/
	if(switch_State == 1)
	{
		DIO_SetPinVal('A',0,1);
		func_led1();
		trace_printf("%s", "LED_ON ");
	}
	else
	{
		DIO_SetPinVal('A',0,0);
	}
	//DIO_SetPinVal('A',0,1);

}

void UART_CTR(void)
{
	u8 switch_State;
	u8 number;
	static u8 counter[4]={48,48,48,48};
	switch_State = getSwitchState(SWITCH1);
	trace_printf("%d", switch_State);
	//switch_state = DIO_GetPinVal(SWITCH_1_PORT, SWITCH_1_PIN);

	if(switch_State == 1)
	{

		counter[3]++;
		if(counter[3] == (9+48))
		{
			counter[3] = 48;
			counter[2]++;
			if(counter[2] == (9+48))
			{
				counter[2] = 48;
				counter[1]++;
				if(counter[1] == (9+48))
				{
					counter[1] = 48;
					counter[0]++;
					if(counter[0] == (9+48))
					{
						counter[0] = 48;
						counter[1] = 48;
						counter[2] = 48;
						counter[3] = 48;

					}
				}
			}
		}
		//number= counter+48;
		//USRTH_u8SendingReq(arr, 6, 0);
		USRTH_u8SendingReq(counter, 4, 0);
		DIO_SetPinVal('A',0,1);
		trace_printf("%s", "LED_ON ");
	}
	else
	{
		DIO_SetPinVal('A',0,0);
	}
	//DIO_SetPinVal('A',0,1);

}


