#ifndef __COOP__GRAY_IMG__H_
#define __COOP__GRAY_IMG__H_

#include "COOP.h"
#include "vector.h"
#include <stdint.h>
#include <stdbool.h>

#define MOVE_TAG ((void*)0xDEADBEEF)

DEF_CLASS(GrayImage);
uint8_t* image_buffer; 
MEM_SIZE_T width;      
MEM_SIZE_T height;     
MEM_SIZE_T stride;     
size_t* refCount;     //how much pointer we need?    
size_t offset;
END_DEF(GrayImage);

//------

FUNCTIONS(GrayImage, MEM_SIZE_T width, MEM_SIZE_T height, Vector_char* data_vector);

/*
 * 2. פונקציות Init (שאר הבנאים)
 * אלו פונקציות עזר גלובליות (לא ב-VTable)
 */

 /**
  * @brief (בנאי העתקה - Shallow Copy)
  * יוצר כותרת (Header) חדשה המצביעה לאותו זיכרון. מעלה את מונה ההפניות.
  */
FUN_DECL(GrayImage_init_copy, GrayImage* _this, GrayImage const* other);

/**
 * @brief (בנאי העברה - Move Ctor)
 * "גונב" את המשאבים מהמקור ומאפס את המקור.
 */
FUN_DECL(GrayImage_init_move, GrayImage* _this, GrayImage* other, void* tag);

/**
 * @brief (בנאי ROI - View Ctor)
 * יוצר "מבט" (View) על אזור בתמונה קיימת. חולק זיכרון ומעלה מונה הפניות.
 */
FUN_DECL(GrayImage_init_ROI, GrayImage* _this, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height);

/**
 * @brief (פונקציית Clone - Deep Copy)
 * יוצר עותק חדש ועצמאי (Deep Copy) של התמונה.
 */
FUN_DECL(GrayImage_init_clone, GrayImage* _this, GrayImage const* other);


/*
 * 3. פונקציות חבר (VTable)
 */
MEM_FUN_DECL(GrayImage, get_width, MEM_SIZE_T* width_out);
MEM_FUN_DECL(GrayImage, get_height, MEM_SIZE_T* height_out);
MEM_FUN_DECL(GrayImage, get_stride, MEM_SIZE_T* stride_out);
MEM_FUN_DECL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr);

END_FUNCTIONS(GrayImage);

#endif