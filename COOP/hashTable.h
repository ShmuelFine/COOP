#pragma once
#include "coop.h"
DEF_CLASS(DataItem);
int key;
int data;
END_DEF(DataItem);
FUNCTIONS(DataItem, int key, int data);
END_FUNCTIONS(DataItem);

DEF_CLASS(DataItemArry);
int sizeOfArray;
DataItem* arry;
END_DEF(DataItemArry);
FUNCTIONS(DataItemArry);
END_FUNCTIONS(DataItemArry);

DEF_CLASS(HashTable);
int capacity;
int amountOfElements ;
DataItemArry* table;
END_DEF(HashTable);
FUNCTIONS(HashTable,  int capacity);
MEM_FUN_DECL(HashTable,  getHashIndex,  int key, int*retVal );
MEM_FUN_DECL(HashTable, search, int key, DataItem* retVal);
MEM_FUN_DECL(HashTable, insert, DataItem newDataItem, int* retVal);
END_FUNCTIONS(HashTable);
