#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char* alloc_err = "unable to allocate memeory on heap\n";

static char *states[] = {
    [_SLEEPING]  "sleep ",
    [_RUNNABLE]  "runble",
    [_RUNNING]   "run   ",
    [_ZOMBIE]    "zombie"
    };

int main() {
    int buf = 8;
    struct procinfo* procs = (struct procinfo*)malloc(buf * sizeof(struct procinfo));


    if (!procs)
        fprintf(2, "%s", alloc_err), exit(-1);

    int ps;

    while (1) {
        ps = ps_listinfo(procs, buf);

        if (ps == -2) 
            fprintf(2, "error"), exit(-2);

        if (ps >= 0)
            break;

        buf <<= 1;
        free(procs);
        procs = malloc(buf * sizeof(struct procinfo));
        if (!procs)
            fprintf(2, "%s", alloc_err), exit(-1);
    }

    int idmax = 0;

    for (int i = 0; i < ps; ++i)
        idmax = idmax < procs[i].pid ? procs[i].pid : idmax;

    char** names = (char**)malloc(sizeof(char*) * (idmax + 1));

    for (int i = 0; i < ps; ++i)
        if (procs[i].pid >= 0)
            names[procs[i].pid] = procs[i].name;

    printf(" pid:  name:  state:  ppid:  pname: \n");

    for (int i = 0; i < ps; ++i) {
        printf(" %d ", procs[i].pid),
        printf(" %s ", procs[i].name),
        printf(" %s ", states[procs[i].state]),
        printf(" %d ", procs[i].ppid);

        if (procs[i].ppid >= 0)
            printf(" %s \n", names[procs[i].ppid]);
        else
            printf(" %s \n", "init parentless");
    }

    exit(0);
}