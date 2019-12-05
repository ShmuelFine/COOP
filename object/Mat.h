#ifndef __MAT__
#define __MAT__

#include <stdbool.h>
#include <stdlib.h>
#include <memory.h>
#include "Globals.h"
#include "ExportDefs.h"
//#include "coop/coop/object.h"
//#include "buildCOOP/buildCOOP/object.h"
#include "object.h"

#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define MIN(a,b) ((a) > (b) ? (b) : (a))
#define SGN(x) ((x) < 0? -1 : 1)
#define ALLOC_CACHE_BUFF(name, sizeInBytes) (FLOAT_TYPE *)Cache_Fetch_Assert(&TheGlobalCache, name, sizeInBytes)->buff
typedef struct
{
	int width;
	int height;
}Size;
Size BuildSize(int w, int h);


DEF_CLASS(Mat)

	FLOAT_TYPE* buff;
    object name;
	int width;
	int height;
	int step;
	const char* key_name;
	bool isConsistent;
END_DEF(Mat)


FUNCTIONS(Mat)
//FUNCTION_PTR(Mat, Print, int i, int j);
END_FUNCTIONS(Mat)

DEF_INIT_CLASS(Mat);


void _INNER_INIT(Mat* m, bool isConsistent);


#define IS_IN_BOUNDS(m, row, col) (0 <= row && row < (m).height && 0 <= col && col < (m).width)
#define MAT_PTR_OF(m, row, col) ((m).buff + (m).width*(row)*(m).step + (col)*(m).step)
FLOAT_TYPE * MatSafePtr(Mat m, int row, int col);
#define MAT_AT(m, row, col) (*MatSafePtr(m,row,col))
#define MAT_POS(m, pos) (*MatSafePtr(m, (pos) / (m).width, (pos) % (m).width))


Size size(Mat * m);
int area(Size s);
void Reset(Mat* m);

//Mat ViewTransposed(Mat * m);
#define LAST_ELEM(m) ((m).buff[area(size(&m))-1])

void AllocMat(Mat *m, int w, int h);
void RestoreConsistentMat(Mat* m);

typedef struct _scope_mats_list_t {
	Mat* m;
	struct _scope_mats_list_t* next;
}_scope_mats_list_;

void _scope_mats_list__init(_scope_mats_list_* anchor);
void _scope_mats_list__add(_scope_mats_list_* anchor, Mat* m);
void _scope_mats_list__free(_scope_mats_list_* anchor);

#define DO_MAT(name) CREATE_OBJ(Mat,name); name.key_name = #name; _INNER_INIT(&name, false); 
#define DO_CONSISTANT_MAT(name) CREATE_OBJ(Mat,name); name.key_name = #name; _INNER_INIT(&name, true); RestoreConsistentMat(&name);
#define DO_ALLOC_MAT(name, w, h) DO_MAT(name); AllocMat(&name, w, h);//AllocMat(&name, w, h);

#define ALLOC_SINGLE_VAL_MAT(val, name) AllocMat(&name, 1, 1); name.buff[0] = val;
#define ALLOC_CONST_VAL_MAT(name, sz, val) AllocMat(&name, sz.width, sz.height); Mat_set_scalar(&name, val);
#define ALLOC_ZEROES_MAT(name, sz)		ALLOC_CONST_VAL_MAT(name, sz, 0);
#define ALLOC_ONES_MAT(name, sz)			ALLOC_CONST_VAL_MAT(name, sz, 1);

#define CLONE_MAT(origPtr, outClonedPtr) AllocMat((outClonedPtr), (origPtr)->width, (origPtr)->height); CloneMat_noBuild((origPtr), (outClonedPtr));






void PrintMat(Mat const * m);

void FillMatWithSeq(Mat* m, FLOAT_TYPE startVal, FLOAT_TYPE step);
void CreateMatBySeq(Mat* m, FLOAT_TYPE startVal, FLOAT_TYPE step, FLOAT_TYPE endVal, bool isColumn);

void DivMat(Mat * m, FLOAT_TYPE val);

Mat ViewRowRange(Mat * orig, int start_row, int end_row);

Mat ViewRow(Mat * orig, int row);

Mat ViewCol(Mat * orig, int col);

Mat ViewColsOfRow(Mat * orig, int row, int start_col, int end_col);

void Mat_x_scalar(Mat * m, FLOAT_TYPE s);

void Mat_exp(Mat * m);

FLOAT_TYPE Mat_sum(Mat * m);

void Mat_abs(Mat * m);

void Mat_x_Mat_elementWise(Mat * m, Mat const * other);

void Mat_div_Mat_elementWise(Mat * m, Mat * other);

void Mat_add_Mat_elementWise(Mat * m, Mat * other);

void Mat_sub_Mat_elementWise(Mat * m, Mat * other);

void Mat_add_scalar(Mat * m, FLOAT_TYPE s);

void Mat_ceil(Mat * m);

FLOAT_TYPE Mat_max(Mat * m, int * idx);

FLOAT_TYPE Mat_min(Mat * m, int * idx);

void Mat_set_scalar(Mat * m, FLOAT_TYPE s);

void CloneMat_noBuild(Mat * orig, Mat * outCloned);

void BuildSuffledMat(Mat * src, Mat * shuffle, Mat * out, int inHeight);

void ConcatMats_byRows(Mat * a, Mat * b, Mat * out);

void ConcatMats_byCols(Mat * a, Mat * b, Mat * out);

void Transpose(Mat * m, Mat * mt);

void find2(Mat * m1, Mat * m2, bool(*Condition)(Mat *, Mat *, int row, int col), Mat * out);

void find_with_thresh(Mat * m1, Mat * m2, FLOAT_TYPE thresh, bool(*Condition)(Mat *, Mat *, FLOAT_TYPE, int row, int col), Mat * out);

void find(Mat * m, bool (*Condition)(Mat * , int row, int col), Mat * out);

void Check(Mat * m, bool(*Condition)(Mat *, int row, int col), Mat * out);

void diff_cols(Mat * m, Mat * out);

void diff_rows(Mat * m, Mat * out);

void ismember(Mat * src, Mat * elem, Mat * out);

bool isempty(Mat * m);

void MeanPerCol(Mat * src, Mat * out);

void SumPerCol(Mat * src, Mat * out);

void repmat(Mat * src, int times_height, int times_width, Mat * out);

FLOAT_TYPE Dot(Mat * a, Mat * b);

void Mat_x_Mat(Mat * a, Mat * b, Mat * out);

void cov(Mat * x, Mat * out);

void Mat_round(Mat * X);

void diag(Mat * r, Mat * out);

void Mat_sqrt(Mat * m);

FLOAT_TYPE Mat_sqr_sum(Mat * m);

void OuterProduct(Mat * x, Mat * y, Mat * out);

void correl(Mat * x, Mat * out);

#endif
