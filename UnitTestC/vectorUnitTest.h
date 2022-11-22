#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/vectorIterator.h"
#include "ScopeTester.h"

// #include "../COOP_C_TestProxy/ScopeTester.h"
#include "../COOP/vector.h"

FUN_DECL(vector_push_back_SanityTest);
FUN_DECL(vector_at_throws_when_idx_is_outOfRange, int* tester);
FUN_DECL(vector_iteration_SanityTest);
