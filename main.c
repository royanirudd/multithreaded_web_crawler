#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "url_queue.h"
#include "visited_set.h"
#include "crawler.h"
#include "html_parser.h"

#define MAX_URLS_TO_CRAWL 10

int main(void) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CrawlerConfig config;
    initCrawler(&config, MAX_DEPTH);

    printf("Web Crawler Test\n");

    const char* seedUrl = "https://example.com";  // Use example.com for testing
    printf("Starting crawl with seed URL: %s\n", seedUrl);

    enqueue(config.urlQueue, seedUrl);

    int urlsCrawled = 0;
    char url[MAX_URL_LENGTH];
    char* content = NULL;

    while (dequeue(config.urlQueue, url) && urlsCrawled < MAX_URLS_TO_CRAWL) {
        if (isVisited(config.visitedSet, url)) {
            continue;
        }

        printf("Crawling URL %d: %s\n", urlsCrawled + 1, url);

        if (fetchUrl(url, &content) == 0) {
            addToVisited(config.visitedSet, url);
            
            // Count links before extraction
            int initialQueueSize = config.urlQueue->size;
            
            extractLinks(content, url, config.urlQueue);
            
            // Count links after extraction
            int newLinks = config.urlQueue->size - initialQueueSize;
            printf("  Found %d new links\n", newLinks);

            urlsCrawled++;
        } else {
            printf("  Failed to fetch URL\n");
        }

        free(content);
        content = NULL;
    }

    printf("\nCrawl complete. Visited %d URLs.\n", urlsCrawled);

    // Cleanup
    curl_global_cleanup();
    freeQueue(config.urlQueue);
    freeSet(config.visitedSet);

    return 0;
}
