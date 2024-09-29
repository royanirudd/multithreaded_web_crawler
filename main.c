#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "url_queue.h"
#include "visited_set.h"
#include "crawler.h"

int main(int argc, char *argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CrawlerConfig config;
    initCrawler(&config, MAX_DEPTH);

    printf("Web Crawler initialized.\n");

    // TODO: Implement command-line argument parsing
    const char* seedUrl = "http://example.com";  // Default seed URL
    
    startCrawling(&config, seedUrl);

    // Cleanup
    curl_global_cleanup();
    freeQueue(config.urlQueue);
    freeSet(config.visitedSet);

    return 0;
}
