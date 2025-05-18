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
    double **arr1 = palloc(&a, BIG_NUM * sizeof(double *), alignof(double *));
    for (size_t i = 0; i < BIG_NUM; i++) {
        arr1[i] = NULL;
        arr1[i] = palloc(&a, sizeof(double), alignof(double));
        if(!arr1[i]) {
            printf("palloc failed...\n");
            ret = 1;
            break;
        }
        *arr1[i] = 1.5;
        if(verbose) {
            printf("allocd at: %p, val: %f\n", (void *)arr1[i], *arr1[i]);
        }
    }

    // test zeroing function
    char *arr2 = pzalloc(&a, BIG_NUM * 17, alignof(char));
    for (size_t i = 0; i < BIG_NUM * 17; i++) {
        arr2[i] = 'c';
        if(verbose) {
            printf("%c", arr2[i]);
        }
    }
    printf("\n");

    printArenaInfo(&a);
    printf("resetting arena...\n");
    resetArena(&a);
    printf("arena reset...\n");
    printArenaInfo(&a);
    termArena(&a);
    return ret;
}