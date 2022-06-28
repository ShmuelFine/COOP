#include "../COOP/COOP.h"
#include "../COOP/hashTable.h"
#include "test_defs.h"

FUN_DECL(check_insert_SanityTest);
FUN_DECL(check_insert_when_already_exists_then_throw);
FUN_DECL(check_search_when_founded_then_return_instance);
FUN_DECL(check_search_when_not_founded_then_return_null);
FUN_DECL(check_delete_when_exist_then_delete);