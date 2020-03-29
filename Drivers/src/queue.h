typedef struct
{
	u8 taskID;
	u8 task_row;
	u8 task_col;
	u8* task_string;
	u8 task_length;
}taskInfo_t;


typedef struct
{
	u8 queue_index , queue_size;
	taskInfo_t *runningtask;
}queue;

u8 QueueInit(queue *q , u8 size ,taskInfo_t* taskinfo);
u8 QueueIsEmpty(queue* q);
u8 QueueIsFull(queue* q);
u8 Enqueue(queue* q,taskInfo_t task);
u8 Dequeue(queue* q ,taskInfo_t* task );
