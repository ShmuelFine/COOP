#include "ThresholdProcessor.h"


DEF_DERIVED_CTOR(ThresholdProcessor, ImageProcessor, uint8_t low_thresh, uint8_t high_thresh)
SUPER ME
{
    _this->low_thresh = low_thresh;
    _this->high_thresh = high_thresh;
} END_DERIVED_CTOR

DEF_DERIVED_DTOR(ThresholdProcessor, ImageProcessor) {} END_DERIVED_DTOR

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

FUN_OVERRIDE_IMPL(ThresholdProcessor, ImageProcessor, process, GrayImage* img)
{
    FUN(__hysteresis_thresholding) img, _this->low_thresh, _this->high_thresh CALL;
}
END_FUN

INIT_DERIVED_CLASS(ThresholdProcessor, ImageProcessor);
BIND_OVERIDE(ThresholdProcessor, ImageProcessor, process);
END_INIT_CLASS(ThresholdProcessor)