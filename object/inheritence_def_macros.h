#ifndef __COOP_INHERITENCE_DEF_MACROS__
#define __COOP_INHERITENCE_DEF_MACROS__

/////////////////// Inheritence and Virtual Functions: ///////////////
///////////////////////  The H File Structure ////////////////////////

// Macros for inner use:
#define DELTA_SIZE(classA, classB) (sizeof(classA) - sizeof(classB))
#define PLACE_HOLDER_MEM_BUFF(base) char __dummy__[DELTA_SIZE(base, object) > 0 ? DELTA_SIZE(base, object) : 1]

// Macro that defines a DERRIVED class:
#define DEF_DERIVED_CLASS(name, base)											\
typedef struct name ##VirtualTable_t name ##VirtualTable;						\
typedef struct name ##_t {														\
union {																			\
	base _base;																	\
	struct {																	\
		/*this first part is parallel to _base structure and of the same size:*/\
		object *_next;															\
		name ##VirtualTable* vTable;											\
		PLACE_HOLDER_MEM_BUFF(base)

// Define the derived class members here, in between.

// Finish definition of derived class members section:
#define END_DEF_DERIVED(name) };};                                             \
}name;  \
COOP_API extern bool is_ ##name ##VirtualTable__initialized


// Macro that begins derived class func definition section
#define DERIVED_FUNCTIONS(name, base, ...)			\
void __ctor__ ##name(name * _this, __VA_ARGS__);	\
void __dtor__ ##name(name * _this);					\
typedef struct name ##VirtualTable_t{				\
base ##VirtualTable _base;							\
void (*_ctor)(name *_this, __VA_ARGS__);			\
void (*_dtor)(name *_this)            

// Macro that defines an overriding version of a virtual function:
#define OVERIDE_FUNCTION_PTR(functionName, ...)	\
struct functionName ##_t_ __ ##functionName;	\
struct functionName ##_t_  *functionName

// The derived class' function section ends with:
#define END_DERIVED_FUNCTIONS(type) END_FUNCTIONS(type)

// The final line in an H file of Derived class definitions
#define DEF_INIT_DERIVED_CLASS(type) DEF_INIT_CLASS(type)

///////////////////////  The C File Structure ////////////////////////
#define SUPER (&(_this->_base)
#define ME );

// Macro that begins a derived ctor definition.
// Should be used followed bu SUPER <base class ctor args> ME
// In order to call the base class ctor.
#define DEF_DERIVED_CTOR(name, baseName, ...)    \
void __ctor__ ##name(name * _this, __VA_ARGS__)  \
{                                                \
	__ctor__ ##baseName

// Implement derived ctor here in between
#define END_DERIVED_CTOR }

// Macro that begins a derived dtor definiton. It calls the base class dtor, 
// and you should fill in the extra work needed.
#define DEF_DERIVED_DTOR(name, BaseName) void __dtor__ ##name(name * _this)	\
{																			\
	__dtor__ ##BaseName(&(_this->_base));

// Implement derived dtor here in between
#define END_DERIVED_DTOR }


// The binding of virtual and overriding funcions differs from the usual one:
#define INIT_DERIVED_CLASS(type,base)					\
bool is_ ##type ##VirtualTable__initialized = false;	\
/*COOP_API*/ type ##VirtualTable type ##VTable;			\
	void type ##_init(){								\
	if(!(is_ ##base ##VirtualTable__initialized))		\
	base ##_init();										\
	ATTACH_TORs_ToClass(type);							\
	type ##VTable._base = base ##VTable

#define BIND_OVERIDE(type,base,name) type ##VTable.__##name.func=&(type ##_ ##name);            \
type ##VTable.__ ##name.next = NULL;                                                            \
base ##VTable.__ ##name.next = &(type ##VTable.__ ##name);										\
type ##VTable.name = base ##VTable.name


#endif
