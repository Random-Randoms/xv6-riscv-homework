#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

uint64 sys_add(void)
{
    int l, r, res;
    uint64 out;

    argint(0, &l);
    argint(1, &r);
    argaddr(2, &out);
    if (!out)
        return -1;
    
    res = (uint32)l + (uint32)r;

    int ret = copyout(myproc()->pagetable, out, (char*)&res, 4);

    if (ret)
        return -2;

    return 0;
}