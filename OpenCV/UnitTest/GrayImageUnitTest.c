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
             [4, 5, 6] ]  (2Є3)  */
    const uint8_t A[] = {
        1, 2, 3,
        4, 5, 6
    };

    /* B = [ [7,  8],
             [9, 10],
             [11,12] ] (3Є2) */
    const uint8_t B[] = {
         7,  8,
         9, 10,
        11, 12
    };

    /* Out = A*B = [ [ 58,  64],
                     [139, 154] ]  (бъзен 0..255) */
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

INIT_TEST_SUITE(GrayImageTest)
BIND_TEST(GrayImageTest, create_SanityTest);
BIND_TEST(GrayImageTest, add_saturate_basic3x3);
BIND_TEST(GrayImageTest, sub_default_zero_3x3);
BIND_TEST(GrayImageTest, sub_abs_matches_absdiff_3x3);
BIND_TEST(GrayImageTest, mul_scalar_round_and_saturate_3x3);
BIND_TEST(GrayImageTest, mul_mat_linear_multiply_2x2);
END_INIT_TEST_SUITE(GrayImageTest)