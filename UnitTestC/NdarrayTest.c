#include "NdarrayTest.h"

FUN_IMPL(Ndarray_init_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 0,0 };

	//Act
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Assert
	TEST_ASSERT(arr.size == 12);
	TEST_ASSERT(arr.ndim == dims);
	for (size_t i = 0; i < dims; i++)
	{
		TEST_ASSERT(arr.shape[i] == shape[i]);
	}	

	for (size_t i = 0; i < arr.size; i++)
	{
		TEST_ASSERT(arr.data[i] == 0);
	}
}
END_FUN

FUN_IMPL(Ndarray_init_invalid_shape)
{	//Arragne
	int dims = 2;
	int shape[] = { 0, 4 };
	int pos[] = { 0,0 };

	//Act
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Assert
	TEST_ASSERT(arr.data == NULL);

}
END_FUN

FUN_IMPL(Ndarray_get_location_3D)
{	//Arrange
	int dims = 3;
	int shape[] = { 3, 4, 5 };
	int pos[] = { 2,3,4 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Act
	int actual = -1;
	FUN(&arr, get_location), pos, &actual CALL;

	//Assert
	TEST_ASSERT(actual == 59);

}
END_FUN

FUN_IMPL(Ndarray_set_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);
	
	//Act
	FUN(&arr, set), pos, 5 CALL;
	
	//Assert
	TEST_ASSERT(arr.data[7] == 5);
	
}
END_FUN

FUN_IMPL(Ndarray_set_index_out_of_range)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 2,4 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Act
	FUN(&arr, set), pos, 5 CALL;

	//Assert
	//TEST_ASSERT(arr.data[12] == 5);
}
END_FUN

FUN_IMPL(Ndarray_at_sanity_case)
{
	// Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Act
	FUN(&arr, set), pos, 5 CALL;
	float actual = 0;
	FUN(&arr, at), pos, &actual  CALL;

	//Assert
	TEST_ASSERT(actual == 5);
}
END_FUN

FUN_IMPL(Ndarray_at_index_out_of_range)
{
}
END_FUN

FUN_IMPL(Ndarray_fill_sanity_case)
{
	// Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), dims, shape);

	//Act
	int val = 5;
	FUN(&arr, fill),val CALL;
	
	//Assert

	for (size_t i = 0; i < arr.size; i++)
	{
		TEST_ASSERT(arr.data[i] == val);
	}
}
END_FUN