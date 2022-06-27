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
	_this->size = 0;
}
END_CTOR
DEF_DTOR(DataItemArry)
{
	////how to free the memory??
	//very important
}
END_DTOR

DEF_CTOR(HashTable, int capacity)
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
MEM_FUN_IMPL(HashTable, getHashIndex, int key, int* retVal)
{
	int HashIndex = key % _this->size;
	*retVal = HashIndex;
}
END_FUN;
MEM_FUN_IMPL(HashTable, search, int key, DataItem* retVal)
{
	int HashIndex = getHashIndex(key);
	DataItemArry currentDataItemArry = _this->table[HashIndex];
	while (currentDataItemArry.arry != NULL) {
		if ((*currentDataItemArry.arry).key == key) {
			*retVal = *currentDataItemArry.arry;
			BREAK;
		}
		else
		{
			currentDataItemArry.arry++;
		}
	}
}
END_FUN;
MEM_FUN_IMPL(HashTable, insert, DataItem newDataItem, int* retVal)
{
	int HashIndex = getHashIndex(newDataItem.key);
	DataItemArry currentDataItemArry = _this->table[HashIndex];
	//to add try and catch 
	currentDataItemArry.arry[currentDataItemArry.size] = newDataItem;
	currentDataItemArry.size++;
	_this->size++;
	*retVal = 1;
	///////////to complete!!!



}
END_FUN;
INIT_CLASS(HashTable)
BIND(HashTable, getHashIndex);
BIND(HashTable, search);
BIND(HashTable, insert);
END_INIT_CLASS

