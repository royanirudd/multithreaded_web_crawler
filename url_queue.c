#include <stdlib.h>
#include <string.h>
#include "url_queue.h"

URLQueue* initQueue() {
    URLQueue* queue = (URLQueue*)malloc(sizeof(URLQueue));
    queue->urls = (URL*)malloc(INITIAL_QUEUE_SIZE * sizeof(URL));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = INITIAL_QUEUE_SIZE;
    return queue;
}

void enqueue(URLQueue* queue, const char* url) {
    if (queue->size == queue->capacity) {
        // Resize queue if full
        queue->capacity *= 2;
        queue->urls = (URL*)realloc(queue->urls, queue->capacity * sizeof(URL));
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    strncpy(queue->urls[queue->rear].url, url, MAX_URL_LENGTH - 1);
    queue->urls[queue->rear].url[MAX_URL_LENGTH - 1] = '\0';
    queue->size++;
}

int dequeue(URLQueue* queue, char* url) {
    if (queue->size == 0) return 0;
    strncpy(url, queue->urls[queue->front].url, MAX_URL_LENGTH);
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return 1;
}

void freeQueue(URLQueue* queue) {
    free(queue->urls);
    free(queue);
}
