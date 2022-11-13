#include "NdarrayTest.h"

FUN_IMPL(Ndarray_init_sanity_case)
{
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 0,0 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), 2, shape);

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
{
	int dims = 2;
	int shape[] = { 0, 4 };
	int pos[] = { 0,0 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), 2, shape);

	
	TEST_ASSERT(arr.data == NULL);

}
END_FUN

FUN_IMPL(Ndarray_set_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY);
	CREATE(Ndarray, arr), 2, shape);
	//Act
	
	FUN(&arr, set), pos, 5 CALL;
	
	TEST_ASSERT(arr.data[7] == 5);
	
}
END_FUN