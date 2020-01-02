//
#include "Globals.h"
//

COOP_API InMemoryCache TheGlobalCache;
//COOP_API Cache TheGlobalConsistantMatsMetadata;

//void make_global_cache(int size)
//{
//	CREATE_OBJECT(InMemoryCache, TheGlobalCache, size);
//	if (!is_InMemoryCacheVirtualTable__initialized)
//		InMemoryCache_init(); 
//	TheGlobalCache.vTable=&InMemoryCacheVTable;					 
//	TheGlobalCache.vTable->_ctor(&TheGlobalCache, size);
//}