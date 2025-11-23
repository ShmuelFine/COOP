#include "NMSProcessor.h"

DEF_DERIVED_CTOR(NMSProcessor, ImageProcessor, GrayImage const* direction_image)
SUPER ME
{
    _this->direction_image = direction_image;
} END_DERIVED_CTOR

DEF_DERIVED_DTOR(NMSProcessor, ImageProcessor) {} END_DERIVED_DTOR

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

FUN_OVERRIDE_IMPL(NMSProcessor, ImageProcessor, process, GrayImage* img)
{
    THROW_MSG_UNLESS(_this->direction_image != NULL, "NMSProcessor requires a non-NULL direction image input");
    FUN(__non_maximum_suppression) img, _this->direction_image CALL;
}
END_FUN

INIT_DERIVED_CLASS(NMSProcessor, ImageProcessor);
BIND_OVERIDE(NMSProcessor, ImageProcessor, process);
END_INIT_CLASS(NMSProcessor)