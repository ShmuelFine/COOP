#ifndef __COOP__LIST__H_
#define __COOP__LIST__H_

#include "COOP.h"
#include "Iterator.h"
#include "SharedObjPtr.h"
#include <stddef.h> 
#include <stdbool.h> 
#include <stdint.h> 

DEF_CLASS(ListNode);
	ListNode* prev;
	ListNode* next;
	unsigned char payload[];
END_DEF(ListNode);


DEF_CLASS(GenericList);
	MEM_SIZE_T size;           // מספר איברים
	MEM_SIZE_T elementSize;    // גודל איבר בבתים
	ListNode* head;           // צומת ראשון
	ListNode* tail;           // צומת אחרון
END_DEF(GenericList)


DEF_DERIVED_CLASS(ListIterator, Iterator)
	GenericList* owner;
	ListNode* node;
END_DEF_DERIVED(ListIterator)


FUNCTIONS(GenericList, MEM_SIZE_T dataTypeSize);

/* List Functions*/
	MEM_FUN_DECL(GenericList, push_back_generic, const void* src, MEM_SIZE_T size);
	MEM_FUN_DECL(GenericList, push_front_generic, const void* src, MEM_SIZE_T size);
	MEM_FUN_DECL(GenericList, pop_back_generic, void* dst, MEM_SIZE_T size);   // אם dst==NULL => size חייב להיות 0
	MEM_FUN_DECL(GenericList, pop_front_generic, void* dst, MEM_SIZE_T size);
	MEM_FUN_DECL(GenericList, front_generic, void* dst, MEM_SIZE_T size);
	MEM_FUN_DECL(GenericList, back_generic, void* dst, MEM_SIZE_T size);
	MEM_FUN_DECL(GenericList, size, MEM_SIZE_T* out_size);
	MEM_FUN_DECL(GenericList, empty, bool* out_is_empty);
	MEM_FUN_DECL(GenericList, clear);
	
	
	/* Iterators */
	MEM_FUN_DECL(GenericList, begin, ListIterator** out_it); // begin()
	MEM_FUN_DECL(GenericList, end, ListIterator** out_it); // end()  (node==NULL)
	
	/* Insert / Erase by Iterator*/
	MEM_FUN_DECL(GenericList, insert_before_generic, ListIterator* pos, const void* src, MEM_SIZE_T size, ListIterator** out_it);
	MEM_FUN_DECL(GenericList, insert_after_generic, ListIterator* pos, const void* src, MEM_SIZE_T size, ListIterator** out_it);
	MEM_FUN_DECL(GenericList, erase_generic, ListIterator* pos, void* dst, MEM_SIZE_T size, ListIterator** out_next);
END_FUNCTIONS(GenericList);


DERIVED_FUNCTIONS(ListIterator, Iterator);
    MEM_FUN_DECL(ListIterator, equals,   object* other, bool* out_equal);
    MEM_FUN_DECL(ListIterator, next);
    MEM_FUN_DECL(ListIterator, prev);
    MEM_FUN_DECL(ListIterator, get_ref,  void** out_ptr);
    MEM_FUN_DECL(ListIterator, get_cref, const void** out_ptr);
    MEM_FUN_DECL(ListIterator, distance, object* other, ptrdiff_t* out_dist);
    MEM_FUN_DECL(ListIterator, advance,  ptrdiff_t n);
	END_DERIVED_FUNCTIONS(ListIterator)

#define DECLARE_SPECIFIC_LIST_TYPE(type_name, c_type)                        
DEF_DERIVED_CLASS(List_##type_name, GenericList);
END_DEF_DERIVED(List_##type_name);

DERIVED_FUNCTIONS(List_##type_name, GenericList);
	MEM_FUN_DECL(List_##type_name, push_back, c_type val);
	MEM_FUN_DECL(List_##type_name, push_front, c_type val);
	MEM_FUN_DECL(List_##type_name, pop_back, c_type* out);
	MEM_FUN_DECL(List_##type_name, pop_front, c_type* out);
	MEM_FUN_DECL(List_##type_name, front, c_type* out);
	MEM_FUN_DECL(List_##type_name, back, c_type* out);
	MEM_FUN_DECL(List_##type_name, insert_before, ListIterator* pos, c_type val, ListIterator** out_it);
	MEM_FUN_DECL(List_##type_name, insert_after, ListIterator* pos, c_type val, ListIterator** out_it);
	MEM_FUN_DECL(List_##type_name, erase, ListIterator* pos, c_type* out, ListIterator** out_next);
END_DERIVED_FUNCTIONS(List_##type_name);

DECLARE_SPECIFIC_LIST_TYPE(int, int);
//char, float, ptr..

#endif
