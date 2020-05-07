#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "DIO_interface.h"
#include "DIO_register.h"




void DIO_SetPinMode	(GPIO_Prepihral* Port , u8 Pin , u8 Mode)
{
		if(Pin <=7)
		{	//since all the pins have 4 bits we set pin *4
			Port->CRL &= ~(0b1111 << (Pin *4));//clear the 4 bits
			Port->CRL |= (Mode << (Pin*4));//set them to the new value
		}
		else if(Pin <=15)
		{	//shift it back 8 as it is in CRH which starts from pin 8
			Pin = Pin - 8;
			Port->CRH &= ~(0b1111 << (Pin *4));//clear the 4 bits
			Port->CRH |= (Mode << (Pin*4));//set them to the new value
		}
}
void DIO_SetPinVal 	(GPIO_Prepihral* Port , u8 Pin , u8 Val)
{
		if(Val == 1)
			SET_BIT(Port->ODR,Pin);
		else
			RESET_BIT(Port->ODR,Pin);

}





#define OUTPUT 				0x01
#define INPUT				0x00


u8 GPIO_Config(GPIO_t* ledpin)
{
	u8 localStatus = OK;

	for(u8 i = 0 ; i < 16 ; i++)
	{
		if((((ledpin->mode)>>(4*i))&0xF) == 0xC)
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

u8 GPIO_GetPinVal(GPIO_Prepihral*  port ,u8 pin , u8*value)
{
	Status localStatus = OK;

	*value = (port -> IDR >> pin) & 1;

	return localStatus;
}
