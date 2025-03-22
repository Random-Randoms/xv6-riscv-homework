#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    struct procinfo procs[32];

    printf("Correct usage with buffer:\n");
    int ps = ps_listinfo(procs, 32);
    if (ps < 0) fprintf(2, "oops...: exit code: %d", ps), exit(-1);
    printf("there are %d processes\n", ps);
    for (int i = 0; i < ps; ++i)
        printf("process: %d, parent: %d\n", procs[i].pid, procs[i].ppid);
    printf("\n");

    printf("Correct usage without buffer:\n");
    ps = ps_listinfo(0, 32);
    if (ps < 0) fprintf(2, "oops...: exit code: %d", ps), exit(-1);
    printf("there are %d processes\n", ps);
    printf("no additional data\n\n");

    printf("Bad adress:\n");
    ps = ps_listinfo((void*)1, 32);
    printf("exit code is %d\n\n", ps);
    
    printf("Too small buffer:\n");
    ps = ps_listinfo(procs, 1);
    printf("exit code is %d\n", ps);

    exit(0);
}
