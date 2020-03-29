#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "LCD_interface.h"
#include "LCD_config.h"
#include "DIO_interface.h"

#define FSM_STEPS 6
#define STRING_SIZE	17
typedef void(*FSM_step)(void);

u8 Data;
u8 Data_string [STRING_SIZE];
u8 init_complete = 0;
u8 write_req = 0;
u8 xPos;
u8 yPos;
u8 Currnet_Runnable = IDLE_RUNNABEL;

static void LCD_apply_step1(u8 Copy_u8Data)
{
	/* Set RS = 1 */
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,1);

	/* Set RW = 0 */
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	CLCD_voidSetDataPort(Copy_u8Data);

	//CLCD_voidSendPulse();
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);

}

static void LCD_apply_step2(void)
{
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
}

void CLCD_voidSetDataPort(u8 Copy_u8Data)
{
	u8 Local_u8BitVal;

	/* Set Data on Data port */
	Local_u8BitVal = GET_BIT(Copy_u8Data, 0);
	DIO_SetPinVal(CLCD_u8_D0_PORT,CLCD_u8_D0,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 1);
	DIO_SetPinVal(CLCD_u8_D1_PORT,CLCD_u8_D1,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 2);
	DIO_SetPinVal(CLCD_u8_D2_PORT,CLCD_u8_D2,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 3);
	DIO_SetPinVal(CLCD_u8_D3_PORT,CLCD_u8_D3,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 4);
	DIO_SetPinVal(CLCD_u8_D4_PORT,CLCD_u8_D4,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 5);
	DIO_SetPinVal(CLCD_u8_D5_PORT,CLCD_u8_D5,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 6);
	DIO_SetPinVal(CLCD_u8_D6_PORT,CLCD_u8_D6,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data, 7);
	DIO_SetPinVal(CLCD_u8_D7_PORT,CLCD_u8_D7,Local_u8BitVal);
	/* Enable Pulse */


}

static void LCD_voidinitFSM_step1(void)
{
	//CLCD_voidWriteCmd(CLCD_u8_FUNCTION_SET_8BITS_2LINES);
	/* Set RS = 0 */
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

	/* Set RW = 0 */
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	/* Set Data on Data port */
	CLCD_voidSetDataPort(CLCD_u8_FUNCTION_SET_8BITS_2LINES);
	//CLCD_voidSendPulse();
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);

}

static void LCD_voidinitFSM_step2(void)
{
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
}

static void LCD_voidinitFSM_step3(void)
{
	//CLCD_voidWriteCmd(CLCD_u8_DISPLAY_ON_CONTROL);
	/* Set RS = 0 */
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

	/* Set RW = 0 */
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	/* Set Data on Data port */
	CLCD_voidSetDataPort(CLCD_u8_DISPLAY_ON_CONTROL);
	//CLCD_voidSendPulse();
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);

}

static void LCD_voidinitFSM_step4(void)
{
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
}

static void LCD_voidinitFSM_step5(void)
{
	//CLCD_voidClearScr();

	/* Set RS = 0 */
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

	/* Set RW = 0 */
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
	/* Set Data on Data port */
	CLCD_voidSetDataPort(0b00000001);
	//CLCD_voidSendPulse();
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);

}

static void LCD_voidinitFSM_step6(void)
{
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
}

static FSM_step steps[FSM_STEPS]= 
{LCD_voidinitFSM_step1, 
		LCD_voidinitFSM_step2,
		LCD_voidinitFSM_step3,
		LCD_voidinitFSM_step4,
		LCD_voidinitFSM_step5,
		LCD_voidinitFSM_step6};

static void LCD_voidinitFSM_step(void)
{
	static index=0;
	if(index<FSM_STEPS)
	{
		steps[index]();
		index++;
	}
	else
	{
		init_complete = 1;
	}

}

void LCD_voidRunnable(void)
{
	u8 enable;
	enable = DIO_GetPinVal(CLCD_u8_E_PORT, CLCD_u8_E_PIN);
	if(init_complete)
	{
		//m7tageen nzbt el enable !!
		if(Currnet_Runnable == WRITE_LCD_RUNNABLE)
		{
			//enable=0;
			//write_req=0;
			LCD_apply_string(Data_string);
		}
		else if(Currnet_Runnable == CLR_LCD_RUNNABLE)
		{
			LCD_apply_CLR();
		}
		else if (Currnet_Runnable == MOVE_LCD_RUNNABLE)
		{
			LCD_apply_Move_CRS(xPos, yPos);
		}

	}


	else
	{
		LCD_voidinitFSM_step();
	}


}

/*
void LCD_voidRunnable(void)
{
	u8 enable;
	enable = DIO_GetPinVal(CLCD_u8_E_PORT, CLCD_u8_E_PIN);
	if(init_complete)
	{
		if(write_req)
		{
			//m7tageen nzbt el enable !!
			if(Currnet_Runnable == WRITE_LCD_RUNNABLE)
			{
				//enable=0;
				//write_req=0;
				LCD_apply(Data);
			}
			else
			{

			}

		}
	}

	else
	{
		LCD_voidinitFSM_step();
	}


}*/

Status LCD_write_string(u8* copy_u8Char)
{
	Status status = NOT_OK;

	if(Currnet_Runnable == IDLE_RUNNABEL)
	{
		//write_req = 1;
		Currnet_Runnable = WRITE_LCD_RUNNABLE;
		u8 index=0;
		while(copy_u8Char[index] != '\0' && index < (STRING_SIZE-1))
		{
			Data_string[index] = copy_u8Char[index];
			index++;
		}
		Data_string[index] = '\0';

		status = OK;
	}
	return status;
}

Status LCD_write(u8 copy_u8Char)
{
	Status status = NOT_OK;

	if(Currnet_Runnable == IDLE_RUNNABEL)
	{
		//write_req = 1;
		Currnet_Runnable = WRITE_LCD_RUNNABLE;
		Data = copy_u8Char;
		status = OK;
	}
	return status;

}
Status LCD_CLR(void)
{
	Status status = NOT_OK;

	if(Currnet_Runnable == IDLE_RUNNABEL)
	{
		//write_req = 1;
		Currnet_Runnable = CLR_LCD_RUNNABLE;
		status = OK;
	}
	return status;
}

void LCD_apply_CLR(void)
{
	static u8 apply_flag = 0;
	if(apply_flag == 0)
	{
		/* Set RS = 0 */
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

		/* Set RW = 0 */
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		/* Set Data on Data port */
		CLCD_voidSetDataPort(CLEAR_CMD);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		apply_flag =1;
	}
	else if (apply_flag == 1)
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		apply_flag =0;
		Currnet_Runnable = IDLE_RUNNABEL;
		//write_req = 0;
	}
}


Status LCD_MOVE_CRS(u8 x, u8 y)
{
	Status status = NOT_OK;

	if((x>=0) && (x<16) && (y>=0) && (y<2))
	{
		if(Currnet_Runnable == IDLE_RUNNABEL)
		{
			//write_req = 1;
			Currnet_Runnable = MOVE_LCD_RUNNABLE;
			xPos = x;
			yPos = y;
			status = OK;
		}
	}

	return status;
}

void LCD_apply_Move_CRS(u8 copy_u8x, u8 copy_u8y)
{
	static u8 apply_flag = 0;
	u8 line;
	if(copy_u8y == 0)
	{
		line = (0x80 + copy_u8x);
	}
	else if(copy_u8y == 1)
	{
		line = (0xC0 + copy_u8x);
	}

	if(apply_flag == 0)
	{
		/* Set RS = 0 */
		DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);
		/* Set RW = 0 */
		DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
		/* Set Data on Data port */
		CLCD_voidSetDataPort(line);
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
		apply_flag =1;
	}
	else if (apply_flag == 1)
	{
		DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
		apply_flag =0;
		Currnet_Runnable = IDLE_RUNNABEL;
		//write_req = 0;
	}
}

void LCD_apply(u8 Copy_u8Data)
{
	static u8 apply_flag = 0;
	if(apply_flag == 0)
	{
		LCD_apply_step1(Copy_u8Data);
		apply_flag =1;
	}
	else if (apply_flag == 1)
	{
		LCD_apply_step2();
		apply_flag =0;
		Currnet_Runnable = IDLE_RUNNABEL;
		//write_req = 0;
	}
}

void LCD_apply_string(u8* Copy_u8Data)
{
	static u8 apply_flag = 0;
	static u8 index=0;

	if(Copy_u8Data[index] != '\0')
	{

		if(apply_flag == 0)
		{
			LCD_apply_step1(Copy_u8Data[index]);
			apply_flag =1;
		}
		else if (apply_flag == 1)
		{
			LCD_apply_step2();
			apply_flag =0;
			//Currnet_Runnable = IDLE_RUNNABEL;
			index++;
			//write_req = 0;
		}
	}
	else
	{
		Currnet_Runnable = IDLE_RUNNABEL;
		index=0;
	}
}

void writeCMD(u8 CMD)
{
	static u8 write_CMD_flag = 0;
		if(write_CMD_flag == 0)
		{
			LCD_apply_step1(CMD);
			write_CMD_flag =1;
		}
		else if (write_CMD_flag == 1)
		{
			LCD_apply_step2();
			write_CMD_flag =0;
			//write_req = 0;
		}
}
