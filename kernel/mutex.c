#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "stat.h"
#include "proc.h"

int mutexalloc(struct file **f) {
    *f = 0;
    struct sleeplock* lock = 0;

    if ((*f = filealloc()) == 0)
        goto bad;
    if ((lock = (struct sleeplock*)kalloc()) == 0)
        goto bad;
    initsleeplock(lock, "mutex");
    (*f)->type = FD_MUTEX;
    (*f)->readable = 0;
    (*f)->writable = 0;
    (*f)->lock = lock;
    printf("mutex at %p alloc'd\n", *f);
    return 0;

bad:
    if (*f) fileclose(*f);
    if (lock) kfree(lock);
    return -1;
}

void mutexclose(struct file *f) {
    printf("mutex at %p closed\n", f);
    kfree(f->lock);
}
