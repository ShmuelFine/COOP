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
        int size = 0;
        for (int i = 0; i < N; ++i){
            size = get_size(&bt);
        }
    }
    else if (strcmp(phase, "empty") == 0) {
        bool isEmpty = false;
        for (int i = 0; i < N; ++i) {
            isEmpty = is_empty(&bt);
        }
    }
    else {
        for (int i = 0; i < N; ++i)
            insert(&bt, i);

        int size = 0;
        for (int i = 0; i < N; ++i) {
            size = get_size(&bt);
        }

		bool isEmpty = false;
        for (int i = 0; i < N; ++i) {
            isEmpty = is_empty(&bt);
		}

        for (int i = 0; i < N; ++i){
            bool ok = bt_remove(&bt, i);
        }
    }

    bt_destroy(&bt);
    return 0;
}
