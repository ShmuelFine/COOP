#include "HashTable.h"
#include "MathUtils.h"
#include <stdio.h> 



DEF_CTOR(HashTableItem, char* key, char* value)
{
	_this->key = key;
	_this->value = value;
}
END_CTOR

DEF_CTOR(HashTable)
{

}
END_CTOR

DEF_DTOR(HashTableItem)
{

}
END_DTOR

MEM_FUN_IMPL(HashTableItem, createHashTableItem, char* key, char* value, HashTableItem* output)
{
	HashTableItem* item = (HashTableItem*)malloc(sizeof(HashTableItem));
	item->key = (char*)malloc(strlen(key) + 1);
	item->value = (char*)malloc(strlen(value) + 1);
	strcpy(item->key, key);
	strcpy(item->value, value);
	output = item;
}
END_FUN;

MEM_FUN_IMPL(HashTableItem, hashFunction, char* str, long* output)
{
	unsigned long i = 0;

	for (int j = 0; str[j]; j++)
		i += str[j];

	*output = i % CAPACITY;
}
END_FUN;

MEM_FUN_IMPL(HashTable, createTable, int size, HashTableItem** output)
{
	// Creates a new HashTable.
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	table->size = size;
	table->count = 0;
	table->items = (HashTableItem**)calloc(table->size, sizeof(HashTableItem*));

	for (int i = 0; i < table->size; i++)
		table->items[i] = NULL;

	*output= table;

}
END_FUN;



INIT_CLASS(HashTableItem)
BIND(HashTableItem, createHashTableItem);
BIND(HashTableItem, hashFunction);
END_INIT_CLASS(HashTableItem)

