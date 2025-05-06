#include "types.h"
#include "riscv.h"
#include "defs.h"

// return value should be interpreted as int64
uint64 sys_time(void) {
    return rtcgettime();
}
