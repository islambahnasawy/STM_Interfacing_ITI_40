#ifndef _USART_MYTYPES_H_
#define _USART_MYTYPES_H_

typedef struct
{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 GTPR;
}USART_t;

typedef void (*Handler_t)(void);
typedef void (*M_handler_t)(USART_t* USART);

#endif 
