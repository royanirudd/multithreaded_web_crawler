#ifndef URL_QUEUE_H
#define URL_QUEUE_H

#define MAX_URL_LENGTH 2048
#define INITIAL_QUEUE_SIZE 100

typedef struct URL {
    char url[MAX_URL_LENGTH];
} URL;

typedef struct URLQueue {
    URL* urls;
    int front;
    int rear;
    int size;
    int capacity;
} URLQueue;

URLQueue* initQueue();
void enqueue(URLQueue* queue, const char* url);
int dequeue(URLQueue* queue, char* url);
void freeQueue(URLQueue* queue);

#endif // URL_QUEUE_H
