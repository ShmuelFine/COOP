#ifndef __COOP__GRAY_IMG__H_
#define __COOP__GRAY_IMG__H_

#include "COOP.h"
#include "vector.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>


DEF_CLASS(GrayImage);
uint8_t* image_buffer; 
MEM_SIZE_T width;      
MEM_SIZE_T height;     
MEM_SIZE_T stride;     
size_t* refCount;     //how much pointer we need?    
size_t offset;
END_DEF(GrayImage);
//------

#pragma pack(push,1)
typedef struct {
    uint16_t signature;         // File type identifier ('BM' = 0x4D42)
    uint32_t totalFileSize;     
    uint16_t reservedField1;    
    uint16_t reservedField2;    
    uint32_t pixelDataOffset;
} BMP_FILE_HDR;

typedef struct {
    uint32_t headerSize;            // Size of this header structure (40 bytes)
    int32_t  imageWidth;            // Image width in pixels
    int32_t  imageHeight;           // Image height in pixels (negative = top-down)
    uint16_t colorPlanes;           // Number of color planes (must be 1)
    uint16_t bitsPerPixel;          // Bits per pixel (8 for grayscale)
    uint32_t compressionMethod;     // Compression type (0 = BI_RGB = no compression)
    uint32_t imageDataSize;         // Size of the raw bitmap data (including padding)
    int32_t  horizontalResolution;  // Horizontal resolution, pixels per meter
    int32_t  verticalResolution;    // Vertical resolution, pixels per meter
    uint32_t colorsInPalette;       // Number of colors used in the palette (256 for grayscale)
    uint32_t importantColors;       // Number of important colors (0 = all are important)
} BMP_INFO_HDR;
#pragma pack(pop)



FUNCTIONS(GrayImage);
MEM_FUN_DECL(GrayImage, init, MEM_SIZE_T width, MEM_SIZE_T height, Vector_uint8_t* data_vector);
MEM_FUN_DECL(GrayImage, get_width, MEM_SIZE_T* width_out);
MEM_FUN_DECL(GrayImage, get_height, MEM_SIZE_T* height_out);
MEM_FUN_DECL(GrayImage, get_stride, MEM_SIZE_T* stride_out);
MEM_FUN_DECL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr);
MEM_FUN_DECL(GrayImage, clone, GrayImage* out_clone); 
MEM_FUN_DECL(GrayImage, equals, GrayImage const* other, GrayImage* out_comparison_image);
MEM_FUN_DECL(GrayImage, init_copy, GrayImage const* other);
MEM_FUN_DECL(GrayImage, init_move, GrayImage* other);
MEM_FUN_DECL(GrayImage, init_ROI, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height);
MEM_FUN_DECL(GrayImage, save_to_bmp, const char *path);
MEM_FUN_DECL(GrayImage, add, GrayImage const* other, GrayImage* out);
MEM_FUN_DECL(GrayImage, sub_default, GrayImage const* other, GrayImage* out);
MEM_FUN_DECL(GrayImage, sub_abs, GrayImage const* other, GrayImage* out);
MEM_FUN_DECL(GrayImage, mul_scalar, double alpha, GrayImage* out);
MEM_FUN_DECL(GrayImage, mul_mat, GrayImage const* other, GrayImage* out);
END_FUNCTIONS(GrayImage);
#endif