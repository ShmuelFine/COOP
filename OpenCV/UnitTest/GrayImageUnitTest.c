#include "GrayImage.h"
#include "GrayImageUnitTest.h"
#include "ExportDefs.h"
#include "DynamicMemoryManagement.h"
#include "vector.h"

TEST_FUN_IMPL(GrayImageTest, create_SanityTest)
{
    // Arrange
    CREATE(Vector_uint8_t, vec) CALL;

    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i%255 CALL;
    }
    END_LOOP;

    // Act
	CREATE(GrayImage, img), 1920, 1080, &vec CALL;

    // Assert

} END_FUN

//
TEST_FUN_IMPL(GrayImageTest, clone_CreatesIndependentDeepCopy)
{
    CREATE(Vector_uint8_t, vec) CALL;
    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i % 255 CALL;
    }
    END_LOOP;
    CREATE(GrayImage, img), 1920, 1080, & vec CALL;


    // Act: 
    GrayImage* img_clone = NULL;
    MFUN(&img, clone), & img_clone CALL;

    // 1.(Shallow Copy Check) ---
    NTEST_ASSERT(img_clone != NULL);
    NTEST_ASSERT(img_clone->width == 1920);
    NTEST_ASSERT(img_clone->height == 1080);
    NTEST_ASSERT(img_clone->stride == 1920);
    NTEST_ASSERT(img_clone->offset == 0);
    NTEST_ASSERT(*(img_clone->refCount) == 1);

    NTEST_ASSERT(img.image_buffer != img_clone->image_buffer);
    NTEST_ASSERT(img.refCount != img_clone->refCount);

    //// 2.(Deep Copy Check) ---
    uint8_t* original_pixel_ptr = NULL;
    uint8_t* clone_pixel_ptr = NULL;
    const MEM_SIZE_T test_row = 500;
    const MEM_SIZE_T test_col = 500;

    MFUN(img_clone, get_pixel_ptr), test_row, test_col, & clone_pixel_ptr CALL;
    uint8_t original_value = *clone_pixel_ptr;

    MFUN(&img, get_pixel_ptr), test_row, test_col, & original_pixel_ptr CALL;
    *original_pixel_ptr = (original_value == 255) ? 0 : 255; 

    MFUN(img_clone, get_pixel_ptr), test_row, test_col, & clone_pixel_ptr CALL;
    NTEST_ASSERT(*clone_pixel_ptr == original_value);
}
END_FUN

TEST_FUN_IMPL(GrayImageTest, clone_FromROI_IsCorrectAndContiguous)
{
    CREATE(Vector_uint8_t, vec) CALL;
    FOR(int i = 1; i <= 2073600; ++i)
    {
        MFUN(&vec, push_back), i % 255 CALL;
    }
    END_LOOP;
    CREATE(GrayImage, img), 1920, 1080, & vec CALL;

    // img_clone
    GrayImage* img_clone = NULL;

    // ROI with VTable)
    CREATE(GrayImage, img_roi), 1, 1, NULL CALL;
    MFUN(&img_roi, init_ROI), & img, 5, 5, 10, 10 CALL;
    MFUN(&img_roi, clone), & img_clone CALL;

    // Assert 1: 
    NTEST_ASSERT(img_clone != NULL);
    NTEST_ASSERT(img_clone->width == 10);
    NTEST_ASSERT(img_clone->height == 10);
    NTEST_ASSERT(img_clone->stride == 10); 
    NTEST_ASSERT(img_clone->offset == 0);  
    NTEST_ASSERT(img_clone->image_buffer != img.image_buffer);

    // Assert 2:
    uint8_t* clone_ptr = NULL;

    // Index: 5*1920 + 5 = 9605. i = 9606. Value = 9606 % 255 = 171.
    MFUN(img_clone, get_pixel_ptr), 0, 0, & clone_ptr CALL;
    NTEST_ASSERT(*clone_ptr == 171);

    // Index: 14*1920 + 14 = 26894. i = 26895. Value = 26895 % 255 = 120.
    MFUN(img_clone, get_pixel_ptr), 9, 9, & clone_ptr CALL;
    NTEST_ASSERT(*clone_ptr == 120);

    // Assert 3:(img_base)-ROI
    uint8_t* base_ptr = NULL;

    //Index: 0. i = 1. Value = 1 % 255 = 1.
    MFUN(&img, get_pixel_ptr), 0, 0, & base_ptr CALL;
    NTEST_ASSERT(*base_ptr == 1);

}END_FUN

TEST_FUN_IMPL(GrayImageTest, equals_CorrectlyComparesPixels)
{
    uint8_t* p = NULL;
    uint8_t* temp_ptr = NULL;

    CREATE(GrayImage, img1), 5, 5, NULL CALL;
    CREATE(GrayImage, img2), 5, 5, NULL CALL;
    CREATE(GrayImage, img3), 5, 5, NULL CALL;

    // fill_image
    MFUN(&img1, get_pixel_ptr), 0, 0, & temp_ptr CALL;
    *temp_ptr = 100;

    GrayImage* result_eq = NULL;
    GrayImage* result_neq = NULL;

    MFUN(&img1, equals), & img2, & result_eq CALL;

    MFUN(result_eq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 0); 
    MFUN(result_eq, get_pixel_ptr), 4, 4, & p CALL;
    NTEST_ASSERT(*p == 255);

    // Act 2:
    MFUN(&img2, equals), & img3, & result_neq CALL;

    // Assert 2:
    MFUN(result_neq, get_pixel_ptr), 0, 0, & p CALL;
    NTEST_ASSERT(*p == 255);
    MFUN(result_neq, get_pixel_ptr), 4, 4, & p CALL;
    NTEST_ASSERT(*p == 255);

    DELETE(result_eq);
    DELETE(result_neq);

    CREATE(GrayImage, base), 20, 20, NULL CALL;

    CREATE(GrayImage, roi1), 1, 1, NULL CALL;
    CREATE(GrayImage, roi2), 1, 1, NULL CALL;
    MFUN(&roi1, init_ROI), & base, 5, 5, 8, 8 CALL;
    MFUN(&roi2, init_ROI), & base, 5, 5, 8, 8 CALL;

    CREATE(GrayImage, roi3), 1, 1, NULL CALL;
    MFUN(&roi3, init_ROI), & base, 0, 0, 8, 8 CALL;

    MFUN(&roi1, get_pixel_ptr), 0, 0, & temp_ptr CALL;
    *temp_ptr = 77;

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

INIT_TEST_SUITE(GrayImageTest)
BIND_TEST(GrayImageTest, create_SanityTest);
BIND_TEST(GrayImageTest, clone_CreatesIndependentDeepCopy);
BIND_TEST(GrayImageTest, clone_FromROI_IsCorrectAndContiguous);
BIND_TEST(GrayImageTest, equals_CorrectlyComparesPixels);
END_INIT_TEST_SUITE(GrayImageTest)