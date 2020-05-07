#ifndef _QUEUE_H_
#define _QUEUE_H_

/*The information about the object that will be enlisted and delisted from the queue*/
typedef struct
{
	u8 taskID;
	u8 task_row;
	u8 task_col;
	u8* task_string;
	u8 task_length;
}taskInfo_t;

/*Queue information*/
typedef struct
{
	u8 queue_index , queue_size;
	taskInfo_t *runningtask;
}queue;

/*function to Initializing the queue*/
u8 QueueInit(queue *q , u8 size ,taskInfo_t* taskinfo);
/*function to check if the queue is empty*/
u8 QueueIsEmpty(queue* q);
/*function to check if the queue is full*/
u8 QueueIsFull(queue* q);
/*function to enlist and element in the queue */
u8 Enqueue(queue* q,taskInfo_t task);
/*function to delist and element in the queue */
u8 Dequeue(queue* q ,taskInfo_t* task );


#endif
