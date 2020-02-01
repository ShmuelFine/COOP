
#include "ProxyExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API DECL_FUN(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy);
	COOP_API DECL_FUN(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy);
	COOP_API DECL_FUN(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);

#ifdef __cplusplus
}
#endif