#include <cstdlib>
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
MallocNode * findTheLastAlloc(Heap *h)
{
	MallocNode *ptr = h->head;
	for (; ptr->next!= NULL; ptr = ptr->next);
	return ptr;
}
MallocNode * findSomeAlloc(Heap *h,char * str)
{
	MallocNode *ptr = h->head;
	for (; ptr->next != NULL&&ptr!=str; ptr = ptr->next);
	if (ptr != NULL)
		return ptr;
	return NULL;
}
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
				MallocNode *last= findTheLastAlloc(h);
				last->next = returnValue;
			}
			
			return returnValue;
		}
		temp++;
	}
	return NULL;
}
void myFree(Heap * h,char *str)
{
	MallocNode *toDelete = findSomeAlloc(h, str);
	for (int i = 0; i < toDelete->size; i++)
	{
		str[i] = "";
	}
}
void main()
{
	char data[10 * 1024]="";
	Heap* h;
	h->buffer = data;
	h->buffSize = 10240;
	h->tail = h->head=NULL;
	char *str= myMalloc(h, 40);
	myFree(h, str);
}
