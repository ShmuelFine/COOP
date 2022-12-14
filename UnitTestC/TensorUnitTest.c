#include "TensorUnitTest.h"
#include "ExportDefs.h"
#include "Tensor.h"
#include "ScopeTester.h"


TEST_FUN_IMPL(TensorTest, ctor_Sanity)
{
	// Arrange
	MEM_SIZE_T dims = 2;
	MEM_SIZE_T shape[] = { 3, 4 };

	// Act
	CREATE(Tensor_int, tensor), dims, shape CALL;

	// Assert
	NTEST_ASSERT(tensor._base.size_bytes == 3 * 4 * sizeof(int));
	NTEST_ASSERT(tensor._base.shape._base.size == dims);

	FOR (MEM_SIZE_T i = 0; i < dims; i++)
	{
		int ith_dim = 0;
		MFUN(&tensor._base.shape, get), i, & ith_dim CALL;
		NTEST_ASSERT(ith_dim == shape[i]);
	}END_LOOP;
}
END_FUN

TEST_FUN_IMPL(TensorTest, ctor_ThrowsWhenInvalidDims)
{
	// Arragne
	MEM_SIZE_T dims = 0;
	MEM_SIZE_T shape[] = { 0, 4 };

	// Act, Assert
	EXPECT_THROW;
	CREATE(Tensor_int, arr), dims, shape CALL;
	ASSERT_THROW;
}
END_FUN

TEST_FUN_IMPL(TensorTest, ctor_ThrowsWhenInvalidShape)
{
	// Arragne
	MEM_SIZE_T dims = 2;
	MEM_SIZE_T shape[] = { 0, 4 };

	// Act, Assert
	EXPECT_THROW;
	CREATE(Tensor_int, arr), dims, shape CALL;
	ASSERT_THROW;
}
END_FUN

TEST_FUN_IMPL(TensorTest, at_ThrowsWhenIdxIsOutOfRange)
{
	// Arrange
	MEM_SIZE_T shape[] = { 1,2,3 };
	CREATE(Tensor_int, v1), 3, shape CALL;

	int* retValPtr = NULL;
	MEM_SIZE_T pos[] = { 0,0,3 };

	// Act, Assert
	EXPECT_THROW;
	MFUN(&v1, at), pos, & retValPtr CALL;
	ASSERT_THROW;

}END_FUN

TEST_FUN_IMPL(TensorTest, get_set_Sanity)
{
	// Arrange
	MEM_SIZE_T dims = 4;
	MEM_SIZE_T shape[] = { 3, 4, 5, 2 };

	CREATE(Tensor_int, tensor), dims, shape CALL;

	// Act, Assert
	{
		int value = 0;
		FOR (MEM_SIZE_T i = 0; i < shape[0]; i++) {
			FOR (MEM_SIZE_T j = 0; j < shape[1]; j++) {
				FOR (MEM_SIZE_T k = 0; k < shape[2]; k++) {
					FOR (MEM_SIZE_T l = 0; l < shape[3]; l++) {
						MEM_SIZE_T coords[] = { i,j,k,l };
						MFUN(&tensor, set), coords, value CALL;
						value++;
					}END_LOOP;
				}END_LOOP;
			}END_LOOP;
		}END_LOOP;
	}

	{
		int expectedValue = 0;
		int actualValue = 0;
		FOR (MEM_SIZE_T i = 0; i < shape[0]; i++) {
			FOR (MEM_SIZE_T j = 0; j < shape[1]; j++) {
				FOR (MEM_SIZE_T k = 0; k < shape[2]; k++) {
					FOR (MEM_SIZE_T l = 0; l < shape[3]; l++) {
						MEM_SIZE_T coords[] = { i,j,k,l };
						MFUN(&tensor, get), coords, &actualValue CALL;
						NTEST_ASSERT(actualValue == expectedValue);
						expectedValue++;
					}END_LOOP;
				}END_LOOP;
			}END_LOOP;
		}END_LOOP;
	}
}END_FUN

TEST_FUN_IMPL(TensorTest, reshape_Sanity)
{
	// Arrange
	MEM_SIZE_T orig_dims = 4;
	MEM_SIZE_T orig_shape[] = { 3, 4, 5, 2 };
	MEM_SIZE_T new_dims = 2;
	MEM_SIZE_T new_shape[] = { 3 * 4, 5 * 2 };

	CREATE(Tensor_int, tensor), orig_dims, orig_shape CALL;

	// Act
	MFUN(&tensor, reshape), new_dims, new_shape CALL;

	// Assert
	NTEST_ASSERT(new_dims == tensor._base.shape._base.size);
	FOR (MEM_SIZE_T i = 0; i < new_dims; i++)
	{
		int ith_dim = 0;
		MFUN(&tensor._base.shape, get), i, & ith_dim CALL;
		NTEST_ASSERT(new_shape[i] == ith_dim);
	}END_LOOP;
}
END_FUN

TEST_FUN_IMPL(TensorTest, reshape_ThrowsWhenDimensionsDoNotMatch)
{
	// Arrange
	MEM_SIZE_T orig_dims = 4;
	MEM_SIZE_T orig_shape[] = { 3, 4, 5, 2 };
	MEM_SIZE_T new_dims = 2;
	MEM_SIZE_T new_shape[] = { 3 * 4 - 1, 5 * 2 };

	CREATE(Tensor_int, tensor), orig_dims, orig_shape CALL;

	// Act, Assert
	EXPECT_THROW;
	MFUN(&tensor, reshape), new_dims, new_shape CALL;
	ASSERT_THROW;
}
END_FUN

TEST_FUN_IMPL(TensorTest, zero_all_Sanity)
{
	// Arrange
	MEM_SIZE_T dims = 3;
	MEM_SIZE_T shape[] = { 1, 5, 2, 3 };

	CREATE(Tensor_int, tensor), dims, shape CALL;
	{
		int value = 0;
		FOR (MEM_SIZE_T i = 0; i < shape[0]; i++) {
			FOR (MEM_SIZE_T j = 0; j < shape[1]; j++) {
				FOR (MEM_SIZE_T k = 0; k < shape[2]; k++) {
					FOR (MEM_SIZE_T l = 0; l < shape[3]; l++) {
						MEM_SIZE_T coords[] = { i,j,k,l };
						MFUN(&tensor, set), coords, value CALL;
						value++;
					}END_LOOP;
				}END_LOOP;
			}END_LOOP;
		}END_LOOP;
	}

	// Act
	MFUN(&tensor, zero_all) CALL;

	// Assert
	{
		FOR (MEM_SIZE_T i = 0; i < shape[0]; i++) {
			FOR (MEM_SIZE_T j = 0; j < shape[1]; j++) {
				FOR (MEM_SIZE_T k = 0; k < shape[2]; k++) {
					FOR (MEM_SIZE_T l = 0; l < shape[3]; l++) {
						MEM_SIZE_T coords[] = { i,j,k,l };
						int actualValue = 10;
						MFUN(&tensor, get), coords, & actualValue CALL;
						NTEST_ASSERT(0 == actualValue);
					}END_LOOP;
				}END_LOOP;
			}END_LOOP;
		}END_LOOP;
	}

}
END_FUN


INIT_TEST_SUITE(TensorTest)
BIND_TEST(TensorTest, ctor_Sanity);
BIND_TEST(TensorTest, ctor_ThrowsWhenInvalidDims);
BIND_TEST(TensorTest, ctor_ThrowsWhenInvalidShape);
BIND_TEST(TensorTest, at_ThrowsWhenIdxIsOutOfRange);
BIND_TEST(TensorTest, get_set_Sanity);
BIND_TEST(TensorTest, reshape_Sanity);
BIND_TEST(TensorTest, reshape_ThrowsWhenDimensionsDoNotMatch);
BIND_TEST(TensorTest, zero_all_Sanity);
END_INIT_TEST_SUITE(TensorTest)