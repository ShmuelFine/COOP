#include "GrayImage.h"
#include "DynamicMemoryManagement.h"
#include "Functions.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


DEF_CTOR(GrayImage)
{
    _this->width = 0;
    _this->height = 0;
    _this->stride = 0;
    _this->offset = 0;
	_this->refCount = NULL;
	_this->image_buffer = NULL;
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

MEM_FUN_IMPL(GrayImage, init, MEM_SIZE_T width, MEM_SIZE_T height, Vector_uint8_t* data_vector) {
    THROW_MSG_UNLESS(width > 0 && height > 0, "Width and Height must be positive");
    THROW_MSG_UNLESS(_this->refCount == NULL && _this->image_buffer == NULL, "Image already initialized - cannot init twice");

    ALLOC_ARRAY(_this->image_buffer, uint8_t, height * width);
    ASSERT_NOT_NULL(_this->image_buffer);

    ALLOC(_this->refCount, size_t);
    *(_this->refCount) = 1;

    _this->width = width;
    _this->height = height;
    _this->stride = width;
    _this->offset = 0;

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
}END_FUN

MEM_FUN_IMPL(GrayImage, init_copy, GrayImage const* other) {
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");
    THROW_MSG_UNLESS(other->refCount != NULL, "Source image not properly initialized");

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
    THROW_MSG_UNLESS(other->refCount != NULL, "Source image not properly initialized");
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
    THROW_MSG_UNLESS(other->refCount != NULL, "Source image not properly initialized");
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


MEM_FUN_IMPL(GrayImage, clone, GrayImage* out_clone)
{
    THROW_MSG_UNLESS(out_clone != NULL, "Output pointer cannot be NULL");
    THROW_MSG_UNLESS(_this->refCount != NULL, "Source image not properly initialized");
	MFUN(out_clone, init), _this->width, _this->height, NULL CALL;

    // 5.copy row - row
    FOR(MEM_SIZE_T r = 0; r < _this->height; ++r)
    {
        //row source
        uint8_t* src_row_ptr = _this->image_buffer + _this->offset + (r * _this->stride);
        // row destination
        uint8_t* dst_row_ptr = out_clone->image_buffer + (out_clone->stride * r);  
        memcpy(dst_row_ptr, src_row_ptr, _this->width);
    }
    END_LOOP;

}
END_FUN

MEM_FUN_IMPL(GrayImage, get_width, MEM_SIZE_T* width_out) 
{
    THROW_MSG_UNLESS(width_out != NULL, "Output pointer cannot be NULL");
    *width_out = _this->width;
} 
END_FUN

MEM_FUN_IMPL(GrayImage, get_height, MEM_SIZE_T* height_out) 
{
	THROW_MSG_UNLESS(height_out != NULL, "Output pointer cannot be NULL");
    *height_out = _this->height;
} 
END_FUN

MEM_FUN_IMPL(GrayImage, get_stride, MEM_SIZE_T* stride_out)
{
	THROW_MSG_UNLESS(stride_out != NULL, "Output pointer cannot be NULL");
    *stride_out = _this->stride;
}
END_FUN

MEM_FUN_IMPL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr)
{
    THROW_MSG_UNLESS(out_ptr != NULL, "Output pointer cannot be NULL");
    THROW_MSG_UNLESS(_this->refCount != NULL, "Image not properly initialized");
    THROW_MSG_UNLESS(row < _this->height && col < _this->width, "Pixel out of bounds");

    *out_ptr = _this->image_buffer + _this->offset + (row * _this->stride) + col;
}
END_FUN

MEM_FUN_IMPL(GrayImage, add, GrayImage const* other, GrayImage* out)
{
    THROW_MSG_UNLESS(_this != NULL && other != NULL && out != NULL, "NULL image in arithmetic op");
    THROW_MSG_UNLESS(_this->refCount != NULL && other->refCount != NULL && out->refCount != NULL, "Images not properly initialized");
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
    THROW_MSG_UNLESS(_this->refCount != NULL && other->refCount != NULL && out->refCount != NULL, "Images not properly initialized");
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
    THROW_MSG_UNLESS(_this->refCount != NULL && other->refCount != NULL && out->refCount != NULL, "Images not properly initialized");
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
    THROW_MSG_UNLESS(_this->refCount != NULL && out->refCount != NULL, "Images not properly initialized");
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
    THROW_MSG_UNLESS(_this->refCount != NULL && other->refCount != NULL && out->refCount != NULL, "Images not properly initialized");
    THROW_MSG_UNLESS(_this->width == other->height, "Inner dims mismatch: A.width must equal B.height");
    THROW_MSG_UNLESS(out->height == _this->height && out->width == other->width, "Output size must be (A.height × B.width)");

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

MEM_FUN_IMPL(GrayImage, equals, GrayImage const* other, GrayImage* out_comparison_image)
{
    THROW_MSG_UNLESS(_this->refCount != NULL, "This image not properly initialized");
    THROW_MSG_UNLESS(other != NULL, "Other image cannot be NULL");
    THROW_MSG_UNLESS(other->refCount != NULL, "Other image not properly initialized");
    THROW_MSG_UNLESS(out_comparison_image != NULL, "Output pointer cannot be NULL");
    THROW_MSG_UNLESS(_this->width == other->width && _this->height == other->height, "Image dimensions must match for comparison");
    THROW_MSG_UNLESS(out_comparison_image->width == _this->width && out_comparison_image->height == _this->height, "Output image dimensions must match inputs");

    FOR(MEM_SIZE_T r = 0; r < _this->height; ++r)
    {
        uint8_t* this_row = _this->image_buffer + _this->offset + (r * _this->stride);

        uint8_t* other_row = other->image_buffer + other->offset + (r * other->stride);

        uint8_t* result_row = out_comparison_image->image_buffer + (r * out_comparison_image->stride);

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

    TRY{
        FUN(_bmp_calc_sizes)
        imageWidth, imageHeight,
        & paletteSizeBytes,& alignedRowSizeBytes,
        & imageDataSizeBytes,& totalFileSizeBytes,& pixelDataOffsetBytes CALL;
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
        FUN(_bmp_write_file_header)      outFile,& fileHeader CALL;
        FUN(_bmp_write_info_header)      outFile,& infoHeader CALL;
        FUN(_bmp_write_palette_256_gray) outFile CALL;
        FUN(_bmp_write_rows_8bit)
            outFile, _this, imageWidth, imageHeight, alignedRowSizeBytes CALL;

        // -------------------------
        // Finalize file and clean up
        // -------------------------
        fclose(outFile);
		outFile = NULL;
    }
    CATCH{
         IF(outFile) {
          fclose(outFile);
         }END_IF
        THROW;   
    }
    END_TRY;
}
END_FUN

MEM_FUN_IMPL(GrayImage, load_from_bmp, const char* path)
{
    THROW_MSG_UNLESS(path != NULL, "Path cannot be NULL");
    THROW_MSG_UNLESS(_this != NULL, "out_img cannot be NULL");

    FILE* f = fopen(path, "rb");
    THROW_MSG_UNLESS(f != NULL, "Failed to open file for reading");

    BMP_FILE_HDR fh;
    BMP_INFO_HDR ih;

    TRY{
        THROW_MSG_UNLESS(fread(&fh, sizeof(fh), 1, f) == 1, "Failed to read BMP file header");
        THROW_MSG_UNLESS(fread(&ih, sizeof(ih), 1, f) == 1, "Failed to read BMP info header");
        THROW_MSG_UNLESS(fh.signature == (uint16_t)0x4D42, "Not a BMP file ('BM' signature mismatch)");
        THROW_MSG_UNLESS(ih.headerSize >= (uint32_t)sizeof(BMP_INFO_HDR), "Unsupported BMP info header size (<40)");
        THROW_MSG_UNLESS(ih.colorPlanes == 1, "Unsupported BMP: colorPlanes != 1");
        THROW_MSG_UNLESS(ih.compressionMethod == 0U, "Unsupported BMP compression (must be BI_RGB)");
        THROW_MSG_UNLESS(ih.bitsPerPixel == 8U || ih.bitsPerPixel == 24U, "Only 8-bit or 24-bit BMP is supported");
        THROW_MSG_UNLESS(ih.imageWidth > 0 && ih.imageHeight != 0, "Invalid BMP dimensions");

        const MEM_SIZE_T width = (MEM_SIZE_T)ih.imageWidth;
        const MEM_SIZE_T height = (MEM_SIZE_T)(ih.imageHeight > 0 ? ih.imageHeight : -ih.imageHeight);
        const bool bottom_up = (ih.imageHeight > 0);
        const uint32_t bytesPerPixel = (ih.bitsPerPixel == 24U) ? 3U : 1U;

        const uint32_t rowSizeFile = (((uint32_t)width * bytesPerPixel + 3U) / 4U) * 4U;

        THROW_MSG_UNLESS(fseek(f, (long)fh.pixelDataOffset, SEEK_SET) == 0, "Failed to seek to pixel data");


        IF(_this->refCount != NULL)
        {
            (*(_this->refCount))--;
            IF((*(_this->refCount)) == 0)
            {
                FREE(_this->image_buffer);
                FREE(_this->refCount);
            }
            END_IF;
            _this->refCount = NULL;
            _this->image_buffer = NULL;
        }
        END_IF;

        ALLOC_ARRAY(_this->image_buffer, uint8_t, height * width);
        THROW_MSG_UNLESS(_this->image_buffer != NULL, "Failed to allocate image buffer");
        ALLOC(_this->refCount, size_t);
        IF(_this->refCount == NULL)
        {
            FREE(_this->image_buffer);
            THROW_MSG("Failed to allocate refCount");
        }
        END_IF;
        *(_this->refCount) = 1;

        _this->width = width;
        _this->height = height;
        _this->stride = width;
        _this->offset = 0;

        uint8_t* fileRow = NULL;
        ALLOC_ARRAY(fileRow, uint8_t, rowSizeFile);
        THROW_MSG_UNLESS(fileRow != NULL, "Failed to allocate row buffer");

        FOR(MEM_SIZE_T r = 0; r < height; ++r)
        {
            const MEM_SIZE_T src_r = bottom_up ? (height - 1U - r) : r;
            const long rowOffset = (long)fh.pixelDataOffset + (long)(src_r * rowSizeFile);

            IF(fseek(f, rowOffset, SEEK_SET) != 0)
            {
                FREE(fileRow);
                THROW_MSG("Failed to seek to row");
            }
            END_IF;

            IF(fread(fileRow, 1, rowSizeFile, f) != rowSizeFile)
            {
                FREE(fileRow);
                THROW_MSG("Failed to read BMP row");
            }
            END_IF;

            uint8_t* dst = _this->image_buffer + _this->offset + r * _this->stride;

            IF(bytesPerPixel == 1U)
            {
                memcpy(dst, fileRow, (size_t)width);
            }
            ELSE
            {
                FOR(MEM_SIZE_T c = 0; c < width; ++c)
                {
                    const uint8_t B = fileRow[c * 3U + 0U];
                    const uint8_t G = fileRow[c * 3U + 1U];
                    const uint8_t R = fileRow[c * 3U + 2U];
                    const uint32_t y = 77U * (uint32_t)R + 150U * (uint32_t)G + 29U * (uint32_t)B + 128U;
                    dst[c] = (uint8_t)(y >> 8);
                }
                END_LOOP;
            }
            END_IF;
        }
        END_LOOP;

        FREE(fileRow);
        fclose(f);
    }
    CATCH{
        fclose(f);
        THROW;
    }
    END_TRY;
}
END_FUN

MEM_FUN_IMPL(GrayImage, canny, uint8_t low_thresh, uint8_t high_thresh)
{
    THROW_MSG_UNLESS(_this->refCount != NULL, "Canny: Source image not initialized properly");
    THROW_MSG_UNLESS(low_thresh <= high_thresh, "Canny: The low threshold cannot be higher than the high threshold");

    MEM_SIZE_T width = _this->width;
    MEM_SIZE_T height = _this->height;

    THROW_MSG_UNLESS(width >= 3 && height >= 3, "Canny: Image dimensions must be at least 3x3");

    // Create a reference image to store the gradient directions
    CREATE(GrayImage, direction_image) CALL;

    // Step 1: Gaussian Blur (Noise Reduction)
    FUN(__gaussian_blur) _this CALL;

    // Step 2: Sobel Filter (Gradient Calculation)
    FUN(__sobel_filter) _this, & direction_image CALL;

    // Stage 3: Non-Maximum Suppression (NMS)
    FUN(__non_maximum_suppression) _this, & direction_image CALL;

    // Step 4: Double Threshold (Hysteresis)
    FUN(__hysteresis_thresholding) _this, low_thresh, high_thresh CALL;

    // Step 5: Reset the picture frame
    FUN(__zero_border) _this CALL;
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
BIND(GrayImage, save_to_bmp);
BIND(GrayImage, load_from_bmp);
BIND(GrayImage, canny);
BIND(GrayImage, add);
BIND(GrayImage, sub_default);
BIND(GrayImage, sub_abs);
BIND(GrayImage, mul_scalar);
BIND(GrayImage, mul_mat);
BIND(GrayImage, equals);
BIND(GrayImage, init);
END_INIT_CLASS(GrayImage);