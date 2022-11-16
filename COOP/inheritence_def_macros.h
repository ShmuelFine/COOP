#ifndef __COOP_INHERITENCE_DEF_MACROS__
#define __COOP_INHERITENCE_DEF_MACROS__

/////////////////// Inheritence and Virtual Functions: ///////////////
///////////////////////  The H File Structure ////////////////////////

// Macros for inner use:
#define DELTA_SIZE(classA, classB) (sizeof(classA) - sizeof(classB))
#define PLACE_HOLDER_MEM_BUFF(base) char __dummy__[DELTA_SIZE(base, object) > 0 ? DELTA_SIZE(base, object) : 1]

// Macro that defines a DERRIVED class:
#define DEF_DERIVED_CLASS(class_name, base)											\
typedef struct class_name ##VirtualTable_t class_name ##VirtualTable;						\
typedef struct class_name ##_t {														\
union {																			\
	base _base;																	\
	struct {																	\
		/*this first part is parallel to _base structure and of the same size:*/\
		object *_next;															\
		class_name ##VirtualTable* vTable;											\
		PLACE_HOLDER_MEM_BUFF(base)

// Define the derived class members here, in between.

// Finish definition of derived class members section:
#define END_DEF_DERIVED(class_name) };};								\
}class_name;															\
COOP_API extern bool is_ ##class_name ##VirtualTable__initialized


// Macro that begins derived class func definition section
#define DERIVED_FUNCTIONS(class_name, base, ...)			\
	int __ctor__ ##class_name(class_name * _this, __VA_ARGS__);	\
	int __dtor__ ##class_name(class_name * _this);					\
	typedef struct class_name ##VirtualTable_t{				\
	base ##VirtualTable _base;								\
	int (*_ctor)(class_name * _this, __VA_ARGS__);			\
	int (*_dtor)(class_name * _this); 

// Macro that defines an overriding version of a virtual function:
#define FUN_OVERRIDE(base_class, function_name, ...)	\
struct base_class ##_ ##function_name ##_t_ function_name

// The derived class' function section ends with:
#define END_DERIVED_FUNCTIONS(type) END_FUNCTIONS(type)

///////////////////////  The C File Structure ////////////////////////

// (See next macro comment)
#define SUPER (&(_this->_base)
#define ME );

// Macro that begins a derived ctor definition.
// Should be used followed by SUPER <base class ctor args> ME
// In order to call the base class ctor. e.g.:
// DEF_DERIVED_CTOR(Derived_Class, Base_Class, width, height, type, step) SUPER, width, height, type ME

//#define DEF_DERIVED_CTOR(class_name, baseName, ...)    \
//int __ctor__ ##class_name(class_name * _this, __VA_ARGS__)  \
//{                                                \
//	__ctor__ ##baseName
//
//// Implement derived ctor here in between
//#define END_DERIVED_CTOR }

#define DEF_DERIVED_CTOR(class_name, baseName, ...)  FUN_IMPL(__ctor__ ##class_name, class_name * _this, __VA_ARGS__)  \
{ \
	__ctor__ ##baseName
#define  END_DERIVED_CTOR }END_FUN

// Macro that begins a derived dtor definiton. It calls the base class dtor, 
// and you should fill in the extra work needed.

//#define DEF_DERIVED_DTOR(class_name, BaseName) int __dtor__ ##class_name(class_name * _this)	\
//{																			\
//	__dtor__ ##BaseName(&(_this->_base));
//
//// Implement derived dtor here in between
//#define END_DERIVED_DTOR }

#define DEF_DERIVED_DTOR(class_name, BaseName) FUN_IMPL(__dtor__ ##class_name, class_name * _this) \
{  \
	__dtor__ ##BaseName(&(_this->_base));

#define END_DERIVED_DTOR }END_FUN

/*int (*type ##_ ##function_name ##_outer_function(type * _this))(void* _this, __VA_ARGS__)\
{																		\
	struct base ##_ ##function_name ##_t_ * toRun = &_this->vTable->function_name;			\
	while(toRun->next) { toRun = toRun->next; }							\
	return toRun->inner_function;										\
}																		\
																		\
*/
#define FUN_OVERRIDE_IMPL(type, base, function_name, ...)\
FUN_IMPL(inner_function_ ##type ##_ ##function_name, type * _this, __VA_ARGS__)



// And finally we init the vTable.
// NOTE: The binding of virtual and overriding funcions differs from the usual one:
// It begins with:
#define INIT_DERIVED_CLASS(type,base)					\
bool is_ ##type ##VirtualTable__initialized = false;	\
type ##VirtualTable type ##VTable;						\
void type ##_init()		{							\
	if(!(is_ ##base ##VirtualTable__initialized))		\
		base ##_init();									\
	ATTACH_TORs_ToClass(type);							\
	type ##VTable._base = base ##VTable

// Than, for each implemented, overriding func. : 
#define BIND_OVERIDE(type,base,function) \
	type ##VTable.function.next = NULL;\
	type ##VTable.function.inner_function = inner_function_ ##type ##_ ##function;\
	type ##VTable._base.function.next = &(type ##VTable.##function);\
	type ##VTable.function.outer_function = type ##VTable._base.function.outer_function;

//type ##VTable.__ ##function.outer_function = NULL; /*outer function called only on the base. otherwise let's crash.*/\
//
// Or, if it's not overriding anything it's just BIND or BIND...
// And finally, we put END_INIT_CLASS as ususal.

#endif
