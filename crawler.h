#ifndef CRAWLER_H
#define CRAWLER_H

#include <curl/curl.h>
#include "url_queue.h"
#include "visited_set.h"

#define MAX_DEPTH 5

typedef struct {
    URLQueue* urlQueue;
    VisitedSet* visitedSet;
    int maxDepth;
} CrawlerConfig;

void initCrawler(CrawlerConfig* config, int maxDepth);
void startCrawling(CrawlerConfig* config, const char* seedUrl);
int fetchUrl(const char* url, char** content);

#endif // CRAWLER_H
