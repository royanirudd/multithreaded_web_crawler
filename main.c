#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "url_queue.h"
#include "visited_set.h"
#include "crawler.h"

int main(int argc, char *argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Initialize URL queue and visited set
    URLQueue* urlQueue = initQueue();
    VisitedSet* visitedSet = initSet();

    printf("Web Crawler initialized.\n");

    // TODO: Implement command-line argument parsing
    // TODO: Set up threads
    // TODO: Start crawling process
    // TODO: Clean up and exit

    // Cleanup
    curl_global_cleanup();
    freeQueue(urlQueue);
    freeSet(visitedSet);

    return 0;
}
