#include "HashTableUnitTest.h"
FUN_IMPL(check_insert_SanityTest)
{
	// Arrange
	CREATE(HashTable, hashTable), 20 );
	CREATE(DataItem, item), 6,8 );
 
	// Act
	int retVal = 0;
	FUN( &hashTable,  insert), item, &retVal  CALL;

	// Assert
	TEST_ASSERT(retVal == 1);

}END_FUN
FUN_IMPL(check_insert_when_already_exists_then_throw)
{
	// Arrange
	
	CREATE(HashTable, hashTable), 20 );
	CREATE(DataItem, item1), 6, 8 );
	CREATE(DataItem, item2), 6, 7 );
	

	// Act
	int retVal = -1;
	FUN(&hashTable, insert), item1, &retVal  CALL;
	FUN(&hashTable, insert), item2, &retVal  CALL;

	// Assert
	TEST_ASSERT(retVal == -1);

}END_FUN
FUN_IMPL(check_search_when_founded_then_return_instance)
{
	// Arrange
	CREATE(HashTable, hashTable), 20 );
	CREATE(DataItem, item1), 6, 8 );
	CREATE(DataItem, item2), 4, 14 );
	CREATE(DataItem, item3), 6, 16 );
	CREATE(DataItem, item4), 45, 320 );

	// Act
	DataItem* retVal = NULL;
	NEW(retVal, DataItem*);

	FUN(&hashTable, insert), item1,  retVal  CALL;
	FUN(&hashTable, insert), item2,  retVal  CALL;
	FUN(&hashTable, insert), item3,  retVal  CALL;
	FUN(&hashTable, insert), item4,  retVal  CALL;
	FUN(&hashTable, search), 6,  &retVal  CALL;

	// Assert
	// //why not?
	//TEST_ASSERT(*retVal  == item1);
	TEST_ASSERT(retVal->key == item1.key && retVal->data == item1.data);

}END_FUN
FUN_IMPL(check_search_when_not_founded_then_return_null)
{
	// Arrange
	CREATE(HashTable, hashTable), 20 );
	CREATE(DataItem, item1), 6, 8 );
	CREATE(DataItem, item2), 4, 14 );
	CREATE(DataItem, item3), 6, 16 );
	CREATE(DataItem, item4), 45, 320 );

	// Act
	DataItem* retVal = NULL;
	NEW(retVal, DataItem*);

	FUN(&hashTable, insert), item1, retVal  CALL;
	FUN(&hashTable, insert), item2, retVal  CALL;
	FUN(&hashTable, insert), item3, retVal  CALL;
	FUN(&hashTable, insert), item4, retVal  CALL;
	FUN(&hashTable, search), 1234, retVal  CALL;

	// Assert
	// //why not?
	//TEST_ASSERT(*retVal  == item1);
	TEST_ASSERT(retVal==NULL);

}END_FUN
FUN_IMPL(check_delete_when_exist_then_delete)
{
	// Arrange
	CREATE(HashTable, hashTable), 20 );
	CREATE(DataItem, item1), 6, 8 );
	CREATE(DataItem, item2), 4, 14 );

	// Act
	int retVal =-1;
	

	FUN(&hashTable, insert), item1, &retVal  CALL;
	FUN(&hashTable, insert), item2, &retVal  CALL;

	FUN(&hashTable, deleteItem), item2.key, &retVal  CALL;
	TEST_ASSERT(retVal == 1);

	FUN(&hashTable, search), item2.key, retVal  CALL;
	TEST_ASSERT(retVal == 0);

}END_FUN