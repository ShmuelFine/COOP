#ifndef __OBJECT__H_
#define __OBJECT__H_

#include "ExportDefs.h"
#include "LinkedLists.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//generic type of class virtual tables
typedef struct object_t object;
typedef struct virtualTable_t {
	void (*_ctor)(object* _this, ...);
	void (*_dtor)(object* _this, ...);
	//const char* type;
}virtualTable;

//the classes list struct
typedef struct vTabsElements_t {

	//a pointer to the virtual table of the class
	virtualTable* _vTab;

	struct vTabsElements_t* _next;
}vTabsElement;

//generic type of class instances
typedef struct object_t {

	struct object_t* _next;
	virtualTable* vTable;



}object;

//typedef struct block_s {
//	char* buff;
//	int size;
//	struct block_s* next;
//}Block;



#define DEF_CLASS(name)                                              \
typedef struct name ##VirtualTable_t name ##VirtualTable;            \
typedef struct name ##_t{                                            \
	object *_next;                                                   \
	name ##VirtualTable* vTable;

#define END_DEF(name)												\
}name;																\
COOP_API extern bool is_ ##name ##VirtualTable__initialized


#define DERIVED_EXTRA_SIZE(name, base) __dummy__[(sizeof(base) - sizeof(object*) - sizeof(base ##VirtualTable*)) > 0 ? (sizeof(base) - sizeof(object*) - sizeof(base ##VirtualTable*)) : 1]

#define DEF_DERIVED_CLASS(name, base)                                          \
typedef struct name ##VirtualTable_t name ##VirtualTable;                     \
typedef struct name ##_t {                                                            \
union {                                                                       \
	base _base;/*Contains _next and vTable of type baseVirtualTable*  */      \
	struct {                                                                  \
		object *_next;                                                        \
		name ##VirtualTable* vTable;                                          \
		char DERIVED_EXTRA_SIZE(name,base);

#define END_DEF_DERIVED(name) };};                                             \
}name;  \
COOP_API extern bool is_ ##name ##VirtualTable__initialized



#define DEF_CTOR(name, ...) void __ctor__ ##name(name * _this, __VA_ARGS__){   
#define END_CTOR }

#define SUPER (&(_this->_base)
#define ME );

#define DEF_DERIVED_CTOR(name, baseName, ...)     \
void __ctor__ ##name(name * _this, __VA_ARGS__)  \
{                                                \
__ctor__ ##baseName

#define END_DERIVED_CTOR }

#define DEF_DTOR(name) void __dtor__ ##name(name * _this) {
#define END_DTOR }

#define DEF_DERIVED_DTOR(name, BaseName) void __dtor__ ##name(name * _this)\
{__dtor__ ##BaseName(&(_this->_base));

#define END_DERIVED_DTOR }

#define FUNCTIONS(name, ...)                     \
void __ctor__ ##name(name * _this, __VA_ARGS__); \
void __dtor__ ##name(name * _this);              \
typedef struct name ##VirtualTable_t{            \
void (*_ctor)(name * _this, __VA_ARGS__);        \
void (*_dtor)(name * _this); 


#define END_FUNCTIONS(name) }name ##VirtualTalbe;   \
COOP_API extern name ##VirtualTalbe name ##VTable

#define DERIVED_FUNCTIONS(name, base, ...)         \
void __ctor__ ##name(name * _this, __VA_ARGS__);  \
void __dtor__ ##name(name * _this);               \
typedef struct name ##VirtualTable_t{             \
base ##VirtualTable _base;                        \
void (*_ctor)(name *_this, __VA_ARGS__);          \
void (*_dtor)(name *_this);            

#define DEF_INIT_CLASS(type)    COOP_API void type ##_init()

#define DEF_INIT_DERIVED_CLASS(type,base) COOP_API void type ##_init()  

#define FUNCTION_PTR(type, functionName, ...) int (* functionName)(type  *  _this, __VA_ARGS__)

#define BASE_FUNCTION_PTR(type, functionName, ...) FUNCTION_TYPE(type,functionName,__VA_ARGS__) 
//struct functionName ##_t_ *functionName;

#ifdef __cplusplus
#define OVERIDE_FUNCTION_PTR(functionName, ...) struct functionName ##_t_ __ ##functionName; \
struct ::functionName ##_t_  *functionName
#else
#define OVERIDE_FUNCTION_PTR(functionName, ...) struct functionName ##_t_ __ ##functionName; \
struct functionName ##_t_  *functionName
#endif

#define FUNCTION_H(type,functionName, ...) int type ##_ ##functionName(type  *  _this, __VA_ARGS__)
#define BASE_FUNCTION_H(type,functionName, ...) FUNCTION_H(type,functionName, __VA_ARGS__) //\
//typedef struct functionName ##_t_ functionName ##_t;

//#define OVERIDE_FUNCTION_H(type,functionName, ...) FUNCTION_H(type,functionName, __VA_ARGS__)

#define FUNCTION_IMPL(type, functionName, ...) int type ##_ ##functionName(type * _this, __VA_ARGS__){
#define FUNCTION_IMPL(type, functionName, ...) int type ##_ ##functionName(type * _this, __VA_ARGS__){
//TODO: somehow extract the args without the types
//#define BASE_FUNCTION_IMPL(type, functionName, ...)						\
//void type ##_ ##functionName(type  *  _this, __VA_ARGS__){				\
//if(_this->vTable->functionName.next != NULL){							\
//	_this->vTable->functionName.next->func(_this, __VA_ARGS__);			\
//	return;																\
//}
//#define OVERIDE_FUNCTION_IMPL(type, functionName, ...) BASE_FUNCTION_IMPL(type,functionName, __VA_ARGS__)

#define END_FUNCTION_IMPL return 0;}

#define ATTACH_TORs_ToClass(name)       \
name ##VTable._ctor = __ctor__ ##name;  \
name ##VTable._dtor = __dtor__ ##name; 

#define INIT_CLASS(type)                        \
bool is_ ##type ##VirtualTable__initialized = false;\
 type ##VirtualTable type ##VTable; \
	void type ##_init(){                        \
	ATTACH_TORs_ToClass(type)

#define END_INIT_CLASS } 

#define INIT_DERIVED_CLASS(type,base)				   \
bool is_ ##type ##VirtualTable__initialized = false;   \
/*COOP_API*/ type ##VirtualTable type ##VTable;			   \
	void type ##_init(){							   \
	if(!(is_ ##base ##VirtualTable__initialized))      \
	base ##_init();									   \
	ATTACH_TORs_ToClass(type);						   \
	type ##VTable._base = base ##VTable;

//#define BIND(type,name) type ##VTable.name=type ##_ ##name; 
#define BIND(type,name) type ##VTable.name=type ##_ ##name

#define BASE_BIND(type,name)\
type ##VTable.__ ##name.func=type ##_ ##name;  \
type ##VTable.name = &type ##VTable.__ ##name; \
type ##VTable.name->next = NULL

#define BIND_OVERIDE(type,base,name) type ##VTable.__##name.func=&(type ##_ ##name);            \
type ##VTable.__ ##name.next = NULL;                                                            \
base ##VTable.__ ##name.next = &(type ##VTable.__ ##name);											\
type ##VTable.name = base ##VTable.name



void _scope_obj_list_add(object* scope_list, object* obj);

void _scope_obj_list_free(object* scope_list);


#define INIT_MAT                 \
MatVTable._ctor = __ctor__Mat;   \
MatVTable._dtor = __dtor__Mat;

extern bool isCurrentlyThrowing;

//#define CATCH_LABEL__4
//#define CATCH_LABEL__3
//#define CATCH_LABEL__2
//#define CATCH_LABEL__1
//#define CATCH_LABEL
//
//#define DEF_NEW_CATCH_LABEL\
//#undef CATCH_LABEL__4\
//#define CATCH_LABEL__4  CATCH_LABEL__3\
//#undef CATCH_LABEL__3\
//#define CATCH_LABEL__3  CATCH_LABEL__2\
//#undef CATCH_LABEL__2\
//#define CATCH_LABEL__2  CATCH_LABEL__1\
//#undef CATCH_LABEL__1\
//#define CATCH_LABEL__1  CATCH_LABEL\
//#undef CATCH_LABEL\
//#define CATCH_LABEL next_catch_ ##__LINE__
//
//#define POP_LAST_CATCH_LABEL\
//#undef CATCH_LABEL\
//#define CATCH_LABEL CATCH_LABEL__1\
//#undef CATCH_LABEL__1\
//#define CATCH_LABEL__1 CATCH_LABEL__2\
//#undef CATCH_LABEL__2\
//#define CATCH_LABEL__2 CATCH_LABEL__3\
//#undef CATCH_LABEL__3\
//#define CATCH_LABEL__3 CATCH_LABEL__4\
//#undef CATCH_LABEL__4\
//#define CATCH_LABEL__4 

#include <setjmp.h>

extern jmp_buf SCOPE_FALLBACK_ADDR[10];
extern int _CurrScope_Idx;


#define ERROR_VALUE -1

#define TRY if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx]))

#define CATCH \
else { 

#define END_TRY }



#define THROW \
longjmp(SCOPE_FALLBACK_ADDR[--_CurrScope_Idx],1)



#define SCOPE_START \
object _scope_obj_list; \
int __RET_VAL__ = 0;\
if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx++]))\
{  \
	_scope_obj_list.vTable=NULL; \
	_scope_obj_list._next=NULL		


void FreeMostInnerScope(object* _scope_obj_list);

#define SCOPE_END\
	FreeMostInnerScope(&_scope_obj_list); \
	_CurrScope_Idx--;\
	return __RET_VAL__;\
}else{\
	FreeMostInnerScope(&_scope_obj_list);\
	if(_CurrScope_Idx > 0)\
		_CurrScope_Idx--;\
	__RET_VAL__ = -1;\
	return __RET_VAL__;\
}

//_CurrScope_Idx--; \


#define LOCAL_SCOPE_START \
if(!setjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx++]))\
{\
	object LocalScopeStartMarker; \
	LocalScopeStartMarker.vTable=NULL;\
	LocalScopeStartMarker._next=NULL; \
	_scope_obj_list_add(&_scope_obj_list,&LocalScopeStartMarker)


#define LOCAL_SCOPE_END \
	FreeMostInnerScope(&_scope_obj_list);\
	_CurrScope_Idx--; \
}\
else {\
	FreeMostInnerScope(&_scope_obj_list);\
	_CurrScope_Idx--; \
	longjmp(SCOPE_FALLBACK_ADDR[_CurrScope_Idx], 1);\
}

#define REGISTER_CLASS(vTable)  _scope_class_list_add(&_scope_class_list,(vTabsElement*)&vTable);

#define REGISTER_OBJECT(obj) _scope_obj_list_add(&_scope_obj_list, (object*)obj)

#define GET_VIRTUAL_TABLE(type) is_in_scope_class_list(type,&_scope_class_list);

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



#define CREATE_OBJECT(type, instance_name, ...)						\
	if (! is_ ##type ##VirtualTable__initialized) type ##_init();   \
	type instance_name;                  							\
	instance_name.vTable=&type ##VTable;							\
	instance_name.vTable->_ctor(&instance_name, __VA_ARGS__);		\
	instance_name._next= NULL;										\
	REGISTER_OBJECT(&instance_name)

#define CREATE_DERIVED_OBJECT(type, base, instance_name, ...)	 \
	CREATE_OBJECT(type,instance_name, __VA_ARGS__)


#endif