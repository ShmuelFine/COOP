#include "bt_pure.h"
#include <stdio.h>
#include <stddef.h>

/* Helper functions for type int */
static int compare_ints(const void* a, const void* b) {
    int left = *(const int*)a;
    int right = *(const int*)b;
    if (left < right) return -1;
    if (left > right) return  1;
    return 0;
}
static void print_int(const void* value, size_t element_size) {
    (void)element_size;
    printf("%d ", *(const int*)value);
}

/* ----- Another example type ----- */
typedef struct { int id; char grade; } Record;

static void print_record(const void* value, size_t element_size) {
    (void)element_size;
    const Record* r = (const Record*)value;
    printf("{id:%d, grade:%c} ", r->id, r->grade);
}

static int compare_record(const void* a, const void* b) {
    const Record* ra = (const Record*)a;
    const Record* rb = (const Record*)b;
    if (ra->id != rb->id) return (ra->id > rb->id) - (ra->id < rb->id);
    return (ra->grade > rb->grade) - (ra->grade < rb->grade);
}

/* Example of a traversal operation: Schema of all values */
static void action_sum_ints(BT_Tree* owner, const void* value) {
    (void)owner;
    static long long sum_values = 0;
    sum_values += *(const int*)value;
    if ((sum_values % 50) == 0) {
        printf("[partial sum=%lld] ", sum_values);
    }
}

int main(void) {
    BT_Tree int_tree;
    init(&int_tree, sizeof(int), compare_ints, print_int);

    int values_to_insert[] = { 10, 20, 30, 40, 50, 60 };
    for (size_t i = 0; i < sizeof(values_to_insert) / sizeof(values_to_insert[0]); ++i) {
        insert(&int_tree, &values_to_insert[i]);
    }

    printf("PRE : "); print(&int_tree, BT_PRE);
    printf("IN  : "); print(&int_tree, BT_IN);
    printf("POST: "); print(&int_tree, BT_POST);

    traverse_in(&int_tree, action_sum_ints);
    printf("\n");

    int key = 20;
    printf("remove 20? %s\n", bt_remove(&int_tree, &key) ? "yes" : "no");
    printf("IN after remove: "); print(&int_tree, BT_IN);

    destroy(&int_tree);

    BT_Tree record_tree;
    init(&record_tree, sizeof(Record), compare_record, print_record);

    Record r1 = { 101, 'A' };
    Record r2 = { 102, 'B' };
    Record r3 = { 103, 'A' };

    insert(&record_tree, &r1);
    insert(&record_tree, &r2);
    insert(&record_tree, &r3);

    printf("Records IN: "); print(&record_tree, BT_IN);

    Record key_rec = { 102, 'B' };
    printf("remove {102,'B'}? %s\n", bt_remove(&record_tree, &key_rec) ? "yes" : "no");
    printf("Records IN after remove: "); print(&record_tree, BT_IN);

    destroy(&record_tree);
    return 0;
}
