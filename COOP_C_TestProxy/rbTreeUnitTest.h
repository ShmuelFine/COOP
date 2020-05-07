#ifndef __RBTREE_UNIT_TEST__H__
#define __RBTREE_UNIT_TEST__H__

#include "ExportDefs.h"
#include "COOP.h"

#ifdef __cplusplus
extern "C" {
#endif

	COOP_API FUN_DECL(rbTree_insert_SanityTest);
	COOP_API FUN_DECL(rbTree_find_SanityTest);
	COOP_API FUN_DECL(rbTree_delete_SanityTest);
	COOP_API FUN_DECL(rbTree_iteration_SanityTest);
	COOP_API FUN_DECL(rbTree_rectangle_as_node_data_insert_SanityTest);
	COOP_API FUN_DECL(rbTree_rectangle_as_node_data_find_SanityTest);
	COOP_API FUN_DECL(rbTree_rectangle_as_node_data_delete_SanityTest);

#endif


#ifdef __cplusplus
}
#endif
