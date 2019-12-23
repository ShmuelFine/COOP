
#ifndef __OBJECT__H_
#define __OBJECT__H_
#include "ExportDefs.h"
#include <stdlib.h>
#include <stdbool.h>
//generic type of class virtual tables
typedef struct object_t object;
typedef struct virtualTable_t
{
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
	bool _drived_;
	const char* _type_;


}object;



#define DEF_CLASS(name)                                              \
typedef struct name ##VirtualTable_t name ##VirtualTable;            \
struct name ##_t{                                                    \
	object *_next;                                                   \
	name ##VirtualTable* vTable;                                     \
	bool _drived_;                                                   \
	const char* _type_;

#define END_DEF(name)                       \
}name ##_defult = {NULL,NULL,false,#name};  \
typedef struct name ##_t name;

#define DEF_DRIVED_CLASS(name, base)                        \
typedef struct name ##VirtualTable_t name ##VirtualTable;   \
struct name ##_t{                                           \
object *_next;                                              \
name ##VirtualTable* vTable;                                \
bool _drived_;                                              \
const char* _type_;                                         \
base _BASE; 

#define END_DEF_DRIVED(name)               \
}name ##_defult = {NULL,NULL,true,#name};  \
typedef struct name ##_t name;


#define DEF_CTOR(name, ...) void __ctor__ ##name(name * _this, __VA_ARGS__){    \
 _this->_drived_ =false;                                                        \
_this->_type_ = #name;

#define END_CTOR }

#define SUPER (&(_this->_BASE)
#define ME );

#define DEF_DRIVED_CTOR(name, baseName, ...)     \
void __ctor__ ##name(name * _this, __VA_ARGS__)  \
{ _this->_drived_=true;                          \
_this->_type_ = #name;\
__ctor__ ##baseName

#define END_DRIVED_CTOR }

#define DEF_DTOR(name) void __dtor__ ##name(name * _this)
#define END_DTOR

#define DEF_DRIVED_DTOR(name, BaseName) void __dtor__ ##name(name * _this)\
{__dtor__ ##BaseName(&(_this->_BASE));

#define END_DRIVED_DTOR }

#define FUNCTIONS(name, ...)                     \
void __ctor__ ##name(name * _this, __VA_ARGS__); \
void __dtor__ ##name(name * _this);              \
typedef struct name ##VirtualTable_t{            \
void (*_ctor)(name * _this, __VA_ARGS__);        \
void (*_dtor)(name * _this); 


#define END_FUNCTIONS(name) }name ##VirtualTalbe;   \
COOP_API extern name ##VirtualTalbe name ##VTable;

#define DRIVED_FUNCTIONS(name, base, ...)         \
void __ctor__ ##name(name * _this, __VA_ARGS__);  \
void __dtor__ ##name(name * _this);               \
typedef struct name ##VirtualTable_t{             \
base ##VirtualTable _BASE;                        \
void (*_ctor)(name *_this, __VA_ARGS__);          \
void (*_dtor)(name *_this);            

#define DEF_INIT_CLASS(type) COOP_API void type ##_init(); 

#define DEF_INIT_DRIVED_CLASS(type,base) COOP_API void type ##_init();  

#define FUNCTION_PTR(type, functionName, ...) void (* functionName)(type  *  _this, __VA_ARGS__) 
#define FUNCTION_H(type,functionName, ...) void type ##_ ##functionName(type  *  _this, __VA_ARGS__);
#define FUNCTION_IMPL(type, functionName, ...) void type ##_ ##functionName(type  *  _this, __VA_ARGS__)

#define ATTACH_TORs_ToClass(name)       \
name ##VTable._ctor = __ctor__ ##name;  \
name ##VTable._dtor = __dtor__ ##name; 

#define INIT_CLASS(type)                        \
COOP_API type ##VirtualTable type ##VTable;     \
	void type ##_init(){                        \
	ATTACH_TORs_ToClass(type);              

#define END_INIT_CLASS } 

#define INIT_DRIVED_CLASS(type,base)\
COOP_API type ##VirtualTable type ##VTable;     \
	void type ##_init(){                        \
	base ##_init();                             \
	ATTACH_TORs_ToClass(type);  

#define BIND(type,name) type ##VTable.name=type ##_ ##name; 






void _scope_obj_list_add(object* scope_list, object* obj);

void _scope_obj_list_free(object* scope_list);


#define INIT_MAT                 \
MatVTable._ctor = __ctor__Mat;   \
MatVTable._dtor = __dtor__Mat;


//#define NULL ((void*)0)

#define SCOPE_START object _scope_obj_list; \
_scope_obj_list.vTable=NULL;                \
_scope_obj_list._next=NULL;

void FreeMostInnerScope(object* _scope_obj_list);
#define SCOPE_END FreeMostInnerScope(&_scope_obj_list);

#define LOCAL_SCOPE_START object object1; object1.vTable=NULL;object1._next=NULL; \
_scope_obj_list_add(&_scope_obj_list,&object1);


#define LOCAL_SCOPE_END FreeMostInnerScope(&_scope_obj_list);



#define REGISTER_OBJECT(obj)  _scope_obj_list_add(&_scope_obj_list,(object*)obj);

#define REGISTER_CLASS(vTable)  _scope_class_list_add(&_scope_class_list,(vTabsElement*)&vTable);

#define GET_VIRTUAL_TABLE(type) is_in_scope_class_list(type,&_scope_class_list);


#define CREATE_OBJ(Type, instance_name)            \
	Type instance_name;							   \
	instance_name.vTable=&Type ##VTable;           \
	instance_name.vTable->_ctor(&instance_name);   \
	instance_name._next= NULL;					   \
	REGISTER_OBJECT(&instance_name)

#define CREATE_OBJECT(type, instance_name)   \
	type instance_name = type ##_defult;     \
	instance_name.vTable=&type ##VTable; 


#define CREATE_DRIVED_OBJECT(type, base, instance_name)               \
	type instance_name = type ##_defult;                              \
	instance_name.vTable=&type ##VTable;                              \
	if (instance_name._BASE._drived_)                                 \
		CREATE_DRIVED_OBJECT(instance_name._BASE._type_,              \
		instance_name._BASE._BASE._type_, instance_name ##_b)         \                                                                            \
	CREATE_OBJECT(instance_name._BASE._type_,instance_name ##_b)
	


#endif