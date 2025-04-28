#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64 sys_time(void) {
    return rtcgettime();
}
