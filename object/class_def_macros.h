#ifndef __COOP_CLASS_DEF_MACROS__
#define __COOP_CLASS_DEF_MACROS__
///////////// The H file structure : //////////////////////////////////
// Macro that begins class name + member definitions:
#define DEF_CLASS(name)                                              \
typedef struct name ##VirtualTable_t name ##VirtualTable;            \
typedef struct name ##_t{                                            \
	object *_next;                                                   \
	name ##VirtualTable* vTable;

// Define your members here, between DEF_CLASS and END_DEF

// Macro that ends class member definitions:
#define END_DEF(name)												\
}name;																\
COOP_API extern bool is_ ##name ##VirtualTable__initialized

// Macro that begins class function definitions section:
#define FUNCTIONS(name, ...)                     \
void __ctor__ ##name(name * _this, __VA_ARGS__); \
void __dtor__ ##name(name * _this);              \
typedef struct name ##VirtualTable_t{            \
void (*_ctor)(name * _this, __VA_ARGS__);        \
void (*_dtor)(name * _this); 

// Define your members here, between FUNCTIONS and END_FUNCTIONS,
// using FUNCTION_PTR for non virtual functions, 
// or BASE_FUNCTION_PTR and DERIVED_FUNCTION_PTR for virtual functions.
#define FUNCTION_PTR(type, functionName, ...) int (* functionName)(type  *  _this, __VA_ARGS__)

// Macro that defines a virtual function:
#define BASE_FUNCTION_PTR(type, functionName, ...) FUNCTION_TYPE(type,functionName,__VA_ARGS__) 

// Macro that ends class function definitions section:
#define END_FUNCTIONS(name) }name ##VirtualTalbe;   \
COOP_API extern name ##VirtualTalbe name ##VTable

// The final line in each class definition, 
#define DEF_INIT_CLASS(type)    COOP_API void type ##_init()

///////////// The C file structure : //////////////////////////////////

// Macros that defines a constructor:
#define DEF_CTOR(name, ...) void __ctor__ ##name(name * _this, __VA_ARGS__){   
#define END_CTOR }

// Macros that defines a destructor:
#define DEF_DTOR(name) void __dtor__ ##name(name * _this) {
#define END_DTOR }

// Macros that define implementation of previously declared function:
#define FUNCTION_IMPL(type, functionName, ...) int type ##_ ##functionName(type * _this, __VA_ARGS__){
#define END_FUNCTION_IMPL return 0;}

// Macro for inner use in INIT_CLASS:
#define ATTACH_TORs_ToClass(name)       \
name ##VTable._ctor = __ctor__ ##name;  \
name ##VTable._dtor = __dtor__ ##name; 

// At the end of the C file, we bind the function implementations to the pointers of the vTable.
// It begins with:
#define INIT_CLASS(type)                        \
bool is_ ##type ##VirtualTable__initialized = false;\
 type ##VirtualTable type ##VTable; \
	void type ##_init(){                        \
	ATTACH_TORs_ToClass(type)

// And then all bindings take place:
#define BIND(type,name) type ##VTable.name=type ##_ ##name

#define BASE_BIND(type,name)\
type ##VTable.__ ##name.func=type ##_ ##name;  \
type ##VTable.name = &type ##VTable.__ ##name; \
type ##VTable.name->next = NULL

// Finally, it ends with:
#define END_INIT_CLASS } 

//////////////////////////////////////////////////////////////////////


#endif
