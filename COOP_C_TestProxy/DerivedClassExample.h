#ifndef __DERIVED_CLASS_EXAMPLE__H__
#define __DERIVED_CLASS_EXAMPLE__H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ExportDefs.h"
#include "BaseClassExample.h"

	DEF_DERIVED_CLASS(DerivedClassExample, BaseClassExample);
	int depth;
	END_DEF_DERIVED(DerivedClassExample);

	DERIVED_FUNCTIONS(DerivedClassExample, BaseClassExample, int hight, int width, int depth)
	FUN_OVERRIDE(BaseClassExample, GetVolume, int* retVal);
	MEM_FUN_DECL(DerivedClassExample, JustReturn10, int* retVal);
	END_DERIVED_FUNCTIONS(DerivedClassExample);

#ifdef __cplusplus
}
#endif

#endif