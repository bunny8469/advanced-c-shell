#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include "structs.h"

void initQueue(Queue* queue);
char isFull(Queue* queue);
char isEmpty(Queue* queue);
void enqueue(Queue* queue, char* string);
void dequeue(Queue* queue, char* dequeued_string);
void display(Queue* queue);
char saveQueueToFile(Queue* queue, const char* filename);
char loadQueueFromFile(Queue* queue, const char* filename);

#endif /* CIRCULAR_QUEUE_H */
