#ifndef __COOP__RECTANGLE__H_
#define __COOP__RECTANGLE__H_

#include "COOP.h"

DEF_CLASS(Rectangle);
// Define member fields here. e.g:
int hight;
int width;
END_DEF(Rectangle);

FUNCTIONS(Rectangle, /*ctor params:*/ int hight, int width);
MEM_FUN_DECL(Rectangle, /*func. name:*/ GetSideLength, /*func. params:*/int side_idx, int* ret_val);
MEM_FUN_DECL(Rectangle, /*func. name:*/ GetArea, /*func. params:*/ int* ret_val);
END_FUNCTIONS(Rectangle);

#endif