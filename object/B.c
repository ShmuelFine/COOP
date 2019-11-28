#include "B.h"
#include "Mat.h"

DEF_CTOR(B)
{	
	BASE();
	_this->Calculated_value = 0;
}
int aaa(int a, int b)
{
	return a + b;
		 
 }

DEF_CTOR(B, int val, int cVal)
{
	BASE(val);
	_this->Calculated_value = cVal;
}

DEF_DTOR(B)
{
}

INIT_CLASS(B)
END_INIT_CLASS


void main()
{	
	Mat_init();
	Mat m;
	m.vTable->_ctor(&m);

	CREATE_OBJ(Mat, m1);

	B_init();
	B *b;
	b->vTable->_ctor(1,23);

}


