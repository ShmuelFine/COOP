#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "bt_pure.h"

/* ---- phase helpers ---- */
static void phase_insert(GenericBinaryTree* bt, int N) {
    for (int i = 0; i < N; ++i) {
        insert(bt, i);
    }
}

static void phase_remove(GenericBinaryTree* bt, int N) {
    /* prepare */
    for (int i = 0; i < N; ++i) {
        insert(bt, i);
    }
    /* remove */
    for (int i = 0; i < N; ++i) {
        (void)bt_remove(bt, i);
    }
}

static void phase_size(GenericBinaryTree* bt, int N) {
    for (int i = 0; i < N; ++i) {
        (void)get_size(bt);
    }
}

static void phase_empty(GenericBinaryTree* bt, int N) {
    for (int i = 0; i < N; ++i) {
        (void)is_empty(bt);
    }
}

static void phase_all(GenericBinaryTree* bt, int N) {
    for (int i = 0; i < N; ++i) {
        insert(bt, i);
        (void)get_size(bt);
        (void)is_empty(bt);
    }
    for (int i = 0; i < N; ++i) {
        (void)bt_remove(bt, i);
    }
}

/* ---- main ---- */
int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    int N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : 1000000;

    GenericBinaryTree bt;
    bt_init(&bt);

    if (strcmp(phase, "insert") == 0)      phase_insert(&bt, N);
    else if (strcmp(phase, "remove") == 0) phase_remove(&bt, N);
    else if (strcmp(phase, "size") == 0)   phase_size(&bt, N);
    else if (strcmp(phase, "empty") == 0)  phase_empty(&bt, N);
    else                                   phase_all(&bt, N);

    bt_destroy(&bt);
    return 0;
}
