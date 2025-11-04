#include "GrayImage.h"
#include "GrayImageUnitTest.h"
#include "ExportDefs.h"
#include "DynamicMemoryManagement.h"
#include "vector.h"

TEST_FUN_IMPL(GrayImageTest, create_SanityTest)
{
    init_global_memory(sizeof(int) * 10, HEAP_BASED_MEMORY);

    // Arrange
    CREATE(Vector_uint8_t, vec) CALL;

    FOR(int i = 1; i <= 10000; ++i)
    {
        MFUN(&vec, push_back), i%255 CALL;
    }
    END_LOOP;
    // Act
	CREATE(GrayImage, img), 100, 100, &vec CALL;

    // Assert

} END_FUN


/* ========= Suite Binding ========= */

INIT_TEST_SUITE(GrayImageTest)
BIND_TEST(GrayImageTest, create_SanityTest);
END_INIT_TEST_SUITE(GrayImageTest)