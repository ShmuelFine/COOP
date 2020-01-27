#ifndef __COOP_OBJ_BASE_STRUCTS__
#define __COOP_OBJ_BASE_STRUCTS__

// Generic type of class virtual tables
typedef struct object_t object;
typedef struct virtualTable_t {
	void (*_ctor)(object* _this, ...);
	void (*_dtor)(object* _this, ...);
} virtualTable;

// The classes list struct
typedef struct vTabsElements_t {

	// a pointer to the virtual table of the class
	virtualTable* _vTab;

	struct vTabsElements_t* _next;
}vTabsElement;

// Generic type of class instances
typedef struct object_t {
	struct object_t* _next;
	virtualTable* vTable;
}object;

#endif
