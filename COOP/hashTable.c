#include"hashTable.h"
DEF_CTOR(DataItem, int key, int data)
{
	_this->key = key;
	_this->data = data;
}
END_CTOR
DEF_DTOR(DataItem)
{}
END_DTOR

DEF_CTOR(DataItemArry)
{
	int* arry = NULL;
	NEW(arry, DataItem*);
}
END_CTOR
DEF_DTOR(DataItemArry)
{
	////how to free the memory??
	//very important
}
END_DTOR

DEF_CTOR(HashTable,int capacity)
{
	_this->capacity = capacity;
	_this->size = 0;
	int* table = NULL;
	NEW(table, DataItemArry*);
}
END_CTOR
DEF_DTOR(HashTable)
{
	///how to free the memory??
	//very important
}
END_DTOR
MEM_FUN_IMPL(HashTable, GetHashIndex, int key, int*retVal)
{
	int HashIndex = key% _this->size;
	*retVal=HashIndex;
}
END_FUN;

