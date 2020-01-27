#include "CacheTest.h"
#include "ProxyExportDefs.h"
//#include "COOP.h"
//
//#include "Cache.h"
//#include "HeapCache.h"
//#include "InMemoryCache.h"

#define _ASSERT_FALSE(x) if (!(x)) return -1;

//
//int Create_CreateGlobalCache_CreatesCorrectCache()
//{
//		CreateGlobalCache(1000, IN_MEMORY_CACHE_);
//		//_ASSERT_FALSE(((InMemoryCache*)TheGlobalCache)->vTable->_ctor != &__ctor__InMemoryCache);
//		//DestroyGlobalCache();
//
//		//CreateGlobalCache(2000, HEAP_CACHE_);
//		//_ASSERT_FALSE(((HeapCache*)TheGlobalCache)->vTable->_ctor != &__ctor__HeapCache);
//		//DestroyGlobalCache();
//
//		return 1;
//}
//
//int Destroy_DeatroyCache_DeatroysGlobalCache()
//{
//	CreateGlobalCache(1000, "GlobalCache", IN_MEMORY_CACHE_);
//	_ASSERT_FALSE(NULL==((InMemoryCache*)TheGlobalCache)->buffer);
//	DestroyGlobalCache();
//	_ASSERT_FALSE(NULL != ((InMemoryCache*)TheGlobalCache)->buffer);
//
//	return 1;
//}