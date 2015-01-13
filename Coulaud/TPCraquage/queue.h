#ifndef	_QUEUE_H_
#define _QUEUE_H_

/* A basic API for task queues */
typedef struct task{
	unsigned long long first;
	unsigned long long last;
	unsigned long long length;
} Task;

typedef struct {
	Task * elements;
	int front;
	int rear;
	int size;
	int cur_size;
} Queue;

Queue* queue_init(int size);

int queue_isEmpty(Queue* q);
int queue_length(Queue* q);

void queue_push(Queue* q, Task element);
Task queue_pop(Queue* q);

void queue_delete(Queue* q);


#endif