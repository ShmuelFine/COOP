#ifndef __COOP_TESTS__BASE_CLASS_EXAMPLE___H__
#define __COOP_TESTS__BASE_CLASS_EXAMPLE___H__

#ifdef __cplusplus
extern "C" {
#endif

//#include "coop.h"
#include "../COOP/COOP.h"

	DEF_CLASS(BaseClassExample);
	int hight;
	int width;
	END_DEF(BaseClassExample);

	FUNCTIONS(BaseClassExample, int hight, int width);
	MEM_FUN_DECL(BaseClassExample, GetVolume, int* retVal);
	END_FUNCTIONS(BaseClassExample);


#ifdef __cplusplus
}
#endif

#endif