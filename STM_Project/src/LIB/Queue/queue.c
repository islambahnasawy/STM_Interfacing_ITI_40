#include "STD_TYPES.h"
#include "queue.h"

u8 QueueInit(queue *q , u8 size ,taskInfo_t* taskinfo)
{
	u8 local_Status = OK;
	if(q){
	q ->queue_index = 0;
	q ->queue_size = size;
	q ->runningtask = taskinfo;
	}
	else
		local_Status =NOT_OK;
	return local_Status;
}
u8 QueueIsEmpty(queue* q)
{
	return (q->queue_index == 0);
}
u8 QueueIsFull(queue* q)
{
	return (q->queue_index == q-> queue_size);
}
u8 Enqueue(queue* q,taskInfo_t task)
{
	u8 local_Status = OK;
	if(q&&!QueueIsFull(q))
	{
		q -> runningtask[q ->queue_index ] = task;
		q -> queue_index++;
	}
	else
		local_Status =NOT_OK;
	return local_Status;
}

u8 Dequeue(queue* q ,taskInfo_t* task )
{
	u8 local_Status =OK;
	if(q&&task&&!QueueIsEmpty(q))
	{
		*task = q->runningtask[0];
		q->queue_index--;
		for(u8 i=0;i<q->queue_index;i++)
		{
			q->runningtask[i]=q->runningtask[i+1];
		}
	}
	else
	{
		local_Status = NOT_OK;
	}
	return local_Status;
}
