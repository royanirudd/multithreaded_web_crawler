#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>

#define MAX_URLS 1000
#define NUM_THREADS 4

typedef struct {
    char *urls[MAX_URLS];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} url_queue_t;

url_queue_t url_queue;
pthread_mutex_t visited_mutex;
char *visited_urls[MAX_URLS];
int visited_count = 0;

void url_queue_init(url_queue_t *queue) {
    queue->front = queue->rear = queue->count = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
}

void url_queue_push(url_queue_t *queue, const char *url) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->count == MAX_URLS) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }
    queue->urls[queue->rear] = strdup(url);
    queue->rear = (queue->rear + 1) % MAX_URLS;
    queue->count++;
    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
}

char *url_queue_pop(url_queue_t *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->count == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }
    char *url = queue->urls[queue->front];
    queue->front = (queue->front + 1) % MAX_URLS;
    queue->count--;
    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return url;
}

int is_visited(const char *url) {
    pthread_mutex_lock(&visited_mutex);
    for (int i = 0; i < visited_count; i++) {
        if (strcmp(visited_urls[i], url) == 0) {
            pthread_mutex_unlock(&visited_mutex);
            return 1;
        }
    }
    if (visited_count < MAX_URLS) {
        visited_urls[visited_count++] = strdup(url);
    }
    pthread_mutex_unlock(&visited_mutex);
    return 0;
}

void *crawler_thread(void *arg) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Curl initialization failed\n");
        return NULL;
    }

    while (1) {
        char *url = url_queue_pop(&url_queue);
        if (!url) break;  // No more URLs to process

        if (!is_visited(url)) {
            printf("Crawling: %s\n", url);
            // Perform the actual crawling here
            // Extract links and add them to the queue
            // url_queue_push(&url_queue, new_url);
        }

        free(url);
    }

    curl_easy_cleanup(curl);
    return NULL;
}

int main() {
    url_queue_init(&url_queue);
    pthread_mutex_init(&visited_mutex, NULL);

    // Add initial URL
    url_queue_push(&url_queue, "https://example.com");

    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, crawler_thread, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&visited_mutex);
    pthread_mutex_destroy(&url_queue.mutex);
    pthread_cond_destroy(&url_queue.not_empty);
    pthread_cond_destroy(&url_queue.not_full);

    return 0;
}
