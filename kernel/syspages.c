#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "fcntl.h"

uint64
sys_pagedump() {
    uint64 va;
    uint64 len;
    int msk;

    argaddr(0, &va);
    argaddr(1, &len);
    argint(2, &msk);

    return pagedump(va, len, msk);
}

uint64 
sys_rmflags() {
    uint64 va;
    uint64 len;
    int msk;

    argaddr(0, &va);
    argaddr(1, &len);
    argint(2, &msk);

    return rmflags(va, len, msk);
}