#ifndef __BASE_CLASS_EXAMPLE___H__
#define __BASE_CLASS_EXAMPLE___H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../COOP/COOP.h"
//#include "coop.h"

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