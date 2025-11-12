#include "BorderProcessor.h"


DEF_DERIVED_CTOR(BorderProcessor, ImageProcessor) SUPER ME {} END_DERIVED_CTOR
DEF_DERIVED_DTOR(BorderProcessor, ImageProcessor) {} END_DERIVED_DTOR

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
        MFUN(img, get_pixel_ptr), r, c_left, & p CALL;
        *p = 0;
        MFUN(img, get_pixel_ptr), r, c_right, & p CALL;
        *p = 0;
    }
    END_LOOP;

}END_FUN

FUN_OVERRIDE_IMPL(BorderProcessor, ImageProcessor, process, GrayImage* img)
{
    FUN(__zero_border) img CALL;
}
END_FUN

INIT_DERIVED_CLASS(BorderProcessor, ImageProcessor);
BIND_OVERIDE(BorderProcessor, ImageProcessor, process);
END_INIT_CLASS(BorderProcessor)