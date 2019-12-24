#pragma once

#if _COOP_EXPORTING_
#define COOP_API __declspec(dllexport)
#else
#define COOP_API __declspec(dllimport)
#endif

//#define COOP_API __declspec(dllexport)

#define int double
