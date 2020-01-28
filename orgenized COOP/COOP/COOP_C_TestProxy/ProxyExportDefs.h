#ifndef __EXPORT_DEFS__H___1
#define __EXPORT_DEFS__H___1
#include "Imports.h"
#if _COOP_PROXY_EXPORTING_
	#define COOP_PROXY_API __declspec(dllexport)
#else
	#define COOP_PROXY_API __declspec(dllimport)
#endif

#endif
