#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int mt;

void print_unsafe(int argc, char** argv) {
    printf("%d:", getpid());

    for (int i = 0; i < argc; ++i)
        for (char* c = argv[i]; *c; ++c)
            printf("arg %d, char ", i),
            write(0, c, 1), printf("\n");
}

void print_safe(int argc, char** argv) {
    mutex_lock(mt);
    printf("%d:", getpid());

    for (int i = 0; i < argc; ++i)
        for (char* c = argv[i]; *c; ++c)
            printf("arg %d, char ", i),
            write(0, c, 1), printf("\n");
    mutex_unlock(mt);
}

int main(int argc, char** argv) {
    printf("test without mutex\n");

    int pr = fork();

    if (pr < 0) fprintf(2, "fork error"), exit(-1);

    print_unsafe(argc, argv);

    if (pr == 0) exit(0);
    else wait(0);

    printf("test with mutex\n");
    mt = mutex();

    pr = fork();
    if (pr < 0) fprintf(2, "fork error"), exit(-1);

    print_safe(argc, argv);

    if (pr == 0) exit(0);
    else wait(0);

    exit(0);
}