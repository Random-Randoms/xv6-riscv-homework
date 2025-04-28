#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MICR_NANO   1000
#define MILL_MICR   1000
#define SEC_MILL    1000
#define MIN_SEC     60
#define HR_MIN      60
#define DAY_HR      24

#define NORMAL_YEAR 365
#define LEAP_YEAR   366

#define PERIOD  400
#define CENTURY 100
#define LEAP    4

#define PERIOD_DAYS  (NORMAL_YEAR * PERIOD + PERIOD / LEAP - PERIOD / CENTURY + 1)
#define CENTURY0_END (CENTURY * PERIOD + CENTURY / LEAP - 1)

#define SHORT_QUAD_DAYS (NORMAL_YEAR * LEAP)
#define QUAD_DAYS       (SHORT_QUAD_DAYS + 1)

#define UNIX_DAYS_ANNO_DOMINI (1970 * NORMAL_YEAR + 1970 / LEAP - 1970 / CENTURY + 1970 / PERIOD)

#define JAN        31
#define FEB        28
#define FEB_LEAP   29
#define FFEB(leap) (leap ? FEB_LEAP : FEB)
#define MAR        31
#define APR        30
#define MAY        31
#define JUN        30
#define JUL        31
#define AUG        31
#define SEP        30
#define OCT        31
#define NOV        30
#define DEC        31

static const char* months[] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec" };

void print3(short num) {
    if (num > 99)
        printf("%d", num);
    else if (num > 9)
        printf("0%d", num);
    else 
        printf("00%d", num);
}

void print2(short num) {
    if (num > 9)
        printf("%d", num);
    else 
        printf("0%d", num);
}

int main() {
    uint64 t = time();
    uint64 days = t / ((uint64)DAY_HR * HR_MIN * MIN_SEC * SEC_MILL * MILL_MICR * MICR_NANO) + UNIX_DAYS_ANNO_DOMINI;
    int hour    = t / ((uint64)HR_MIN * MIN_SEC * SEC_MILL * MILL_MICR * MICR_NANO) % DAY_HR;
    int minute  = t / ((uint64)MIN_SEC * SEC_MILL * MILL_MICR * MICR_NANO) % HR_MIN;
    int second  = t / ((uint64)SEC_MILL * MILL_MICR * MICR_NANO) % MIN_SEC;
    int milli   = t / ((uint64)MILL_MICR * MICR_NANO) % SEC_MILL;
    int micro   = t / MICR_NANO % MILL_MICR;
    int nano    = t % MICR_NANO;

    uint64 period = days / (uint64)PERIOD_DAYS;
    int period_day = days % PERIOD_DAYS;
    int century = period_day / CENTURY0_END;
    int century_day = period_day % CENTURY0_END;
    if (century == 3 && century_day == 0) century_day = CENTURY0_END;
    int quadrennium = century_day / QUAD_DAYS;
    int quad_day = century_day % QUAD_DAYS; 
    int day = quad_day % NORMAL_YEAR, year = quad_day / NORMAL_YEAR, leap = 0;
    if (year == 4) year = 3, day = NORMAL_YEAR, leap = 1;

    year += period * PERIOD + century * CENTURY + quadrennium * LEAP;

    short month = 0;

    if (day >= JAN)        day -= JAN,        ++month; else goto __print;
    if (day >= FFEB(leap)) day -= FFEB(leap), ++month; else goto __print;
    if (day >= MAR)        day -= MAR,        ++month; else goto __print;
    if (day >= APR)        day -= APR,        ++month; else goto __print;
    if (day >= MAY)        day -= MAY,        ++month; else goto __print;
    if (day >= JUN)        day -= JUN,        ++month; else goto __print;
    if (day >= JUL)        day -= JUL,        ++month; else goto __print;
    if (day >= AUG)        day -= AUG,        ++month; else goto __print;
    if (day >= SEP)        day -= SEP,        ++month; else goto __print;
    if (day >= OCT)        day -= OCT,        ++month; else goto __print;
    if (day >= NOV)        day -= NOV,        ++month; else goto __print;

__print:

    print2(day + 1);
    printf(" %s %d ", months[month], year);
    print2(hour);
    printf(":");
    print2(minute);
    printf(":");
    print2(second);
    printf(".");
    print3(milli);
    printf(".");
    print3(micro);
    printf(".");
    print3(nano);
    printf("\n");

    exit(0);
}