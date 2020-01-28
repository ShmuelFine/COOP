#ifndef __EXPORT_DEFS__H_
#define __EXPORT_DEFS__H_

#if _BASE_COOP_EXPORTING_
	#define COOP_API __declspec(dllexport)
#else
	#define COOP_API __declspec(dllimport)
#endif

#endif
