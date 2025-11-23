#include "GaussianProcessor.h"


DEF_DERIVED_CTOR(GaussianProcessor, ImageProcessor) SUPER ME {} END_DERIVED_CTOR
DEF_DERIVED_DTOR(GaussianProcessor, ImageProcessor) {} END_DERIVED_DTOR

FUN_IMPL(__init_gaussian_kernel, GrayImage* kernel_img)
{
    /* init 3x3 image, pixels will hold the kernel coefficients */
    MFUN(kernel_img, init), (MEM_SIZE_T)3, (MEM_SIZE_T)3, NULL CALL;

    /* stride == width == 3 after init, so row-major index is row * stride + col */
    FOR(MEM_SIZE_T r = 0; r < (MEM_SIZE_T)3; ++r)
    {
        FOR(MEM_SIZE_T c = 0; c < (MEM_SIZE_T)3; ++c)
        {
            kernel_img->image_buffer[r * kernel_img->stride + c] =
                GAUSSIAN_KERNEL_VALS[(size_t)r][(size_t)c];
        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN;

FUN_IMPL(__convolve, GrayImage* img, GrayImage* kernel)
{
    THROW_MSG_UNLESS(img, "Convolve: img is NULL");
    THROW_MSG_UNLESS(kernel, "Convolve: kernel is NULL");
    THROW_MSG_UNLESS(img->refCount, "Convolve: img not initialized");
    THROW_MSG_UNLESS(kernel->refCount, "Convolve: kernel not initialized");

    MEM_SIZE_T width = 0, height = 0;
    MEM_SIZE_T k_width = 0, k_height = 0;

    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;
    MFUN(kernel, get_width), & k_width CALL;
    MFUN(kernel, get_height), & k_height CALL;

    THROW_MSG_UNLESS(k_width > 0 && k_height > 0, "Convolve: kernel size must be positive");
    THROW_MSG_UNLESS((k_width % 2U) == 1U && (k_height % 2U) == 1U, "Convolve: kernel dimensions must be odd");
    THROW_MSG_UNLESS(width >= k_width && height >= k_height, "Convolve: kernel larger than image");

    /* Precompute kernel sum */
    int kernel_sum = 0;
    FOR(MEM_SIZE_T kr = 0; kr < k_height; ++kr)
    {
        FOR(MEM_SIZE_T kc = 0; kc < k_width; ++kc)
        {
            uint8_t* kptr = NULL;
            MFUN(kernel, get_pixel_ptr), kr, kc, & kptr CALL;
            kernel_sum += (int)(*kptr);
        }
        END_LOOP;
    }
    END_LOOP;

    /* If sum is 0 (e.g., some edge-detection kernels), we will skip normalization */
    /* For Gaussian, sum > 0 */

    CREATE(GrayImage, temp_img) CALL;
    MFUN(img, clone), & temp_img CALL;

    const MEM_SIZE_T half_h = k_height / 2U;
    const MEM_SIZE_T half_w = k_width / 2U;

    /* Process only inner region: borders stay as in temp_img (unchanged) */
    FOR(MEM_SIZE_T r = half_h; r < height - half_h; ++r)
    {
        FOR(MEM_SIZE_T c = half_w; c < width - half_w; ++c)
        {
            uint32_t acc = 0U;

            FOR(MEM_SIZE_T kr = 0; kr < k_height; ++kr)
            {
                FOR(MEM_SIZE_T kc = 0; kc < k_width; ++kc)
                {
                    MEM_SIZE_T src_r = r + kr - half_h;
                    MEM_SIZE_T src_c = c + kc - half_w;

                    uint8_t* src_pixel = NULL;
                    MFUN(&temp_img, get_pixel_ptr), src_r, src_c, & src_pixel CALL;

                    uint8_t* kptr = NULL;
                    MFUN(kernel, get_pixel_ptr), kr, kc, & kptr CALL;

                    acc += (uint32_t)(*src_pixel) * (uint32_t)(*kptr);
                }
                END_LOOP;
            }
            END_LOOP;

            uint32_t value = acc;

            /* Normalize by kernel_sum when it is positive (Gaussian and similar) */
            IF(kernel_sum > 0)
            {
                value = acc / (uint32_t)kernel_sum;
            }
            END_IF;

            IF(value > 255U)
            {
                value = 255U;
            }
            END_IF;

            uint8_t* dst_pixel = NULL;
            MFUN(img, get_pixel_ptr), r, c, & dst_pixel CALL;
            *dst_pixel = (uint8_t)value;
        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(__gaussian_blur, GrayImage* img)
{
    THROW_MSG_UNLESS(img, "Input image pointer is NULL");

    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;
    THROW_MSG_UNLESS(width >= 3 && height >= 3,
        "Image dimensions must be at least 3x3 for a 3x3 Gaussian kernel.");

    /* Build 3x3 Gaussian kernel as GrayImage */
    CREATE(GrayImage, kernel_img) CALL;
    FUN(__init_gaussian_kernel) &kernel_img CALL;

    /* Apply generic convolution with this kernel */
    FUN(__convolve) img, & kernel_img CALL;
}END_FUN

FUN_OVERRIDE_IMPL(GaussianProcessor, ImageProcessor, process, GrayImage* img)
{
    FUN(__gaussian_blur) img CALL;
}
END_FUN

INIT_DERIVED_CLASS(GaussianProcessor, ImageProcessor);
BIND_OVERIDE(GaussianProcessor, ImageProcessor, process);
END_INIT_CLASS(GaussianProcessor)