#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

#define BUF 128

// DEV_NULL device
int null_write(int usr_src, uint64 src, int n) {
    return n;
}

int null_read(int usr_dst, uint64 dst, int n) {
    return 0;
}


// DEV_ZERO device
struct spinlock zero_lock;

int zero_write(int usr_src, uint64 src, int n) {
    return -1;
}

int zero_read(int usr_dst, uint64 dst, int n) {
    if (n > BUF)
        n = BUF;

    acquire(&zero_lock);
    char buf[BUF];

    for(int i = 0; i < BUF; ++i)
        buf[i] = 0;

    int result = either_copyout(usr_dst, dst, buf, n) ? -1 : n;
    release(&zero_lock);

    return result;
}


// DEV_URANDOM device
#define VUC volatile unsigned char
struct spinlock urnd_lock;
VUC seed = 1, val = 0, mul = 119;

unsigned char urnd_next() {
    val = val * mul + seed;
    return val;
}

int urnd_write(int usr_src, uint64 src, int n) {
    if (n != sizeof(seed)) return -1;

    acquire(&urnd_lock);
    seed += 666;
    release(&urnd_lock);

    return n;
}

int urnd_read(int usr_dst, uint64 dst, int n) {
    if (n > BUF)
        n = BUF;

    acquire(&urnd_lock);
    char buf[BUF];

    for (int i = 0; i < n; ++i)
        buf[i] = urnd_next();

    int result = either_copyout(usr_dst, dst, buf, n) ? -1 : n;
    release(&urnd_lock);

    return result;
}


// DEV_NULLSTAT device
struct spinlock nstt_lock;
volatile uint64 stat = 0;

int nstt_write(int usr_src, uint64 src, int n) {
    acquire(&nstt_lock);
    stat += n;
    release(&nstt_lock);

    return n;
}

int nstt_read(int usr_dst, uint64 dst, int n) {
    if (n != sizeof(uint64)) return -1;

    acquire(&nstt_lock);
    int result = either_copyout(usr_dst, dst, (void*)&stat, n) ? -1 : n;
    release(&nstt_lock);

    return result;
}

// DEV read
int dev_read(int usr_dst, uint64 dst, int n, short minor) {
    switch(minor) {
        case DEV_NULL: return null_read(usr_dst, dst, n);
        case DEV_ZERO: return zero_read(usr_dst, dst, n);
        case DEV_URND: return urnd_read(usr_dst, dst, n);
        case DEV_NSTT: return nstt_read(usr_dst, dst, n);
        default: return -1;
    }
}

// DEV write
int dev_write(int usr_src, uint64 src, int n, short minor) {
    switch(minor) {
        case DEV_NULL: return null_write(usr_src, src, n);
        case DEV_ZERO: return zero_write(usr_src, src, n);
        case DEV_URND: return urnd_write(usr_src, src, n);
        case DEV_NSTT: return nstt_write(usr_src, src, n);
        default: return -1;
    }
}

// DEV init
void devinit() {
    devsw[DEV].read  = dev_read;
    devsw[DEV].write = dev_write;

    initlock(&zero_lock, "dev_zero");
    initlock(&urnd_lock, "dev_urnd");
    initlock(&nstt_lock, "dev_nstt");
}