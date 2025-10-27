#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "bt_pure.h"

int main(int argc, char** argv)
{
    const char* phase = (argc > 1) ? argv[1] : "all";
    int  N = (argc > 2) ? (int)strtoull(argv[2], NULL, 10) : 1000000;

    GenericBinaryTree bt;
    bt_init(&bt);

    if (strcmp(phase, "insert") == 0) {
        for (int i = 0; i < N; ++i) {
            insert(&bt, i);
        }
    }
    else if (strcmp(phase, "remove") == 0) {
        for (int i = 0; i < N; ++i) {
            insert(&bt, i);
        }
        for (int i = 0; i < N; ++i) {
            (void)bt_remove(&bt, i);
        }
    }
    else if (strcmp(phase, "size") == 0) {
        for (int i = 0; i < N; ++i){
            (void)get_size(&bt);
        }
    }
    else if (strcmp(phase, "empty") == 0) {
        for (int i = 0; i < N; ++i) {
            (void)is_empty(&bt);
        }
    }
    else {
        int size = 0;
		bool isEmpty = false;

        for (int i = 0; i < N; ++i) {
            insert(&bt, i);
            size = get_size(&bt);
            isEmpty = is_empty(&bt);
        }

        for (int i = 0; i < N; ++i){
            (void)bt_remove(&bt, i);
        }
    }

    bt_destroy(&bt);
    return 0;
}
