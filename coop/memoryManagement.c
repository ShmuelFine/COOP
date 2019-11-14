#include <stdlib.h>
typedef struct malloc
{
	char* startAdress;
	int size;
	struct malloc * next;
}MallocNode;
typedef struct heap {
	char* buffer;
	int buffSize;
	MallocNode * head;
	MallocNode* tail;
}Heap;
char * myMalloc(Heap * h,int size)
{
	char* temp = h->buffer;
	char* returnValue;
	while (temp)
	{
		returnValue = temp;
		int i = 0;
		for (; i < size && *temp == ""; i++);
		if (i == size)
		{

			MallocNode* ptr;
			ptr->startAdress = returnValue;
			ptr->size = size;
			ptr->next = NULL;
			if (h->head == NULL)
				 h->head =ptr;
			else
			{

			}
			
			return returnValue;
		}
		temp++;
	}
	return NULL;
}
char* myFree(Heap * h, int location,int size)
{

}
void main()
{
	char data[10 * 1024]="";
	Heap* h;
	h->buffer = data;
	h->buffSize = 10240;
	h->tail = h->head=NULL;
	char *str= myMalloc(h, 40);
	myFree()
}
