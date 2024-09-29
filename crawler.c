#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crawler.h"

// Callback function for cURL to write received data
static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    char** content = (char**)userp;
    
    *content = realloc(*content, realsize + 1);
    if(*content == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 0;
    }
    
    memcpy(*content, contents, realsize);
    (*content)[realsize] = 0;
    
    return realsize;
}

void initCrawler(CrawlerConfig* config, int maxDepth) {
    config->urlQueue = initQueue();
    config->visitedSet = initSet();
    config->maxDepth = maxDepth;
}

void startCrawling(CrawlerConfig* config, const char* seedUrl) {
    enqueue(config->urlQueue, seedUrl);
    
    char url[MAX_URL_LENGTH];
    char* content = NULL;
    
    while (dequeue(config->urlQueue, url)) {
        if (isVisited(config->visitedSet, url)) {
            continue;
        }
        
        printf("Crawling: %s\n", url);
        
        if (fetchUrl(url, &content) == 0) {
            addToVisited(config->visitedSet, url);
            // TODO: Parse content for links and add to queue
            // TODO: Extract and process data from content
        }
        
        free(content);
        content = NULL;
    }
}

int fetchUrl(const char* url, char** content) {
    CURL* curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)content);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return -1;
        }
        
        return 0;
    }
    
    return -1;
}
