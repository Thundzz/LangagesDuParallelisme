#include "queue.h"

#include <stdlib.h>
#include <assert.h>

Queue* queue_init(int size)
{
	Queue * q = (Queue *) malloc(sizeof(Queue));
	q->elements = (Task*) malloc(size*sizeof(Task));
	q->front = 0;
	q->rear = 0;
	q->cur_size = 0;
	q->size = size;
	return q;
}


int queue_isEmpty(Queue* q){
	return (q->front == q->rear);
}

void queue_push(Queue* q, Task element){
	assert(q->cur_size < q->size);
	int front = q->front;
	q->elements[front] = element;
	q->front = (q->front +1) % q->size;
	q-> cur_size ++;
}
Task queue_pop(Queue* q){
	assert(q->cur_size >=0);
	Task element= q->elements[q->rear];
	q-> cur_size --;
	q->rear = (q->rear +1) % q->size;
	return element;
}

void queue_delete(Queue* q){
	free(q-> elements);
	free(q);
}

int queue_length(Queue* q){
	return (q->cur_size);
}