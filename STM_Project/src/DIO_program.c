#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "DIO_register.h"




void DIO_SetPinMode	(u8 Port , u8 Pin , u8 Mode)
{
	switch(Port)
	{
	case 'A' :
		if(Pin <=7)
		{	//since all the pins have 4 bits we set pin *4
			PORTA_CRL &= ~(0b1111 << (Pin *4));//clear the 4 bits
			PORTA_CRL |= (Mode << (Pin*4));//set them to the new value
		}
		else if(Pin <=15)
		{	//shift it back 8 as it is in CRH which starts from pin 8
			Pin = Pin - 8;
			PORTA_CRH &= ~(0b1111 << (Pin *4));//clear the 4 bits
			PORTA_CRH |= (Mode << (Pin*4));//set them to the new value
		}
		break;
	case 'B' :
		if(Pin <=7)
		{	//since all the pins have 4 bits we set pin *4
			PORTB_CRL &= ~(0b1111 << (Pin *4));//clear the 4 bits
			PORTB_CRL |= (Mode << (Pin*4));//set them to the new value
		}
		else if(Pin <=15)
		{	//shift it back 8 as it is in CRH which starts from pin 8
			Pin = Pin - 8;
			PORTB_CRH &= ~(0b1111 << (Pin *4));//clear the 4 bits
			PORTB_CRH |= (Mode << (Pin*4));//set them to the new value
		}
		break;
	case 'C' :

		if ((Pin>=13)&&(Pin <=15))
		{	//shift it back 8 as it is in CRH which starts from pin 8
			Pin = Pin - 8;
			PORTC_CRH &= ~(0b1111 << (Pin *4));//clear the 4 bits
			PORTC_CRH |= (Mode << (Pin*4));//set them to the new value
		}
		break ;
	}
}
void DIO_SetPinVal 	(u8 Port , u8 Pin , u8 Val)
{
	switch (Port)
	{
	case 'A':
		if(Val == 1)
			SET_BIT(PORTA_ODR,Pin);
		else
			RESET_BIT(PORTA_ODR,Pin);
		break;
	case 'B':
		if(Val == 1)
			SET_BIT(PORTB_ODR,Pin);
		else
			RESET_BIT(PORTB_ODR,Pin);
		break;
	case 'C':
		if(Val == 1)
			SET_BIT(PORTC_ODR,Pin);
		else
			RESET_BIT(PORTC_ODR,Pin);
		break;
	}
}
u8 DIO_GetPinVal 	(u8 Port , u8 Pin)
{	u8 result ;
switch(Port)
{
case 'A' :
	result = GET_BIT(PORTA_IDR,Pin);break;
case 'B' :
	result = GET_BIT(PORTB_IDR,Pin);break;
case 'C' :
	result = GET_BIT(PORTC_IDR,Pin);break;

}
return result ;
}






#define OUTPUT 				0x01
#define INPUT				0x00


u8 GPIO_Config(GPIO_t* ledpin)
{
	u8 localStatus = OK;

	for(u8 i = 0 ; i < 16 ; i++)
	{
		if((((ledpin->mode)>>(4*i))&0xC) == 0xC)
		{
			ledpin->mode &= ~((0xfULL)<<(4*i));
			ledpin->mode |= (0x8ULL)<<(4*i);
			(ledpin->port)->BSRR = 1<<i;
		}
		else if ((((ledpin->mode)>>(4*i))& 8) == 8)
		{
			(ledpin->port)->BRR = 1<<i;
		}
	}
	*((u64*)(&((ledpin->port)->CRL))) &= ledpin->pin;
	*((u64*)(&((ledpin->port)->CRL))) |= ledpin->mode;

	return localStatus;
}

u8 GPIO_SetPinVal(GPIO_v* ledval)
{
	Status localStatus = OK;

	(ledval->port)->BSRR= ledval->pin_value;

	return localStatus ;
}

u8 GPIO_GetPinVal(u8 pin , GPIO_Prepihral*  port , u8*value)
{
	Status localStatus = OK;

	*value = (port -> IDR >> pin) & 1;

	return localStatus;
}
