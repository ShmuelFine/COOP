#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>

// --- GQueue Wrapper Functions (Aligned to COOP Logic) ---

// push_back (g_queue_push_tail)
void push_back(GQueue* q, int N)
{
    // LOGIC: Same as COOP list push_back
    for (int i = 0; i < N; ++i)
    {
        g_queue_push_tail(q, GINT_TO_POINTER((int)i));
    }
}

// push_front (g_queue_push_head)
void push_front(GQueue* q, int N)
{
    // LOGIC: Same as COOP list push_front
    for (int i = 0; i < N; ++i)
    {
        g_queue_push_head(q, GINT_TO_POINTER((int)i));
    }
}

// pop_back (g_queue_pop_tail)
void pop_back(GQueue* q, int N)
{

    for (int i = 0; i < N; ++i)
    {
        int out = 0;
        gpointer p = g_queue_pop_tail(q);
        // Note: In COOP, pop_back receives &out. We simulate the same logic here.
        if (p) { out = GPOINTER_TO_INT(p); }
        (void)out; // avoid warnings
    }
}

// pop_front (g_queue_pop_head)
void pop_front(GQueue* q, int N)
{
    for (int i = 0; i < N; ++i)
    {
        int out = 0;
        gpointer p = g_queue_pop_head(q);
        // Note: In COOP, pop_front receives &out. We simulate the same logic here.
        if (p) { out = GPOINTER_TO_INT(p); }
        (void)out; // avoid warnings
    }
}

// front (g_queue_peek_head)
void front(GQueue* q, int N)
{
    // LOGIC: Aligned to COOP list front: push one element (123), then peek N times.
    g_queue_push_tail(q, GINT_TO_POINTER(123));

    int pFront = 0;
    for (int i = 0; i < N; ++i)
    {
        gpointer p = g_queue_peek_head(q);
        if (p) { pFront = GPOINTER_TO_INT(p); }
    }
    (void)pFront; // avoid warnings
    g_queue_clear(q); // Clear after test for clean exit, as COOP list object is local to main.
}

// back (g_queue_peek_tail)
void back(GQueue* q, int N)
{
    // LOGIC: Aligned to COOP list back: push one element (123), then peek N times.
    g_queue_push_tail(q, GINT_TO_POINTER(123));

    int pBack = 0;
    for (int i = 0; i < N; ++i)
    {
        gpointer p = g_queue_peek_tail(q); // Equivalent to List_int->back
        if (p) { pBack = GPOINTER_TO_INT(p); }
    }
    (void)pBack; // avoid warnings
    g_queue_clear(q); // Clear after test for clean exit.
}

// size (g_queue_get_length)
void size(GQueue* q, int N)
{
    // LOGIC: Same as COOP list size: Loop N times, calling size each time.
    int size = 0;
    for (int i = 0; i < N; ++i)
    {
        size = (int)g_queue_get_length(q);
    }
    (void)size; // avoid warnings
}

// empty (g_queue_is_empty)
void empty(GQueue* q, int N)
{
    // LOGIC: Same as COOP list empty: Loop N times, calling empty each time.
    bool is_empty = false;
    for (int i = 0; i < N; ++i)
    {
        is_empty = g_queue_is_empty(q);
    }
    (void)is_empty; // avoid warnings
}

// clear (g_queue_clear)
void clear(GQueue* q, int N)
{
    g_queue_clear(q);
}

// all (Combines several operations)
void all(GQueue* q, int N)
{
    // LOGIC: Aligned to COOP list all
    bool is_empty = false;
    int size = 0;
    int out_val = 0;

    // Phase 1: Mixed Push/Peek/Size/Empty (N/2 iterations)
    for (int i = 0; i < N / 2; ++i)
    {
        g_queue_push_tail(q, GINT_TO_POINTER((int)i)); // push_back
        g_queue_push_head(q, GINT_TO_POINTER((int)i)); // push_front

        // Front (peek_head)
        gpointer pHead = g_queue_peek_head(q);
        if (pHead) { out_val = GPOINTER_TO_INT(pHead); }

        // Back (peek_tail)
        gpointer pTail = g_queue_peek_tail(q);
        if (pTail) { out_val = GPOINTER_TO_INT(pTail); }

        // Size & Empty
        size = (int)g_queue_get_length(q);
        is_empty = g_queue_is_empty(q);
    }
    (void)out_val; (void)size; (void)is_empty;

    // Phase 2: Mixed Pop (N/2 iterations)
    int out = 0;
    for (int i = 0; i < N / 2; ++i)
    {
        gpointer p1 = g_queue_pop_tail(q);  // pop_back
        if (p1) { out = GPOINTER_TO_INT(p1); }

        gpointer p2 = g_queue_pop_head(q);  // pop_front
        if (p2) { out = GPOINTER_TO_INT(p2); }
    }
    (void)out;

    // Phase 3: Push N and Clear
    for (int i = 0; i < N; ++i)
    {
        g_queue_push_tail(q, GINT_TO_POINTER((int)i));
    }
    g_queue_clear(q);
}


// --- Main Function ---

int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    int  N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : (int)1000000;

    GQueue* q = g_queue_new();
    if (!q) return 1;

    if (strcmp(phase, "push_back") == 0)
        push_back(q, N);
    else if (strcmp(phase, "push_front") == 0)
        push_front(q, N);
    else if (strcmp(phase, "pop_back") == 0) {
        push_back(q, N);
        pop_back(q, N);
    }
    else if (strcmp(phase, "pop_front") == 0) {
        push_front(q, N);
        pop_front(q, N);
    }
    else if (strcmp(phase, "front") == 0)
        front(q, N);
    else if (strcmp(phase, "back") == 0)
        back(q, N);
    else if (strcmp(phase, "size") == 0)
        size(q, N);
    else if (strcmp(phase, "empty") == 0)
        empty(q, N);
    else if (strcmp(phase, "clear") == 0)
        clear(q, N);
    else // The 'all' phase
        all(q, N);

    // Note: The individual functions clear the queue only if it's necessary for the test logic (front/back).
    // The main queue cleanup is done here.
    g_queue_free(q);
    return 0;
}