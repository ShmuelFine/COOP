// =============================
// File: Queue_ooc.c
// =============================
#include "Queue_ooc.h"
/* ---------------- Queue (base) ---------------- */
ClassMembers(Queue, Base)
List   list;
size_t count;
queue_item_destroyer dtor;
EndOfClassMembers;

static void Queue_initialize(Class this);
static void Queue_finalize(Class this);
static void Queue_constructor(Queue self, const void* params);
static void Queue_destructor(Queue self, QueueVtable vtab);
static int  Queue_copy(Queue self, const Queue from);
static void QueueInt_finalize(Class this) { (void)this; }

static int           Queue_enqueue_copy(Queue self, const void* src, size_t size);
static int           Queue_dequeue_copy(Queue self, void* dst, size_t size);
static void* Queue_front(Queue self);
static const void* Queue_front_cref(Queue self);
static ListIterator  Queue_begin(Queue self);
static ListIterator  Queue_end(Queue self);
static void          Queue_clear(Queue self);
static int           Queue_empty(Queue self);
static size_t        Queue_size(Queue self);

AllocateClass(Queue, Base);

static struct QueueVtable_stru QueueVtableInstance = {
    .Base = {._class = &QueueClass },
    .enqueue_copy = Queue_enqueue_copy,
    .dequeue_copy = Queue_dequeue_copy,
    .front = Queue_front,
    .front_cref = Queue_front_cref,
    .begin = Queue_begin,
    .end = Queue_end,
    .clear = Queue_clear,
    .empty = Queue_empty,
    .size = Queue_size,
};

static void Queue_initialize(Class this) { (void)this; }
static void Queue_finalize(Class this) { (void)this; }

static void Queue_constructor(Queue self, const void* params)
{
    chain_constructor(Queue, self, NULL);
    const QueueParams* p = (const QueueParams*)params;
    self->dtor = p ? p->destroyer : ooc_free;    /* default: manage bytes */
    self->list = list_new(self->dtor);         /* own & free items on delete */
    self->count = 0;
}

static void Queue_destructor(Queue self, QueueVtable vtab)
{
    (void)vtab;
    if (self->list) {
        Queue_clear(self);
        ooc_delete((Object)self->list);
        self->list = NULL;
    }
}

/* NOTE: 'copy' copies pointers (not deep copying item bytes), like a shallow copy of a list of nodes. */
static int Queue_copy(Queue self, const Queue from)
{
    self->dtor = from->dtor;
    self->list = list_new(self->dtor);
    self->count = 0;
    for (ListIterator it = list_first(from->list); it; it = list_next(from->list, it)) {
        list_append(self->list, list_get_item(it));
        self->count++;
    }
    return OOC_COPY_DONE;
}

static int Queue_enqueue_copy(Queue self, const void* src, size_t size)
{
    assert(src && size > 0);
    void* mem = ooc_memdup(src, size);
    list_append(self->list, mem);
    self->count++;
    return TRUE;
}

static int Queue_dequeue_copy(Queue self, void* dst, size_t size)
{
    (void)size; /* caller guarantees size correctness */
    ListIterator it = list_first(self->list);
    if (!it) return FALSE;
    void* item = list_get_item(it);
    if (dst) memcpy(dst, item, size);
    list_delete_item(self->list, it); /* invokes destroyer */
    self->count--;
    return TRUE;
}

static void* Queue_front(Queue self)
{
    ListIterator it = list_first(self->list);
    return it ? list_get_item(it) : NULL;
}
static const void* Queue_front_cref(Queue self) { return Queue_front(self); }

static ListIterator Queue_begin(Queue self) { return list_first(self->list); }
static ListIterator Queue_end(Queue self) { (void)self; return NULL; }

static void Queue_clear(Queue self)
{
    for (ListIterator it = list_first(self->list); it; it = list_first(self->list))
        list_delete_item(self->list, it);
    self->count = 0;
}
static int    Queue_empty(Queue self) { return self->count == 0 ? TRUE : FALSE; }
static size_t Queue_size(Queue self) { return self->count; }


/* ---------------- QueueInt (typed) ---------------- */
ClassMembers(QueueInt, Queue)
EndOfClassMembers;

static void QueueInt_initialize(Class this);
static void QueueInt_constructor(QueueInt self, const void* params);
static void QueueInt_destructor(QueueInt self, QueueIntVtable vtab);
static int  QueueInt_copy(QueueInt self, const QueueInt from);

static int        QueueInt_enqueue(QueueInt self, int value);
static int        QueueInt_dequeue(QueueInt self, int* out);
static int* QueueInt_front_t(QueueInt self);
static const int* QueueInt_front_cref(QueueInt self);

AllocateClass(QueueInt, Queue);

static struct QueueIntVtable_stru QueueIntVtableInstance = {
    .Queue = {.Base = {._class = &QueueClass },
               .enqueue_copy = Queue_enqueue_copy,
               .dequeue_copy = Queue_dequeue_copy,
               .front = Queue_front,
               .front_cref = Queue_front_cref,
               .begin = Queue_begin,
               .end = Queue_end,
               .clear = Queue_clear,
               .empty = Queue_empty,
               .size = Queue_size },
    .enqueue = QueueInt_enqueue,
    .dequeue = QueueInt_dequeue,
    .front_t = QueueInt_front_t,
    .front_cref_t = QueueInt_front_cref,
};

static void QueueInt_initialize(Class this) { (void)this; }
static void QueueInt_constructor(QueueInt self, const void* params)
{
    QueueParams qp = { .destroyer = ooc_free };       /* POD bytes managed by list */
    chain_constructor(QueueInt, self, &qp);
    (void)params;
}
static void QueueInt_destructor(QueueInt self, QueueIntVtable vtab)
{
    (void)self; (void)vtab;
}
static int  QueueInt_copy(QueueInt self, const QueueInt from)
{
    (void)self; (void)from; return OOC_COPY_DEFAULT;
}

static int QueueInt_enqueue(QueueInt self, int value)
{
    return QueueIntVirtual(self)->Queue.enqueue_copy((Queue)self, &value, sizeof value);
}
static int QueueInt_dequeue(QueueInt self, int* out)
{
    return QueueIntVirtual(self)->Queue.dequeue_copy((Queue)self, out, sizeof * out);
}
static int* QueueInt_front_t(QueueInt self)
{
    return (int*)QueueIntVirtual(self)->Queue.front((Queue)self);
}
static const int* QueueInt_front_cref(QueueInt self)
{
    return (const int*)QueueIntVirtual(self)->Queue.front_cref((Queue)self);
}


/* ---------------- C wrappers (COOP-like) ---------------- */
static void Queue_once_init(void)
{
    static int inited = 0; if (inited) return; inited = 1;
    ooc_init_class(List);
    ooc_init_class(Queue);
    ooc_init_class(QueueInt);
}

void Queue_int_ctor(Queue_int* q)
{
    Queue_once_init();
    q->impl = (QueueInt)ooc_new(QueueInt, NULL);
}

void Queue_int_dtor(Queue_int* q)
{
    if (q && q->impl) { ooc_delete((Object)q->impl); q->impl = NULL; }
}

int Queue_int_enqueue(Queue_int* q, int value)
{
    return QueueIntVirtual(q->impl)->enqueue(q->impl, value);
}

int Queue_int_dequeue(Queue_int* q, int* out)
{
    return QueueIntVirtual(q->impl)->dequeue(q->impl, out);
}

void Queue_int_front(Queue_int* q, int** out_ptr)
{
    if (out_ptr) *out_ptr = QueueIntVirtual(q->impl)->front_t(q->impl);
}

void Queue_int_front_cref(const Queue_int* q, const int** out_ptr)
{
    if (out_ptr) *out_ptr = QueueIntVirtual(q->impl)->front_cref_t((QueueInt)q->impl);
}

size_t Queue_int_size(const Queue_int* q)
{
    return QueueIntVirtual(q->impl)->Queue.size((Queue)q->impl);
}

bool Queue_int_empty(const Queue_int* q)
{
    return QueueIntVirtual(q->impl)->Queue.empty((Queue)q->impl) ? true : false;
}

void Queue_int_clear(Queue_int* q)
{
    QueueIntVirtual(q->impl)->Queue.clear((Queue)q->impl);
}


void Queue_int_print(const Queue_int* q)
{
    List L = queue_get_list((Queue)q->impl);
    printf("[ ");
    for (ListIterator it = list_first(L); it; it = list_next(L, it)) {
        int* pi = (int*)list_get_item(it);
        printf("%d%s", pi ? *pi : 0, list_next(L, it) ? ", " : "");
    }
    printf(" ]\n");
}
