// =============================
// File: Queue_ooc.h
// =============================
#ifndef QUEUE_OOC_H
#define QUEUE_OOC_H 1

#include "ooc.h"
#include "list.h"
#include <stddef.h>
#include <stdbool.h>

/*
 * OOC-Style Queue + COOP-like C API wrappers
 * -------------------------------------------
 * - Queue base class (inherits Base), implemented on top of OOC List.
 * - QueueInt typed class for convenient int usage.
 * - C wrappers with names your COOP tests expect: Queue_int_*.
 */

 /* ================== OOC classes ================== */
DeclareClass(Queue, Base);

Virtuals(Queue, Base)
int          (*enqueue_copy)(Queue, const void* src, size_t size);
int          (*dequeue_copy)(Queue, void* dst, size_t size);
void* (*front)(Queue);
const void* (*front_cref)(Queue);
ListIterator(*begin)(Queue);
ListIterator(*end)(Queue);
void         (*clear)(Queue);
int          (*empty)(Queue);
size_t(*size)(Queue);
EndOfVirtuals;

/* typed int queue */
DeclareClass(QueueInt, Queue);
Virtuals(QueueInt, Queue)
int         (*enqueue)(QueueInt, int value);
int         (*dequeue)(QueueInt, int* out);
int* (*front_t)(QueueInt);
const int* (*front_cref_t)(QueueInt);
EndOfVirtuals;

/* params for Queue constructor */
typedef void (*queue_item_destroyer)(void*);
typedef struct QueueParams_ {
    queue_item_destroyer destroyer; /* ooc_free for POD; (ooc_destroyer)ooc_delete for objects */
} QueueParams;

/* helper to access underlying list from wrappers */
List queue_get_list(Queue self);

/* ================== C wrappers (COOP-like) ================== */
typedef struct Queue_int { QueueInt impl; } Queue_int;

void   Queue_int_ctor(Queue_int* q);
void   Queue_int_dtor(Queue_int* q);
int    Queue_int_enqueue(Queue_int* q, int value);
int    Queue_int_dequeue(Queue_int* q, int* out);
void   Queue_int_front(Queue_int* q, int** out_ptr);
void   Queue_int_front_cref(const Queue_int* q, const int** out_ptr);
size_t Queue_int_size(const Queue_int* q);
bool   Queue_int_empty(const Queue_int* q);
void   Queue_int_clear(Queue_int* q);
void   Queue_int_print(const Queue_int* q);

#endif /* QUEUE_OOC_H */
