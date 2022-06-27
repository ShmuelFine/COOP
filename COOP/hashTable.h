#pragma once
#include "coop.h"
DEF_CLASS(DataItem);
int key;
int data;

END_DEF(DataItem);
FUNCTIONS(DataItem, int key, int data);
END_FUNCTIONS(DataItem);

DEF_CLASS(DataItemArry);
DataItem* arry;
END_DEF(DataItemArry);
FUNCTIONS(DataItemArry);
END_FUNCTIONS(DataItemArry);

DEF_CLASS(HashTable);
int capacity;
int size ;
DataItemArry* table;
END_DEF(HashTable);
FUNCTIONS(HashTable,  int capacity);
MEM_FUN_DECL(HashTable,  GetHashIndex,  int key, int*retVal );
END_FUNCTIONS(HashTable);
