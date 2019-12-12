#pragma once
#include <cstdlib>
typedef struct malloc MallocNode;
typedef struct heap Heap;
MallocNode * findTheLastAlloc(Heap *h);
MallocNode * findSomeAlloc(Heap *h, char * str);
char * myMalloc(Heap * h, int size);
void myFree(Heap * h, char *str);
void main();