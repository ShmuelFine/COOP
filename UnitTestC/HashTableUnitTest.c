//#include "HashTableUnitTest.h"
//FUN_IMPL(check_insert_SanityTest)
//{
//	// Arrange
//	CREATE(HashTable, hashTable), 20  CALL;
//	CREATE(DataItem, item), 6,8  CALL;
// 
//	// Act
//	int retVal = 0;
//	MFUN( &hashTable,  insert), item, &retVal  CALL;
//
//	// Assert
//	NTEST_ASSERT(retVal == 1);
//
//}END_FUN
//FUN_IMPL(check_insert_when_already_exists_then_throw)
//{
//	// Arrange
//	
//	CREATE(HashTable, hashTable), 20  CALL;
//	CREATE(DataItem, item1), 6, 8  CALL;
//	CREATE(DataItem, item2), 6, 7  CALL;
//	
//
//	// Act
//	int retVal = -1;
//	MFUN(&hashTable, insert), item1, &retVal  CALL;
//	MFUN(&hashTable, insert), item2, &retVal  CALL;
//
//	// Assert
//	NTEST_ASSERT(retVal == -1);
//
//}END_FUN
//FUN_IMPL(check_search_when_founded_then_return_instance)
//{
//	// Arrange
//	CREATE(HashTable, hashTable), 20  CALL;
//	CREATE(DataItem, item1), 6, 8  CALL;
//	CREATE(DataItem, item2), 4, 14  CALL;
//	CREATE(DataItem, item3), 6, 16  CALL;
//	CREATE(DataItem, item4), 45, 320  CALL;
//
//	// Act
//	DataItem* retVal = NULL;
//	ALLOC_VARIABLE(retVal, DataItem*);
//
//	MFUN(&hashTable, insert), item1,  retVal  CALL;
//	MFUN(&hashTable, insert), item2,  retVal  CALL;
//	MFUN(&hashTable, insert), item3,  retVal  CALL;
//	MFUN(&hashTable, insert), item4,  retVal  CALL;
//	MFUN(&hashTable, search), 6,  &retVal  CALL;
//
//	// Assert
//	// //why not?
//	//NTEST_ASSERT(*retVal  == item1);
//	NTEST_ASSERT(retVal->key == item1.key && retVal->data == item1.data);
//
//}END_FUN
//FUN_IMPL(check_search_when_not_founded_then_return_null)
//{
//	// Arrange
//	CREATE(HashTable, hashTable), 20  CALL;
//	CREATE(DataItem, item1), 6, 8  CALL;
//	CREATE(DataItem, item2), 4, 14  CALL;
//	CREATE(DataItem, item3), 6, 16  CALL;
//	CREATE(DataItem, item4), 45, 320  CALL;
//
//	// Act
//	DataItem* retVal = NULL;
//	ALLOC_VARIABLE(retVal, DataItem*);
//
//	MFUN(&hashTable, insert), item1, retVal  CALL;
//	MFUN(&hashTable, insert), item2, retVal  CALL;
//	MFUN(&hashTable, insert), item3, retVal  CALL;
//	MFUN(&hashTable, insert), item4, retVal  CALL;
//	MFUN(&hashTable, search), 1234, retVal  CALL;
//
//	// Assert
//	// //why not?
//	//NTEST_ASSERT(*retVal  == item1);
//	NTEST_ASSERT(retVal==NULL);
//
//}END_FUN
//FUN_IMPL(check_delete_when_exist_then_delete)
//{
//	// Arrange
//	CREATE(HashTable, hashTable), 20  CALL;
//	CREATE(DataItem, item1), 6, 8  CALL;
//	CREATE(DataItem, item2), 4, 14  CALL;
//
//	// Act
//	int retVal =-1;
//	
//
//	MFUN(&hashTable, insert), item1, &retVal  CALL;
//	MFUN(&hashTable, insert), item2, &retVal  CALL;
//
//	MFUN(&hashTable, deleteItem), item2.key, &retVal  CALL;
//	NTEST_ASSERT(retVal == 1);
//
//	MFUN(&hashTable, search), item2.key, retVal  CALL;
//	NTEST_ASSERT(retVal == 0);
//
//}END_FUN