//
// rtc support
//

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define Reg(reg) ((volatile uint32 *)(RTC + (reg)))

#define LOW  0
#define HIGH 4

#define ReadReg(reg) (*(Reg(reg)))

struct spinlock rtc_lock;

void rtcinit() {
    initlock(&rtc_lock, "main");
}

// return value should be interpreted as int64
uint64 rtcgettime() {
    acquire(&rtc_lock);
    uint32 low = ReadReg(LOW);
    uint32 high = ReadReg(HIGH);
    release(&rtc_lock);

    return low + (((uint64)high) << 32);
}
