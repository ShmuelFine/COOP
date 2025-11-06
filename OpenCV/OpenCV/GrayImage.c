#include "GrayImage.h"
#include "DynamicMemoryManagement.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


DEF_CTOR(GrayImage, MEM_SIZE_T width, MEM_SIZE_T height, Vector_uint8_t* data_vector)
{
    THROW_MSG_UNLESS(width > 0 && height > 0, "Width and Height must be positive");

    ALLOC_ARRAY(_this->image_buffer, uint8_t, height * width);
    ASSERT_NOT_NULL(_this->image_buffer);

    ALLOC(_this->refCount, size_t);
    *(_this->refCount) = 1;

    _this->width = width;
    _this->height = height;
    _this->stride = width;
    _this->offset = 0;
	printf("GrayImage Constructor: width=%zu, height=%zu\n", width, height);

    IF(data_vector != NULL) 
    {
        MEM_SIZE_T vec_size;
        MFUN(data_vector, size), & vec_size CALL;
        THROW_MSG_UNLESS(vec_size == (width * height), "Data vector size mismatch");

        char* vec_data_ptr = NULL;
        MFUN(data_vector, dataPtr), &vec_data_ptr CALL;
        memcpy(_this->image_buffer, (uint8_t*)vec_data_ptr, vec_size);
    }
    ELSE
    {
        memset(_this->image_buffer, 0, width * height);
    }
    END_IF;
}
END_CTOR

DEF_DTOR(GrayImage)
{
    IF(_this->refCount != NULL)
    {
        (*(_this->refCount))--;
        IF((*(_this->refCount)) == 0)
        {
            FREE(_this->image_buffer);
            FREE(_this->refCount);
        }
        END_IF;
    }
    END_IF;
}
END_DTOR

MEM_FUN_IMPL(GrayImage, init_copy, GrayImage const* other) {
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");

    _this->image_buffer = other->image_buffer;
    _this->width = other->width;
    _this->height = other->height;
    _this->stride = other->stride;
    _this->offset = other->offset;
    _this->refCount = other->refCount;

    (*(_this->refCount))++;
}
END_FUN

MEM_FUN_IMPL(GrayImage, init_move, GrayImage* other) {
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");

    _this->image_buffer = other->image_buffer;
    _this->width = other->width;
    _this->height = other->height;
    _this->stride = other->stride;
    _this->offset = other->offset;
    _this->refCount = other->refCount;

    other->image_buffer = NULL;
    other->refCount = NULL;
    other->width = 0;
    other->height = 0;
    other->stride = 0;
    other->offset = 0;
}
END_FUN

MEM_FUN_IMPL(GrayImage, init_ROI, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height) {
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");
    THROW_MSG_UNLESS(col + ROI_width <= other->width && row + ROI_height <= other->height, "ROI boundary error");

    _this->image_buffer = other->image_buffer;
    _this->refCount = other->refCount;
    (*(_this->refCount))++;

    _this->width = ROI_width;
    _this->height = ROI_height;
    _this->stride = other->stride;

    _this->offset = other->offset + (row * other->stride) + col;
}
END_FUN


MEM_FUN_IMPL(GrayImage, clone, GrayImage** out_clone)
{
    THROW_MSG_UNLESS(out_clone != NULL, "Output pointer cannot be NULL");

    GrayImage* new_image = NULL;

    ALLOC_ARRAY(new_image, GrayImage, 1);
    INITIALIZE_INSTANCE(GrayImage, (*new_image)), _this->width, _this->height, NULL CALL;
    ASSERT_NOT_NULL(new_image);

    // 5.copy row - row
    FOR(MEM_SIZE_T r = 0; r < _this->height; ++r)
    {
        //row source
        uint8_t* src_row_ptr = _this->image_buffer + _this->offset + (r * _this->stride);
        // row destination
        uint8_t* dst_row_ptr = new_image->image_buffer + (r * new_image->stride);
        // copy
        memcpy(dst_row_ptr, src_row_ptr, _this->width);
    }
    END_LOOP;

    *out_clone = new_image;
}
END_FUN

MEM_FUN_IMPL(GrayImage, get_width, MEM_SIZE_T* width_out) 
{
    *width_out = _this->width;
} 
END_FUN

MEM_FUN_IMPL(GrayImage, get_height, MEM_SIZE_T* height_out) 
{
    *height_out = _this->height;
} 
END_FUN

MEM_FUN_IMPL(GrayImage, get_stride, MEM_SIZE_T* stride_out)
{
    *stride_out = _this->stride;
}
END_FUN

MEM_FUN_IMPL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr)
{
    THROW_MSG_UNLESS(row < _this->height && col < _this->width, "Pixel out of bounds");

    *out_ptr = _this->image_buffer + _this->offset + (row * _this->stride) + col;
}
END_FUN

MEM_FUN_IMPL(GrayImage, add, GrayImage const* other, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && other != NULL && out != NULL, "NULL image in arithmetic op");
    THROW_MSG_UNLESS(_this->width == other->width && _this->height == other->height, "Size mismatch between inputs");
    THROW_MSG_UNLESS(out->width == _this->width && out->height == _this->height, "Output size mismatch");
    
    MEM_SIZE_T height = _this->height;
    MEM_SIZE_T width = _this->width;

    FOR (MEM_SIZE_T row = 0; row < height; ++row)
    {
        uint8_t* dst = out->image_buffer + out->offset + row * out->stride;
        uint8_t const* a = _this->image_buffer + _this->offset + row * _this->stride;
        uint8_t const* b = other->image_buffer + other->offset + row * other->stride;

        FOR (MEM_SIZE_T col = 0; col < width; ++col)
        {
            uint16_t s = (uint16_t)a[col] + (uint16_t)b[col];
            dst[col] = (uint8_t)(s > 255U ? 255U : s);
        }
        END_LOOP;
    }
	END_LOOP;
}
END_FUN

MEM_FUN_IMPL(GrayImage, sub_default, GrayImage const* other, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && other != NULL && out != NULL, "NULL image in arithmetic op");
    THROW_MSG_UNLESS(_this->width == other->width && _this->height == other->height, "Size mismatch between inputs");
    THROW_MSG_UNLESS(out->width == _this->width && out->height == _this->height, "Output size mismatch");

    MEM_SIZE_T height = _this->height;
    MEM_SIZE_T width = _this->width;

    FOR (MEM_SIZE_T row = 0; row < height; ++row)
    {
        uint8_t* dst = out->image_buffer + out->offset + row * out->stride;
        uint8_t const* a = _this->image_buffer + _this->offset + row * _this->stride;
        uint8_t const* b = other->image_buffer + other->offset + row * other->stride;

        FOR (MEM_SIZE_T col = 0; col < width; ++col)
        {
            int diff = (int)a[col] - (int)b[col];
            dst[col] = (diff < 0) ? 0 : (uint8_t)diff;
        }
		END_LOOP;
    }
	END_LOOP;
}
END_FUN

MEM_FUN_IMPL(GrayImage, sub_abs, GrayImage const* other, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && other != NULL && out != NULL, "NULL image in arithmetic op");
    THROW_MSG_UNLESS(_this->width == other->width && _this->height == other->height, "Size mismatch between inputs");
    THROW_MSG_UNLESS(out->width == _this->width && out->height == _this->height, "Output size mismatch");

    MEM_SIZE_T height = _this->height;
    MEM_SIZE_T width = _this->width;

    FOR(MEM_SIZE_T row = 0; row < height; ++row)
    {
        uint8_t* dst = out->image_buffer + out->offset + row * out->stride;
        uint8_t const* a = _this->image_buffer + _this->offset + row * _this->stride;
        uint8_t const* b = other->image_buffer + other->offset + row * other->stride;

        FOR(MEM_SIZE_T col = 0; col < width; ++col)
        {
            int diff = (int)a[col] - (int)b[col];
            dst[col] = (uint8_t)(diff >= 0 ? diff : -diff);
        }
		END_LOOP;
    }
	END_LOOP;
}
END_FUN

MEM_FUN_IMPL(GrayImage, mul_scalar, double alpha, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && out != NULL, "NULL image in mul_scalar");
    THROW_MSG_UNLESS(out->width == _this->width && out->height == _this->height, "Output size mismatch");

    const MEM_SIZE_T height = _this->height, width = _this->width;

    FOR(MEM_SIZE_T row = 0; row < height; ++row)
    {
        uint8_t* dst = out->image_buffer + out->offset + row * out->stride;
        const uint8_t* src = _this->image_buffer + _this->offset + row * _this->stride;

        FOR(MEM_SIZE_T col = 0; col < width; ++col)
        {
            double value = (double)src[col] * alpha;

            IF (!isfinite(value))
            {
                value = 0.0;
            }
			END_IF;

            long int_value = (long)llround(value);
            IF(int_value < 0)
            {
                int_value = 0;
            }
            ELSE_IF(int_value > 255)
            {
                int_value = 255;
            }
            END_IF;
            dst[col] = (uint8_t)int_value;
        }
        END_LOOP;
    }
    END_LOOP;
}
END_FUN

/* ===== Linear algebraic matrix-multiply =====
   A: _this  (H_THIS � W_THIS) | B: other  (W_THIS � W_OTHER) | Out: out  (H_THIS � W_OTHER).
   Accumulate in 32-bit, clamp to 255 on store (CV_8U semantics). */

MEM_FUN_IMPL(GrayImage, mul_mat, GrayImage const* other, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && other != NULL && out != NULL, "NULL image in mul_mat");
    /* Dimension check: inner dims must match; output must be H_THIS � W_OTHER */
    THROW_MSG_UNLESS(_this->width == other->height, "Inner dims mismatch: A.width must equal B.height");
    THROW_MSG_UNLESS(out->height == _this->height && out->width == other->width, "Output size must be (A.height � B.width)");

    const MEM_SIZE_T H_THIS = _this->height;
    const MEM_SIZE_T W_THIS = _this->width;     /* also B.height */
    const MEM_SIZE_T W_OTHER = other->width;    /* also Out.width */

    FOR (MEM_SIZE_T r = 0; r < H_THIS; ++r)
    {
        uint8_t* dst_row = out->image_buffer + out->offset + r * out->stride;
        const uint8_t* a_row = _this->image_buffer + _this->offset + r * _this->stride;

        FOR (MEM_SIZE_T c = 0; c < W_OTHER; ++c)
        {
            /* sum over k: A[r,k] * B[k,c] */
            uint32_t sum = 0U;

            FOR (MEM_SIZE_T k = 0; k < W_THIS; ++k)
            {
                const uint8_t a_val = a_row[k];
                const uint8_t* b_row = other->image_buffer + other->offset + k * other->stride;
                const uint8_t b_val = b_row[c];

                sum += (uint32_t)a_val * (uint32_t)b_val;  /* up to 255*255*W_THIS */
            }
			END_LOOP;

            /* clamp to 0..255 for CV_8U output */
            dst_row[c] = (uint8_t)(sum > 255U ? 255U : sum);
        }
		END_LOOP;
    }
	END_LOOP;
}
END_FUN


// --- Add this implementation to GrayImage.c ---

MEM_FUN_IMPL(GrayImage, equals, GrayImage const* other, GrayImage** out_comparison_image)
{
   
    THROW_MSG_UNLESS(other != NULL, "Other image cannot be NULL");
    THROW_MSG_UNLESS(out_comparison_image != NULL, "Output pointer cannot be NULL");
    THROW_MSG_UNLESS(_this->width == other->width && _this->height == other->height, "Image dimensions must match for comparison");

    GrayImage* result_image = NULL;
    ALLOC_ARRAY(result_image, GrayImage, 1);
    INITIALIZE_INSTANCE(GrayImage, (*result_image)), _this->width, _this->height, NULL CALL;
    ASSERT_NOT_NULL(result_image);

    result_image->width = _this->width;  
    result_image->height = _this->height;
    result_image->stride = _this->width; 
    result_image->offset = 0;            

    // We must iterate by row and column to correctly handle the offset and stride
    // of *both* input images, which may be different ROIs.
    FOR(MEM_SIZE_T r = 0; r < _this->height; ++r)
    {
        // Get a pointer to the start of the current row for '_this' image
        uint8_t* this_row = _this->image_buffer + _this->offset + (r * _this->stride);

        // Get a pointer to the start of the current row for the 'other' image
        uint8_t* other_row = other->image_buffer + other->offset + (r * other->stride);

        // Get a pointer to the start of the current row for the 'result' image
        uint8_t* result_row = result_image->image_buffer + (r * result_image->stride);

        FOR(MEM_SIZE_T c = 0; c < _this->width; ++c)
        {
            IF(this_row[c] == other_row[c])
            {
                result_row[c] = 255;
            }
            ELSE
            {
                result_row[c] = 0;
            }
            END_IF;
        }
        END_LOOP;
    }
    END_LOOP;

    *out_comparison_image = result_image;
}
END_FUN


INIT_CLASS(GrayImage);
BIND(GrayImage, get_height);
BIND(GrayImage, get_stride);
BIND(GrayImage, get_width);
BIND(GrayImage, get_pixel_ptr);
BIND(GrayImage, clone);
BIND(GrayImage, init_copy);
BIND(GrayImage, init_move);
BIND(GrayImage, init_ROI);
BIND(GrayImage, add);
BIND(GrayImage, sub_default);
BIND(GrayImage, sub_abs);
BIND(GrayImage, mul_scalar);
BIND(GrayImage, mul_mat);
BIND(GrayImage, equals);
END_INIT_CLASS(GrayImage);