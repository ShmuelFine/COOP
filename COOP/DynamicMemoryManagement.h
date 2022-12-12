#ifndef __DYNAMIC_MEMORY_MANAGEMENT__H_
#define __DYNAMIC_MEMORY_MANAGEMENT__H_

#include "ExportDefs.h"
#include "coop.h"
#include "ICache.h"
#include <stdbool.h>


COOP_API extern ICache* TheGlobalCache;

typedef enum CACHE_TYPES_t { STACK_BASED_MEMORY, HEAP_BASED_MEMORY, NUM_MEMORY_TYPES } CACHE_TYPES;


FUN_DECL(init_global_memory, int size, CACHE_TYPES type);


#define ALLOC(dest, whatToPutThere)\
	MFUN(TheGlobalCache, AddNewBlock),(MEM_SIZE_T)(sizeof(whatToPutThere)),(void*)&(dest) CALL;\
	THROW_MSG_UNLESS(dest, "could not allocate sizeof " #whatToPutThere " for " #dest); \

#define ALLOC_ARRAY(dest, type, howMuchToPutThere)\
	MFUN(TheGlobalCache, AddNewBlock),(MEM_SIZE_T)(sizeof(type)*(howMuchToPutThere)),(void*)&(dest) CALL;\
	THROW_MSG_UNLESS(dest, "could not allocate " #howMuchToPutThere " x sizeof " #type " for " #dest); \

#define ALLOC_INIT_INSTANCE_PTR(type, instance_name)\
	ALLOC(instance_name, type)\
	ASSERT_NOT_NULL(instance_name);	\
	INITIALIZE_INSTANCE(type, (*instance_name))

FUN_DECL(get_total_free_bytes, MEM_SIZE_T* out_count);


#define CREATE_PTR(type, instance_name)			\
	type * instance_name = NULL;                \
	ALLOC_INIT_INSTANCE_PTR(type, instance_name)

#define FREE(buff) MFUN(TheGlobalCache, RemoveBlock), buff CALL;
#define DELETE(instance_ptr) {DESTROY(instance_ptr); FREE(instance_ptr); instance_ptr = NULL;}


#endif


