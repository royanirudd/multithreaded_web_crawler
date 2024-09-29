#include <string.h>
#include <stdlib.h>
#include <myhtml/api.h>
#include "html_parser.h"
#include "url_queue.h"

// Simple function to resolve relative URLs
static void resolveUrl(const char* base, const char* relative, char* result, size_t resultSize) {
    if (strncmp(relative, "http://", 7) == 0 || strncmp(relative, "https://", 8) == 0) {
        // Absolute URL
        strncpy(result, relative, resultSize - 1);
        result[resultSize - 1] = '\0';
    } else if (relative[0] == '/') {
        // Relative to root
        const char* scheme_end = strstr(base, "://");
        if (scheme_end) {
            const char* domain_end = strchr(scheme_end + 3, '/');
            if (domain_end) {
                size_t base_len = domain_end - base;
                if (base_len < resultSize) {
                    strncpy(result, base, base_len);
                    strncpy(result + base_len, relative, resultSize - base_len - 1);
                    result[resultSize - 1] = '\0';
                }
            }
        }
    } else {
        // Relative to current path
        const char* last_slash = strrchr(base, '/');
        if (last_slash) {
            size_t base_len = last_slash - base + 1;
            if (base_len < resultSize) {
                strncpy(result, base, base_len);
                strncpy(result + base_len, relative, resultSize - base_len - 1);
                result[resultSize - 1] = '\0';
            }
        }
    }
}

// Callback function for myhtml to process each node
static bool processNode(myhtml_tree_node_t* node, URLQueue* urlQueue, const char* baseUrl) {
    if (myhtml_node_tag_id(node) == MyHTML_TAG_A) {
        myhtml_tree_attr_t* href = myhtml_attribute_by_key(node, "href", 4);
        if (href) {
            const char* link = myhtml_attribute_value(href, NULL);
            if (link) {
                char resolvedUrl[MAX_URL_LENGTH];
                resolveUrl(baseUrl, link, resolvedUrl, MAX_URL_LENGTH);
                enqueue(urlQueue, resolvedUrl);
            }
        }
    }
    return true;
}

// Function to traverse the HTML tree
static void traverseTree(myhtml_tree_node_t* node, URLQueue* urlQueue, const char* baseUrl) {
    if (node == NULL)
        return;

    processNode(node, urlQueue, baseUrl);

    myhtml_tree_node_t* child = myhtml_node_child(node);
    while (child != NULL) {
        traverseTree(child, urlQueue, baseUrl);
        child = myhtml_node_next(child);
    }
}

void extractLinks(const char* html, const char* baseUrl, URLQueue* urlQueue) {
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

    myhtml_tree_node_t* node = myhtml_tree_get_node_html(tree);
    traverseTree(node, urlQueue, baseUrl);

    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
}
