#include "queue.h"
#include <stdio.h>
#include <string.h>

void initQueue(Queue* queue) {
    queue->front = queue->rear = -1;
    queue->filled = 0;
}

char isFull(Queue* queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

char isEmpty(Queue* queue) {
    return queue->front == -1;
}

void enqueue(Queue* queue, char* string) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    if (isEmpty(queue)) {
        queue->front = queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }
    queue->filled += 1;
    strcpy(queue->items[queue->rear], string);
    // printf("%s enqueued to the queue.\n", string);
}

void dequeue(Queue* queue, char* dequeued_string) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return;
    }
    queue->filled -= 1; 
    strcpy(dequeued_string, queue->items[queue->front]);
    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
}

void display(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }
    // printf("Queue elements: ");
    int i = queue->front;
    int count = 1;
    do {
        printf("%d. %s\n", count, queue->items[i]);
        i = (i + 1) % MAX_SIZE;
        count++;
    } while (i != (queue->rear + 1) % MAX_SIZE);
}

char saveQueueToFile(Queue* queue, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Unable to open file for writing.\n");
        return 0;
    }

    fwrite(queue, sizeof(Queue), 1, file);

    fclose(file);
    return 1;
}

char loadQueueFromFile(Queue* queue, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Unable to open file for reading.\n");
        return 0;
    }

    fread(queue, sizeof(Queue), 1, file);

    fclose(file);
    return 1;
}

void clearQueue(Queue* queue) {
    char str[DEFAULT_CHARS];
    while (!isEmpty(queue)) {
        dequeue(queue, str);
    }
}
