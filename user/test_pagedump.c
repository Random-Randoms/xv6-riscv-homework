#include "kernel/types.h"
#include "user/user.h"

#define BUFSIZE 10000
#define INTS sizeof(int)

#define NONE   0
#define FLAG_A 1
#define FLAG_D 2

void pagedump_error() {
    fprintf(2, "pagedump failed\n");
    exit(-1);
}

void malloc_failed() {
    fprintf(2, "memory allocation failed :(\n"), exit(-1);
}

int main() {
    int kek;
    printf("initial memory condition\n");
    if (pagedump(0, 0, NONE)) pagedump_error();
    
    printf("\nallocate %ldb on heap\n", BUFSIZE * INTS);
    int* lol = (int*)malloc(BUFSIZE * sizeof(int));
    if (!lol) malloc_failed();
    if (pagedump(0, 0, NONE)) pagedump_error();
    
    printf("\nremove all flags and write to the stack\n");
    rmflags(0, 0, FLAG_A | FLAG_D);
    kek = 1;
    if (pagedump(&kek, INTS, NONE)) pagedump_error();

    printf("\nread from the first page, write to the second page of the heap\n");
    lol[2000] = lol[1];
    if (pagedump(lol, BUFSIZE * INTS, NONE)) pagedump_error();

    printf("\nremove flags from heap memory\n");
    rmflags(lol, BUFSIZE * INTS, FLAG_A | FLAG_D);
    if (pagedump(lol, BUFSIZE * INTS, NONE)) pagedump_error();

    printf("\nfree heap memory\n");
    free(lol);
    if (pagedump(0, 0, NONE)) pagedump_error();

    exit(0);
}