#include "SobelProcessor.h"

DEF_DERIVED_CTOR(SobelProcessor, ImageProcessor, GrayImage* out_direction_image)
SUPER ME
{
    _this->out_direction_image = out_direction_image;
} END_DERIVED_CTOR

DEF_DERIVED_DTOR(SobelProcessor, ImageProcessor) {} END_DERIVED_DTOR

FUN_IMPL(__sobel_filter, GrayImage* img, GrayImage* out_dir) {
    THROW_MSG_UNLESS(img && out_dir, "Input image pointer is NULL");

    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    THROW_MSG_UNLESS(width >= 3 && height >= 3, "Image dimensions must be at least 3x3 for a 3x3 Sobel kernel.");

    CREATE(GrayImage, temp_img) CALL;
    MFUN(img, clone), & temp_img CALL;

    FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
    {
        FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
        {
            int32_t sum_x = 0;
            int32_t sum_y = 0;

            FOR(int kr = -1; kr <= 1; ++kr)
            {
                FOR(int kc = -1; kc <= 1; ++kc)
                {
                    uint8_t* src_pixel = NULL;
                    // Get pixel value from the *cloned* image (temp_img)
                    MFUN(&temp_img, get_pixel_ptr), r + kr, c + kc, & src_pixel CALL;
                    const int pixel_val = (int)*src_pixel;

                    // Apply Gx kernel
                    sum_x += pixel_val * (int)SOBEL_GX_KERNEL[kr + 1][kc + 1];
                    // Apply Gy kernel
                    sum_y += pixel_val * (int)SOBEL_GY_KERNEL[kr + 1][kc + 1];
                }
                END_LOOP;
            }
            END_LOOP;

            //The formula G = |S_x| + |S_y| is the L_1-norm approximation for the gradient magnitude
            //This approximation is used instead of the exact L_2-norm, G = \sqrt{S_x^2 + S_y^2},
            //to avoid calculating the square root, which significantly improves speed (performance).
            uint32_t gradient_mag = (uint32_t)abs(sum_x) + (uint32_t)abs(sum_y);

            uint8_t new_val = (uint8_t)(gradient_mag > 255U ? 255U : gradient_mag);

            // Update the pixel in the *original* image (img)
            uint8_t* dst_pixel = NULL;
            MFUN(img, get_pixel_ptr), r, c, & dst_pixel CALL;
            *dst_pixel = new_val;

            // Store direction in out_dir image
            uint8_t dir = 0;
            const int32_t abs_x = abs(sum_x);
            const int32_t abs_y = abs(sum_y);

            /*
                dir = 0: Horizontal (slope < 22.5 degree)
                dir = 2: Vertical (slope > 67.5 degree)
                dir = 1: Positive Diagonal (45 degree, same signs)
                dir = 3: Negative Diagonal (135 , opposite signs)
             */
            IF((abs_y * TAN_APPROX_DENOMINATOR) <= (abs_x * TAN_APPROX_NUMERATOR))
            {
                dir = 0;
            }
            ELSE_IF((abs_x * TAN_APPROX_DENOMINATOR) <= (abs_y * TAN_APPROX_NUMERATOR))
            {
                dir = 2;
            }
            ELSE_IF((sum_x > 0 && sum_y > 0) || (sum_x < 0 && sum_y < 0))
            {
                dir = 1;
            }
            ELSE
            {
                dir = 3;
            }
            END_IF;

            uint8_t* dst_dir_pixel = NULL;
            MFUN(out_dir, get_pixel_ptr), r, c, & dst_dir_pixel CALL;
            *dst_dir_pixel = dir;

        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(__sobel_convolve_x, GrayImage* img, GrayImage* out_x)
{
    THROW_MSG_UNLESS(img && out_x, "NULL image pointer");
    MEM_SIZE_T width, height;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    CREATE(GrayImage, temp_img) CALL;
    MFUN(img, clone), & temp_img CALL;

    FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
    {
        FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
        {
            int32_t sum_x = 0;
            FOR(int kr = -1; kr <= 1; ++kr)
            {
                FOR(int kc = -1; kc <= 1; ++kc)
                {
                    uint8_t* src_pixel = NULL;
                    MFUN(&temp_img, get_pixel_ptr), r + kr, c + kc, & src_pixel CALL;
                    sum_x += (int32_t)(*src_pixel) * (int32_t)SOBEL_GX_KERNEL[kr + 1][kc + 1];
                }
                END_LOOP;
            }
            END_LOOP;


            uint32_t abs_sum_x = (uint32_t)abs(sum_x);
            uint8_t clamped_val = (uint8_t)(abs_sum_x > 255U ? 255U : abs_sum_x);

            uint8_t* dst_pixel = NULL;
            MFUN(out_x, get_pixel_ptr), r, c, & dst_pixel CALL;
            *dst_pixel = clamped_val;
        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(__sobel_convolve_y, GrayImage* img, GrayImage* out_y)
{
    THROW_MSG_UNLESS(img && out_y, "NULL image pointer");
    MEM_SIZE_T width, height;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    CREATE(GrayImage, temp_img) CALL;
    MFUN(img, clone), & temp_img CALL;

    FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
    {
        FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
        {
            int32_t sum_y = 0;
            FOR(int kr = -1; kr <= 1; ++kr)
            {
                FOR(int kc = -1; kc <= 1; ++kc)
                {
                    uint8_t* src_pixel = NULL;
                    MFUN(&temp_img, get_pixel_ptr), r + kr, c + kc, & src_pixel CALL;
                    sum_y += (int32_t)(*src_pixel) * (int32_t)SOBEL_GY_KERNEL[kr + 1][kc + 1];
                }
                END_LOOP;
            }
            END_LOOP;


            uint32_t abs_sum_y = (uint32_t)abs(sum_y);
            uint8_t clamped_val = (uint8_t)(abs_sum_y > 255U ? 255U : abs_sum_y);

            uint8_t* dst_pixel = NULL;
            MFUN(out_y, get_pixel_ptr), r, c, & dst_pixel CALL;
            *dst_pixel = clamped_val;
        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(__sobel_magnitude, GrayImage* img_x, GrayImage* img_y, GrayImage* out_mag)
{
    THROW_MSG_UNLESS(img_x && img_y && out_mag, "NULL image pointer");
    MEM_SIZE_T width, height;
    MFUN(img_x, get_width), & width CALL;
    MFUN(img_x, get_height), & height CALL;

    FOR(MEM_SIZE_T r = 0; r < height; ++r)
    {
        FOR(MEM_SIZE_T c = 0; c < width; ++c)
        {
            uint8_t* x_ptr = NULL;
            uint8_t* y_ptr = NULL;
            uint8_t* mag_ptr = NULL;

            //CHECK:Processor speed because the function is not called?
            uint8_t const* row_x = img_x->image_buffer + img_x->offset + r * img_x->stride;
            uint8_t const* row_y = img_y->image_buffer + img_y->offset + r * img_y->stride;
            uint8_t* row_mag = out_mag->image_buffer + out_mag->offset + r * out_mag->stride;

            //The formula G = |S_x| + |S_y| is the L_1-norm approximation for the gradient magnitude
            //This approximation is used instead of the exact L_2-norm, G = \sqrt{S_x^2 + S_y^2},
            //to avoid calculating the square root, which significantly improves speed (performance).
            uint32_t sum = (uint32_t)row_x[c] + (uint32_t)row_y[c];

            uint8_t clamped_val = (uint8_t)(sum > 255U ? 255U : sum);
            row_mag[c] = clamped_val;
        }
        END_LOOP;
    }
    END_LOOP;
}END_FUN

FUN_IMPL(__sobel_filter_split, GrayImage* img)
{
    THROW_MSG_UNLESS(img, "Input image pointer is NULL");
    MEM_SIZE_T width, height;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    CREATE(GrayImage, img_x) CALL;
    CREATE(GrayImage, img_y) CALL;

    MFUN(&img_x, init), width, height, NULL CALL;
    MFUN(&img_y, init), width, height, NULL CALL;

    //|Sx|
    FUN(__sobel_convolve_x) img, & img_x CALL;

    //|Sy|
    FUN(__sobel_convolve_y) img, & img_y CALL;

    FUN(__sobel_magnitude)& img_x, & img_y, img CALL;

}END_FUN

FUN_OVERRIDE_IMPL(SobelProcessor, ImageProcessor, process, GrayImage* img)
{
    THROW_MSG_UNLESS(_this->out_direction_image != NULL, "SobelProcessor requires a non-NULL direction image output");
    FUN(__sobel_filter) img, _this->out_direction_image CALL;
}
END_FUN

INIT_DERIVED_CLASS(SobelProcessor, ImageProcessor);
BIND_OVERIDE(SobelProcessor, ImageProcessor, process);
END_INIT_CLASS(SobelProcessor)