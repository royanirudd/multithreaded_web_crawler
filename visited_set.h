#ifndef VISITED_SET_H
#define VISITED_SET_H

#define INITIAL_SET_SIZE 100

typedef struct VisitedSet {
    char** urls;
    int size;
    int capacity;
} VisitedSet;

VisitedSet* initSet();
void addToVisited(VisitedSet* set, const char* url);
int isVisited(VisitedSet* set, const char* url);
void freeSet(VisitedSet* set);

#endif // VISITED_SET_H
