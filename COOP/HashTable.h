
#include "COOP.h"
#define CAPACITY 50000 // Size of the HashTable.


DEF_CLASS(HashTableItem);
char* key;
char* value;
END_DEF(HashTableItem);
 
DEF_CLASS(HashTable);
HashTableItem** items;
int size;
int count;
END_DEF(HashTable);


FUNCTIONS(HashTableItem);
MEM_FUN_DECL(HashTableItem, createHashTableItem, char* key, char* value, HashTableItem* output);
MEM_FUN_DECL(HashTableItem, hashFunction, char* str, long* output);
END_FUNCTIONS(HashTableItem);

FUNCTIONS(HashTable);
MEM_FUN_DECL(HashTable, createTable, int size, HashTableItem** output);
END_FUNCTIONS(HashTable);

