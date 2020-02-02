
#include "ProxyExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API FUN_DECL(New_WhenAllocatingBuffer_ThenReturnsRightAmountOfMemroy);
	COOP_API FUN_DECL(New_WhenAllocatingClassInstance_ThenReturnsRightAmountOfMemroy);
	COOP_API FUN_DECL(Delete_WhenDeletingAllocMemory_ThenItFreesTheResource);

#ifdef __cplusplus
}
#endif