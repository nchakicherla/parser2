#include <stdio.h>
#include <string.h>
#include <stdalign.h>
#include <stdbool.h>

#include "rkp_error.h"
#include "rkp_arena.h"

//// TESTS_BIN
int main() {
//// TESTS_BIN

    // test allocator
    printf("testing aligned arena...\n");
    bool verbose = true;
    int ret = 0;

    Arena a;
    initArena(&a);
    
    #define BIG_NUM 3
    double **ptr_array = palloc(&a, BIG_NUM * sizeof(double *), alignof(double *));
    for (size_t i = 0; i < BIG_NUM; i++) {
        ptr_array[i] = NULL;
        ptr_array[i] = palloc(&a, sizeof(double), alignof(double));
        if(!ptr_array[i]) {
            printf("palloc failed...\n");
            ret = 1;
            break;
        }
        *ptr_array[i] = 1.5;
        if (verbose) {
            printf("allocd at: %p, val: %f\n", (void *)ptr_array[i], *ptr_array[i]);
        }
    }

    if (BIG_NUM > 1 && ret == 0) {
        printf("test diff: %zu\n", (ptrdiff_t)(ptr_array[1] - ptr_array[0]));
    }

    resetArena(&a);
    termArena(&a);
    return ret;
}