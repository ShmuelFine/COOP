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
	DataItem* arry = NULL;
	NEW(arry, DataItem*);
	_this->size = 0;
}
END_CTOR
DEF_DTOR(DataItemArry)
{
	for (int i = 0; i < _this->size; i++)
	{
		DELETE_OBJ(_this->arry[i]);
	}
}
END_DTOR

DEF_CTOR(HashTable, int capacity)
{
	_this->capacity = capacity;
	_this->size = 0;
	DataItemArry* table = NULL;
	/*NEW(table, DataItemArry* );*/
	NEW_OF_SIZE(_this->table, DataItemArry*, capacity);
}
END_CTOR
DEF_DTOR(HashTable)
{
	for (int i = 0; i < _this->capacity; i++)
	{
		for (int j = 0; j < _this->table[i].size; j++)
		{
		/*	DataItem obj = ;&obj*/
			DELETE_OBJ(&(_this->table[i].arry[j]));
		}
		DELETE_OBJ(&(_this->table[i]));
	}
}
END_DTOR

MEM_FUN_IMPL(HashTable, getHashIndex, int key, int* retVal)
{
	int HashIndex = key % _this->capacity;
	*retVal = HashIndex;
}
END_FUN;
MEM_FUN_IMPL(HashTable, search, int key, DataItem* retVal)
{
	int HashIndex = getHashIndex(key);
	DataItemArry currentDataItemArry = _this->table[HashIndex];
	while (currentDataItemArry.arry != NULL) {
		if ((*currentDataItemArry.arry).key == key) {
			DataItem res = *currentDataItemArry.arry;
			*retVal = res;
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

