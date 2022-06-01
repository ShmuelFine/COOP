#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/redBlackTree.h"
#include "../COOP_C_TestProxy/Rectangle.h"

FUN_DECL(rectComp, void* a, void* b, bool* retBool);
FUN_DECL(intComparison, void* a, void* b, bool* retBool);
FUN_DECL(rbTree_insert_SanityTest);
FUN_DECL(rbTree_find_SanityTest);
FUN_DECL(rbTree_delete_SanityTest);
FUN_DECL(rbTree_iteration_SanityTest);
FUN_DECL(rbTree_rectangle_as_node_data_insert_SanityTest);
FUN_DECL(rbTree_rectangle_as_node_data_find_SanityTest);
FUN_DECL(rbTree_rectangle_as_node_data_delete_SanityTest);

