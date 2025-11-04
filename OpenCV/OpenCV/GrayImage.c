#include "GrayImage.h"
#include "DynamicMemoryManagement.h"
#include <string.h>
#include <stdlib.h>


DEF_CTOR(GrayImage, MEM_SIZE_T width, MEM_SIZE_T height, Vector_uint8_t* data_vector)
{
    THROW_MSG_UNLESS(width > 0 && height > 0, "Width and Height must be positive");

    ALLOC_ARRAY(_this->image_buffer, uint8_t, height * width);
    ASSERT_NOT_NULL(_this->image_buffer);

    ALLOC(_this->refCount, int);
    *(_this->refCount) = 1;

    _this->width = width;
    _this->height = height;
    _this->stride = width;
    _this->offset = 0;

    IF(data_vector != NULL) 
    {
        MEM_SIZE_T vec_size = 0;
        MFUN(data_vector, size), & vec_size CALL;
        THROW_MSG_UNLESS(vec_size == (width * height), "Data vector size mismatch");

        uint8_t* vec_data_ptr = NULL;
        MFUN(data_vector, dataPtr), (char**)&vec_data_ptr CALL;
        memcpy(_this->image_buffer, vec_data_ptr, vec_size);
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

FUN_IMPL(GrayImage_init_copy, GrayImage* _this, GrayImage const* other)
{
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

FUN_IMPL(GrayImage_init_move, GrayImage* _this, GrayImage* other)
{
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

FUN_IMPL(GrayImage_init_ROI, GrayImage* _this, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height)
{
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


INIT_CLASS(GrayImage);
BIND(GrayImage, get_height);
BIND(GrayImage, get_stride);
BIND(GrayImage, get_width);
BIND(GrayImage, get_pixel_ptr);
END_INIT_CLASS(GrayImage);