#ifndef __COOP__GRAY_IMG__H_
#define __COOP__GRAY_IMG__H_

#include "COOP.h"
#include "vector.h"
#include <stdint.h>
#include <stdbool.h>


DEF_CLASS(GrayImage);
uint8_t* image_buffer; 
MEM_SIZE_T width;      
MEM_SIZE_T height;     
MEM_SIZE_T stride;     
size_t* refCount;     //how much pointer we need?    
size_t offset;
END_DEF(GrayImage);

//------

FUNCTIONS(GrayImage, MEM_SIZE_T width, MEM_SIZE_T height, Vector_uint8_t* data_vector);
MEM_FUN_DECL(GrayImage, get_width, MEM_SIZE_T* width_out);
MEM_FUN_DECL(GrayImage, get_height, MEM_SIZE_T* height_out);
MEM_FUN_DECL(GrayImage, get_stride, MEM_SIZE_T* stride_out);
MEM_FUN_DECL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr);
MEM_FUN_DECL(GrayImage, clone, GrayImage** out_clone);
MEM_FUN_DECL(GrayImage, equals, GrayImage const* other, GrayImage** out_comparison_image);
END_FUNCTIONS(GrayImage);

FUN_DECL(GrayImage_init_copy, GrayImage* _this, GrayImage const* other);
FUN_DECL(GrayImage_init_move, GrayImage* _this, GrayImage* other);
FUN_DECL(GrayImage_init_ROI, GrayImage* _this, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height);
#endif