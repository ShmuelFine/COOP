#include "NdarrayTest.h"

TEST_FUN_IMPL(NDarray_Tests, init_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	//Act
	CREATE(Ndarray, arr), dims, shape CALL;

	//Assert
	NTEST_ASSERT(arr.size == 12);
	NTEST_ASSERT(arr.ndim == dims);
	for (size_t i = 0; i < dims; i++)
	{
		NTEST_ASSERT(arr.shape[i] == shape[i]);
	}

	for (size_t i = 0; i < arr.size; i++)
	{
		NTEST_ASSERT(arr.data[i] == 0);
	}
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, init_invalid_dims)
{
	//Arragne
	int dims = -1;
	int shape[] = { 0, 4 };
	//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	TRY
	{
		//Act
		CREATE(Ndarray, arr), dims, shape CALL;

	//Assert
	NTEST_ASSERT(false); // we shouldn't have gotten here.
	}
		CATCH{
		//NTEST_ASSERT(strcmp(LAST_EXCEPTION_ERROR_MSG, "dimension can't be negative")==0);
	}
	END_TRY;

}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, init_invalid_shape)
{
	//Arragne
	int dims = 2;
	int shape[] = { 0, 4 };
	//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;

	TRY{
		//Act
		CREATE(Ndarray, arr), dims, shape CALL;

	//Assert
	NTEST_ASSERT(false);
	}CATCH{
			NTEST_ASSERT(true);
	}END_TRY
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, get_location_3D)
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
	NTEST_ASSERT(actual == 59);

}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, set_sanity_case)
{	//Arrange
	int dims = 2;
	int shape[] = { 3, 4 };
	int pos[] = { 1,3 };
	//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;

	//Act
	FUN(&arr, set), pos, 5 CALL;

	//Assert
	NTEST_ASSERT(arr.data[7] == 5);

}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, set_index_out_of_range)
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
	NTEST_ASSERT(false);
	}
		CATCH{
			NTEST_ASSERT(true);
	}
		END_TRY
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, at_sanity_case)
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
	NTEST_ASSERT(actual == 5);
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, at_index_out_of_range)
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
	NTEST_ASSERT(false);
	}CATCH{

		NTEST_ASSERT(true);
	}
		END_TRY
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, fill_sanity_case)
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
		NTEST_ASSERT(arr.data[i] == val);
	}
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, contains_when_true)
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
	NTEST_ASSERT(actual == true);
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, contains_when_false)
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
	NTEST_ASSERT(actual == false);
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, min_sanity_case)
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
	NTEST_ASSERT(actual == -1.5);
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, max_sanity_case)
{
	// Arrange
	int dims = 3;
	int shape[] = { 3, 4 ,5 };
	//CREATE(MemoryManager, memManager), sizeof(int) * 10, HEAP_BASED_MEMORY CALL;
	CREATE(Ndarray, arr), dims, shape CALL;
	CREATE(Ndarray, arr1), dims, shape CALL;
	CREATE(Ndarray, arr2), dims, shape CALL;
	CREATE(Ndarray, arr3), dims, shape CALL;
	CREATE(Ndarray, arr4), dims, shape CALL;

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
	NTEST_ASSERT(actual == 7);
}
END_FUN

TEST_FUN_IMPL(NDarray_Tests, general_test)
{
	// Arrange
	int dims = 4;
	int shape[] = { 2,3, 4 ,5 };

	//Act

	CREATE(Ndarray, arr), dims, shape CALL;
	float value = 0;
	for (int i = 0; i < shape[0]; i++) {
		for (int j = 0; j < shape[1]; j++) {
			for (int k = 0; k < shape[2]; k++) {
				for (int l = 0; l < shape[3]; l++) {
					int coords[] = { i,j,k,l };
					FUN(&arr, set), coords, value CALL;
					value += 1;
				}
			}
		}
	}

	//Assert
	NTEST_ASSERT(arr.size == shape[0] * shape[1] * shape[2] * shape[3]);
	for (int i = 0; i < arr.size; i++) {
		NTEST_ASSERT(arr.data[i] == i);
	}
	value = 0;
	for (int i = 0; i < shape[0]; i++) {
		for (int j = 0; j < shape[1]; j++) {
			for (int k = 0; k < shape[2]; k++) {
				for (int l = 0; l < shape[3]; l++) {
					int coords[] = { i,j,k,l };
					float actual_value = 0;
					FUN(&arr, at), coords, & actual_value CALL;
					NTEST_ASSERT(actual_value == value);
					value += 1;
				}
			}
		}
	}
}
END_FUN




INIT_TEST_SUITE(NDarray_Tests)
BIND_TEST(NDarray_Tests, init_sanity_case)
BIND_TEST(NDarray_Tests, init_invalid_dims)
BIND_TEST(NDarray_Tests, init_invalid_shape)
BIND_TEST(NDarray_Tests, get_location_3D)
BIND_TEST(NDarray_Tests, set_sanity_case)
BIND_TEST(NDarray_Tests, set_index_out_of_range)
BIND_TEST(NDarray_Tests, at_sanity_case)
BIND_TEST(NDarray_Tests, at_index_out_of_range)
BIND_TEST(NDarray_Tests, fill_sanity_case)
BIND_TEST(NDarray_Tests, contains_when_true)
BIND_TEST(NDarray_Tests, contains_when_false)
BIND_TEST(NDarray_Tests, min_sanity_case)
BIND_TEST(NDarray_Tests, max_sanity_case)
BIND_TEST(NDarray_Tests, general_test)
END_INIT_TEST_SUITE(NDarray_Tests)