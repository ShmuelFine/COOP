#include "GrayImage.h"
#include "DynamicMemoryManagement.h"
#include <string.h> // for memset/memcpy
#include <stdlib.h> // for NULL

/* ============================================= */
/* === 1. בנאי ראשי (CTOR) === */
/* ============================================= */

DEF_CTOR(GrayImage, MEM_SIZE_T width, MEM_SIZE_T height, Vector_char* data_vector)
{
    THROW_MSG_UNLESS(width > 0 && height > 0, "Width and Height must be positive");

    // 1. הקצאת זיכרון לנתונים
    ALLOC_ARRAY(_this->image_buffer, uint8_t, height * width);
    ASSERT_NOT_NULL(_this->image_buffer);

    // 2. הקצאת מונה הפניות
    ALLOC(_this->refCount, int);
    *(_this->refCount) = 1;

    // 3. הגדרת מטא-דאטה (אנחנו הבעלים, בתחילת הזיכרון)
    _this->width = width;
    _this->height = height;
    _this->stride = width;
    _this->offset = 0;

    // 4. אתחול הנתונים
    IF(data_vector != NULL) {
        // אם סופק וקטור, העתק ממנו
        MEM_SIZE_T vec_size = 0;
        MFUN(data_vector, size), & vec_size CALL;
        THROW_MSG_UNLESS(vec_size == (width * height), "Data vector size mismatch");

        uint8_t* vec_data_ptr = NULL;
        MFUN(data_vector, dataPtr), (char**)&vec_data_ptr CALL;
        memcpy(_this->image_buffer, vec_data_ptr, vec_size);
    }
    ELSE{
        // אם לא סופק וקטור (NULL), אפס את הזיכרון
        memset(_this->image_buffer, 0, width * height);
    }
    END_IF;
}
END_CTOR

/* ============================================= */
/* === 2. דסטרקטור (DTOR) === */
/* ============================================= */

DEF_DTOR(GrayImage)
{
    IF(_this->refCount != NULL) {
        // 1. הורד את מונה ההפניות
        (*(_this->refCount))--;

        // 2. בדוק אם אנחנו האחרונים שמחזיקים במשאבים
        IF((*(_this->refCount)) == 0) {
            // 3. אם כן, שחרר את *כל* המשאבים המשותפים
            FREE(_this->image_buffer);
            FREE(_this->refCount);
        }
        END_IF;
    }END_IF;
}
END_DTOR

/* ============================================= */
/* === 3. מימוש פונקציות Init === */
/* ============================================= */

/**
 * @brief (בנאי העתקה - Shallow Copy)
 * מדמה את התנהגות ברירת המחדל של cv::Mat (מעלה refCount)
 */
    FUN_IMPL(GrayImage_init_copy, GrayImage* _this, GrayImage const* other)
{
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");

    // 1. העתקת המטא-דאטה והמצביעים (העתקה שטחית)
    _this->image_buffer = other->image_buffer;
    _this->width = other->width;
    _this->height = other->height;
    _this->stride = other->stride;
    _this->offset = other->offset;
    _this->refCount = other->refCount;

    // 2. הגדלת מונה ההפניות המשותף
    (*(_this->refCount))++;
}
END_FUN

/**
 * @brief (בנאי העברה - Move Ctor)
 */
    FUN_IMPL(GrayImage_init_move, GrayImage* _this, GrayImage* other, void* tag)
{
    THROW_MSG_UNLESS(tag == MOVE_TAG, "Invalid tag used for Move Ctor");
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");

    // 1. "גניבת" כל המשאבים והמטא-דאטה
    _this->image_buffer = other->image_buffer;
    _this->width = other->width;
    _this->height = other->height;
    _this->stride = other->stride;
    _this->offset = other->offset;
    _this->refCount = other->refCount;

    // 2. איפוס המקור (כדי שה-DTOR שלו לא יפעל)
    other->image_buffer = NULL;
    other->refCount = NULL;
    other->width = 0;
    other->height = 0;
    other->stride = 0;
    other->offset = 0;
}
END_FUN

/**
 * @brief (בנאי ROI - View Ctor)
 */
    FUN_IMPL(GrayImage_init_ROI, GrayImage* _this, GrayImage const* other, MEM_SIZE_T row, MEM_SIZE_T col, MEM_SIZE_T ROI_width, MEM_SIZE_T ROI_height)
{
    // 1. ולידציה של גבולות
    THROW_MSG_UNLESS(other != NULL, "Source image (other) cannot be NULL");
    THROW_MSG_UNLESS(col + ROI_width <= other->width && row + ROI_height <= other->height, "ROI boundary error");

    // 2. העתקת מצביעים משותפים
    _this->image_buffer = other->image_buffer;
    _this->refCount = other->refCount;
    (*(_this->refCount))++; // הגדלת המונה המשותף

    // 3. הגדרת המטא-דאטה החדש (ה-View)
    _this->width = ROI_width;
    _this->height = ROI_height;
    _this->stride = other->stride; // ה-Stride נשאר זהה למקור!

    // 4. חישוב ה-Offset החדש
    _this->offset = other->offset + (row * other->stride) + col;
}
END_FUN

/**
 * @brief (פונקציית Clone - Deep Copy)
 * מדמה את cv::Mat::clone()
 */
    FUN_IMPL(GrayImage_init_clone, GrayImage* _this, GrayImage const* other)
{
    // 1. קרא לבנאי הראשי כדי להקצות זיכרון חדש (w, h, data=NULL)
    DEF_CTOR_CALL(GrayImage, _this), other->width, other->height, (Vector_char*)NULL CALL;

    // 2. בצע העתקה עמוקה (Deep Copy)
    // אם המקור הוא ROI, צריך להעתיק פיקסל-פיקסל (לא ניתן להשתמש ב-memcpy)
    IF(other->stride == other->width) {
        // המקור רציף, אפשר להשתמש ב-memcpy
        uint8_t* src_ptr = other->image_buffer + other->offset;
        memcpy(_this->image_buffer, src_ptr, _this->width * _this->height);
    }
    ELSE{
        // המקור אינו רציף (הוא ROI), חייבים העתקה שורה-שורה
        uint8_t * src_start = other->image_buffer + other->offset;
        uint8_t* dst_start = _this->image_buffer;

        FOR(MEM_SIZE_T r = 0; r < _this->height; ++r) {
            memcpy(dst_start, src_start, _this->width);
            src_start += other->stride; // קפוץ שורה שלמה במקור
            dst_start += _this->width;  // קפוץ שורה (רציפה) ביעד
        }
        END_LOOP;
    }
    END_IF;
}
END_FUN


/* ============================================= */
/* === 4. פונקציות חבר (VTable) === */
/* ============================================= */

MEM_FUN_IMPL(GrayImage, get_width, MEM_SIZE_T* width_out) {
    *width_out = _this->width;
} END_FUN

MEM_FUN_IMPL(GrayImage, get_height, MEM_SIZE_T* height_out) {
    *height_out = _this->height;
} END_FUN

MEM_FUN_IMPL(GrayImage, get_stride, MEM_SIZE_T* stride_out) {
    *stride_out = _this->stride;
} END_FUN

/**
 * @brief מחזיר מצביע לפיקסל ספציפי (כולל חישוב offset)
 */
    MEM_FUN_IMPL(GrayImage, get_pixel_ptr, MEM_SIZE_T row, MEM_SIZE_T col, uint8_t** out_ptr)
{
    THROW_MSG_UNLESS(row < _this->height && col < _this->width, "Pixel out of bounds");

    // חישוב הכתובת הסופית:
    // בסיס + היסט כללי (של ה-View) + היסט שורה + היסט עמודה
    *out_ptr = _this->image_buffer + _this->offset + (row * _this->stride) + col;
}
END_FUN


/* ============================================= */
/* === 5. אתחול המחלקה === */
/* ============================================= */

INIT_CLASS(GrayImage);
BIND(GrayImage, get_height);
BIND(GrayImage, get_stride);
BIND(GrayImage, get_width);
BIND(GrayImage, get_pixel_ptr);
END_INIT_CLASS(GrayImage);