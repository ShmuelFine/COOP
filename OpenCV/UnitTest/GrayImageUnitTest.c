#include "GrayImage.h"
#include "GrayImageUnitTest.h"
#include "ExportDefs.h"
#include "DynamicMemoryManagement.h"
#include "vector.h"

//
// Test: create_SanityTest
// Checks that a GrayImage can be created from a vector of pixels correctly.
//
TEST_FUN_IMPL(GrayImageTest, create_SanityTest)
{
    // Arrange
    CREATE(Vector_uint8_t, vec) CALL;
    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i % 255 CALL;
    }
    END_LOOP;

    // Act
    CREATE(GrayImage, img), 1920, 1080, & vec CALL;

    // Assert
    // (Nothing to check yet - creation sanity)
} END_FUN


//
// Test: clone_CreatesIndependentDeepCopy
// Verifies that clone() creates a true deep copy.
//
TEST_FUN_IMPL(GrayImageTest, clone)
{
    // Arrange
    CREATE(Vector_uint8_t, vec) CALL;
    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i % 255 CALL;
    }
    END_LOOP;
    CREATE(GrayImage, img), 1920, 1080, & vec CALL;

    // Act
    GrayImage* img_clone = NULL;
    MFUN(&img, clone), & img_clone CALL;

    // Assert
    // Check basic clone properties
    NTEST_ASSERT(img_clone != NULL);
    NTEST_ASSERT(img_clone->width == 1920);
    NTEST_ASSERT(img_clone->height == 1080);
    NTEST_ASSERT(img_clone->stride == 1920);
    NTEST_ASSERT(img_clone->offset == 0);
    NTEST_ASSERT(*(img_clone->refCount) == 1);

    // Assert - deep copy validation
    NTEST_ASSERT(img.image_buffer != img_clone->image_buffer);
    NTEST_ASSERT(img.refCount != img_clone->refCount);

    // Act - change original pixel to verify deep copy
    uint8_t* original_pixel_ptr = NULL;
    uint8_t* clone_pixel_ptr = NULL;
    const MEM_SIZE_T test_row = 500;
    const MEM_SIZE_T test_col = 500;

    MFUN(img_clone, get_pixel_ptr), test_row, test_col, & clone_pixel_ptr CALL;
    uint8_t original_value = *clone_pixel_ptr;

    MFUN(&img, get_pixel_ptr), test_row, test_col, & original_pixel_ptr CALL;
    *original_pixel_ptr = (original_value == 255) ? 0 : 255;

    // Assert - clone unaffected
    MFUN(img_clone, get_pixel_ptr), test_row, test_col, & clone_pixel_ptr CALL;
    NTEST_ASSERT(*clone_pixel_ptr == original_value);
}
END_FUN


//
// Test: clone_FromROI_IsCorrectAndContiguous
// Ensures that cloning from a Region of Interest (ROI) produces a proper contiguous image.
//
TEST_FUN_IMPL(GrayImageTest, clone_FromROI)
{
    // Arrange
    CREATE(Vector_uint8_t, vec) CALL;
    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i % 255 CALL;
    }
    END_LOOP;
    CREATE(GrayImage, img), 1920, 1080, & vec CALL;

    // Act
    GrayImage* img_clone = NULL;
    CREATE(GrayImage, img_roi), 1, 1, NULL CALL;
    MFUN(&img_roi, init_ROI), & img, 5, 5, 10, 10 CALL;
    MFUN(&img_roi, clone), & img_clone CALL;

    // Assert
    // Check clone dimensions and buffer independence
    NTEST_ASSERT(img_clone != NULL);
    NTEST_ASSERT(img_clone->width == 10);
    NTEST_ASSERT(img_clone->height == 10);
    NTEST_ASSERT(img_clone->stride == 10);
    NTEST_ASSERT(img_clone->offset == 0);
    NTEST_ASSERT(img_clone->image_buffer != img.image_buffer);

    // Assert - check pixel values from ROI area
    uint8_t* clone_ptr = NULL;
    MFUN(img_clone, get_pixel_ptr), 0, 0, & clone_ptr CALL;
    NTEST_ASSERT(*clone_ptr == 171);
    MFUN(img_clone, get_pixel_ptr), 9, 9, & clone_ptr CALL;
    NTEST_ASSERT(*clone_ptr == 120);

    // Assert - base image remains unchanged
    uint8_t* base_ptr = NULL;
    MFUN(&img, get_pixel_ptr), 0, 0, & base_ptr CALL;
    NTEST_ASSERT(*base_ptr == 1);
} END_FUN


//
// Test: init_copy_IncrementsAndDecrementsRefCount
// Verifies that init_copy() correctly
//
TEST_FUN_IMPL(GrayImageTest, init_copy)
{
    // Arrange
    CREATE(GrayImage, img1), 10, 5, NULL CALL;
    size_t* original_ref_count_ptr = img1.refCount;
    NTEST_ASSERT(*original_ref_count_ptr == 1);

    // Act
    IF(true)
    {
        CREATE(GrayImage, img2), 1, 1, NULL CALL;
        MFUN(&img2, init_copy), & img1 CALL;

        // Assert inside scope
        MEM_SIZE_T dest_w = 0;
        MFUN(&img2, get_width), & dest_w CALL;
        NTEST_ASSERT(dest_w == 10);
        NTEST_ASSERT(img2.refCount == original_ref_count_ptr);
        NTEST_ASSERT(*original_ref_count_ptr == 2);
    } END_IF

        // Assert - ref count after scope
        NTEST_ASSERT(*original_ref_count_ptr == 1);

    MEM_SIZE_T src_w_after = 0;
    MFUN(&img1, get_width), & src_w_after CALL;
    NTEST_ASSERT(src_w_after == 10);
}
END_FUN


//
// Test: init_move_TransfersOwnershipAndNullifiesSource
// Verifies that init_move() transfers ownership and clears the source image.
//
TEST_FUN_IMPL(GrayImageTest, init_move)
{
    // Arrange
    CREATE(GrayImage, img1), 10, 5, NULL CALL;
    size_t* original_ref_count_ptr = img1.refCount;
    uint8_t* original_buffer_ptr = img1.image_buffer;
    MEM_SIZE_T original_w = 0;
    MFUN(&img1, get_width), & original_w CALL;
    NTEST_ASSERT(original_w == 10);

    // Act
    CREATE(GrayImage, img2), 1, 1, NULL CALL;
    MFUN(&img2, init_move), & img1 CALL;

    // Assert
    MEM_SIZE_T dest_w = 0;
    MFUN(&img2, get_width), & dest_w CALL;
    NTEST_ASSERT(dest_w == 10);
    NTEST_ASSERT(img2.refCount == original_ref_count_ptr);
    NTEST_ASSERT(img2.image_buffer == original_buffer_ptr);
    NTEST_ASSERT(*img2.refCount == 1);

    // Assert - source cleared
    MEM_SIZE_T src_w_after = 99;
    MFUN(&img1, get_width), & src_w_after CALL;
    NTEST_ASSERT(src_w_after == 0);
    NTEST_ASSERT(img1.refCount == NULL);
    NTEST_ASSERT(img1.image_buffer == NULL);
}
END_FUN


//
// Test: equals_CorrectlyComparesPixels
// Checks that equals() compares pixel data correctly for full images and ROIs.
//
TEST_FUN_IMPL(GrayImageTest, equals)
{
    // Arrange
    uint8_t* p = NULL;
    uint8_t* temp_ptr = NULL;
    CREATE(GrayImage, img1), 5, 5, NULL CALL;
    CREATE(GrayImage, img2), 5, 5, NULL CALL;
    CREATE(GrayImage, img3), 5, 5, NULL CALL;

    MFUN(&img1, get_pixel_ptr), 0, 0, & temp_ptr CALL;
    *temp_ptr = 100;

    // Act
    GrayImage* result_eq = NULL;
    GrayImage* result_neq = NULL;
    MFUN(&img1, equals), & img2, & result_eq CALL;

    // Assert - difference map (img1 vs img2)
    MFUN(result_eq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 0);
    MFUN(result_eq, get_pixel_ptr), 4, 4, & p CALL;
    NTEST_ASSERT(*p == 255);

    // Act
    MFUN(&img2, equals), & img3, & result_neq CALL;

    // Assert - (img2 vs img3)
    MFUN(result_neq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 255);
    MFUN(result_neq, get_pixel_ptr), 4, 4, & p CALL;
    NTEST_ASSERT(*p == 255);

    DELETE(result_eq);
    DELETE(result_neq);

    // Arrange - test ROI comparison
    CREATE(GrayImage, base), 20, 20, NULL CALL;
    CREATE(GrayImage, roi1), 1, 1, NULL CALL;
    CREATE(GrayImage, roi2), 1, 1, NULL CALL;
    MFUN(&roi1, init_ROI), & base, 5, 5, 8, 8 CALL;
    MFUN(&roi2, init_ROI), & base, 5, 5, 8, 8 CALL;
    CREATE(GrayImage, roi3), 1, 1, NULL CALL;
    MFUN(&roi3, init_ROI), & base, 0, 0, 8, 8 CALL;

    MFUN(&roi1, get_pixel_ptr), 0, 0, & temp_ptr CALL;
    *temp_ptr = 77;

    // Act
    GrayImage* result_roi_eq = NULL;
    GrayImage* result_roi_neq = NULL;
    MFUN(&roi1, equals), & roi2, & result_roi_eq CALL;

    // Assert
    MFUN(result_roi_eq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 255);

    // Act
    MFUN(&roi1, equals), & roi3, & result_roi_neq CALL;

    // Assert
    MFUN(result_roi_neq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 0);

    DELETE(result_roi_eq);
    DELETE(result_roi_neq);
}
END_FUN


/* ========= Suite Binding ========= */
// Binds all tests to the suite
INIT_TEST_SUITE(GrayImageTest)
BIND_TEST(GrayImageTest, create_SanityTest);
BIND_TEST(GrayImageTest, clone);
BIND_TEST(GrayImageTest, clone_FromROI);
BIND_TEST(GrayImageTest, equals);
BIND_TEST(GrayImageTest, init_copy);
BIND_TEST(GrayImageTest, init_move);
END_INIT_TEST_SUITE(GrayImageTest)
