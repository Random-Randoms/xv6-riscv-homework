#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("check read/write\n");
    int mt = mutex();
    if (mt < 0) fprintf(2, "mutex creation fail error\n"), exit(-1);

    printf("write exit code: %d\n", write(mt, "kek\n", 5));
    char buf[1];
    printf("read exit code: %d\n", read(mt, buf, 1));

    printf("\ncheck close my mutex\n");
    if (mutex_lock(mt)) fprintf(2, "mutex lock failed\n"), exit(-2);
    printf("exit code: %d\n", close(mt));

    printf("\ncheck close foreign mutex\n");
    mt = mutex();
    mutex_lock(mt);

    int pr = fork();
    if (pr < 0) fprintf(2, "fork failed\n"), exit(-3);
    if (pr == 0) {
        printf("exit code: %d\n", close(mt));
        exit(0);
    } else wait(0);
    close(mt);

    printf("\ncheck unlock locked mutex\n");

    mt = mutex();
    mutex_lock(mt);
    pr = fork();
    if (pr < 0) fprintf(2, "fork failed\n"), exit(-3);
    if (pr == 0) {
        mutex_lock(mt);
        printf("lock in child\n");
        sleep(10);
        exit(0);
    } else  {
        sleep(30);
        mutex_unlock(mt);
        wait(0);
        if (close(mt)) fprintf(2, "mutex closing failed\n"), exit(-4);
    }

    printf("\ndouble lock\n");
    mt = mutex();
    printf("first lock exit code: %d\n", mutex_lock(mt));
    printf("second lock exit code: %d\n", mutex_lock(mt));
    mutex_unlock(mt);
    close(mt);

    printf("\nleave unclosed mutices\n");
    mt = mutex();
    int mt2 = mutex();
    mutex_lock(mt2);
    exit(0);
}