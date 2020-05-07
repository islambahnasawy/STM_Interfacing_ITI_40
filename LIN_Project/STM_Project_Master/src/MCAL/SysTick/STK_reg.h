/****************************************************/
/* Author : ANWAR									*/
/* Date : 8 MARCH 2020								*/
/* Version : V01									*/
/****************************************************/
/****************************************************/

#ifndef STK_REGISTER_H_
#define STK_REGISTER_H_

#define STK_CTRL 	*((volatile u32*)0xE000E010) 
#define STK_LOAD 	*((volatile u32*)0xE000E014) 
#define STK_VAL 	*((volatile u32*)0xE000E018) 

#endif