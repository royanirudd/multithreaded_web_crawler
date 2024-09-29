#include <stdlib.h>
#include <string.h>
#include "visited_set.h"

VisitedSet* initSet() {
    VisitedSet* set = (VisitedSet*)malloc(sizeof(VisitedSet));
    set->urls = (char**)malloc(INITIAL_SET_SIZE * sizeof(char*));
    set->size = 0;
    set->capacity = INITIAL_SET_SIZE;
    return set;
}

void addToVisited(VisitedSet* set, const char* url) {
    if (set->size == set->capacity) {
        // Resize set if full
        set->capacity *= 2;
        set->urls = (char**)realloc(set->urls, set->capacity * sizeof(char*));
    }
    set->urls[set->size] = strdup(url);
    set->size++;
}

int isVisited(VisitedSet* set, const char* url) {
    for (int i = 0; i < set->size; i++) {
        if (strcmp(set->urls[i], url) == 0) return 1;
    }
    return 0;
}

void freeSet(VisitedSet* set) {
    for (int i = 0; i < set->size; i++) {
        free(set->urls[i]);
    }
    free(set->urls);
    free(set);
}
