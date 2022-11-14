#include "NdarrayTest.h"

FUN_IMPL(Ndarray_init_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	//Act
	CREATE(Ndarray, arr), dims, shape CALL;

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

FUN_IMPL(Ndarray_init_invalid_dims)
{	//Arragne
	int dims = -1;
	int shape[] = { 0, 4 };
	//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	TRY{
		//Act
		CREATE(Ndarray, arr), dims, shape CALL;

	//Assert
	TEST_ASSERT(false); // we shouldn't have gotten here.
	}
	CATCH{
			TEST_ASSERT(true);
	}
	END_TRY

}
END_FUN

FUN_IMPL(Ndarray_init_invalid_shape)
{	//Arragne
	int dims = 2;
	int shape[] = { 0, 4 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	TRY{
		//Act
		CREATE(Ndarray, arr), dims, shape CALL;

	//Assert
	TEST_ASSERT(false); // we shouldn't have gotten here.
	}CATCH{
			TEST_ASSERT(true);
	}END_TRY
}
END_FUN

FUN_IMPL(Ndarray_get_location_3D)
{	//Arrange
	int dims = 3;
	int shape[] = { 3, 4, 5 };
	int pos[] = { 2,3,4 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	int actual = -1;
	FUN(&arr, get_location), pos, & actual CALL;

	//Assert
	TEST_ASSERT(actual == 59);

}
END_FUN

FUN_IMPL(Ndarray_set_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	FUN(&arr, set), pos, 5 CALL;

	//Assert
	TEST_ASSERT(arr.data[7] == 5);

}
END_FUN

FUN_IMPL(Ndarray_set_index_out_of_range)
{
	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,4 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	TRY{
		//Act
		FUN(&arr, set), pos, 5 CALL;

		//Assert
		TEST_ASSERT(false); 
	}
	CATCH{
		TEST_ASSERT(true);
	}
	END_TRY
}
END_FUN

FUN_IMPL(Ndarray_at_sanity_case)
{
	// Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	FUN(&arr, set), pos, 5 CALL;
	float actual = 0;
	FUN(&arr, at), pos, & actual  CALL;

	//Assert
	TEST_ASSERT(actual == 5);
}
END_FUN

FUN_IMPL(Ndarray_at_index_out_of_range)
{	// Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 4,3 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;
	
	TRY{
		//Act
		FUN(&arr, set), pos, 5 CALL;

		//Assert
		TEST_ASSERT(false);
	}CATCH{

		TEST_ASSERT(true);
	}
	END_TRY
}
END_FUN

FUN_IMPL(Ndarray_fill_sanity_case)
{
	// Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	float val = 5;
	FUN(&arr, fill), val CALL;

	//Assert

	for (size_t i = 0; i < arr.size; i++)
	{
		TEST_ASSERT(arr.data[i] == val);
	}
}
END_FUN

FUN_IMPL(Ndarray_contains_when_true)
{
	// Arrange
	int dims = 3;
	int shape[] = { 3, 4 ,5 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;
	int pos[] = { 2,2,2 };

	//Act
	float val = 5;
	FUN(&arr, set), pos, val CALL;
	bool actual;
	FUN(&arr, contains), val, & actual CALL;

	//Assert
	TEST_ASSERT(actual == true);
}
END_FUN

FUN_IMPL(Ndarray_contains_when_false)
{
	// Arrange
	int dims = 3;
	int shape[] = { 3, 4 ,5 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	float val = 5;
	bool actual;
	FUN(&arr, contains), val, & actual CALL;

	//Assert
	TEST_ASSERT(actual == false);
}
END_FUN

FUN_IMPL(Ndarray_min_sanity_case)
{
	// Arrange
	int dims = 3;
	int shape[] = { 3, 4 ,5 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//float values[] = { 4,5,2,7,4,3,6 };
	//for (int i = 0; i < sizeof(values) / sizeof(float); i++)
	//{
	//	int pos[] = { i%shape[0],(i+1)%shape[2],(i+2)%shape[3]};
	//	FUN(&arr, set),pos, values[i] CALL;
	//}
	int pos[] = { 0,1,2 };
	FUN(&arr, set), pos, -1 CALL;
	int pos2[] = { 0,1,4 };
	FUN(&arr, set), pos2, -1.5 CALL;

	//Act
	float actual;
	FUN(&arr, min), & actual CALL;

	//Assert
	TEST_ASSERT(actual == -1.5);
}
END_FUN

FUN_IMPL(Ndarray_max_sanity_case)
{
	// Arrange
	int dims = 3;
	int shape[] = { 3, 4 ,5 };
//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//float values[] = { 4,5,2,7,4,3,6 };
	//for (int i = 0; i < sizeof(values) / sizeof(float); i++)
	//{
	//	int pos[] = { i%shape[0],(i+1)%shape[2],(i+2)%shape[3]};
	//	FUN(&arr, set),pos, values[i] CALL;
	//}
	int pos[] = { 0,1,2 };
	FUN(&arr, set), pos, 7 CALL;
	int pos2[] = { 0,1,4 };
	FUN(&arr, set), pos2, 2 CALL;

	//Act
	float actual;
	FUN(&arr, max), & actual CALL;

	//Assert
	TEST_ASSERT(actual == 7);
}
END_FUN