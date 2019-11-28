#pragma once
#include <stdlib.h>
#include <stdbool.h>
typedef struct malloc
{
	char* startAdress;
	int size;
	struct malloc* next;
}MallocNode;
typedef struct heap {
	char* buffer;
	int buffSize;
	MallocNode* head;
	MallocNode* tail;
	bool flagArrFull[10240];
}Heap;
#ifdef __exporting__
#define api __declspec(dllexport)
#else
#define api __declspec(dllimport)
#endif

api MallocNode * findTheLastAlloc(Heap *h);
api MallocNode * deleteSomeAlloc(Heap *h, char * str);
//int  aaa(int a, int b);
api char * myMalloc(Heap * h, int size);
api void myFree(Heap * h, char *str);
//void main();