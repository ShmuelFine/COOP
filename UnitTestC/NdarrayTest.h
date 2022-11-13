#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/vectorIterator.h"
#include "ScopeTester.h"
#include "../COOP/Ndarray.h"

FUN_DECL(Ndarray_init_sanity_case);
FUN_DECL(Ndarray_init_invalid_shape);//not throw
FUN_DECL(Ndarray_init_invalid_dims);//not throw
FUN_DECL(Ndarray_set_sanity_case);

