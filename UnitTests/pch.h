//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

#ifdef _DEBUG
#pragma comment(lib, "COOP_C_TestProxy_d.lib")
#else
#pragma comment(lib, "COOP_C_TestProxy.lib")
#endif
