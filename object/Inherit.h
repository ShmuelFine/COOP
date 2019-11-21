#pragma once

#define Inherit(name, base)                                              \
typedef struct base ##VirtualTable_t base ##VirtualTable;            \
typedef struct base ##_t{                                            \
	object *_next;                                                   \
	name ##VirtualTable* vTable;
#define END_DEF(base)      \
typedef struct name ##VirtualTable_t name ##VirtualTable;            \
typedef struct name ##_t{                                            \
	object *_next;                                                   \
	name ##VirtualTable* vTable;

#define END_DEF(name)      \
}name;