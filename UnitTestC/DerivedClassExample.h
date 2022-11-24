#ifndef __COOP_TESTS__DERIVED_CLASS_EXAMPLE__H__
#define __COOP_TESTS__DERIVED_CLASS_EXAMPLE__H__


#include "BaseClassExample.h"
#include "../COOP/ExportDefs.h"

DEF_DERIVED_CLASS(DerivedClassExample, BaseClassExample);
int depth;
END_DEF_DERIVED(DerivedClassExample);

DERIVED_FUNCTIONS(DerivedClassExample, BaseClassExample, int hight, int width, int depth)
FUN_OVERRIDE(BaseClassExample, GetVolume, int* retVal);
MEM_FUN_DECL(DerivedClassExample, JustReturn10, int* retVal);
END_DERIVED_FUNCTIONS(DerivedClassExample);


#endif