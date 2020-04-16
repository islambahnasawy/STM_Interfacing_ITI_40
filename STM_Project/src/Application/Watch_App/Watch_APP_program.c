#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/USART_Handler/USRTH_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "Watch_App_interface.h"
#include <stdio.h>
#include "../../Com_Protocols/A_Protocol.h"

static void update_Watch(void);//1000ms
static void data_Extract(void);//mesh m3ana
static void sendFrame(void);//50ms
static void receiveFrame(void);//25ms
static void config_Watch(void);//25ms
static void watch_App_Task(void);

const task_t Watch_App_task = {.handler=watch_App_Task,.preodicityMS=25};

#define CURSOR_COL_INIT_POS		(5)
#define CURSOR_ROW_INIT_POS		(0)
#define CURSOR_COL_END_POS		(12)
#define FIRST_DEL				(CURSOR_COL_INIT_POS+2)
#define SECOND_DEL				(CURSOR_COL_INIT_POS+5)

#define HOURS_TENS				(CURSOR_COL_INIT_POS)
#define HOURS_ONES				(CURSOR_COL_INIT_POS+1)

#define MINUTES_TENS			(CURSOR_COL_INIT_POS+3)
#define MINUTES_ONES			(CURSOR_COL_INIT_POS+4)

#define SECONDS_TENS			(CURSOR_COL_INIT_POS+6)
#define SECONDS_ONES			(CURSOR_COL_INIT_POS+7)

typedef struct
{
	u8 switches[5];
}Switches_t;

typedef struct
{
	s8 seconds;
	s8 minutes;
	s8 hours;

}Watch_t;

static u8 cfg_Mode;

static Watch_t watch={-1,0,0};
static u8 time [20];

static u8 TxDataBuffer[20];
static u8 RxDataBuffer[20];

static Switches_t RxSwitches;

static u8 hours_flag;
static u8 minutes_flag;
static u8 seconds_flag;

static void watch_App_Task(void)
{
	static u8 send_counter , updateWatch_counter;
	send_counter ++;
	updateWatch_counter++;
	if(updateWatch_counter == 40)
	{
		updateWatch_counter=0;
		update_Watch();
	}
	receiveFrame();
	config_Watch();
	if(send_counter == 2)
	{
		send_counter = 0;
		sendFrame();
	}

}

static void sendFrame(void)
{
	u8 *ptrTxDataBuffer = TxDataBuffer;
	u8 **pptrTxDataBuffer = &ptrTxDataBuffer;
	u8 frame_Length;
	frame_Length = AProtocol_ConstructFrame(SWITCHES_SIGNAL_ID,pptrTxDataBuffer);

	Switches_t * watch_Switches = (Switches_t *)ptrTxDataBuffer;


	watch_Switches->switches[SWITCH_UP] = getSwitchState(SWITCH_UP);
	watch_Switches->switches[SWITCH_DOWN] = getSwitchState(SWITCH_DOWN);
	watch_Switches->switches[SWITCH_LEFT] = getSwitchState(SWITCH_LEFT);
	watch_Switches->switches[SWITCH_RIGHT] = getSwitchState(SWITCH_RIGHT);
	watch_Switches->switches[SWITCH_CFG] = getSwitchState(SWITCH_CFG);

	USRTH_DMASend(USART1, TxDataBuffer, frame_Length, 0);

}



static void receiveFrame(void)
{

	u8 frame_Length = AProtocol_getFrameLength(SWITCHES_SIGNAL_ID);
	USRTH_DMARecieve(USART1, RxDataBuffer, frame_Length, data_Extract);

}


static void data_Extract(void)
{
	u8 *ptrRxDataBuffer = RxDataBuffer;
	u8 ** pptrRxDataBuffer = &ptrRxDataBuffer;
	u8 local_Status = OK;
	local_Status = AProtocol_extractData(SWITCHES_SIGNAL_ID, pptrRxDataBuffer);
	if (local_Status == OK)
	{
		Switches_t* watch_Switches = (Switches_t *)ptrRxDataBuffer;

		RxSwitches.switches[SWITCH_UP] = watch_Switches->switches[SWITCH_UP];
		RxSwitches.switches[SWITCH_DOWN] = watch_Switches->switches[SWITCH_DOWN];
		RxSwitches.switches[SWITCH_LEFT] = watch_Switches->switches[SWITCH_LEFT];
		RxSwitches.switches[SWITCH_RIGHT] = watch_Switches->switches[SWITCH_RIGHT];
		RxSwitches.switches[SWITCH_CFG] = watch_Switches->switches[SWITCH_CFG];
	}

}


static void update_Watch(void)
{
	/*Display*/

	if(cfg_Mode != 1 || seconds_flag != 1)
	{
		watch.seconds++;
	}
	if(watch.seconds == 60)
	{
		watch.seconds = 0;
		if(cfg_Mode != 1 || minutes_flag != 1)
		{
			watch.minutes++;
		}
		if(watch.minutes == 60)
		{
			watch.minutes = 0;
			if(cfg_Mode != 1 || hours_flag != 1)
			{
				watch.hours++;
			}
			if(watch.hours == 24)
			{
				watch.hours = 0;
			}
		}
	}
	if(cfg_Mode == 0)
	{
		LCD_OS_BlinkCursorOff();
		LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,CURSOR_COL_INIT_POS);
		sprintf(time,"%02u:%02u:%02u",watch.hours,watch.minutes,watch.seconds);
		LCD_OS_write(time);
		LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,CURSOR_COL_INIT_POS);
	}
}


static void config_Watch(void)
{
	static Watch_t local_watch  ;
	static u8 cursorIdx=CURSOR_COL_INIT_POS;
	static u8 cfg_sw_flag;
	static u8 left_sw_flag;
	static u8 right_sw_flag;
	static u8 up_sw_flag;
	static u8 down_sw_flag;
	if(RxSwitches.switches[SWITCH_CFG] == SWITCH_PRESSED)
	{
		if(cfg_sw_flag == 0)
		{
			cfg_sw_flag =1 ;
			if(cfg_Mode == 0)
			{
				cfg_Mode = 1;
				local_watch = watch;
				cursorIdx=CURSOR_COL_INIT_POS;

			}
			else
			{
				cfg_Mode = 0;
				hours_flag=0;
				minutes_flag=0;
				seconds_flag=0;
			}
		}
	}
	else
	{
		cfg_sw_flag =0 ;
	}

	if(cfg_Mode == 1)
	{

		LCD_OS_BlinkCursorOn();
		if(RxSwitches.switches[SWITCH_UP] == SWITCH_PRESSED)
		{
			if(up_sw_flag == 0)
			{
				up_sw_flag = 1;
			switch(cursorIdx)
			{
			case HOURS_TENS :
				local_watch.hours+=10;
				if(local_watch.hours > 23)local_watch.hours-=10;
				watch.hours=local_watch.hours;
				hours_flag=1;
				break;
			case HOURS_ONES:
				local_watch.hours+=1;
				if(local_watch.hours > 23)local_watch.hours-=1;
				watch.hours=local_watch.hours;
				hours_flag=1;
				break;
			case MINUTES_TENS:
				local_watch.minutes+=10;
				if(local_watch.minutes > 59)local_watch.minutes-=10;
				watch.minutes=local_watch.minutes;
				minutes_flag=1;
				break;
			case MINUTES_ONES:
				local_watch.minutes+=1;
				if(local_watch.minutes > 59)local_watch.minutes-=1;
				watch.minutes=local_watch.minutes;
				minutes_flag=1;
				break;
			case SECONDS_TENS:
				local_watch.seconds+=10;
				if(local_watch.seconds > 59)local_watch.seconds-=10;
				watch.seconds=local_watch.seconds;
				seconds_flag=1;
				break;
			case SECONDS_ONES:
				local_watch.seconds+=1;
				if(local_watch.seconds > 59)local_watch.seconds-=1;
				watch.seconds=local_watch.seconds;
				seconds_flag=1;
				break;
			}
			LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,CURSOR_COL_INIT_POS);
			sprintf(time,"%02u:%02u:%02u",local_watch.hours,local_watch.minutes,local_watch.seconds);
			LCD_OS_write(time);
			LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,cursorIdx);
			}
		}
		else
		{
			up_sw_flag = 0;
		}
		if(RxSwitches.switches[SWITCH_DOWN] == SWITCH_PRESSED)
		{
			if(down_sw_flag == 0)
			{
				down_sw_flag = 1;
				switch(cursorIdx)
				{
				case HOURS_TENS :
					local_watch.hours-=10;
					if(local_watch.hours < 0)local_watch.hours+=10;
					watch.hours=local_watch.hours;
					hours_flag=1;
					break;
				case HOURS_ONES:
					local_watch.hours-=1;
					if(local_watch.hours < 0)local_watch.hours+=1;
					watch.hours=local_watch.hours;
					hours_flag=1;
					break;
				case MINUTES_TENS:
					local_watch.minutes-=10;
					if(local_watch.minutes < 0)local_watch.minutes+=10;
					watch.minutes=local_watch.minutes;
					minutes_flag=1;
					break;
				case MINUTES_ONES:
					local_watch.minutes-=1;
					if(local_watch.minutes < 0)local_watch.minutes+=1;
					watch.minutes=local_watch.minutes;
					minutes_flag=1;
					break;
				case SECONDS_TENS:
					local_watch.seconds-=10;
					if(local_watch.seconds < 0)local_watch.seconds+=10;
					watch.seconds=local_watch.seconds;
					seconds_flag=1;
					break;
				case SECONDS_ONES:
					local_watch.seconds-=1;
					if(local_watch.seconds < 0)local_watch.seconds+=1;
					watch.seconds=local_watch.seconds;
					seconds_flag=1;
					break;
				}
				LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,CURSOR_COL_INIT_POS);
				sprintf(time,"%02u:%02u:%02u",local_watch.hours,local_watch.minutes,local_watch.seconds);
				LCD_OS_write(time);
				LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,cursorIdx);
			}
		}
		else
		{
			down_sw_flag = 0;
		}
		if(RxSwitches.switches[SWITCH_LEFT] == SWITCH_PRESSED)
		{
			if(left_sw_flag == 0)
			{
				left_sw_flag=1;
				cursorIdx--;
				if(cursorIdx < CURSOR_COL_INIT_POS) cursorIdx=CURSOR_COL_INIT_POS;
				if((cursorIdx == FIRST_DEL) ||(cursorIdx == SECOND_DEL))
				{
					cursorIdx--;
				}
				LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,cursorIdx);
			}
		}
		else
		{
			left_sw_flag =0;
		}
		if(RxSwitches.switches[SWITCH_RIGHT] == SWITCH_PRESSED)
		{
			if(right_sw_flag == 0)
			{
				right_sw_flag=1;
				cursorIdx++;
				if(cursorIdx > CURSOR_COL_END_POS) cursorIdx=CURSOR_COL_END_POS;
				if((cursorIdx == FIRST_DEL) ||(cursorIdx == SECOND_DEL))
				{
					cursorIdx++;
				}
				LCD_OS_move_cursor(CURSOR_ROW_INIT_POS,cursorIdx);
			}
		}
		else
		{
			right_sw_flag =0;
		}
	}
}





