#include "GrayImage.h"
#include <stdint.h>

static const uint8_t GAUSSIAN_KERNEL_VALS[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
static const int GAUSSIAN_DIVISOR = 16;

FUN_IMPL(__gaussian_blur, GrayImage* img)
{
    THROW_MSG_UNLESS(img, "Input image pointer is NULL");
    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;
    THROW_MSG_UNLESS(width >= 3 && height >= 3, "Image dimensions must be at least 3x3 for a 3x3 Gaussian kernel.");

    CREATE(GrayImage, temp_img) CALL;
    MFUN(img, clone), & temp_img CALL;

    FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
    {
        FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
        {
            uint32_t sum = 0;

            FOR(int kr = -1; kr <= 1; ++kr)
            {
                FOR(int kc = -1; kc <= 1; ++kc)
                {
                    uint8_t* src_pixel = NULL;
                    MFUN(&temp_img, get_pixel_ptr), r + kr, c + kc, & src_pixel CALL;

                    int kernel_val = GAUSSIAN_KERNEL_VALS[kr + 1][kc + 1];
                    sum += (uint32_t)(*src_pixel) * (uint32_t)kernel_val;
                }
                END_LOOP;
            }
            END_LOOP;

            uint8_t new_val = (uint8_t)(sum / GAUSSIAN_DIVISOR);

            uint8_t* dst_pixel = NULL;
            MFUN(img, get_pixel_ptr), r, c, & dst_pixel CALL;
            *dst_pixel = new_val;
        }
        END_LOOP;
    }
    END_LOOP;

}END_FUN