#ifndef __BASE_CLASS_EXAMPLE___H__
#define __BASE_CLASS_EXAMPLE___H__

#ifdef __cplusplus
extern "C" {
#endif

#include "coop.h"

	DEF_CLASS(BaseClassExample)
	int hight;
	int width;
	END_DEF(BaseClassExample);

	FUNCTIONS(BaseClassExample, int hight, int width)
	FUNCTION_PTR(BaseClassExample, GetVolume, int* retVal);
	END_FUNCTIONS(BaseClassExample);

	FUNCTION_H(BaseClassExample, GetVolume, int* retVal);

	DEF_INIT_CLASS(BaseClassExample);

#ifdef __cplusplus
}
#endif

#endif