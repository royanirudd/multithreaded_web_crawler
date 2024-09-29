#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <myhtml/api.h>
#include "url_queue.h"

void extractLinks(const char* html, const char* baseUrl, URLQueue* urlQueue);

#endif // HTML_PARSER_H
