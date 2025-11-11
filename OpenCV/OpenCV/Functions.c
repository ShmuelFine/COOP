#include "Functions.h"
#include <stdint.h>

// Static kernels for Gaussian Blur operation
static const uint8_t GAUSSIAN_KERNEL_VALS[3][3] = { {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };
static const int GAUSSIAN_DIVISOR = 16;

// Static kernels for Sobel operation
static const int8_t SOBEL_GX_KERNEL[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
static const int8_t SOBEL_GY_KERNEL[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };

//Constants for calculating a fast approximation of the 
//gradient direction (based on tan(22.5) ≈ 41/100)
static const long TAN_APPROX_NUMERATOR = 41L; // The numerator of the approximation (41)
static const long TAN_APPROX_DENOMINATOR = 100L; // The denominator of the approximation (100)

// Hysteresis edge classification values
static const uint8_t HYST_EDGE_STRONG = 255;
static const uint8_t HYST_EDGE_WEAK = 128;
static const uint8_t HYST_EDGE_NONE = 0;

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

FUN_IMPL(__sobel_filter, GrayImage * img, GrayImage* out_dir) {
    THROW_MSG_UNLESS(img && out_dir, "Input image pointer is NULL");

    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

	MFUN(out_dir, init), width, height, NULL CALL;

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
                dir = 0: Horizontal (slope < 22.5°)
                dir = 2: Vertical (slope > 67.5°)
                dir = 1: Positive Diagonal (45°, same signs)
                dir = 3: Negative Diagonal (135°, opposite signs) 
             */
            IF ((abs_y * TAN_APPROX_DENOMINATOR) <= (abs_x * TAN_APPROX_NUMERATOR))
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
            MFUN(out_dir, get_pixel_ptr), r, c,& dst_dir_pixel CALL;
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

FUN_IMPL(__non_maximum_suppression, GrayImage* img, GrayImage const* img_dir)
{
    THROW_MSG_UNLESS(img && img_dir, "NULL image pointer");
    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    MEM_SIZE_T dir_w, dir_h;
    MFUN(img_dir, get_width), & dir_w CALL;
    MFUN(img_dir, get_height), & dir_h CALL;

    THROW_MSG_UNLESS(width >= 3 && height >= 3, "Image must be at least 3x3");
    THROW_MSG_UNLESS(width == dir_w && height == dir_h, "Direction image size mismatch");

    CREATE(GrayImage, temp_mag_src) CALL;
    MFUN(img, clone), & temp_mag_src CALL;

    FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
    {
        FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
        {
            uint8_t* center_mag_ptr = NULL;
            MFUN(&temp_mag_src, get_pixel_ptr), r, c, & center_mag_ptr CALL;
            const uint8_t center_mag = *center_mag_ptr;

            uint8_t* dir_ptr = NULL;
            MFUN(img_dir, get_pixel_ptr), r, c, & dir_ptr CALL;
            const uint8_t dir = *dir_ptr;

            IF(center_mag == 0)
            {
                uint8_t* dst_pixel_ptr = NULL;
                MFUN(img, get_pixel_ptr), r, c, & dst_pixel_ptr CALL;
                *dst_pixel_ptr = 0;
                CONTINUE;
            }
            END_IF;

            uint8_t* neighbor1_ptr = NULL;
            uint8_t* neighbor2_ptr = NULL;

            switch (dir)
            {
            case 0:
                MFUN(&temp_mag_src, get_pixel_ptr), r, c - 1, & neighbor1_ptr CALL;
                MFUN(&temp_mag_src, get_pixel_ptr), r, c + 1, & neighbor2_ptr CALL;
                break;
            case 1:
                MFUN(&temp_mag_src, get_pixel_ptr), r - 1, c + 1, & neighbor1_ptr CALL;
                MFUN(&temp_mag_src, get_pixel_ptr), r + 1, c - 1, & neighbor2_ptr CALL;
                break;
            case 2:
                MFUN(&temp_mag_src, get_pixel_ptr), r - 1, c, & neighbor1_ptr CALL;
                MFUN(&temp_mag_src, get_pixel_ptr), r + 1, c, & neighbor2_ptr CALL;
                break;
            case 3:
                MFUN(&temp_mag_src, get_pixel_ptr), r - 1, c - 1, & neighbor1_ptr CALL;
                MFUN(&temp_mag_src, get_pixel_ptr), r + 1, c + 1, & neighbor2_ptr CALL;
                break;
            }


            IF(center_mag >= (*neighbor1_ptr) && center_mag >= (*neighbor2_ptr))
            {
                uint8_t* dst_pixel_ptr = NULL;
                MFUN(img, get_pixel_ptr), r, c, & dst_pixel_ptr CALL;
                *dst_pixel_ptr = center_mag;
            }
            ELSE
            {
                uint8_t * dst_pixel_ptr = NULL;
                MFUN(img, get_pixel_ptr), r, c,& dst_pixel_ptr CALL;
                *dst_pixel_ptr = 0;
            }
            END_IF;
        }
        END_LOOP;
    }
    END_LOOP;

}END_FUN

FUN_IMPL(__hysteresis_thresholding, GrayImage* img, uint8_t low_thresh, uint8_t high_thresh)
{
    THROW_MSG_UNLESS(img, "Input image pointer is NULL");
    THROW_MSG_UNLESS(low_thresh <= high_thresh, "low_threshold cannot be greater than high_threshold");

    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;
    THROW_MSG_UNLESS(width >= 3 && height >= 3, "Image must be at least 3x3 for neighborhood processing");

    CREATE(GrayImage, temp_mag_src) CALL;
    MFUN(img, clone), & temp_mag_src CALL;

    // First pass: Pixel classification
    // Classify each pixel as STRONG, WEAK, or NONE, based on the thresholds.
    FOR(MEM_SIZE_T r = 0; r < height; ++r)
    {
        FOR(MEM_SIZE_T c = 0; c < width; ++c)
        {
            uint8_t* mag_pixel_ptr = NULL;
            MFUN(&temp_mag_src, get_pixel_ptr), r, c, & mag_pixel_ptr CALL;
            const uint8_t mag = *mag_pixel_ptr;

            uint8_t* dst_pixel_ptr = NULL;
            MFUN(img, get_pixel_ptr), r, c, & dst_pixel_ptr CALL;

            IF(mag >= high_thresh)
            {
                *dst_pixel_ptr = HYST_EDGE_STRONG;
            }
            ELSE_IF(mag >= low_thresh)
            {
                *dst_pixel_ptr = HYST_EDGE_WEAK;
            }
            ELSE
            {
                *dst_pixel_ptr = HYST_EDGE_NONE;
            }
            END_IF;
        }
        END_LOOP;
    }
    END_LOOP;

    // Second pass: Hysteresis
    // We will continue to loop as long as we find weak pixels to become strong.
    bool changed_in_pass = true;
    WHILE(changed_in_pass)
    {
        changed_in_pass = false;

        FOR(MEM_SIZE_T r = 1; r < height - 1; ++r)
        {
            FOR(MEM_SIZE_T c = 1; c < width - 1; ++c)
            {
                uint8_t* center_pixel_ptr = NULL;
                MFUN(img, get_pixel_ptr), r, c, & center_pixel_ptr CALL;

                IF(*center_pixel_ptr == HYST_EDGE_WEAK)
                {
                    bool found_strong_neighbor = false;
                    FOR(int kr = -1; kr <= 1; ++kr)
                    {
                        FOR(int kc = -1; kc <= 1; ++kc)
                        {
                            IF(kr != 0 || kc != 0)
                            {
                                uint8_t* neighbor_ptr = NULL;
                                MFUN(img, get_pixel_ptr), r + kr, c + kc, & neighbor_ptr CALL;

                                IF(*neighbor_ptr == HYST_EDGE_STRONG)
                                {
                                    found_strong_neighbor = true;
                                    BREAK;
                                }
                                END_IF;
                            }
                            END_IF;
                        }
                        END_LOOP;

                        IF(found_strong_neighbor)
                        {
                            BREAK;
                        }
                        END_IF;
                    }
                    END_LOOP;

                    IF(found_strong_neighbor)
                    {
                        *center_pixel_ptr = HYST_EDGE_STRONG;
                        changed_in_pass = true;
                    }
                    END_IF;
                }
                END_IF;
            }
            END_LOOP;
        }
        END_LOOP;
    }
    END_LOOP;

    // Third pass: Cleanup
    // Any pixel that is still marked as WEAK (128) is not connected to any strong edge.
    // Therefore, we will delete it (turn it into NONE).
    FOR(MEM_SIZE_T r = 0; r < height; ++r)
    {
        FOR(MEM_SIZE_T c = 0; c < width; ++c)
        {
            uint8_t* pixel_ptr = NULL;
            MFUN(img, get_pixel_ptr), r, c, & pixel_ptr CALL;

            IF(*pixel_ptr == HYST_EDGE_WEAK)
            {
                *pixel_ptr = HYST_EDGE_NONE;
            }
            END_IF;
        }
        END_LOOP;
    }
    END_LOOP;

}END_FUN

FUN_IMPL(__zero_border, GrayImage* img)
{
    THROW_MSG_UNLESS(img, "Input image pointer is NULL");

    MEM_SIZE_T width = 0, height = 0;
    MFUN(img, get_width), & width CALL;
    MFUN(img, get_height), & height CALL;

    IF(width < 2 || height < 2)
    {
        RETURN;
    }
    END_IF;

    const MEM_SIZE_T r_top = 0;
    const MEM_SIZE_T r_bottom = height - 1;
    const MEM_SIZE_T c_left = 0;
    const MEM_SIZE_T c_right = width - 1;
    uint8_t* p = NULL;
    FOR(MEM_SIZE_T c = 0; c < width; ++c)
    {
        
        MFUN(img, get_pixel_ptr), r_top, c, & p CALL;
        *p = 0;
        MFUN(img, get_pixel_ptr), r_bottom, c, & p CALL;
        *p = 0;
    }
    END_LOOP;

    FOR(MEM_SIZE_T r = 1; r < r_bottom; ++r)
    {
        uint8_t* p = NULL;
        MFUN(img, get_pixel_ptr), r, c_left, & p CALL;
        *p = 0;
        MFUN(img, get_pixel_ptr), r, c_right, & p CALL;
        *p = 0;
    }
    END_LOOP;

}END_FUN

