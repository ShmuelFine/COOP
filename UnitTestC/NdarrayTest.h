#include "test_defs.h"
#include "../COOP/COOP.h"
#include "../COOP/ExportDefs.h"
#include "../COOP/Ndarray.h"
#include "ScopeTester.h"

FUN_DECL(Ndarray_init_sanity_case);
FUN_DECL(Ndarray_init_invalid_dims);
FUN_DECL(Ndarray_init_invalid_shape);
FUN_DECL(Ndarray_get_location_3D);
FUN_DECL(Ndarray_set_sanity_case);
FUN_DECL(Ndarray_set_index_out_of_range);
FUN_DECL(Ndarray_at_sanity_case);
FUN_DECL(Ndarray_at_index_out_of_range);
FUN_DECL(Ndarray_fill_sanity_case);
FUN_DECL(Ndarray_contains_when_true);
FUN_DECL(Ndarray_contains_when_false);
FUN_DECL(Ndarray_min_sanity_case);
FUN_DECL(Ndarray_max_sanity_case);

