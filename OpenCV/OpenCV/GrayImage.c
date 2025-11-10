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

FUN_IMPL(_bmp_calc_sizes, MEM_SIZE_T width, MEM_SIZE_T height,
    uint32_t* paletteSize, uint32_t* rowSize, uint32_t* imageDataSize,
    uint32_t* fileSize, uint32_t* offBits)
{
    THROW_MSG_UNLESS(paletteSize && rowSize && imageDataSize && fileSize && offBits,
        "NULL out-ptr");

    const uint32_t w = (uint32_t)width;
    const uint32_t h = (uint32_t)height;
    // Each grayscale palette has 256 entries, each entry = 4 bytes (B,G,R,Reserved)
    const uint32_t paletteSizeBytes = 256u * 4u;

    // Row size in bytes must be aligned to a multiple of 4 bytes (BMP rule)
    // So if width = 5 pixels, actual row size = 8 bytes (5 + 3 padding)
    const uint32_t alignedRowSizeBytes = ((w + 3u) / 4u) * 4u;

    // Total image data size (including padding at the end of each row)
    const uint32_t pixelDataSizeBytes = alignedRowSizeBytes * h;

    // Offset from start of file to the pixel data section:
    // = size of file header + info header + color palette
    const uint32_t pixelDataOffsetBytes = (uint32_t)(sizeof(BMP_FILE_HDR) + sizeof(BMP_INFO_HDR) + paletteSizeBytes);

    const uint32_t totalFileSizeBytes = pixelDataOffsetBytes + pixelDataSizeBytes;

    *paletteSize =   paletteSizeBytes;          // Total palette size (1024 bytes for grayscale)
    *rowSize =       alignedRowSizeBytes;       // Bytes per row (aligned to 4)
    *imageDataSize = pixelDataSizeBytes;        // Total image data size (including padding)
    *offBits =       pixelDataOffsetBytes;      // File offset to pixel data
    *fileSize =      totalFileSizeBytes;        // Entire BMP file size
}
END_FUN

FUN_IMPL(_bmp_fill_file_header, BMP_FILE_HDR* outFileHeader, uint32_t fileSize, uint32_t offBits)
{
    THROW_MSG_UNLESS(outFileHeader != NULL, "fh is NULL");
    // Initialize all header fields to zero (safety clear)
    memset(outFileHeader, 0, sizeof(*outFileHeader));

    // BMP file signature: characters 'B' and 'M' (0x4D42 in little-endian)
    // This identifies the file as a valid BMP image
    outFileHeader->signature = 0x4D42; 

    // Total size of the BMP file in bytes (entire file, including headers, palette, and pixel data)
    outFileHeader->totalFileSize = fileSize;

    // Reserved fields - must always be set to zero (not used)
    outFileHeader->reservedField1 = 0;
    outFileHeader->reservedField2 = 0;

    // Offset from file start to the beginning of the pixel data.
    outFileHeader->pixelDataOffset = offBits;
}
END_FUN

FUN_IMPL(_bmp_fill_info_header, BMP_INFO_HDR* outInfoHeader, MEM_SIZE_T width, MEM_SIZE_T height,
    uint32_t imageDataSize)
{
    THROW_MSG_UNLESS(outInfoHeader != NULL, "ih is NULL");

    // Initialize all header fields to zero
    memset(outInfoHeader, 0, sizeof(*outInfoHeader));
    // Size of this header structure (always 40 bytes for BITMAPINFOHEADER)
    outInfoHeader ->headerSize = (uint32_t)sizeof(BMP_INFO_HDR);
   
    // Image dimensions in pixels
    outInfoHeader ->imageWidth = (int32_t)width;
    // Negative height indicates a top-down bitmap (first row = top of image)
    outInfoHeader ->imageHeight = -(int32_t)height;  
    
    outInfoHeader ->colorPlanes = 1;
    outInfoHeader ->bitsPerPixel = 8;

    // Compression type (0 = BI_RGB = no compression)
    outInfoHeader ->compressionMethod = 0;  

    // Size of the raw bitmap data, including row padding
    outInfoHeader ->imageDataSize = imageDataSize; 

    // Number of colors in the palette (256 for grayscale)
    outInfoHeader ->colorsInPalette = 256;
    outInfoHeader ->importantColors = 256;
}
END_FUN

FUN_IMPL(_bmp_write_info_header, FILE* outFile, const BMP_INFO_HDR* infoHeader)
{
    THROW_MSG_UNLESS(outFile && infoHeader, "NULL arg");
    IF(fwrite(infoHeader, sizeof(*infoHeader), 1, outFile) != 1)
    {
        THROW_MSG("Failed to write BMP info header");
    }
    END_IF;
}
END_FUN

FUN_IMPL(_bmp_write_palette_256_gray, FILE* outFile)
{
    THROW_MSG_UNLESS(outFile != NULL, "f is NULL");
    FOR(uint32_t i = 0; i < 256; ++i)
    {
        uint8_t quad[4] = { (uint8_t)i, (uint8_t)i, (uint8_t)i, 0 }; /* B,G,R,0 */
        IF(fwrite(quad, 1, 4, outFile) != 4)
        {
            THROW_MSG("Failed to write palette entry");
        }
        END_IF;
    }
    END_LOOP;
}
END_FUN


FUN_IMPL(_bmp_write_file_header, FILE* outFile, const BMP_FILE_HDR* fileHeader)
{
    THROW_MSG_UNLESS(outFile && fileHeader, "NULL arg");
    IF(fwrite(fileHeader, sizeof(*fileHeader), 1, outFile) != 1)
    {
        THROW_MSG("Failed to write BMP file header");
    }
    END_IF;
}
END_FUN

FUN_IMPL(_bmp_write_rows_8bit, FILE* outFile, GrayImage* image,
    MEM_SIZE_T imageWidth, MEM_SIZE_T imageHeight, uint32_t alignedRowSizeBytes)
{
    // Basic argument validation
    THROW_MSG_UNLESS(outFile != NULL, "outFile is NULL");
    THROW_MSG_UNLESS(image != NULL, "image is NULL");

    // Row-size must be at least width and aligned to 4 bytes
    THROW_MSG_UNLESS(alignedRowSizeBytes >= (uint32_t)imageWidth, "rowSize < width");
    THROW_MSG_UNLESS((alignedRowSizeBytes & 0x3u) == 0u, "rowSize not 4-byte aligned");

    const uint32_t padBytes = alignedRowSizeBytes - (uint32_t)imageWidth;
    THROW_MSG_UNLESS(padBytes <= 3u, "row padding must be in [0..3]");

    // Emit rows from top to bottom (header must use negative height for top-down BMP)
    FOR(MEM_SIZE_T row = 0; row < imageHeight; ++row)
    {
        // Obtain a pointer to the first pixel in this row (column 0)
        uint8_t* rowPtr = NULL;
        MFUN(image, get_pixel_ptr), row, 0, & rowPtr CALL;
        THROW_MSG_UNLESS(rowPtr != NULL, "get_pixel_ptr returned NULL");

        // Write exactly 'imageWidth' bytes (no padding included yet)
        IF(fwrite(rowPtr, 1, (size_t)imageWidth, outFile) != (size_t)imageWidth)
        {
            THROW_MSG("Failed to write image row");
        }
        END_IF;

        // Write zero padding up to a 4-byte boundary (if needed)
        IF(padBytes > 0u)
        {
            // Maximum padding is 3 bytes
            const uint8_t zeros[3] = { 0, 0, 0 };
            IF(fwrite(zeros, 1, padBytes, outFile) != padBytes)
            {
                THROW_MSG("Failed to write row padding");
            }
            END_IF;
        }
        END_IF;
    }
    END_LOOP;
}
END_FUN

MEM_FUN_IMPL(GrayImage, save_to_bmp, const char* filePath)
{
    // -------------------------
    // Validate input parameters
    // -------------------------
    THROW_MSG_UNLESS(filePath != NULL, "filePath cannot be NULL");
    THROW_MSG_UNLESS(_this != NULL, "GrayImage instance is NULL");

    MEM_SIZE_T imageWidth = _this->width;
    MEM_SIZE_T imageHeight = _this->height;

    THROW_MSG_UNLESS(imageWidth > 0 && imageHeight > 0,
        "Image dimensions must be positive");

    // -------------------------
    // Open output file for writing
    // -------------------------
    FILE* outFile = fopen(filePath, "wb");
    THROW_MSG_UNLESS(outFile != NULL, "Failed to open BMP file for writing");

    // -------------------------
    // Compute all derived BMP sizes
    // (palette size, row alignment, total image data, file size, offset)
    // -------------------------
    uint32_t paletteSizeBytes = 0;
    uint32_t alignedRowSizeBytes = 0;
    uint32_t imageDataSizeBytes = 0;
    uint32_t totalFileSizeBytes = 0;
    uint32_t pixelDataOffsetBytes = 0;

    FUN(_bmp_calc_sizes)
        imageWidth, imageHeight,
        & paletteSizeBytes, & alignedRowSizeBytes,
        & imageDataSizeBytes, & totalFileSizeBytes, & pixelDataOffsetBytes CALL;

    // -------------------------
    // Prepare BMP headers
    // -------------------------
    BMP_FILE_HDR fileHeader;
    BMP_INFO_HDR infoHeader;

    // Fill both headers with all necessary metadata
    FUN(_bmp_fill_file_header)
        & fileHeader, totalFileSizeBytes, pixelDataOffsetBytes CALL;

    FUN(_bmp_fill_info_header)
        & infoHeader, imageWidth, imageHeight, imageDataSizeBytes CALL;

    // -------------------------
    // Write all BMP sections sequentially:
    // [File Header] → [Info Header] → [Palette] → [Pixel Rows]
    // -------------------------
    FUN(_bmp_write_file_header)      outFile, & fileHeader CALL;
    FUN(_bmp_write_info_header)      outFile, & infoHeader CALL;
    FUN(_bmp_write_palette_256_gray) outFile CALL;
    FUN(_bmp_write_rows_8bit)
        outFile, _this, imageWidth, imageHeight, alignedRowSizeBytes CALL;

    // -------------------------
    // Finalize file and clean up
    // -------------------------
    fclose(outFile);
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
BIND(GrayImage, save_to_bmp);
END_INIT_CLASS(GrayImage);