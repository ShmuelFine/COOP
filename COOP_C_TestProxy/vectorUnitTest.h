#ifndef __VECTOR_UNIT_TEST__H__
#define __VECTOR_UNIT_TEST__H__

#include "ExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API FUN_DECL(vector_push_back_SanityTest);
	COOP_API FUN_DECL(vector_at_throws_when_idx_is_outOfRange, int* tester);
	COOP_API FUN_DECL(vector_iterator_SanityTest);
	COOP_API FUN_DECL(vector_begin_SanityTest);
	COOP_API FUN_DECL(vector_end_SanityTest);

#endif


#ifdef __cplusplus
}
#endif
