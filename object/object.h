#ifndef __OBJECT__H_
#define __OBJECT__H_

#include "ExportDefs.h"
#include "LinkedLists.h"
#include "obj_base_structs.h"
#include "class_def_macros.h"
#include "inheritence_def_macros.h"
#include "obj_lifecycle_management.h"

//#include <stdlib.h>
//#include <stdbool.h>
//#include <stdio.h>




#ifdef __cplusplus
	#define NEW(obj,typeToAlloc,size)\
	{ \
		void * returned; \
		CALL(AddNewBlock,(sizeof(typeToAlloc)*size),&returned);\
		obj = (typeToAlloc*)returned;\
	}
#else
	#define NEW(obj,typeToAlloc,size)\
	{\
		void * returned; \
		CALL(AddNewBlock,*TheGlobalCache,(sizeof(typeToAlloc)*size),&returned);\
		obj = (typeToAlloc*)returned;\
	}
#endif

#ifdef __cplusplus
	#define DELETE_OBJ(buff) CALL(RemoveBlock,buff); buff = NULL
#else
	#define DELETE_OBJ(buff) CALL(RemoveBlock,*TheGlobalCache,buff); buff = NULL
#endif






#endif