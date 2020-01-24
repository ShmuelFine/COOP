#include "pch.h"


#ifdef _DEBUG
#pragma comment(lib, "object_d.lib")
#else
#pragma comment(lib, "object.lib")
#endif

//#include "..\object\InMemoryCache.h"
#include "../object/Cache.h"

class CacheTest : public ::testing::Test
{
public:

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}
};

TEST_F(CacheTest, tryExternal)
{
	external_symbol();
	ASSERT_TRUE(true);
}