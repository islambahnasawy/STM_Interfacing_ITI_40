/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/
#include "../STD_TYPES.h"
#include "BL_protocol_cfg.h"

const BL_protocol_cfg protocol_cfg [FRAME_TYPE_NUM] =
{
	{.sof=0x55,.ID=0,.eof=0x77,.frame_length=FRAME_OVERHEAD+4},/*write request frame*/
	{.sof=0x55,.ID=1,.eof=0x77,.frame_length=FRAME_OVERHEAD+12},/*start frame*/
	{.sof=0x55,.ID=2,.eof=0x77,.frame_length=FRAME_OVERHEAD},/*data frame (the actual size is not defined here)*/
	{.sof=0x55,.ID=3,.eof=0x77,.frame_length=FRAME_OVERHEAD+1},/*response frame*/
	{.sof=0x55,.ID=3,.eof=0x77,.frame_length=FRAME_OVERHEAD+1}/*final header frame*/
};
