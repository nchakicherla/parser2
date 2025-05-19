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
    bool verbose = false;
    int ret = 0;

    rkp_arena a;
    rkp_arena_init(&a);
    
    #define BIG_NUM 30000
    double **arr1 = rkp_arena_alloc(&a, BIG_NUM * sizeof(double *), alignof(double *));
    for (size_t i = 0; i < BIG_NUM; i++) {
        arr1[i] = NULL;
        arr1[i] = rkp_arena_alloc(&a, sizeof(double), alignof(double));
        if(!arr1[i]) {
            printf("rkp_arena_alloc failed...\n");
            ret = 1;
            break;
        }
        *arr1[i] = 1.5;
        if(verbose) {
            printf("allocd at: %p, val: %f\n", (void *)arr1[i], *arr1[i]);
        }
    }

    // test zeroing function
    char *arr2 = rkp_arena_zalloc(&a, BIG_NUM * 17, alignof(char));
    for (size_t i = 0; i < BIG_NUM * 17; i++) {
        arr2[i] = 'c';
        if(verbose) {
            printf("%c", arr2[i]);
        }
    }
    printf("\n");

    rkp_arena_print_info(&a);
    printf("resetting arena...\n");
    rkp_arena_reset(&a);
    printf("arena reset...\n");
    rkp_arena_print_info(&a);
    rkp_arena_term(&a);

    //test setting log_output_file
    printf("testing setting rkp_error::log_output_file...\n");
    printf("(%d) called rkp_error_set_log_file\n", rkp_error_set_log_file("test.txt"));
    printf("testing closing rkp_error::log_output_file...\n");
    printf("(%d) called rkp_error_close_log_file\n", rkp_error_close_log_file());

    return ret;
}