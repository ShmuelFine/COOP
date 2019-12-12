#include "Mat.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


DEF_CTOR(Mat)
{
	_this->buff = NULL;
}

DEF_DTOR(Mat)
{
	if (_this->buff)
	{
		Cache_RemoveBlockByName(&TheGlobalCache, _this->key_name);
		_this->buff = NULL;
	}
}

INIT_CLASS(Mat)
END_INIT_CLASS

void _scope_mats_list__init(_scope_mats_list_* anchor) { anchor->m = NULL; anchor->next = NULL; }

_scope_mats_list_* _scope_mats_list__find(_scope_mats_list_* anchor, Mat* m)
{
	for (_scope_mats_list_* it = anchor; it != NULL; it = it->next)
	{
		if (it->m && 0 == strcmp(it->m->key_name, m->key_name))
			return it;
	}
	return NULL;
}


Size BuildSize(int w, int h)
{
	Size res;
	res.width = w;
	res.height = h;
	return res;
}

FLOAT_TYPE * MatSafePtr(Mat m, int row, int col) 
{
	if (!IS_IN_BOUNDS(m, row, col))
		__debugbreak();

	return MAT_PTR_OF(m, row, col); 
}

void _INNER_INIT(Mat * m, bool isConsistent)
{
	m->buff = NULL;
	m->width = 0;
	m->height = 0;
	m->step = 1;
	m->isConsistent = isConsistent;
	//m->next = NULL;
 }

void Reset(Mat * m)
{
	if (m->buff)
		Cache_RemoveBlockByName(&TheGlobalCache, m->key_name);
	_INNER_INIT(m, m->isConsistent);
}

Size size(Mat * m)
{
	Size s;
	s.width = m->width;
	s.height = m->height;
	return s;
}
int area(Size s)
{
	return s.height*s.width;
}

void AllocMat(Mat *m, int w, int h)
{
	int sizeInBytes = w * h * sizeof(FLOAT_TYPE);
	if (0 == sizeInBytes)
	{
		Reset(m);
		return;
	}
	Block* existingBlock = Cache_FindBlockByName(&TheGlobalCache, m->key_name);
	bool shouldAlloc = !(existingBlock && existingBlock->size == sizeInBytes);
	if (shouldAlloc)
	{
		Reset(m);

		if (sizeInBytes != 0)
			m->buff = ALLOC_CACHE_BUFF(m->key_name, sizeInBytes);
		else
			m->buff = NULL;
	}
	else
		m->buff = (FLOAT_TYPE *) existingBlock->buff;

	m->width = w;
	m->height = h;
	m->step = 1;

	if (m->isConsistent)
	{
		Cache_FindBlockByName(&TheGlobalCache, m->key_name)->isSealed = true;
		Block * mySizeCache = Cache_Fetch_Assert(&TheGlobalConsistantMatsMetadata, m->key_name, sizeof(int) * 2);
		int* dim = (int*)mySizeCache->buff;
		dim[0] = m->width;
		dim[1] = m->height;
	}
}

void RestoreConsistentMat(Mat* m)
{
	Block* mySizeCache = Cache_FindBlockByName(&TheGlobalConsistantMatsMetadata, m->key_name);
	if (mySizeCache)
	{
		int* dim = (int*)mySizeCache->buff;
		m->width = dim[0];
		m->height = dim[1];
		AllocMat(m, m->width, m->height);
	}
	else
	{
		m->width = m->height = 0;
	}
}


void PrintMat(Mat const * m)
{
	printf("\t-- %s --\t\r\n", m->key_name);

	for (int i = 0; i < m->height; i++)
	{
		printf("%d:\t", i);
		for (int j = 0; j < m->width; j++)
			printf("%f ", MAT_AT(*m, i, j));

		printf("\r\n");
	}
}

void FillMatWithSeq(Mat * m, FLOAT_TYPE startVal, FLOAT_TYPE step)
{
	FLOAT_TYPE val = startVal;
	for (int i = 0; i < m->width*m->height; i++, val += step)
		*(m->buff + i * m->step) = val;
}

void CreateMatBySeq(Mat* m, FLOAT_TYPE startVal, FLOAT_TYPE step, FLOAT_TYPE endVal, bool isColumn)
{
	// length(m) := m.height + 1, for this func.
	// startVal + length(m) * step = endVal;
	// length(m) = (endVal - startVal)/step
	// and add another 1 for placing the endVal itself
	int length = (int)ceil((endVal - startVal) / step) + 1;
	if (length <= 1)
	{
		AllocMat(m, 1, 1);
		*m->buff = endVal;
		return;
	}

	if (isColumn)
		AllocMat(m, 1, length);
	else
		AllocMat(m, length, 1);

	FillMatWithSeq(m, startVal, step);
	
	LAST_ELEM(*m) = endVal;
}

void DivMat(Mat * m, FLOAT_TYPE val)
{
	for (int i = 0; i < m->width*m->height; i++)
		*(m->buff + i * m->step) /= val;
}

Mat ViewRowRange(Mat * orig, int start_row, int end_row)
{
	Mat view;
	view.buff = MatSafePtr(*orig, start_row, 0);
	view.step = 1;
	view.width = orig->width;
	view.height = end_row - start_row + 1;
	return view;
}

Mat ViewRow(Mat * orig, int row)
{
	return ViewRowRange(orig, row, row);
}

Mat ViewCol(Mat * orig, int col)
{
	Mat view;
	view.buff = MatSafePtr(*orig, 0, col);
	view.step = orig->width;
	view.width = 1;
	view.height = orig->height;
	view.key_name = orig->key_name;
	return view;
}

Mat ViewColsOfRow(Mat * orig, int row, int start_col, int end_col)
{
	Mat view;
	view.buff = MatSafePtr(*orig, row, start_col);
	view.step = 1;
	view.width = end_col - start_col + 1;
	view.height = 1;
	return view;
}

//Mat ViewCol(Mat * orig, int start_row, int start_col, int step, int end_col)
//{
//	Mat view;
//	view.buff = MatSafePtr(*orig, start_row, start_col);
//	view.step = step * orig->step;
//	view.width = orig->width / step;
//	if (end_col > start_col)
//		view.width = (end_col - start_col + 1) / step;
//
//	view.height = orig->height / step;
//	return view;
//}


void Mat_x_scalar(Mat * m, FLOAT_TYPE s)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = s * MAT_AT(*m, r, c);
}

void Mat_exp(Mat * m)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = exp(MAT_AT(*m, r, c));
}

FLOAT_TYPE Mat_sum(Mat * m)
{
	FLOAT_TYPE sum = 0;
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			sum += MAT_AT(*m, r, c);

	return sum;
}

void Mat_abs(Mat * m)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = fabs(MAT_AT(*m, r, c));
}


void Mat_x_Mat_elementWise(Mat * m, Mat const * other)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) *= MAT_AT(*other, r, c);
}

void Mat_div_Mat_elementWise(Mat * m, Mat * other)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) /= MAT_AT(*other, r, c);
}

void Mat_add_Mat_elementWise(Mat * m, Mat * other)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) += MAT_AT(*other, r, c);
}
void Mat_copy_from_Mat_elementWise(Mat* m, Mat* other)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = MAT_AT(*other, r, c);
}
void Mat_sub_Mat_elementWise(Mat * m, Mat * other)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) -= MAT_AT(*other, r, c);
}
void Mat_add_scalar(Mat * m, FLOAT_TYPE s)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = s + MAT_AT(*m, r, c);
}
void Mat_ceil(Mat * m)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = ceil(MAT_AT(*m, r, c));
}


FLOAT_TYPE Mat_min_max(Mat * m, int * idx, bool isMax)
{
	if (idx) *idx = 0;
	if (isempty(m))
	{
		__debugbreak();
		return 0;
	}
	FLOAT_TYPE best = MAT_AT(*m, 0, 0);

	for (int r = 0; r < m->height; r++)
	{
		for (int c = 0; c < m->width; c++)
		{
			FLOAT_TYPE curr = MAT_AT(*m, r, c);
			if (
				(isMax && curr > best) ||
				(!isMax && curr < best) )
			{
				best = curr;
				if (idx) *idx = r * m->width + c;
			}
		}
	}

	return best;
}

FLOAT_TYPE Mat_max(Mat * m, int * idx)
{
	return Mat_min_max(m, idx, true);
}

FLOAT_TYPE Mat_min(Mat * m, int * idx)
{
	return Mat_min_max(m, idx, false);
}

void Mat_set_scalar(Mat * m, FLOAT_TYPE s)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = s;
}

void CloneMat_noBuild(Mat * orig, Mat * outCloned)
{
	Mat_copy_from_Mat_elementWise(outCloned, orig);
}

void BuildSuffledMat(Mat * src, Mat * shuffle, Mat * out, int inHeight)
{
	if (isempty(shuffle))
	{
		Reset(out);
		return;
	}

	AllocMat(out, shuffle->width, shuffle->height);

	for (int r = 0; r < shuffle->height; r++)
	{
		for (int c = 0; c < shuffle->width; c++)
		{
			int shuffIdx = (int)MAT_AT(*shuffle, r, c);
			MAT_AT(*out, r, c) = inHeight ? MAT_AT(*src, shuffIdx, 0) : MAT_AT(*src, 0, shuffIdx);
		}
	}
}

void ConcatMats_byRows(Mat * a, Mat * b, Mat * out)
{
	if (isempty(a) && isempty(b)) { Reset(out); return; }

	if (isempty(a))	{		CLONE_MAT(b, out);		return;	}

	if (isempty(b)) {		CLONE_MAT(a, out);		return; }

	if (a->width != b->width)
		return;

	AllocMat(out, a->width, (a->height + b->height));
	Mat a_view = ViewRowRange(out, 0, a->height - 1);
	CloneMat_noBuild(a, &a_view);
	Mat b_view = ViewRowRange(out, a->height,  out->height - 1);
	CloneMat_noBuild(b, &b_view);
}

void ConcatMats_byCols(Mat * a, Mat * b, Mat * out)
{
	if (isempty(a) && isempty(b)) { Reset(out); return; }

	if (isempty(a)) { CLONE_MAT(b, out);		return; }

	if (isempty(b)) { CLONE_MAT(a, out);		return; }


	AllocMat(out, a->width + b->width, a->height);
	for (int r = 0; r < out->height; r++)
	{
		for (int c = 0; c < out->width; c++)
		{
			MAT_AT(*out, r, c) = c < a->width ? MAT_AT(*a, r, c) : MAT_AT(*b, r, c - a->width);
		}
	}
}

void Transpose(Mat * m, Mat * mt)
{
	AllocMat(mt, m->height, m->width);
	for (int r = 0; r < m->height; r++)
	{
		for (int c = 0; c < m->width; c++)
		{
			MAT_AT(*mt, c, r) = MAT_AT(*m, r, c);
		}
	}
}

void find2(Mat * m1, Mat * m2, bool(*Condition)(Mat *, Mat *, int row, int col), Mat * out)
{
	int currIdx = 0;
	for (int r = 0; r < m1->height; r++)
	{
		for (int c = 0; c < m1->width; c++)
		{
			if (Condition(m1, m2, r, c))
			{
				currIdx++;
			}
		}
	}
	if (!currIdx)
	{
		Reset(out);
		return;
	}

	AllocMat(out, 1, currIdx);
	currIdx = 0;
	for (int r = 0; r < m1->height; r++)
	{
		for (int c = 0; c < m1->width; c++)
		{
			if (Condition(m1, m2, r, c))
			{
				out->buff[currIdx] = r * m1->width + c;
				currIdx++;
			}
		}
	}
}

void find_with_thresh(Mat * m1, Mat * m2, FLOAT_TYPE thresh, bool(*Condition)(Mat *, Mat *, FLOAT_TYPE , int row, int col), Mat * out)
{
	int currIdx = 0;
	for (int r = 0; r < m1->height; r++)
	{
		for (int c = 0; c < m1->width; c++)
		{
			if (Condition(m1, m2, thresh, r, c))
			{
				currIdx++;
			}
		}
	}
	if (!currIdx)
	{
		Reset(out);
		return;
	}

	AllocMat(out, 1, currIdx);

	currIdx = 0;
	for (int r = 0; r < m1->height; r++)
	{
		for (int c = 0; c < m1->width; c++)
		{
			if (Condition(m1, m2, thresh, r, c))
			{
				out->buff[currIdx] = r * m1->width + c;
				currIdx++;
			}
		}
	}
}

void find(Mat * m, bool(*Condition)(Mat * , int row, int col), Mat * out)
{
	int currIdx = 0;
	for (int r = 0; r < m->height; r++)
	{
		for (int c = 0; c < m->width; c++)
		{
			if (Condition(m, r, c))
			{
				currIdx++;
			}
		}
	}

	AllocMat(out, 1, currIdx);
	currIdx = 0;
	for (int r = 0; r < m->height; r++)
	{
		for (int c = 0; c < m->width; c++)
		{
			if (Condition(m, r, c))
			{
				out->buff[currIdx] = r * m->width + c;
				currIdx++;
			}
		}
	}
}

void Check(Mat * m, bool(*Condition)(Mat *, int row, int col), Mat * out)
{
	AllocMat(out, m->width, m->height);
	for (int r = 0; r < m->height; r++)
	{
		for (int c = 0; c < m->width; c++)
		{
			MAT_AT(*out, r, c) = Condition(m, r, c);
		}
	}
}


void diff_cols(Mat * m, Mat * out)
{
	AllocMat(out, m->width - 1, m->height);
	for (int c = 1; c < m->width; c++)
	{
		for (int r = 0; r < m->height; r++)
		{
			MAT_AT(*out, r, c - 1) = MAT_AT(*m, r, c) - MAT_AT(*m, r, c - 1);
		}
	}
}

void diff_rows(Mat * m, Mat * out)
{

	AllocMat(out, m->width, m->height - 1);
	for (int c = 0; c < m->width; c++)
	{
		for (int r = 1; r < m->height; r++)
		{
			MAT_AT(*out, r - 1, c) = MAT_AT(*m, r, c) - MAT_AT(*m, r - 1, c);
		}
	}
}


bool ismember_inner(Mat * src, FLOAT_TYPE elem)
{
	for (int r = 0; r < src->height; r++)
		for (int c = 0; c < src->width; c++)
			if (elem == MAT_AT(*src, r, c))
				return true;

	return false;
}

void ismember(Mat * src, Mat * elem, Mat * out)
{
	int count = 0;
	for (int r = 0; r < elem->height; r++)
		for (int c = 0; c < elem->width; c++)
			if (ismember_inner(src, MAT_AT(*elem, r, c)))
				count++;

	AllocMat(out, count, 1);

	count = 0;
	for (int r = 0; r < elem->height; r++)
		for (int c = 0; c < elem->width; c++)
			if (ismember_inner(src, MAT_AT(*elem, r, c)))
				out->buff[count++] = MAT_AT(*elem, r, c);
}

bool isempty(Mat * m)
{
	return 0 == m->width*m->height;
}

void MeanPerCol(Mat * src, Mat * out)
{
	AllocMat(out, src->width, 1);
	for (int col = 0; col < src->width; col++)
	{
		FLOAT_TYPE sum = 0;
		for (int r = 0; r < src->height; r++)
			sum += MAT_AT(*src, r, col);
		sum /= (FLOAT_TYPE)src->height;
		
		MAT_AT(*out, 0, col) = sum;
	}
}

void SumPerCol(Mat * src, Mat * out)
{
	AllocMat(out, src->width, 1);
	for (int col = 0; col < src->width; col++)
	{
		FLOAT_TYPE sum = 0;
		for (int r = 0; r < src->height; r++)
			sum += MAT_AT(*src, r, col);

		MAT_AT(*out, 0, col) = sum;
	}
}

void repmat(Mat * src, int times_height, int times_width, Mat * out)
{
	AllocMat(out, src->width * times_width, src->height * times_height);
	for (int r = 0; r < out->height; r++)
		for (int c = 0; c < out->width; c++)
			MAT_AT(*out, r, c) = MAT_AT(*src, r % src->height, c % src->width);
}

FLOAT_TYPE Dot(Mat * a, Mat * b)
{
	FLOAT_TYPE sum = 0;
	for (int k = 0; k < a->width; k++)
	{
		sum += MAT_AT(*a, 0, k)*MAT_AT(*b, k, 0);
	}
	return sum;
}
void Mat_x_Mat(Mat * a, Mat * b, Mat * out)
{
	AllocMat(out, b->width, a->height);
	for (int r = 0; r < out->height; r++)
	{
		for (int c = 0; c < out->width; c++)
		{
			Mat row = ViewRow(a, r);
			Mat col = ViewCol(b, c);
			MAT_AT(*out, r, c) = Dot(&row, &col);
		}
	}
}
void cov(Mat * x, Mat * out)
{
	SCOPE_START;

	DO_MAT(x_without_mean);
	ALLOC_ZEROES_MAT(x_without_mean, size(x));

	FLOAT_TYPE col_size = x->width;
	for (int r = 0; r < x->height; r++)
	{
		Mat viewCol = ViewRow(x, r);
		FLOAT_TYPE col_sum = Mat_sum(&viewCol);
		FLOAT_TYPE mean = col_sum / col_size;

		Mat dstViewRow = ViewRow(&x_without_mean, r);
		CloneMat_noBuild(&viewCol, &dstViewRow);
		Mat_add_scalar(&dstViewRow, -1 * mean);

	}

	DO_MAT(x_without_mean_T); Transpose(&x_without_mean, &x_without_mean_T);
	Mat_x_Mat(&x_without_mean, &x_without_mean_T, out);
	FLOAT_TYPE denom = x->width > 1 ? x->width - 1 : 1;
	Mat_x_scalar(out, 1. / denom);

	SCOPE_END;
}

void Mat_round(Mat * X)
{
	for (int r = 0; r < X->height; r++)
		for (int c = 0; c < X->width; c++)
			MAT_AT(*X, r, c) = round(MAT_AT(*X, r, c));
}
void diag(Mat * r, Mat * out)
{
	AllocMat(out, 1, r->height);
	for (int i = 0; i < r->height; i++)
		MAT_AT(*out, i, 0) = MAT_AT(*r, i, i);
}
void Mat_sqrt(Mat * m)
{
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			MAT_AT(*m, r, c) = sqrt(MAT_AT(*m, r, c));

}
FLOAT_TYPE Mat_sqr_sum(Mat * m)
{
	FLOAT_TYPE sum = 0;
	for (int r = 0; r < m->height; r++)
		for (int c = 0; c < m->width; c++)
			sum += MAT_AT(*m, r, c) * MAT_AT(*m, r, c);
	return sum;
}
void OuterProduct(Mat * x, Mat * y, Mat * out)
{
	AllocMat(out, x->width, y->height);
	for (int r = 0; r < out->height; r++)
		for (int c = 0; c < out->width; c++)
			MAT_AT(*out,r,c) = MAT_AT(*x, 0, c) * MAT_AT(*y, r, 0);

}
void correl(Mat * x, Mat * out)
{
	SCOPE_START;
	//% CORREL Compute correlation matrix without error checking.

	//Size s = size(x);//[n, m]
	DO_MAT(r);  cov(x, &r);
	DO_MAT(d); diag(&r, &d); Mat_sqrt(&d);//% sqrt first to avoid under / overflow
	DO_MAT(d_t); Transpose(&d, &d_t);
	DO_MAT(dd); OuterProduct(&d_t, &d, &dd);
	Mat_div_Mat_elementWise(&r, &dd);
	// % Fix up possible round - off problems, while preserving NaN : put exact 1 on the
	// % diagonal, and limit off - diag to[-1, 1].
	DO_MAT(r_t); Transpose(&r, &r_t);
	Mat_add_Mat_elementWise(&r, &r_t);
	Mat_x_scalar(&r, .5);
	for (int i = 0; i < r.height; i++)
		MAT_AT(r, i, i) = SGN(MAT_AT(r, i, i));

	CLONE_MAT((&r), out);
	SCOPE_END;
}
