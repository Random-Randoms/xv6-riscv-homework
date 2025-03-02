#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int MODE_A = 0, MODE_B = 1;

int
main(int argc, char** argv)
{
    int mode = MODE_A;

    if (argc >= 2)
        mode = strcmp("--b", argv[1]) ? MODE_A : MODE_B;

    printf("Parent id: %d\n", getpid());

    int pid = fork();

    switch(pid) {
    case (-1):
        fprintf(2, "fork error");
        exit(2);
    case 0:
        sleep(50);
        exit(1);
    default:
        printf("Child id: %d\n", pid);
        int killed = -5;

        if (mode == MODE_A)
            wait(&killed);
        else
            kill(pid), wait(&killed);

        printf("Process %d finished\n", killed);
        exit(0);
    }
}