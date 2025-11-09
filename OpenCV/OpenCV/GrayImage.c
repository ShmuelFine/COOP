#include "GrayImage.h"
#include "DynamicMemoryManagement.h"
#include <string.h>
#include <stdlib.h>


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
BIND(GrayImage, equals);
BIND(GrayImage, init_copy);
BIND(GrayImage, init_move);
BIND(GrayImage, init_ROI);
END_INIT_CLASS(GrayImage);