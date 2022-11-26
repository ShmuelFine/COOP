#ifndef __COOP_OBJ_BASE_STRUCTS__
#define __COOP_OBJ_BASE_STRUCTS__

// Generic type of class virtual tables
typedef struct object_t object;
typedef struct virtualTable_t {
	int (*_ctor)(object* _this, ...);
	int (*_dtor)(object* _this, ...);
} virtualTable;


// Generic type of class instances
typedef struct object_t {
	struct object_t* _next;
	virtualTable* vTable;
}object;

#endif
