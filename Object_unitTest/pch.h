//
// pch.h
// Header for standard system include files.
//
#pragma once
#ifdef _DEBUG
	#pragma comment(lib, "COOP_d.lib")
#else
	#pragma comment(lib, "COOP.lib")
#endif

#include "gtest/gtest.h"
