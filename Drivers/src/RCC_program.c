#include "STD_TYPES.h"

#include "RCC_interface.h"
#define RCC_BASE_ADDRESS 0x40021000
#define RCC ((volatile RCC_Prepihral* const)RCC_BASE_ADDRESS)
#define SW_HSI_MASK				0x00000000
#define SW_HSE_MASK				0x00000001
#define SW_PLL_MASK				0x00000002
#define SWS_MASK 				0x0000000C
#define HSI_RDY_MASK			0x00000002
#define HSE_RDY_MASK			0x00020000
#define PLL_RDY_MASK			0x02000000
#define CLOCK_MASK				0x80000000
#define SW_CLEAR				0xfffffffc
#define PLL_MASK				0x40000000
#define PLL_ClEAR				0xffC0ffff
#define MCO_MASK				0x20000000
#define MCO_CLEAR				0xf8ffffff
#define APB_MASK				0x10000000
#define APB2_CLEAR				0xffffc7ff
#define APB1_CLEAR 				0xfffff800
#define AHB_MASK				0x08000000
#define AHB_CLEAR				0xffffff0f
#define AHB_PERIPHERAL_MASK		0x20000000
#define APB2_PERIPHERAL_MASK	0x40000000
#define APB1_PERIPHERAL_MASK	0x80000000
#define HSE_BYP_MASK			0x00040000
#define PLL_SCR_MASK			0x00010000
#define PLL_XTPRE_MASK			0x00020000

#define PLL_SOURCE_HSE			0x00010000
#define PLL_SOURCE_HSI_2		0x00000000
#define PLL_SOURCE_HSE_2		0x00020000
#define RCC_PLLMUL_POSITION		18

#define AHB_MASKING				0x000000f0

#define AHB_PRE_2				0x00000080
#define AHB_PRE_4				0x00000090
#define AHB_PRE_8				0x000000A0
#define AHB_PRE_16				0x000000B0
#define AHB_PRE_64				0x000000C0
#define AHB_PRE_128				0x000000D0
#define AHB_PRE_256				0x000000E0
#define AHB_PRE_512				0x000000F0

#define APB2_PRE_POSITION		11
#define APB2_MASKING			0x00002000
/*
 * configuration
 */
#define HSE_CRYSTAL_FREQ	8000000UL
#define HSI_FREQ			8000000UL
#define HSE_RC_FREQ			0UL

typedef struct 
{
	u32 CR;
	u32 CFGR;
	u32 CIR;
	u32 APB2RSTR;
	u32 APB1RSTR;
	u32 AHBENR;
	u32 APB2ENR;
	u32 APB1ENR;
	u32 BDCR;
	u32 CSR;
}RCC_Prepihral;


u8 RCC_u8ReadSystemClock(u8* clock)
{
	*clock = (RCC->CFGR) & SWS_MASK ;
	return OK;
}	  

u8 RCC_u8CheckHSI_ON(u8* value)
{
	*value = (RCC->CR) & HSI_RDY_MASK;
	return OK;
}
u8 RCC_u8CheckHSE_ON(u32* value)
{
	*value = (RCC->CR) & HSE_RDY_MASK;
	return OK;
}
u8 RCC_u8CheckPLL_ON(u32* value)
{
	*value = (RCC->CR) & PLL_RDY_MASK;
	return OK;
}
u8 RCC_u8ChangeClockStatus(u32 Clock , u8 mode)
{
	u8 local_Status = OK;
	if((Clock & CLOCK_MASK) == CLOCK_MASK)
	{
		switch(mode)
		{
			case CLK_STATUS_ON :
				RCC->CR |= Clock&(~CLOCK_MASK);
				break;
			case CLK_STATUS_OFF:
				RCC->CR &= ~(Clock&(~CLOCK_MASK));
				break;
			default:
				local_Status = NOT_OK;
				break;
		}
	}
	else
	{
		local_Status = NOT_OK;
	}
	return local_Status;
}

u8 RCC_u8SwitchSystemClock(u8 Clock)
{
	u32 temp = RCC->CFGR & SW_CLEAR;
	temp |= Clock;
	RCC->CFGR = temp ;
	return OK;
}
u8 RCC_HSE_HSEBYP(u32 bypath)
{
	u8 local_Status = OK;
	switch(bypath)
	{
	case CRYSTAL:
		RCC->CR &= CRYSTAL;
		break;
	case RC:
		RCC->CR |= RC;
		break;
	default :
		local_Status = NOT_OK;
	}
	return local_Status;
}

u8 RCC_PLL_Config(u32 mul,u32 clock_source)
{
	u8 local_Status = OK;
	if(((mul & PLL_MASK) == PLL_MASK) && ((clock_source & PLL_MASK) == PLL_MASK))
	{
		u32 temp = RCC->CFGR & PLL_ClEAR;
		temp |= (mul | clock_source);

		RCC->CFGR = (temp& (~PLL_MASK));
	}
	else
	{
		local_Status = NOT_OK;
	}
	return local_Status;
}

u8 RCC_MCO_Config(u32 Clock)
{
	u8 local_Status = OK;
	if((Clock&MCO_MASK) == MCO_MASK)
	{
	u32 temp = RCC->CFGR & MCO_CLEAR;
	temp |= Clock ;
	RCC->CFGR = (temp & (~MCO_MASK));
	}
	else
	{
		 local_Status = NOT_OK;
	}
	return local_Status;
}

u8 RCC_APB2Clock_Config(u32 Prescaler)
{
	u8 local_Status = OK;
	if((Prescaler&APB_MASK)==APB_MASK)
	{
		u32 temp = RCC->CFGR & APB2_CLEAR;
		temp |= Prescaler;
		RCC->CFGR = (temp & (~APB_MASK));
	}
	else
	{
		local_Status =NOT_OK;
	}
	return local_Status;
}

u8 RCC_APB1Clock_Config(u32 Prescaler)
{
	u8 local_Status = OK;
	if((Prescaler&APB_MASK)==APB_MASK)
	{
		u32 temp = RCC->CFGR & APB1_CLEAR;
		temp |= Prescaler;
		RCC->CFGR = (temp & (~APB_MASK));
	}
	else
	{
		local_Status =NOT_OK;
	}
	return local_Status;
}

u8 RCC_AHBClock_Config(u32 Prescaler)
{
	u8 local_Status = OK;
	if((Prescaler&AHB_MASK)==AHB_MASK)
	{
		u32 temp = RCC->CFGR & AHB_CLEAR;
		temp |= Prescaler;
		RCC->CFGR = (temp & (~AHB_MASK));
	}
	else
	{
		local_Status =NOT_OK;
	}
	return local_Status;
}


u8 RCC_Peripheral_Config(u32 Peripheral ,u8 mode)
{
	u8 local_Status = OK;
	if((Peripheral&AHB_PERIPHERAL_MASK)==AHB_PERIPHERAL_MASK)
	{
		switch(mode)
		{
			case PERIPHERAL_ON:
			RCC->AHBENR |= (Peripheral&(~AHB_PERIPHERAL_MASK));
			break;
			case PERIPHERAL_OFF:
			RCC->AHBENR &= ~(Peripheral&(~AHB_PERIPHERAL_MASK));
			break;
			default:
			local_Status = NOT_OK;
		}
	}
	else if((Peripheral&APB1_PERIPHERAL_MASK)==APB1_PERIPHERAL_MASK)
	{
		switch(mode)
		{
			case PERIPHERAL_ON:
			RCC->APB1ENR |= (Peripheral&(~APB1_PERIPHERAL_MASK));
			break;
			case PERIPHERAL_OFF:
			RCC->APB1ENR &= ~(Peripheral&(~APB1_PERIPHERAL_MASK));
			break;
			default:
			local_Status = NOT_OK;
		}
	}
	else if((Peripheral&APB2_PERIPHERAL_MASK)==APB2_PERIPHERAL_MASK)
	{
		switch(mode)
		{
			case PERIPHERAL_ON:
			RCC->APB2ENR |= (Peripheral&(~APB2_PERIPHERAL_MASK));
			break;
			case PERIPHERAL_OFF:
			RCC->APB2ENR &= ~(Peripheral&(~APB2_PERIPHERAL_MASK));
			break;
			default:
			local_Status = NOT_OK;
		}
	}
	else
	{
		local_Status = NOT_OK;
	}
	return local_Status;
}

u32 RCC_u32ReadSystemFreq(void)
{
	u8 sysClock;
	u8 Mul_Factor;
	u32 freq = 0;
	RCC_u8ReadSystemClock(&sysClock);
	if(sysClock == SWS_HSI)
	{
		freq = HSI_FREQ ;
	}
	else if (sysClock == SWS_HSE)
	{
		/*check on the bypath bit*/
		if((RCC->CR & HSE_BYP_MASK) == HSE_BYP_MASK)
		{
			freq = HSE_RC_FREQ;
		}
		else
		{
			freq = HSE_CRYSTAL_FREQ;
		}
	}
	else if(sysClock == SWS_PLL)
	{
		if((RCC->CFGR & PLL_SCR_MASK) == PLL_SOURCE_HSE)
		{
			/*check on the bypath bit*/
			if((RCC->CR & HSE_BYP_MASK) == HSE_BYP_MASK)
			{
				freq = HSE_RC_FREQ;
			}
			else
			{
				freq = HSE_CRYSTAL_FREQ;
			}
			if((RCC->CFGR & PLL_XTPRE_MASK) == PLL_SOURCE_HSE_2)
			{
				freq /= 2 ;
			}

		}
		else
		{
			freq = HSI_FREQ/2;
		}
		Mul_Factor = ((RCC->CFGR >>RCC_PLLMUL_POSITION)&0xF)+2;
		if(Mul_Factor > 16) Mul_Factor = 16;
		freq *= Mul_Factor ;
	}
	return freq;
}

u32 RCC_u32ReadAHBFreq(void)
{
	u32 freq = RCC_u32ReadSystemFreq();
	if((RCC->CFGR & AHB_MASKING) == AHB_PRE_2)
	{
		freq /= 2;
	}
	else if((RCC->CFGR & AHB_MASKING) == AHB_PRE_4)
	{
		freq /= 4;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_8)
	{
		freq /= 8;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_16)
	{
		freq /= 16;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_64)
	{
		freq /= 64;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_128)
	{
		freq /= 128;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_256)
	{
		freq /= 256;
	}
	else if ((RCC->CFGR & AHB_MASKING) == AHB_PRE_512)
	{
		freq /= 512;
	}

	return freq;
}

u32 RCC_u32ReadAPB2Freq(void)
{
	u32 freq = RCC_u32ReadAHBFreq();
	if((RCC->CFGR & APB2_MASKING) == APB2_MASKING)
	{
	u8 Prescaler = 2 << (((RCC->CFGR >> APB2_PRE_POSITION) & 7)-4);
	freq /= Prescaler;
	}
	return freq;
}
