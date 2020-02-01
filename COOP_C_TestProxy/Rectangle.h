#ifndef __COOP__RECTANGLE__H_
#define __COOP__RECTANGLE__H_

#ifdef __cplusplus
extern "C" {
#endif

#include "coop.h"

	DEF_CLASS(Rectangle);
	// Define member fields here. e.g:
	int hight;
	int width;
	END_DEF(Rectangle);

	FUNCTIONS(Rectangle, /*ctor params:*/ int hight, int width);
	FUNCTION_PTR(Rectangle, /*func. name:*/ GetSideLength, /*func. params:*/int side_idx, int* ret_val);
	FUNCTION_PTR(Rectangle, /*func. name:*/ GetArea, /*func. params:*/ int* ret_val);
	END_FUNCTIONS(Rectangle);

#ifdef __cplusplus
}
#endif


#endif