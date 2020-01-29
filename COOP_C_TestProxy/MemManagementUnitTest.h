
#include "ProxyExportDefs.h"


#ifdef __cplusplus
extern "C" {
#endif

	COOP_API int New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy();

	COOP_API int New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy();

	COOP_API int Delete_WhenDeletingAllocMemory_ThenItFreesTheResource();

#ifdef __cplusplus
}
#endif