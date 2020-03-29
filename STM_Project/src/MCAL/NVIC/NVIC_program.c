#include "STD_TYPES.h"
#include "NVIC_interface.h"
#define NVIC_BASEASSRESS 0xE000E100
#define NVIC ((volatile NVIC_t* const)NVIC_BASEASSRESS)
typedef struct
{
	u32 ISER[8];
	u32 reserved0[24];
	u32 ICER[8];
	u32 reserved1[24];
	u32 ISPR[8];
	u32 reserved2[24];
	u32 ICPR[8];
	u32 reserved3[24];
	u32 IABR[8];
	u32 reserved4[56];
	u8 	IPR[240];
	u32 reserved5[644];
	u32 STIR;
}NVIC_t;



/*System block register to configure the number of bits allocated to group priority and subpriority*/
#define SCB_AIRCR *((volatile u32*)0xE000ED0C)

#define SCB_PRIGROUP_MASK	0x0000f8ff

#define MAX_INTERRUPT_NUM	240
#define SIZE_OF_REG			32
void NVIC_SetPriorityGrouping(u32 priority_grouping)
{
	u32 temp = SCB_AIRCR & SCB_PRIGROUP_MASK;
	temp |= priority_grouping;
	SCB_AIRCR = temp;
}

u8 NVIC_EnableIRQ(u8 IRQn)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
		NVIC -> ISER[IRQn/SIZE_OF_REG] |= (1<<IRQn%SIZE_OF_REG);
	else
		local_Status=NOT_OK;
	return local_Status;
}


u8 NVIC_DisableIRQ(u8 IRQn)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
		NVIC -> ICER[IRQn/SIZE_OF_REG] |= (1<<IRQn%SIZE_OF_REG);
	else
		local_Status=NOT_OK;
	return local_Status;
}


u8 NVIC_GetPendingIRQ (u8 IRQn ,u32* value)
{
	u8 local_Status = OK; 
	if(IRQn<=MAX_INTERRUPT_NUM)
		*value = (NVIC->ICPR[IRQn/SIZE_OF_REG]>>(IRQn%SIZE_OF_REG))&1;
	else
		local_Status=NOT_OK;
	return local_Status;
}

u8 NVIC_SetPendingIRQ (u8 IRQn)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
		NVIC -> ISPR[IRQn/SIZE_OF_REG] |= (1<<IRQn%SIZE_OF_REG);
	else
		local_Status=NOT_OK;
	return local_Status;
}

u8 NVIC_ClearPendingIRQ (u8 IRQn)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
		NVIC -> ICPR[IRQn/SIZE_OF_REG] |= (1<<IRQn%SIZE_OF_REG);
	else
		local_Status=NOT_OK;
	return local_Status;
}

u8 NVIC_GetActive (u8 IRQn,u32* value)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
		*value = (NVIC->IABR[IRQn/SIZE_OF_REG]>>(IRQn%SIZE_OF_REG))&1;
	else
		local_Status=NOT_OK;
	return local_Status;
}

u8 NVIC_SetPriority (u8 IRQn, u8 priority)
{
	u8 local_Status = OK;
	if(IRQn<=MAX_INTERRUPT_NUM)
	{
	NVIC->IPR[IRQn]= priority<<4;
	}
	else
		local_Status=NOT_OK;
	return local_Status;
}

u8 NVIC_GetPriority (u8 IRQn , u8*value)
{
	u8 local_Status = OK; 
	if(IRQn<=MAX_INTERRUPT_NUM)
	*value = NVIC->IPR[IRQn];
	else
		local_Status=NOT_OK;
	return local_Status;
}

void NVIC_Enable_GlobalInterrupt(void)
{
	asm("MOV R0, #0");
	asm("MSR PRIMASK, R0");
}
void NVIC_Disable_GlobalInterrupt(void)
{
	asm("MOV R0, #1");
	asm("MSR PRIMASK, R0");
}
void NVIC_Enable_FaultMask(void)
{
	asm("CPSIE F");
}
void NVIC_Disable_FaultMask(void)
{
	asm("CPSID F");
}
void NVIC_Disable_LowerPriority(u8 priority)
{
	asm("LSLS r0, r0, #4");
	asm("MSR BASEPRI, R0");
}
