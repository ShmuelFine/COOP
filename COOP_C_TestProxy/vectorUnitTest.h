#ifndef __VECTOR_UNIT_TEST__H__
#define __VECTOR_UNIT_TEST__H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ExportDefs.h"
#include "COOP.h"

	COOP_API FUN_DECL(vector_push_back_SanityTest);
	COOP_API FUN_DECL(vector_at_throws_when_idx_is_outOfRange);

#endif


#ifdef __cplusplus
}
#endif
