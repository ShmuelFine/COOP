#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>

int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    int  N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : (int)1000000;

    GQueue* q = g_queue_new();
    if (!q) return 1;

    if (strcmp(phase, "pop_back") == 0 ||
        strcmp(phase, "pop_front") == 0)
    {
        for (int i = 0; i < N; ++i)
        {
            g_queue_push_tail(q, GINT_TO_POINTER((int)i));
        }

        if (strcmp(phase, "pop_back") == 0)
        {
            for (int i = 0; i < N; ++i)
            {
                int out = 0;
                gpointer p = g_queue_pop_tail(q);
                out = GPOINTER_TO_INT(p);
                (void)out;
            }
        }
        else if (strcmp(phase, "pop_front") == 0)
        {
            for (int i = 0; i < N; ++i)
            {
                int out = 0;
                gpointer p = g_queue_pop_head(q);
                out = GPOINTER_TO_INT(p);
                (void)out;
            }
        }
    }

    // ---- push_back ----
    if (strcmp(phase, "push_back") == 0)
    {
        for (int i = 0; i < N; ++i)
        {
            g_queue_push_tail(q, GINT_TO_POINTER((int)i));
        }
    }
    // ---- push_front ----
    else if (strcmp(phase, "push_front") == 0)
    {
        for (int i = 0; i < N; ++i)
        {
            g_queue_push_head(q, GINT_TO_POINTER((int)i));
        }
    }
    // ---- size ----
    else if (strcmp(phase, "size") == 0)
    {
        int size = 0;
        for (int i = 0; i < N; ++i)
        {
            size = (int)g_queue_get_length(q);
        }
    }
    // ---- empty (API size==0) ----
    else if (strcmp(phase, "empty") == 0)
    {
        bool is_empty = 0;
        for (int i = 0; i < N; ++i)
        {
            is_empty = g_queue_is_empty(q) ? true : false;
        }
    }
    else if (strcmp(phase, "clear") == 0) {
        for (int i = 0; i < N; ++i)
        {
            g_queue_push_tail(q, GINT_TO_POINTER((int)i));
        }
        g_queue_clear(q);
    }
    else if (strcmp(phase, "front") == 0)
    {
        int pFront = 0;
        for (int i = 0; i < N; ++i)
        {
            gpointer p = g_queue_peek_head(q);
            pFront = GPOINTER_TO_INT(p);
        }
        (void)pFront;
    }
    else
    {
        for (int i = 0; i < N / 2; ++i)
        {
            int size = 0;
            bool is_empty = 0;
            g_queue_push_tail(q, GINT_TO_POINTER((int)i)); // push_back
            g_queue_push_head(q, GINT_TO_POINTER((int)i)); // push_front
            size = (int)g_queue_get_length(q);
            is_empty = g_queue_is_empty(q) ? true : false;
            (void)size; (void)is_empty;
        }

        for (int i = 0; i < N / 2; ++i)
        {
            int out = 0;
            gpointer p1 = g_queue_pop_tail(q);  // pop_back
            out = GPOINTER_TO_INT(p1);
            (void)out;
            gpointer p2 = g_queue_pop_head(q);  // pop_front
            out = GPOINTER_TO_INT(p2);
            (void)out;
        }

        for (int i = 0; i < N; ++i)
        {
            g_queue_push_tail(q, GINT_TO_POINTER((int)i));
        }
        g_queue_clear(q);
    }

    g_queue_free(q);
    return 0;
}
