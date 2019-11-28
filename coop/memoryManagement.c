
#include "memoryManagement.h"

//int  aaa(int a, int b)
//{
//	return  a + b;
//}
MallocNode * findTheLastAlloc(Heap *h)
{
	MallocNode *ptr = h->head;
	for (; ptr->next!= NULL; ptr = ptr->next);
	return ptr;
}
MallocNode * deleteSomeAlloc(Heap *h,char * str)
{
	int size;
	MallocNode* ptr;
	ptr= h->head;
	for (; ptr->next != NULL&&ptr->next!=str; ptr = ptr->next);
	if (ptr != NULL)
	{
		size = ptr->next->size;
		MallocNode *tmp = ptr->next;
		ptr->next = tmp->next;
		tmp=NULL;
		return size;
	}
 	return NULL;
}
char * myMalloc(Heap * h,int size)
{
	int i,j;
	char* temp = h->buffer;
	char* returnValue;
	for ( i = 0; i < h->buffSize; i++)
	{
		if (h->flagArrFull[i] == false)
		{

			for (j = i; j < i+size && h->flagArrFull[j] == false; j++);
			if (j == i+size)
			{
				for (int k = i; k <i+size; k++)
				{
					h->flagArrFull[k] = true;
				}
				returnValue = h->buffer + i;
				MallocNode tmp2;
				tmp2.startAdress = returnValue;
				tmp2.next = NULL;
				if (h->head == NULL)
				{ 
					h->head = &tmp2;
				}
				else
				{
					MallocNode *last = findTheLastAlloc(h);
					last->next = returnValue;
				}
				return returnValue;
			}
		}
	}
	return NULL;
}
void myFree(Heap * h,char *str)
{
	int size = deleteSomeAlloc(h,str);
	int start =str-h->buffer;
	for (int i =start; i < size; i++)
	{
		h->flagArrFull[i] = false;
	}

}
//void main()
//{
//	/*char data[10 * 1024];
//	Heap* h = {};
//	h->buffer = data;
//	h->buffSize = 10240;
//	bool bb[10 * 1024];
//	h->flagArrFull = bb;
//    h->tail = h->head=NULL;
//	char *str= myMalloc(h, 40);
//	myFree(h, str);*/
//}
