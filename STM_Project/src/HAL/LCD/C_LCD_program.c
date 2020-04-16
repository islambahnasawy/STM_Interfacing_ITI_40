/********************************************************/
/*Author : Anwar				 						*/
/*Version : V01					 						*/
/*Date : 9 MARCH 2020								    */
/********************************************************/

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../RTOS/RTOS_interface.h"
#include "C_LCD_interface.h"
#include "C_LCD_private.h"
#include "C_LCD_config.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../LIB/Queue/queue.h"


const task_t LCD_Task = {.handler=LCD_OS_runnable,.preodicityMS=5};

static const handler  handlers[8]=
{
		FSM_task1,FSM_task2,FSM_task3,FSM_task2,FSM_task4,FSM_task2,FSM_task5,FSM_task2
};
static const handler LCD_handlers[5]=
{
		&LCD_apply,&LCD_wipe,&LCD_cursor_moving,&LCD_cursor_blinkingOn,&LCD_cursor_blinkingOff
};

static queue Queue;
static taskInfo_t taskInfo[MAX_QUEUED_TASKS];
static taskInfo_t CurrentTask;
static u8 FSM_Complete ;
static u8 steps = DONE;
static u8 LCD_index;
void FSM_init(void)
{
	static u8 index = 0;
	(handlers[index])();
	index ++;
	if(index == 8) FSM_Complete = 1 ;
}


void CLCD_init(void)
{
	DIO_SetPinMode(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_E_PORT,CLCD_u8_E_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);

	DIO_SetPinMode(CLCD_u8_D0_PORT,CLCD_u8_D0_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D1_PORT,CLCD_u8_D1_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D2_PORT,CLCD_u8_D2_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D3_PORT,CLCD_u8_D3_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D4_PORT,CLCD_u8_D4_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D5_PORT,CLCD_u8_D5_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D6_PORT,CLCD_u8_D6_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D7_PORT,CLCD_u8_D7_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);

	QueueInit(&Queue , MAX_QUEUED_TASKS ,taskInfo);

}

void LCD_OS_runnable(void)
{
	taskInfo_t temptask;
	if(FSM_Complete)
	{
		if(!QueueIsEmpty(&Queue)|| steps!=DONE)
		{
			if(steps == DONE)
			{
				steps = FIRST_STEP;
				Dequeue(&Queue,&temptask);
				CurrentTask.taskID = temptask.taskID;
				CurrentTask.task_length = temptask.task_length;
				CurrentTask.task_string = temptask.task_string;
				CurrentTask.task_row = temptask.task_row;
				CurrentTask.task_col = temptask.task_col;
				LCD_index = temptask.task_length;
			}

			if(CurrentTask.task_length != 0 || CurrentTask.taskID != LCD_WRITE)
			{
				(LCD_handlers[CurrentTask.taskID])();
			}
		}
	}
	else
	{
		trace_printf("LCD init");
		FSM_init();
	}
}
void LCD_OS_clear(void)
{

	/*creating an object of type taskInfo_t*/
	taskInfo_t temptask;
	/*Filling that object with the suitable data*/
	temptask.taskID = LCD_CLEAR;
	/*Enlisting that object in the queue*/
	Enqueue(&Queue,temptask);
}

void LCD_OS_BlinkCursorOn(void)
{
	taskInfo_t temptask;
	temptask.taskID = LCD_CURSOR_BLINK_ON;
	Enqueue(&Queue,temptask);
}

void LCD_OS_BlinkCursorOff(void)
{
	taskInfo_t temptask;
	temptask.taskID = LCD_CURSOR_BLINK_OFF;
	Enqueue(&Queue,temptask);
}
void LCD_OS_move_cursor(u8 row , u8 col)
{
	taskInfo_t temptask;
	temptask.taskID = LCD_MOVE_CURSOR;
	temptask.task_col = col;
	temptask.task_row = row;
	Enqueue(&Queue,temptask);
}
static void LCD_cursor_moving(void)
{
	if(steps == FIRST_STEP)
	{
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		if(CurrentTask.task_row==0){
			if(CurrentTask.task_col<16)
			{
				CLCD_voidSetDataPort(0x80+CurrentTask.task_col);
			}
		}
		else if (CurrentTask.task_row==1){
			if(CurrentTask.task_col<16)
			{
				CLCD_voidSetDataPort(0xC0+CurrentTask.task_col);
			}
		}
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		steps=SECOND_STEP;
	}
	else
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		steps=DONE;
	}
}

static void LCD_cursor_blinkingOn(void)
{

	 if (steps==FIRST_STEP)
	{
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		CLCD_voidSetDataPort(0b00001111);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		steps = SECOND_STEP;
	}
	else
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		steps=DONE;
	}
}

static void LCD_cursor_blinkingOff(void)
{

	 if (steps==FIRST_STEP)
	{
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		CLCD_voidSetDataPort(0b00001100);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		steps = SECOND_STEP;
	}
	else
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		steps=DONE;
	}
}


static void LCD_wipe(void)
{
	if(steps == FIRST_STEP)
	{
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		CLCD_voidSetDataPort(0b00000001);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		steps=SECOND_STEP;
	}
	else
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		steps=DONE;
	}

}
static void LCD_apply(void)
{
	if(steps == FIRST_STEP)
	{
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,1);
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		CLCD_voidSetDataPort(CurrentTask.task_string[(LCD_index-CurrentTask.task_length)]);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		steps = SECOND_STEP;
	}
	else
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		steps = FIRST_STEP;
		CurrentTask.task_length--;
		if(CurrentTask.task_length == 0)
		{
			steps = DONE;
			//notify that the first word is sent
		}
	}
}

void LCD_OS_write(u8* const word)
{
	taskInfo_t temptask;
	temptask.taskID = LCD_WRITE;
	temptask.task_string = word;
	temptask.task_length = 0;
	while(temptask.task_string[temptask.task_length]!= 0 && temptask.task_length<16)
	{
		temptask.task_length++;
	}
	Enqueue(&Queue,temptask);
}
static void FSM_task1(void)
{
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	CLCD_voidSetDataPort(0b00111000);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
}

static void FSM_task2(void)
{
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
}

static void FSM_task3(void)
{
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	CLCD_voidSetDataPort(0b00001100);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
}

static void FSM_task4(void)
{
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	CLCD_voidSetDataPort(0b00000001);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
}

static void FSM_task5(void)
{
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	CLCD_voidSetDataPort(0b00000110);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
}


static void CLCD_voidSetDataPort(u8 Copy_u8Data)
{
	u8 Local_u8BitVal;

	Local_u8BitVal = GET_BIT(Copy_u8Data,0);
	DIO_SetPinVal(CLCD_u8_D0_PORT,CLCD_u8_D0_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,1);
	DIO_SetPinVal(CLCD_u8_D1_PORT,CLCD_u8_D1_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,2);
	DIO_SetPinVal(CLCD_u8_D2_PORT,CLCD_u8_D2_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,3);
	DIO_SetPinVal(CLCD_u8_D3_PORT,CLCD_u8_D3_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,4);
	DIO_SetPinVal(CLCD_u8_D4_PORT,CLCD_u8_D4_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,5);
	DIO_SetPinVal(CLCD_u8_D5_PORT,CLCD_u8_D5_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,6);
	DIO_SetPinVal(CLCD_u8_D6_PORT,CLCD_u8_D6_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,7);
	DIO_SetPinVal(CLCD_u8_D7_PORT,CLCD_u8_D7_PIN,Local_u8BitVal);
}




void CLCD_voidIntialzie(void)
{
	delay_mSec(40);
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	/*first 4 bits */
	CLCD_voidsethalfDataPort(0b0010);
	CLCD_Kick();
	/*second 4 bits */
	CLCD_voidsethalfDataPort(0b0010);
	CLCD_Kick();
	/*third 4 bits */
	CLCD_voidsethalfDataPort(0b1000);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE

#else #error "foo2 ya beh"
#endif
	//set two lines command
	CLCD_voidWriteCmd(0b00111000);
	//_delay_ms(4);
	//dispay on command
	CLCD_voidWriteCmd(0b00001100);

	/*Clears the screen*/
	CLCD_voidWriteCmd(0b00000001);
	delay_mSec(4);
	/*Setting entry mode*/
	CLCD_voidWriteCmd(0b00000110);
}

/*Description : This API shall display data on LCD 		*/
void CLCD_voidWriteData(u8 Copy_u8Data)
{
	/*Set RS = 1*/
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,1);

	/*Set RW = 0*/
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);

	/*Set Data on Data port*/
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	CLCD_voidsethalfDataPort(Copy_u8Data>>4);
	CLCD_Kick();
	CLCD_voidsethalfDataPort(Copy_u8Data);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE
	/*Set Data on Data port*/
	CLCD_voidSetDataPort(Copy_u8Data);
	CLCD_Kick();
#else #error "foo2 ya zboon"
#endif
}

/*Description : This API shall execute command on LCD 	*/
void CLCD_voidWriteCmd(u8 Copy_u8Cmd)
{
	/*Set RS = 0*/
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

	/*Set RW = 0*/
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	CLCD_voidsethalfDataPort(Copy_u8Cmd>>4);
	CLCD_Kick();
	CLCD_voidsethalfDataPort(Copy_u8Cmd);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE
	/*Set Data on Data port*/
	CLCD_voidSetDataPort(Copy_u8Cmd);
	CLCD_Kick();
#else #error "foo2 ya zboon"
#endif
}
static void CLCD_voidsethalfDataPort(u8 Copy_u8Data)
{
	u8 Local_u8BitVal;

	Local_u8BitVal = GET_BIT(Copy_u8Data,0);
	DIO_SetPinVal(CLCD_u8_D0_PORT,CLCD_u8_D4_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,1);
	DIO_SetPinVal(CLCD_u8_D1_PORT,CLCD_u8_D5_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,2);
	DIO_SetPinVal(CLCD_u8_D2_PORT,CLCD_u8_D6_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,3);
	DIO_SetPinVal(CLCD_u8_D3_PORT,CLCD_u8_D7_PIN,Local_u8BitVal);
}

static void CLCD_Kick()
{
	/*Enable pulse*/
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
	delay_mSec(2);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
	delay_mSec(2);
}

void lcd_move_cursor(u8 row,u8 colomn){
	if(row==0){
		if(colomn<16)
			CLCD_voidWriteCmd(0x80+colomn);
	}
	else if (row==1){
		if(colomn<16)
			CLCD_voidWriteCmd(0xC0+colomn);
	}
}


void CLCD_voidClearScreen(void)
{
	CLCD_voidWriteCmd(0b00000001);
	delay_mSec(4);
}

void CLCD_VoidGoToXYPos(u8 Copy_u8XPos,u8 Copy_u8YPos)
{
	u8 local_u8DDRamAdd;
	if(Copy_u8YPos == CLCD_u8_LINE_ONE)
	{
		local_u8DDRamAdd=Copy_u8XPos;
	}
	else if(Copy_u8YPos == CLCD_u8_LINE_ONE)
	{
		local_u8DDRamAdd = Copy_u8XPos + CLCD_u8_LINE_TWO_ADD_OFFSET;
	}
	else
	{
		/*Do Nothing*/
	}
	CLCD_voidWriteCmd(local_u8DDRamAdd | CLCD_u8_SET_DDRAM_ADD_COMM);

}

void CLCD_voidWriteString(const char* Copy_pchString)
{
	while(*(Copy_pchString)!=0)
	{
		CLCD_voidWriteData(*(Copy_pchString++));
	}
}


void CLCD_voidCGRAM(void)
{
	CLCD_voidWriteCmd(0b01000000);
}

void CLCD_voidDDRAM(void)
{
	CLCD_voidWriteCmd(0b10000000);
}
