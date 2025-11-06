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

TEST_FUN_IMPL(GrayImageTest, add_saturate_basic3x3)
{
    // Arrange
    const uint8_t A[] = {
        0,  10, 250,
        20, 200, 100,
        255, 1,  128
    };
    const uint8_t B[] = {
        0,  250, 10,
        50, 100, 200,
        10,  5,  200
    };
    /* expected: min(A+B,255) */
    const uint8_t E[] = {
        0,  255, 255,
        70, 255, 255,
        255, 6,  255
    };

    CREATE(Vector_uint8_t, va) CALL;
    CREATE(Vector_uint8_t, vb) CALL;
    FOR(MEM_SIZE_T i = 0; i < 9; ++i) 
    {
        MFUN(&va, push_back), A[i] CALL;
        MFUN(&vb, push_back), B[i] CALL;
    } 
    END_LOOP;

    CREATE(GrayImage, a), 3, 3, & va CALL;
    CREATE(GrayImage, b), 3, 3, & vb CALL;
    CREATE(GrayImage, out), 3, 3, NULL CALL;

    // Act
    MFUN(&a, add), & b, & out CALL;

    // Assert
    FOR(MEM_SIZE_T r = 0; r < 3; ++r)
    {
        const uint8_t* row = out.image_buffer + out.offset + r * out.stride;
        FOR(MEM_SIZE_T c = 0; c < 3; ++c)
        {
            uint8_t got = row[c];
            uint8_t exp = E[r * 3 + c];
            IF (got != exp) {
                printf("add mismatch at (r=%u,c=%u): got=%u exp=%u\n",
                    (unsigned)r, (unsigned)c, (unsigned)got, (unsigned)exp);
                fflush(stdout);
            }
            END_IF;
            THROW_MSG_UNLESS(got == exp, "add: Pixel mismatch");
        }
        END_LOOP;
    }
    END_LOOP;

}
END_FUN

TEST_FUN_IMPL(GrayImageTest, sub_default_zero_3x3)
{
    // Arrange
    const uint8_t A[] = {
        5,  0,  50,
        100, 10, 3,
        200, 30, 0
    };
    const uint8_t B[] = {
        10, 0,  40,
        80,  20, 5,
        250, 20, 1
    };
    /* expected: max(A-B, 0) */
    const uint8_t E[] = {
        0,  0,  10,
        20,  0,  0,
        0,   10, 0
    };

    CREATE(Vector_uint8_t, va) CALL;
    CREATE(Vector_uint8_t, vb) CALL;
    FOR(MEM_SIZE_T i = 0; i < 9; ++i)
    {
        MFUN(&va, push_back), A[i] CALL; 
        MFUN(&vb, push_back), B[i] CALL;
    }
    END_LOOP;

    CREATE(GrayImage, a), 3, 3, & va CALL;
    CREATE(GrayImage, b), 3, 3, & vb CALL;
    CREATE(GrayImage, out), 3, 3, NULL CALL;

    // Act
    MFUN(&a, sub_default), & b, & out CALL;

    // Assert
    FOR(MEM_SIZE_T r = 0; r < 3; ++r)
    {
        const uint8_t* row = out.image_buffer + out.offset + r * out.stride;
        FOR(MEM_SIZE_T c = 0; c < 3; ++c)
        {
            uint8_t got = row[c];
            uint8_t exp = E[r * 3 + c];
            THROW_MSG_UNLESS(got == exp, "sub_default: Pixel mismatch");
        }
        END_LOOP;
    }
    END_LOOP;

}
END_FUN

/* ========= sub_abs: |A-B| ========= */
TEST_FUN_IMPL(GrayImageTest, sub_abs_matches_absdiff_3x3)
{
    // Arrange
    const uint8_t A[] = {
        0,  100, 255,
        10, 200,  50,
        80,  30,  90
    };
    const uint8_t B[] = {
        10, 50,  0,
        20, 150, 100,
        80, 60,  20
    };
    const uint8_t E[] = {
        10, 50, 255,
        10, 50,  50,
        0,  30,  70
    };

    CREATE(Vector_uint8_t, va) CALL;
    CREATE(Vector_uint8_t, vb) CALL;
    FOR(MEM_SIZE_T i = 0; i < 9; ++i) 
    {
        MFUN(&va, push_back), A[i] CALL;
        MFUN(&vb, push_back), B[i] CALL;
    }
    END_LOOP;

    CREATE(GrayImage, a), 3, 3, & va CALL;
    CREATE(GrayImage, b), 3, 3, & vb CALL;
    CREATE(GrayImage, out), 3, 3, NULL CALL;

    // Act
    MFUN(&a, sub_abs), & b, & out CALL;

    // Assert
    FOR(MEM_SIZE_T r = 0; r < 3; ++r)
    {
        const uint8_t* row = out.image_buffer + out.offset + r * out.stride;
        FOR(MEM_SIZE_T c = 0; c < 3; ++c)
        {
            uint8_t got = row[c];
            uint8_t exp = E[r * 3 + c];
            THROW_MSG_UNLESS(got == exp, "sub_abs: Pixel mismatch");
        }
        END_LOOP;
    }
    END_LOOP;

}
END_FUN

/* ========= mul_scalar: round + saturate ========= */
TEST_FUN_IMPL(GrayImageTest, mul_scalar_round_and_saturate_3x3)
{
    // Arrange
    const uint8_t A[] = {
        0,  1,  2,
        100, 101, 170,
        171, 200, 255
    };
    /* expected = clamp(round(A*1.5), 0..255) */
    const uint8_t E[] = {
        0,   2,   3,
        150, 152, 255,
        255, 255, 255
    };

    CREATE(Vector_uint8_t, va) CALL;
    FOR(MEM_SIZE_T i = 0; i < 9; ++i)
    {
        MFUN(&va, push_back), A[i] CALL; 
    }
    END_LOOP;

    CREATE(GrayImage, a), 3, 3, & va CALL;
    CREATE(GrayImage, out), 3, 3, NULL CALL;

    // Act
    MFUN(&a, mul_scalar), 1.5, & out CALL;

    // Assert
    FOR(MEM_SIZE_T r = 0; r < 3; ++r)
    {
        const uint8_t* row = out.image_buffer + out.offset + r * out.stride;
        FOR(MEM_SIZE_T c = 0; c < 3; ++c)
        {
            uint8_t got = row[c];
            uint8_t exp = E[r * 3 + c];
            THROW_MSG_UNLESS(got == exp, "mul_scalar: Pixel mismatch");
        }
        END_LOOP;
    }
    END_LOOP;

}
END_FUN

/* ========= mul_mat: Linear matrix multiplication ========= */
TEST_FUN_IMPL(GrayImageTest, mul_mat_linear_multiply_2x2)
{
    // Arrange
    /* A = [ [1, 2, 3],
             [4, 5, 6] ]  (2�3)  */
    const uint8_t A[] = {
        1, 2, 3,
        4, 5, 6
    };

    /* B = [ [7,  8],
             [9, 10],
             [11,12] ] (3�2) */
    const uint8_t B[] = {
         7,  8,
         9, 10,
        11, 12
    };

    /* Out = A*B = [ [ 58,  64],
                     [139, 154] ]  (����� 0..255) */
    const uint8_t E[] = {
        58,  64,
        139, 154
    };

    CREATE(Vector_uint8_t, va) CALL;
    CREATE(Vector_uint8_t, vb) CALL;
    FOR(MEM_SIZE_T i = 0; i < 6; ++i) 
    {
        MFUN(&va, push_back), A[i] CALL; 
        MFUN(&vb, push_back), B[i] CALL;
    }
    END_LOOP;

    CREATE(GrayImage, a), 3, 2, & va CALL;
    CREATE(GrayImage, b), 2, 3, & vb CALL;
    CREATE(GrayImage, out), 2, 2, NULL CALL;

    // Act
    MFUN(&a, mul_mat), & b, & out CALL;

    // Assert
    FOR(MEM_SIZE_T r = 0; r < 2; ++r)
    {
        const uint8_t* row = out.image_buffer + out.offset + r * out.stride;
        FOR(MEM_SIZE_T c = 0; c < 2; ++c)
        {
            uint8_t got = row[c];
            uint8_t exp = E[r * 2 + c];
            THROW_MSG_UNLESS(got == exp, "mul_mat: Pixel mismatch");
        }
        END_LOOP;
    }
    END_LOOP;

}
END_FUN

/* ========= Suite Binding ========= */

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
BIND_TEST(GrayImageTest, add_saturate_basic3x3);
BIND_TEST(GrayImageTest, sub_default_zero_3x3);
BIND_TEST(GrayImageTest, sub_abs_matches_absdiff_3x3);
BIND_TEST(GrayImageTest, mul_scalar_round_and_saturate_3x3);
BIND_TEST(GrayImageTest, mul_mat_linear_multiply_2x2);
END_INIT_TEST_SUITE(GrayImageTest)
