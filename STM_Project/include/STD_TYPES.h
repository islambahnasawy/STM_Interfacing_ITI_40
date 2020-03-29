#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

typedef unsigned char u8;
#define u16 unsigned short int
typedef unsigned long u32;
typedef unsigned long long u64;
#define s8 signed char
#define s16 signed short int
#define s32 signed long int

#define f32 float 
#define f64 double 
#define f96 long double

typedef enum
{
	NOT_OK,
	OK
}Status;

#endif
